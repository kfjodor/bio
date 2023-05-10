#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include "UFMatcher.h"

#include "logger.h"

extern Logger logger;
using namespace std;

#define MAX_TEMPLATE_SIZE 384
#define MAX_TEMPLATE_NUM 1000000

class UFMatcher
{
	public:
		int createMatcher();
		int deleteMatcher();
		int compareTemplates( vector<unsigned char> t1, vector<unsigned char> t2 );		
		int identify( vector<unsigned char> t1 );

	//private:
		HUFMatcher hMatcher;
		
		vector< vector<unsigned char> > super_vector;
		
		void hex_to_char( string data, vector<unsigned char> &out );
		string processErrorString( int res );

};
