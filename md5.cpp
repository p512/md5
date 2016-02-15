#include "md5.hpp"

#include <vector>
#include <climits>
#include <cmath>
#include <cstring>
#include <iomanip>
#include <cassert>

std::ostream& operator<<(std::ostream& os, md5 const & obj) {
	os << std::hex << std::setfill('0') << std::setw(8) << obj.a;
	os << std::setw(8) << obj.b;
	os << std::setw(8) << obj.c;
	os << std::setw(8) << obj.d;
	os << std::dec;
	return os;
}

typedef ui32 (*helpfn)(ui32, ui32, ui32);

static ui32 F(ui32 x, ui32 y, ui32 z) {
	return (x&y) | (~x & z);
}

static ui32 G(ui32 x, ui32 y, ui32 z) {
	return (x & z) | (y & ~z);
}

static ui32 H(ui32 x, ui32 y, ui32 z) {
	return x^y^z;
}

static ui32 I(ui32 x, ui32 y, ui32 z) {
	return y ^ (x | ~z);
}

static ui32 rotleft(ui32 v, short n) {
	return (v >> (32-n)) | (v << n);
}

static void md5round(helpfn fn, ui32 &a, ui32 &b, ui32 &c, ui32 &d, ui32 k, ui32 s, ui32 i, ui32 *X, ui32 *T) {
	a = b + rotleft(a + fn(b,c,d) + X[k] + T[i], s);
}

static ui32 endian(ui32 x) {
	return  ((x >> 24 & 0xff) <<  0) | 
		((x >> 16 & 0xff) <<  8) | 
		((x >>  8 & 0xff) << 16) | 
		((x >>  0 & 0xff) << 24);
}

md5ctx::~md5ctx() {
	A = 0, B = 0, C = 0, D = 0;
	memset(buffer, 0, sizeof buffer);
	bufpos = 0;
	tlen = 0;
}

/*
 * runs the md5 algorithm
 * preconditions: 
 * 	buffer contains the data to hash
 * 	bufpos must be a multiple of 16
 * invariant: 
 * 	does not modify the buffer
 * postconditions:
 * 	A,B,C,D will be updated accordingly
 */
void md5ctx::transform() {
	auto const words = reinterpret_cast<ui32*>(buffer);
	ui32 T[65];
	for(short i = 0; i < 65; ++i) {
		//This actually works, although I would not expect
		//a lot of platform compatability
		T[i] = 4294967296l * std::abs(std::sin(i));
	}
	assert(bufpos % (16*WORD_BYTES) == 0); //A single 16-word block must have been filled
	for(ui64 i = 0; i < bufpos/WORD_BYTES/MD5_BLOCK_WORDS; ++i) {
		ui32 X[16];
		for(short j = 0; j < 16; ++j) {
			X[j] = words[i*16+j];
		}
		ui32 AA = A, BB = B, CC = C, DD = D;
		{ //Round 1
			md5round(F, A, B, C, D,  0,  7,  1, X, T);
			md5round(F, D, A, B, C,  1, 12,  2, X, T);
			md5round(F, C, D, A, B,  2, 17,  3, X, T);
			md5round(F, B, C, D, A,  3, 22,  4, X, T);

			md5round(F, A, B, C, D,  4,  7,  5, X, T);
			md5round(F, D, A, B, C,  5, 12,  6, X, T);
			md5round(F, C, D, A, B,  6, 17,  7, X, T);
			md5round(F, B, C, D, A,  7, 22,  8, X, T);

			md5round(F, A, B, C, D,  8,  7,  9, X, T);
			md5round(F, D, A, B, C,  9, 12, 10, X, T);
			md5round(F, C, D, A, B, 10, 17, 11, X, T);
			md5round(F, B, C, D, A, 11, 22, 12, X, T);

			md5round(F, A, B, C, D, 12,  7, 13, X, T);
			md5round(F, D, A, B, C, 13, 12, 14, X, T);
			md5round(F, C, D, A, B, 14, 17, 15, X, T);
			md5round(F, B, C, D, A, 15, 22, 16, X, T);
		}
		{ //Round 2
			md5round(G, A, B, C, D,  1,  5, 17, X, T);
			md5round(G, D, A, B, C,  6,  9, 18, X, T);
			md5round(G, C, D, A, B, 11, 14, 19, X, T);
			md5round(G, B, C, D, A,  0, 20, 20, X, T);

			md5round(G, A, B, C, D,  5,  5, 21, X, T);
			md5round(G, D, A, B, C, 10,  9, 22, X, T);
			md5round(G, C, D, A, B, 15, 14, 23, X, T);
			md5round(G, B, C, D, A,  4, 20, 24, X, T);

			md5round(G, A, B, C, D,  9,  5, 25, X, T);
			md5round(G, D, A, B, C, 14,  9, 26, X, T);
			md5round(G, C, D, A, B,  3, 14, 27, X, T);
			md5round(G, B, C, D, A,  8, 20, 28, X, T);

			md5round(G, A, B, C, D, 13,  5, 29, X, T);
			md5round(G, D, A, B, C,  2,  9, 30, X, T);
			md5round(G, C, D, A, B,  7, 14, 31, X, T);
			md5round(G, B, C, D, A, 12, 20, 32, X, T);
		}
		{ //Round 3
			md5round(H, A, B, C, D,  5,  4, 33, X, T);
			md5round(H, D, A, B, C,  8, 11, 34, X, T);
			md5round(H, C, D, A, B, 11, 16, 35, X, T);
			md5round(H, B, C, D, A, 14, 23, 36, X, T);

			md5round(H, A, B, C, D,  1,  4, 37, X, T);
			md5round(H, D, A, B, C,  4, 11, 38, X, T);
			md5round(H, C, D, A, B,  7, 16, 39, X, T);
			md5round(H, B, C, D, A, 10, 23, 40, X, T);

			md5round(H, A, B, C, D, 13,  4, 41, X, T);
			md5round(H, D, A, B, C,  0, 11, 42, X, T);
			md5round(H, C, D, A, B,  3, 16, 43, X, T);
			md5round(H, B, C, D, A,  6, 23, 44, X, T);

			md5round(H, A, B, C, D,  9,  4, 45, X, T);
			md5round(H, D, A, B, C, 12, 11, 46, X, T);
			md5round(H, C, D, A, B, 15, 16, 47, X, T);
			md5round(H, B, C, D, A,  2, 23, 48, X, T);
		}
		{ //Round 4
			md5round(I, A, B, C, D,  0,  6, 49, X, T);
			md5round(I, D, A, B, C,  7, 10, 50, X, T);
			md5round(I, C, D, A, B, 14, 15, 51, X, T);
			md5round(I, B, C, D, A,  5, 21, 52, X, T);

			md5round(I, A, B, C, D, 12,  6, 53, X, T);
			md5round(I, D, A, B, C,  3, 10, 54, X, T);
			md5round(I, C, D, A, B, 10, 15, 55, X, T);
			md5round(I, B, C, D, A,  1, 21, 56, X, T);

			md5round(I, A, B, C, D,  8,  6, 57, X, T);
			md5round(I, D, A, B, C, 15, 10, 58, X, T);
			md5round(I, C, D, A, B,  6, 15, 59, X, T);
			md5round(I, B, C, D, A, 13, 21, 60, X, T);
			     
			md5round(I, A, B, C, D,  4,  6, 61, X, T);
			md5round(I, D, A, B, C, 11, 10, 62, X, T);
			md5round(I, C, D, A, B,  2, 15, 63, X, T);
			md5round(I, B, C, D, A,  9, 21, 64, X, T);
		}
		A += AA;
		B += BB;
		C += CC;
		D += DD;
	}
}

/*
 * Adds data to the hash
 * client-history-constraint: must not have called finalize
 */
void md5ctx::update(char const *data, ui64 len) {
	tlen += len; //Update total length
	ui64 bcount; //Bytes left in buffer for use
	while(len >= (bcount = MD5_BUF_SIZE-bufpos)) {
		memcpy(buffer, data, bcount); // Completely fill buffer
		bufpos += bcount;
		transform(); //Transform as many times as buffer can be filled 
		len -= bcount;
		data += bcount;
		bufpos = 0; //Full block processed, clear it
	}
	memcpy(buffer, data, len); //Fill buffer with remnants
	bufpos += len;
	assert(bufpos < MD5_BUF_SIZE);
}

/*
 * completes the computation and returns the final hash value
 * client-history-constraint: must not have been called before
 */
md5 md5ctx::finalize() {
	i64 padding = 56 - tlen % 64;
	if(padding <= 0) {
		padding += 64;
	}
	//cannot have 0 space since we would have a complete block
	//that could have been processed
	assert(MD5_BUF_SIZE - bufpos > 0);
	buffer[bufpos++] = 0x80; //single set bit and 7 zeros
	if(MD5_BUF_SIZE - bufpos < 8) { //cannot fit padding + length
		memset(buffer+bufpos, 0, MD5_BUF_SIZE-bufpos);
		padding -= MD5_BUF_SIZE-bufpos;
		bufpos = MD5_BUF_SIZE;
		transform();
		bufpos = 0;
	}
	memset(buffer+bufpos, 0, --padding);
	bufpos += padding + 8;

	auto const words = reinterpret_cast<ui32*>(buffer);
	words[bufpos/WORD_BYTES-2] = tlen*8; //tlen<<3
	words[bufpos/WORD_BYTES-1] = tlen*8>>32; //tlen>>29

	transform(); //final transform
	return md5(endian(A), endian(B), endian(C), endian(D));
}
