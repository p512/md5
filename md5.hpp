#ifndef MD5_HPP_INCLUDED
#define MD5_HPP_INCLUDED

#include <cstdint>
#include <iostream>

typedef int32_t i32;
typedef int64_t i64;
typedef uint32_t ui32;
typedef uint64_t ui64;

struct md5 {
	ui32 a = 0, b = 0, c = 0, d = 0;
	md5() {}
	md5(ui32 a, ui32 b, ui32 c, ui32 d) : a(a), b(b), c(c), d(d) {} 
private:
	friend std::ostream& operator<<(std::ostream& os, md5 const & obj);
};

#define WORD_BYTES (4)
#define MD5_BLOCK_WORDS (16)
#define MD5_BUF_BLOCKS (1<<10) //freely adjustable
#define MD5_BUF_WORDS (MD5_BUF_BLOCKS * MD5_BLOCK_WORDS)
#define MD5_BUF_SIZE (MD5_BUF_WORDS * WORD_BYTES)

class md5ctx {
	ui32 A = 0x67452301, B = 0xefcdab89, C = 0x98badcfe, D = 0x10325476;
	unsigned char buffer[MD5_BUF_SIZE];
	ui32 bufpos = 0; //position in bytes in buffer
	ui64 tlen = 0; //total length in bytes

	void transform();
public:
	md5ctx() {}
	~md5ctx();
	void update(char const *data, ui64 len);
	md5 finalize();
};

#endif //MD5_HPP_INCLUDED
