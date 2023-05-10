#include <stdlib.h>
#include <iostream>
#include <vector>

#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using namespace std;

int main( void )
{
	try
	{
		sql::Driver *driver;
		sql::Connection *con;
		sql::Statement *stmt;
		sql::ResultSet *res;

		/* Create a connection */
		driver = get_driver_instance();
		con = driver->connect( "tcp://127.0.0.1:3306", "root", "root" );
		/* Connect to the MySQL test database */
		con->setSchema( "bio" );

		stmt = con->createStatement();
		res = stmt->executeQuery( "SELECT ID, TEMPLATE FROM FINGERS" );
		//res = stmt->executeQuery( "SELECT 'Hello World!' AS _message" );

		cout << "Rows Count: " << res->rowsCount() << endl;

		while( res->next() )
		{
			cout << "id = " << res->getInt( "ID" ) << endl;
			istream *tmpl = res->getBlob( "TEMPLATE" );
			vector<char> vec;

			tmpl->seekg( 0, ios::end );
			size_t length = tmpl->tellg();

			if( length )
			{
				tmpl->seekg( 0, std::ios::beg );
				vec.resize( static_cast<std::size_t>( length ) );
				tmpl->read( &vec.front(), static_cast<std::size_t>( length ) );
			}

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

	return 0;
}
