#pragma once
#define MD5_DIGEST_LENGTH 16
typedef unsigned int MD5_u32plus;

typedef struct
{
	MD5_u32plus lo, hi;
	MD5_u32plus a, b, c, d;
	unsigned char buffer[64];
	MD5_u32plus block[16];
} MD5_CTX;

extern unsigned char* MD5(const unsigned char* d, size_t n, unsigned char* md);
extern void MD5_Init(MD5_CTX* ctx);
extern void MD5_Update(MD5_CTX* ctx, const void* data, unsigned long size);
extern void MD5_Final(unsigned char* result, MD5_CTX* ctx);
