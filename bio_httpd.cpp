#include "bio_httpd.h"

Json::Value HTTPD::parse_v1_search( Json::Value root )
{
	Json::Value val;

	try
	{
		string tmpl = root.get( "template","" ).asString();

		if( tmpl.length() == 0 )
			throw bio_exception( 1404, "invalid template(empty)" );

		vector<unsigned char> vtemp;
		unifinger->hex_to_char( tmpl, vtemp );

		int match_id = unifinger->identify( Matchers[this_thread::get_id()], vtemp );
		val["match_id"] = match_id;
	}

	catch( bio_exception &ex )
	{
		val["error_code"] = ex.getErrorCode();
		val["error_string"] = ex.what();
		return val;
	}

	val["error_code"] = 0;
	return val;
}

Json::Value HTTPD::parse_v1_add( Json::Value root )
{
	Json::Value val;
	int id = 0;

	try
	{
		string tmpl = root.get( "template","" ).asString();
		id = root.get( "id",0 ).asInt();

		if( tmpl.length() == 0 )
			throw bio_exception( 1404, "invalid template(empty)" );

		if( id == 0 )
			throw bio_exception( 1405, "invalid id" );

		unifinger->checkExistence( id );

		vector<unsigned char> vtemp;
		unifinger->hex_to_char( tmpl, vtemp );

		int seq = unifinger->identify_sequence( Matchers[this_thread::get_id()], vtemp );

		if( seq == -1 )
			unifinger->DBAddFinger( id, vtemp );
		else
		{
			unifinger->rematchMap( seq, id );
		}
	}

	catch( bio_exception &ex )
	{
		val["error_code"] = ex.getErrorCode();
		val["error_string"] = ex.what();
		return val;
	}

	val["id"] = id;
	val["error_code"] = 0;
	return val;
}

Json::Value HTTPD::parse_v1_delete( Json::Value root )
{
	Json::Value val;
	int id = 0;

	try
	{
		id = root.get( "id",0 ).asInt();

		if( id == 0 )
			throw bio_exception( 1405, "invalid id" );

		unifinger->DBDelFinger( id );
	}

	catch( bio_exception &ex )
	{
		val["error_code"] = ex.getErrorCode();
		val["error_string"] = ex.what();
		return val;
	}

	val["id"] = id;
	val["error_code"] = 0;
	return val;
}

string HTTPD::HTTPParser( string uri, string req )
{
	Json::Value value;
	Json::Value root;
	Json::FastWriter writer;
	Json::Reader reader;

	//cout << "Thread " << this_thread::get_id() << " Serving: " << uri << endl;

	try
	{
		if( !reader.parse( req, root, false ) )
		{
			value["error_string"] = "Parse error";
			value["error_code"] = 1410;
		}
		else if( uri.compare( "/v1/search" ) == 0 )
		{
			value = parse_v1_search( root );
		}
		else if( uri.compare( "/v1/add" ) == 0 )
		{
			value = parse_v1_add( root );
		}
		else if( uri.compare( "/v1/delete" ) == 0 )
		{
			value = parse_v1_delete( root );
		}
		else
		{
			value["error_string"] = "Unknown command";
			value["error_code"] = 1410;
		}		
	}
	
	catch( exception &ex )
	{
		value["error_code"] = 1410;
		value["error_string"] = ex.what();	
	}
	
	return writer.write( value );
}

void HTTPD::HTTPRequestHandler( evhttp_request *req, void *me )
{
	HTTPD *ptr = static_cast<HTTPD*>( me );

	auto *OutBuf = evhttp_request_get_output_buffer( req );
	auto *InBuf = evhttp_request_get_input_buffer( req );

	if( !OutBuf || !InBuf )
		return;

	size_t len = evbuffer_get_length( InBuf );
	char *data = NULL;
	data = ( char* ) malloc( len+1 );

	if( data == NULL )
	{
		cerr << "Malloc error" << endl;
		return;
	}

	memset( data, 0, len+1 );
	evbuffer_copyout( InBuf, data, len );

	string jsonreq( data );
	free( data );

	string res = ptr->HTTPParser( req->uri, jsonreq );

	evbuffer_add_printf( OutBuf, "%s", res.c_str() );
	evhttp_send_reply( req, HTTP_OK, "", OutBuf );
};

void HTTPD::threadHandler( HTTPD *ptr )
{
	try
	{
		std::unique_ptr<event_base, decltype( &event_base_free ) > EventBase( event_base_new(), &event_base_free );

		if( !EventBase )
			throw std::runtime_error( "Failed to create new base_event." );

		std::unique_ptr<evhttp, decltype( &evhttp_free ) > EvHttp( evhttp_new( EventBase.get() ), &evhttp_free );

		if( !EvHttp )
			throw std::runtime_error( "Failed to create new evhttp." );

		evhttp_set_gencb( EvHttp.get(), HTTPRequestHandler, ptr );

		if( ptr->Socket == -1 )
		{
			auto *BoundSock = evhttp_bind_socket_with_handle( EvHttp.get(), ptr->SrvAddress.c_str(), ptr->SrvPort );

			if( !BoundSock )
				throw std::runtime_error( "Failed to bind server socket." );

			if( ( ptr->Socket = evhttp_bound_socket_get_fd( BoundSock ) ) == -1 )
				throw std::runtime_error( "Failed to get server socket for next instance." );
		}
		else
		{
			if( evhttp_accept_socket( EvHttp.get(), ptr->Socket ) == -1 )
				throw std::runtime_error( "Failed to bind server socket for new instance." );
		}

		// create Matcher

		ptr->Matchers[ this_thread::get_id()] = ptr->unifinger->createMatcher();
		cout << "Thread started: " << this_thread::get_id() << endl;

		for( ; ptr->IsRun ; )
		{
			event_base_loop( EventBase.get(), EVLOOP_NONBLOCK );
			std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
		}

		// exiting thread
		ptr->unifinger->deleteMatcher( ptr->Matchers[ this_thread::get_id()] );
	}

	catch( ... )
	{
		ptr->InitExcept = std::current_exception();
	}
};

void HTTPD::runThreads( int t_count )
{
	try
	{
		auto ThreadDeleter = [&]( std::thread *t )
		{
			cout << "Thread deleter" << endl;
			IsRun = false;
			t->join();
			delete t;
		};

		typedef std::unique_ptr<std::thread, decltype( ThreadDeleter ) > ThreadPtr;
		typedef std::vector<ThreadPtr> ThreadPool;
		ThreadPool Threads;

		for( int i = 0 ; i < t_count ; ++i )
		{
			ThreadPtr Thread( new std::thread( threadHandler, this ), ThreadDeleter );
			std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );

			if( InitExcept != std::exception_ptr() )
			{
				IsRun = false;
				std::rethrow_exception( InitExcept );
			}

			Threads.push_back( std::move( Thread ) );
		}

		while( 1==1 )
		{
			std::this_thread::sleep_for( std::chrono::seconds( 10 ) );
		}

//		std::cout << "Press Enter for quit." << endl;
//		std::cin.get();
//		IsRun = false;
	}

	catch( std::exception const &e )
	{
		cerr << "Error: " << e.what() << endl;
	}
}
