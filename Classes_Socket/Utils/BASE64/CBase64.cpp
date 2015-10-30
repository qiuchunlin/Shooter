#include "CBase64.h"

const std::string CBase64::encode64( std::string & input )
{
	const std::string alphabet64( "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/," );
	const char pad = '%';
	const char np  = (char)std::string::npos;
	char table64vals[] =
	{
		62, np, np, np, 63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, np, np, np, np, np,
		np, np,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17,
		18, 19, 20, 21, 22, 23, 24, 25, np, np, np, np, np, np, 26, 27, 28, 29, 30, 31,
		32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51
	};

	std::string encoded;
	char c;
	const std::string::size_type length = input.length();

	encoded.reserve( length * 2 );

	for( std::string::size_type i = 0; i < length; ++i )
	{
		c = static_cast<char>( ( input[i] >> 2 ) & 0x3f );
		encoded += alphabet64[c];

		c = static_cast<char>( ( input[i] << 4 ) & 0x3f );
		if( ++i < length )
			c = static_cast<char>( c | static_cast<char>( ( input[i] >> 4 ) & 0x0f ) );
		encoded += alphabet64[c];

		if( i < length )
		{
			c = static_cast<char>( ( input[i] << 2 ) & 0x3c );
			if( ++i < length )
				c = static_cast<char>( c | static_cast<char>( ( input[i] >> 6 ) & 0x03 ) );
			encoded += alphabet64[c];
		}
		else
		{
			++i;
			encoded += pad;
		}

		if( i < length )
		{
			c = static_cast<char>( input[i] & 0x3f );
			encoded += alphabet64[c];
		}
		else
		{
			encoded += pad;
		}
	}

	encoded.insert(5,"H");

	return encoded;
}

std::string CBase64::decode64( const std::string & inputStr )
{
	string input=inputStr.substr(0,5);
	input.append(inputStr.substr(6));

	const std::string alphabet64( "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/," );
	const char pad = '%';
	const char np  = (char)std::string::npos;
	char table64vals[] =
	{
		62, np, np, np, 63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, np, np, np, np, np,
		np, np,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17,
		18, 19, 20, 21, 22, 23, 24, 25, np, np, np, np, np, np, 26, 27, 28, 29, 30, 31,
		32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51
	};

	char c, d;
	const std::string::size_type length = input.length();
	std::string decoded;

	decoded.reserve( length );

	for( std::string::size_type i = 0; i < length; ++i )
	{
		unsigned char cTmp=input[i];
		c = ( cTmp < 43 || cTmp > 122 ) ? np : table64vals[cTmp-43];
		++i;
		cTmp=input[i];
		d = ( cTmp < 43 || cTmp > 122 ) ? np : table64vals[cTmp-43];
		c = static_cast<char>( ( c << 2 ) | ( ( d >> 4 ) & 0x3 ) );
		decoded += c;
		if( ++i < length )
		{
			c = input[i];
			if( pad == c )
				break;
			cTmp=input[i];
			c = ( cTmp < 43 || cTmp > 122 ) ? np : table64vals[cTmp-43];
			d = static_cast<char>( ( ( d << 4 ) & 0xf0 ) | ( ( c >> 2 ) & 0xf ) );
			decoded += d;
		}

		if( ++i < length )
		{
			d = input[i];
			if( pad == d )
				break;
			cTmp=input[i];
			d = ( cTmp < 43 || cTmp > 122 ) ? np : table64vals[cTmp-43];
			c = static_cast<char>( ( ( c << 6 ) & 0xc0 ) | d );
			decoded += c;
		}
	}

	return decoded;
}