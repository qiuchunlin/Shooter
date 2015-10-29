#ifndef _UTILS_BASE64_CBASE64_
#define _UTILS_BASE64_CBASE64_
#include <string>
using namespace std;

class CBase64
{

public:
	static const std::string encode64( std::string & input );

	static std::string decode64(const std::string & input );

};
#endif