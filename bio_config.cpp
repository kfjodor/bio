#include "bio_config.h"

void Config::restore()
{
	if( !in.is_open() )
	{
		throw runtime_error( "Config: Error opening config file" );
	}

	if( !reader.parse( in, root ) )
	{
		throw runtime_error( "Config: Error parsing config file" );
	}

	if( !root.isObject() )
	{
		throw runtime_error( "Config: Object check fail config file" );
	}

	threads_count = root.get( "threads_count", 0 ).asInt();
	server_port = root.get( "server_port", 0 ).asInt();
	server_address = root.get( "server_address", "" ).asString();
	project_id = root.get( "project_id", -1 ).asInt();

	mysql_address = root.get( "mysql_address", "" ).asString();
	mysql_login = root.get( "mysql_login", "" ).asString();
	mysql_password = root.get( "mysql_password", "" ).asString();
	mysql_database = root.get( "mysql_database", "" ).asString();

	if( project_id == -1 )
		throw runtime_error( "Config: Wrong project ID" );

	if( !threads_count )
		throw runtime_error( "Config: Wrong threads number" );

	if( !server_port )
		throw runtime_error( "Config: Wrong server port" );

	if( !server_address.length() )
		throw runtime_error( "Config: Wrong server address" );

	if( !mysql_address.length() )
		throw runtime_error( "Config: Wrong mysql address" );

	if( !mysql_login.length() )
		throw runtime_error( "Config: Wrong mysql login" );

	if( !mysql_password.length() )
		throw runtime_error( "Config: Wrong mysql password" );

	if( !mysql_database.length() )
		throw runtime_error( "Config: Wrong mysql database" );
}
