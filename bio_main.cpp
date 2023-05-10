#include "bio_httpd.h"
#include "bio_unifinger.h"
#include "bio_config.h"

int main()
{
	try
	{
		Config config;
		Unifinger unifinger( config.getMysqlAddress(),
		                     config.getMysqlLogin(),
		                     config.getMysqlPassword(),
		                     config.getMysqlDatabase(),
		                     config.getProjectID()
		                   );

		HTTPD http( &unifinger, config.getServerAddress(), config.getServerPort() );
		http.runThreads( config.getThreadsCount() );
	}

	catch( const exception &ex )
	{
		cerr << "Error - " << ex.what() << endl;
	}

	return 0;
}
