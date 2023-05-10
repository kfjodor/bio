#include <stdlib.h>
#include <iostream>

#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using namespace std;

int main( void )
{
	sql::Driver *driver;
	sql::Connection *con;
	sql::Statement *stmt;

	driver = get_driver_instance();
	con = driver->connect( "tcp://127.0.0.1:3306", "root", "19spray84" );

	stmt = con->createStatement();
	stmt->execute( "USE user_fingerprint" );

	string qqq( "INSERT INTO FINGERS(ID) VALUES (NULL)" );

	for( int i=0; i<999; i++ )
		qqq.append( ",(NULL)" );

	stmt->execute( qqq );

	//stmt->execute( "DROP TABLE IF EXISTS test" );
	//stmt->execute( "CREATE TABLE test(id INT, label CHAR(1))" );
	//for( int i=0; i<1000000; i++ )
	{
		//stmt->execute( "INSERT INTO FINGERS(TEMPLATE) VALUES ('aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa')" );
		//stmt->execute( "INSERT INTO FINGERS(ID) VALUES (NULL)" );
		//INSERT INTO FINGERS( ID ) VALUES( NULL ), ( NULL )
	}

	delete stmt;
	delete con;
}
