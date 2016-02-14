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

md5 hashmd5(char const *data, ui64 const len);

#endif //MD5_HPP_INCLUDED
