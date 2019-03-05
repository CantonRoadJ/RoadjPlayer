#ifndef UTILS_H
#define UTILS_H

#include<string.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))
typedef unsigned char UINT8;
typedef unsigned long long int UINT64;
typedef unsigned long long int uint64_t1;
typedef UINT64 tKeccakLane;

#define ROL64(a, offset) ((((UINT64)a) << offset) ^ (((UINT64)a) >> (64-offset)))
#define i(x, y) ((x)+5*(y))
#define readLane(x, y)          load64((UINT8*)state+sizeof(tKeccakLane)*i(x, y))
#define writeLane(x, y, lane)   store64((UINT8*)state+sizeof(tKeccakLane)*i(x, y), lane)
#define XORLane(x, y, lane)     xor64((UINT8*)state+sizeof(tKeccakLane)*i(x, y), lane)

void KeccakF1600_StatePermute(uint64_t1 * state);
void Keccak(unsigned int rate, unsigned int capacity, const unsigned char *input, unsigned long long int inputByteLen, unsigned char delimitedSuffix, unsigned char *output, unsigned long long int outputByteLen);
void FIPS202_SHA3_512(const unsigned char *input, unsigned int inputByteLen, unsigned char *output);
int LFSR86540(UINT8 *LFSR);

#define NROUNDS 24
#define ROL(a, offset) ((a << offset) ^ (a >> (64-offset)))
static const uint64_t1 KeccakF_RoundConstants[NROUNDS] =
{
    (uint64_t1)0x0000000000000001ULL,
    (uint64_t1)0x0000000000008082ULL,
    (uint64_t1)0x800000000000808aULL,
    (uint64_t1)0x8000000080008000ULL,
    (uint64_t1)0x000000000000808bULL,
    (uint64_t1)0x0000000080000001ULL,
    (uint64_t1)0x8000000080008081ULL,
    (uint64_t1)0x8000000000008009ULL,
    (uint64_t1)0x000000000000008aULL,
    (uint64_t1)0x0000000000000088ULL,
    (uint64_t1)0x0000000080008009ULL,
    (uint64_t1)0x000000008000000aULL,
    (uint64_t1)0x000000008000808bULL,
    (uint64_t1)0x800000000000008bULL,
    (uint64_t1)0x8000000000008089ULL,
    (uint64_t1)0x8000000000008003ULL,
    (uint64_t1)0x8000000000008002ULL,
    (uint64_t1)0x8000000000000080ULL,
    (uint64_t1)0x000000000000800aULL,
    (uint64_t1)0x800000008000000aULL,
    (uint64_t1)0x8000000080008081ULL,
    (uint64_t1)0x8000000000008080ULL,
    (uint64_t1)0x0000000080000001ULL,
    (uint64_t1)0x8000000080008008ULL
};

#endif // UTILS_H
