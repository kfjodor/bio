#include "bio_unifinger.h"

void Unifinger::loadDB( string mysql_address, string mysql_login, string mysql_password, string mysql_database, int project_id )
{
	int i = 0;

	try
	{
		sql::Driver *driver;
		sql::Connection *con;
		sql::Statement *stmt;
		sql::ResultSet *res;

		/* Create a connection */
		driver = get_driver_instance();
		con = driver->connect( mysql_address, mysql_login, mysql_password );
		/* Connect to the MySQL test database */
		con->setSchema( mysql_database );

		stmt = con->createStatement();		
		ostringstream oss;
		oss << "SELECT fingerprint.id, fingerprint.template FROM fingerprint LEFT JOIN user ON fingerprint.user_id = user.id LEFT JOIN project ON user.project_id = project.id WHERE project.id = " << project_id;
		res = stmt->executeQuery( oss.str() );

		int rows_count = res->rowsCount();

		vItems.resize( rows_count );
		vFinger.resize( rows_count );
		vFingerSize.resize( rows_count );
		usersIdx.resize( rows_count );

		cout << "Rows count " << rows_count << endl;

		while( res->next() )
		{
			vector<unsigned char> tmp_vector;
			int id = res->getInt( "ID" );

			istream *tmpl = res->getBlob( "TEMPLATE" );
			tmpl->seekg( 0, ios::end );
			size_t length = tmpl->tellg();

			if( !length )
				continue;

			tmpl->seekg( 0, std::ios::beg );
			tmp_vector.resize( length );
			tmpl->read( ( char* )&tmp_vector.front(), length );

			vItems[i] = tmp_vector;
			vFinger[i] = vItems[i].data();
			vFingerSize[i] = vItems[i].size();
			usersIdx[i] = id;
			i++;
		}

		delete res;
		delete stmt;
		delete con;

	}

	catch( sql::SQLException &e )
	{
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}

	cout << "Loaded " << vItems.size() << " templates" << endl;
}

void Unifinger::rematchMap( int seq, int id )
{
	// map not used, lock free
	if( usersIdx.at( seq ) != 0 )		
		throw bio_exception( 1402, "template already assigned to user" );

	// remap if not found
	std::lock_guard<std::mutex> lock( map_mutex );
	usersIdx[seq] = id;
}

void Unifinger::checkExistence( int id )
{
	std::lock_guard<std::mutex> lock( map_mutex );

	for( auto i: usersIdx )
	{
		if( i == id )
			throw bio_exception( 1403, "id already added" );
	}
}

void Unifinger::DBAddFinger( int id, vector<unsigned char> vtemp )
{
	// Do not add coz search in progress
	while( atomic_counter != 0 )
		std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) ); // half second

	// Lock data
	std::unique_lock<std::mutex> dlock( data_mutex );
	vItems.push_back( vtemp );
	vFinger.push_back( vItems.back().data() );
	vFingerSize.push_back( vItems.back().size() );
	// Unlock data
	dlock.unlock();

	// Lock map
	std::lock_guard<std::mutex> ulock( map_mutex );
	usersIdx.push_back( id );
}

void Unifinger::DBDelFinger( int id )
{
	// Do not add coz search in progress
	while( atomic_counter != 0 )
		std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) ); // hald second

	std::lock_guard<std::mutex> lock( map_mutex );

	for( unsigned int i=0; i<usersIdx.size(); i++ )
	{
		if( usersIdx[i] == id )
		{
			usersIdx[i] = 0;
			return;
		}
	}

	throw bio_exception( 1401, "id not found" );
}

// Returns ID in map<db_id,index>
int Unifinger::identify( HUFMatcher matcher, vector<unsigned char> t1 ) // data to identify
{
	int nMatchIndex = 0;
	UFM_STATUS ufm_res;
	stringstream ss;

	if( vFinger.size() == 0 )
		throw bio_exception( 1401, "Database empty" );

	// Lock data to prevent collision with ADD
	std::unique_lock<std::mutex> dlock( data_mutex );
	// Increment counter to show other process that search in progress
	atomic_counter++;
	// Unlock data coz counter already up
	dlock.unlock();

	// Identify
	ufm_res = UFM_Identify( matcher, t1.data(), t1.size(), vFinger.data(), vFingerSize.data(), vFinger.size(), 0, &nMatchIndex );

	// search complete
	atomic_counter--;

	if( ufm_res != UFM_OK )
	{
		ss << "UFMatcher::identify() - Error: " << processErrorString( ufm_res );
		throw bio_exception( 1410, ss.str() );
	}

	if( nMatchIndex == -1 )
	{		
		throw bio_exception( 1401, "Fingerprint not found" );
	}

	std::lock_guard<std::mutex> ulock( map_mutex );

	int map_id = usersIdx.at( nMatchIndex );

	if( map_id == 0 )
		throw bio_exception( 1401, "Fingerprint not found" );

	return map_id;
}

int Unifinger::identify_sequence( HUFMatcher matcher, vector<unsigned char> t1 ) // data to identify
{
	int nMatchIndex = 0;
	UFM_STATUS ufm_res;
	stringstream ss;

	if( vFinger.size() == 0 )
		return -1;		

	// Lock data to prevent collision with ADD
	std::unique_lock<std::mutex> dlock( data_mutex );
	// Increment counter to show other process that search in progress
	atomic_counter++;
	// Unlock data coz counter already up
	dlock.unlock();

	// Identify
	ufm_res = UFM_Identify( matcher, t1.data(), t1.size(), vFinger.data(), vFingerSize.data(), vFinger.size(), 0, &nMatchIndex );

	// search complete
	atomic_counter--;

	if( ufm_res != UFM_OK )
	{
		ss << "UFMatcher::identify_sequence() - Error: " << processErrorString( ufm_res );
		throw bio_exception( 1410, ss.str() );
	}

	return nMatchIndex; // -1 not found
}


string Unifinger::processErrorString( int res )
{
	string out;
	char m_StrError[128] = {0};

	if( UFM_GetErrorString( res, reinterpret_cast<char*>( &m_StrError ) ) == UFM_OK )
		out.assign( m_StrError );

	return out;
}

HUFMatcher Unifinger::createMatcher()
{
	HUFMatcher myMatcher;
	UFM_STATUS ufm_res;
	ufm_res = UFM_Create( &myMatcher );

	if( ufm_res != UFM_OK )
	{
		stringstream ss;
		ss << "UFMatcher::createMatcher() - Error: " << processErrorString( ufm_res );
		throw runtime_error( ss.str() );
	}

	return myMatcher;
}

void Unifinger::deleteMatcher( HUFMatcher matcher )
{
	UFM_STATUS ufm_res;
	ufm_res = UFM_Delete( matcher );

	if( ufm_res != UFM_OK )
	{
		stringstream ss;
		ss << "UFMatcher::deleteMatcher() - Error: " << processErrorString( ufm_res );

		throw runtime_error( ss.str() );
	}
}

void Unifinger::hex_to_char( string data, vector<unsigned char> &out )
{
	std::istringstream hex_chars_stream( data );

	unsigned int c;

	while( hex_chars_stream >> std::hex >> c )
	{
		out.push_back( c );
	}
}
/*
void Unifinger::printHex( vector<unsigned char> data )
{
	cout << "[" << data.size() << "]";

for( auto i: data )
		printf( "%02X ", i ); //wtf //cout << setw(3) << hex << uppercase << i;

	cout << endl;
}
*/
