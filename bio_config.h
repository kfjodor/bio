#ifndef _FCONFIG
#define _FCONFIG

#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>

#include "json/value.h"
#include "json/writer.h"
#include "json/reader.h"

#define CONFIGFILE "./bio.config"

using namespace std;

class Config
{
	public:
		Config() : threads_count( 0 ), server_port( 0 )
		{
			in.open( CONFIGFILE );
			restore();
		}

		~Config()
		{
			in.close();
		}

		void restore();

		int getServerPort()
		{
			return server_port;
		}

		string getServerAddress()
		{
			return server_address;
		}

		int getThreadsCount()
		{
			return threads_count;
		}

		string getMysqlAddress()
		{
			return mysql_address;
		}

		string getMysqlLogin()
		{
			return mysql_login;
		}

		string getMysqlPassword()
		{
			return mysql_password;
		}

		string getMysqlDatabase()
		{
			return mysql_database;
		}

		int getProjectID()
		{
			return project_id;
		}

	private:
		ifstream in;
		string server_address;
		int threads_count;
		int server_port;
		int project_id;

		string mysql_address;
		string mysql_login;
		string mysql_password;
		string mysql_database;

		Json::Reader reader;
		Json::Value root;
};



#endif
