#include "UFMatcher.h"
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>

using namespace std;

void hex_to_char( string data, vector<unsigned char> &out )
{
	std::istringstream hex_chars_stream( data );

	unsigned int c;

	while( hex_chars_stream >> std::hex >> c )
	{
		out.push_back( c );
	}
}

string processErrorString( int res )
{
	string out;
	char m_StrError[128] = {0};

	if( UFM_GetErrorString( res, reinterpret_cast<char*>( &m_StrError ) ) == UFM_OK )
		out.assign( m_StrError );

	return out;
}


int main()
{
	UFM_STATUS ufm_res;
	HUFMatcher hMatcher;
	UFM_Create( &hMatcher );

// Finger database
	vector<unsigned char*> vFinger;
	vector<int> vFingerSize;
	vector< vector<unsigned char> > vItems;

	vector<unsigned char> btempl1;
	vector<unsigned char> btempl2;
	string stempl1( "44 3B 0F 11 9A 4A 55 46 47 00 4F 09 43 01 CE 8F 17 01 B5 0C 53 01 4F 87 1C 02 B9 08 37 06 56 10 5D 08 C3 8F 24 0B BA 81 3E 0C D7 0D 4E 0F C6 05 26 10 42 04 10 10 40 82 35 10 3E 14 32 14 C7 09 65 15 B2 09 3D 19 4D 11 1B 1C BF 07 2C 1D 4A 86 3A 1E 44 0D 30 1F C1 0C 48 23 D3 8B 43 24 42 09 1B 26 D0 86 3B 26 C3 06 35 26 C1 10 13 29 C9 03 09 2B D2 09 4E 2B BF 8B 3B 2E CD 39 57 2F B7 89 3E 33 B2 E9 77 35 A8 08 2A 39 5D 8B 6D 39 B0 87 41 39 9D 9A 22 3F D7 87 55 42 A6 0B 3F 43 86 11 3B 44 EA 16 29 44 5B 0A 54 50 9D 91 3D 51 78 0C 20 56 DD 08 6A 57 A5 87 40 5E 78 0D 52 5E 06 0D 63 65 9D 89 75 67 9C 04 36 68 E8 86 4B 6A 7D 0C 2D 6C 6A 09 04 6D 5A 11 4A 72 FB 07 22 7A 70 86 2E 7C E5 83 4E 7E 7F 07 58 7E 00 08 31 80 6E 03 5B 85 04 09 FF F7 FF F6 67 7F FF FF F8 87 65 66 77 7F FF FF 77 77 66 67 77 FF FF 57 77 77 76 77 7F FF 56 56 67 77 77 88 88 55 66 66 66 88 89 99 AA 55 66 66 5A AA A9 9A A5 55 55 53 0D CB AA AA 45 55 54 31 ED CB AA A4 44 54 33 10 ED CB AA 44 44 43 31 0E EC BB B4 44 44 33 11 0E DB BB 55 44 33 31 10 ED CC B5 44 43 32 21 0E DC CC 44 33 33 22 10 0E CC C4 43 33 32 21 00 ED CC 33 33 32 21 11 0E DC C0 00 00 00 00 00 00 00 00 00 00 00 00" );

	// this one is corrupted
	string stempl2( "01 B5 0C 53 01 4F 87 1C 02 B9 08 37 06 56 10 5D 08 C3 8F 24 0B BA 81 3E 0C D7 0D 4E 0F C6 05 26 10 42 04 10 10 40 82 35 10 3E 14 32 14 C7 09 65 15 B2 09 3D 19 4D 11 1B 1C BF 07 2C 1D 4A 86 3A 1E 44 0D 30 1F C1 0C 48 23 D3 8B 43 24 42 09 1B 26 D0 86 3B 26 C3 06 35 26 C1 10 13 29 C9 03 09 2B D2 09 4E 2B BF 8B 3B 2E CD 39 57 2F B7 89 3E 33 B2 E9 77 35 A8 08 2A 39 5D 8B 6D 39 B0 87 41 39 9D 9A 22 3F D7 87 55 42 A6 0B 3F 43 86 11 3B 44 EA 16 29 44 5B 0A 54 50 9D 91 3D 51 78 0C 20 56 DD 08 6A 57 A5 87 40 5E 78 0D 52 5E 06 0D 63 65 9D 89 75 67 9C 04 36 68 E8 86 4B 6A 7D 0C 2D 6C 6A 09 04 6D 5A 11 4A 72 FB 07 22 7A 70 86 2E 7C E5 83 4E 7E 7F 07 58 7E 00 08 31 80 6E 03 5B 85 04 09 FF F7 FF F6 67 7F FF FF F8 87 65 66 77 7F FF FF 77 77 66 67 77 FF FF 57 77 77 76 77 7F FF 56 56 67 77 77 88 88 55 66 66 66 88 89 99 AA 55 66 66 5A AA A9 9A A5 55 55 53 0D CB AA AA 45 55 54 31 ED CB AA A4 44 54 33 10 ED CB AA 44 44 43 31 0E EC BB B4 44 44 33 11 0E DB BB 55 44 33 31 10 ED CC B5 44 43 32 21 0E DC CC 44 33 33 22 10 0E CC C4 40 33 32 21 00 ED CC 33 33 32 21 11 0E DC C0 00 00 00 00 00 00 00 00 00 00 00 00" );
	hex_to_char( stempl1, btempl1 );
	hex_to_char( stempl2, btempl2 );

	vItems.push_back( btempl1 );
	vFinger.push_back( vItems.back().data() );
	vFingerSize.push_back( vItems.back().size() );

	// Identify
	// 0 - OK
	// -102 - LICENSE fail
	int nMatchIndex = -1;

	printf( "start match\n" );
	ufm_res = UFM_Identify( hMatcher, btempl1.data(), btempl1.size(), vFinger.data(), vFingerSize.data(), vFinger.size(), 5000, &nMatchIndex );
	cout << ufm_res << endl;

	return 1;
}
