#ifndef _BIO_HTTPD
#define _BIO_HTTPD

#include <stdexcept>
#include <iostream>
#include <memory>
#include <chrono>
#include <string>
#include <thread>
#include <cstdint>
#include <vector>
#include <evhttp.h>

#include <string.h>

#include <json/value.h>
#include <json/writer.h>
#include <json/reader.h>

#include "bio_unifinger.h"

using namespace std;

int const SrvThreadCount = 4;

class HTTPD
{
	public:
		HTTPD( Unifinger *ptr, string address, int port )
		{
			unifinger = ptr;
			SrvPort = port;
			SrvAddress = address;
			IsRun = true;
			Socket = -1;
		};

		void runThreads( int t_count );

	private:
		Unifinger *unifinger;

		string SrvAddress;
		uint16_t SrvPort;
		exception_ptr InitExcept;
		bool volatile IsRun;
		evutil_socket_t Socket;
		
		map<std::thread::id,HUFMatcher> Matchers;

		string HTTPParser( string uri, string req );

		static void threadHandler( HTTPD *ptr );
		static void HTTPRequestHandler( evhttp_request *req, void * );

		Json::Value parse_v1_search( Json::Value root );
		Json::Value parse_v1_add( Json::Value root );
		Json::Value parse_v1_delete( Json::Value root );
};

#endif
