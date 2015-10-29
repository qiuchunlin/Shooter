#ifndef _UTILS_BASE64_CHECKMD5_
#define _UTILS_BASE64_CHECKMD5_
#include <assert.h>
#include <memory.h>
#include <stdio.h>
#include <string.h>
//#pragma package(smart_init)
//#pragma resource "*.dfm"

typedef unsigned       int uint4;
typedef unsigned short int uint2;
typedef unsigned      char uchar;

inline	uint4	rotate_left(uint4 x, uint4 n)
{ return ((x << n) | (x >> (32-n))); }

inline	uint4	F(uint4 x, uint4 y, uint4 z)
{ return ((x & y) | (~x & z)); }

inline  uint4	G(uint4 x, uint4 y, uint4 z)
{ return ((x & z) | (y & ~z)); }

inline  uint4	H(uint4 x, uint4 y, uint4 z)
{ return (x ^ y ^ z); }

inline  uint4	I(uint4 x, uint4 y, uint4 z)
{ return (y ^ (x | ~z)); }

inline	void	FF(uint4& a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac)
{ a += F(b, c, d) + x + ac; a = rotate_left(a, s); a += b; }

inline	void	GG(uint4& a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac)
{ a += G(b, c, d) + x + ac; a = rotate_left(a, s); a += b; }

inline	void	HH(uint4& a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac)
{ a += H(b, c, d) + x + ac; a = rotate_left(a, s); a += b; }

inline	void	II(uint4& a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac)
{ a += I(b, c, d) + x + ac; a = rotate_left(a, s); a += b; }

static unsigned char PADDING[64] =
{
	0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21


class MD5
{
public:
	MD5();
	~MD5();

	char* PrintMD5(uchar md5Digest[16]);
	char* MD5String(const char* szString);
	
protected:
	void	Init();
	void	Update(uchar* chInput, uint4 nInputLen);
	void	Finalize();
	uchar*	Digest() { return m_Digest; }
	void	Transform(uchar* block);
	void	Encode(uchar* dest, uint4* src, uint4 nLength);
	void	Decode(uint4* dest, uchar* src, uint4 nLength);

private:
	uint4	   m_State[4];
	uint4	   m_Count[2];
	uchar	   m_Buffer[64];
	uchar	   m_Digest[16];
	uchar	   m_Finalized;

};
#endif