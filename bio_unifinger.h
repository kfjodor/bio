#ifndef _BIO_UNIFINGER
#define _BIO_UNIFINGER

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <stdexcept>
#include <atomic>
#include <mutex>
#include <chrono>
#include <thread>
#include "UFMatcher.h"

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using namespace std;

class bio_exception : public std::exception
{
	public:
		explicit bio_exception( int id, const char* message ): msg_( message ), id_(id) {}
		explicit bio_exception( int id, const string &message ): msg_( message ), id_(id) {}
		
		virtual ~bio_exception() throw() {}
		
		virtual const char *what() const throw()
		{
			return msg_.c_str();
		}
		
		virtual int getErrorCode() const throw()
		{
			return id_;
		}

	private:
		string msg_;
		int id_;
};


class Unifinger
{
	public:
		Unifinger( string mysql_address, string mysql_login, string mysql_password, string mysql_database, int project_id )
		{
			atomic_counter = 0;
			loadDB( mysql_address, mysql_login, mysql_password, mysql_database, project_id );
		}

		~Unifinger()
		{
		}

		int identify( HUFMatcher, vector<unsigned char> template_to_match );
		int identify_sequence( HUFMatcher matcher, vector<unsigned char> t1 ); // data to identify
		void hex_to_char( string data, vector<unsigned char> &out );
		void DBAddFinger( int id, vector<unsigned char> vtemp );
		void DBDelFinger( int id );
		void rematchMap( int seq, int id );
		void checkExistence( int id );

		HUFMatcher createMatcher();
		void deleteMatcher( HUFMatcher );

	private:
		// Locks
		mutex data_mutex;
		mutex map_mutex;
		atomic<int> atomic_counter;

		// Finger database
		vector<unsigned char*> vFinger; // array of pointers, used for identify
		vector<int> vFingerSize; // array of fingerprints sizes
		vector< vector<unsigned char> > vItems; // array of fingerprints data
		vector<int> usersIdx; // users index - match sequence in vFinger and user ID 

		void loadDB( string, string, string, string, int );
		string processErrorString( int res );
};

#endif
