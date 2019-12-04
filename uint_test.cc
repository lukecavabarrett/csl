
#include "gtest/gtest.h"
#include "csl/uint.h"

namespace csl{
namespace {

TEST(Uint,BitsNumber){

    //128
    EXPECT_EQ(uint128_t::n_bits,128);
    EXPECT_EQ(sizeof(uint128_t),16);

    //512
    EXPECT_EQ(uint256_t::n_bits,256);
    EXPECT_EQ(sizeof(uint256_t),32);

    //1024
    EXPECT_EQ(uint512_t::n_bits,512);
    EXPECT_EQ(sizeof(uint512_t),64);

    //...

    //2048
    EXPECT_EQ(uint2048_t::n_bits,2048);
    EXPECT_EQ(sizeof(uint2048_t),256);
}

TEST(Uint,IntegralConstructor){
    int i = 54;
    uint256_t a(i);
    uint256_t b(54);
    uint1024_t c(54LL);
    uint512_t d = i;
    uint512_t e = 65476424754LL;
}

TEST(Uint,UintConstructor){
    uint128_t u128 = 674786LL;
    uint256_t u256a = u128;
    uint256_t u256b(u128);
    uint256_t u256c = u256a;
    uint256_t u256d(u256b);
    uint128_t u64a(u256a); //explicit narrowing, ok
    //uint128_t u64b = u256b; //implicit narrowing, compiler error
}

TEST(Uint,Addition){
    uint128_t a = 10101010LL, b = 1010101LL;
    a+=b;
    EXPECT_EQ(a,11111111ULL);
    a+=22UL;
    EXPECT_EQ(a,11111133ULL);
    uint256_t c = a;
    a+=c;
    c+=a;
    a+=11111133ULL;
    EXPECT_EQ(a,c);
}

TEST(Uint,Decrement){
    uint128_t a;
    EXPECT_EQ(__builtin_popcount(a),0);
    EXPECT_EQ(a,0ULL);
    --a;
    EXPECT_EQ(__builtin_popcount(a),128);
    ++a;
    EXPECT_EQ(__builtin_popcount(a),0);
    EXPECT_EQ(a,0ULL);

    EXPECT_EQ(__builtin_popcount(a--),0);
    EXPECT_EQ(__builtin_popcount(a++),128);
    EXPECT_EQ(__builtin_popcount(a),0);
}

TEST(Uint,Multiplication){
    uint128_t a = 0x1111LL;
    a*=0xFULL;
    EXPECT_EQ(a,0xFFFFULL);
    uint128_t b = 0x10ULL;
    a*=b;
    EXPECT_EQ(a,0xFFFF0ULL);
}

TEST(Uint,Division){
    uint128_t a = 0x12345LL, b = 0x10;
    uint128_t a_old = a;
    a*=b;
    a/=0x10ULL;
    EXPECT_EQ(a,a_old);
    a*=b;
    a/=b;
    EXPECT_EQ(a,a_old);
}

TEST(Uint,Division2){
    uint128_t a;
    constexpr uint64_t M = std::numeric_limits<uint64_t>::max();
    a.set(); // a = 2^128 -1
    uint128_t b = M;
    ++b; // b = 2^64
    EXPECT_EQ(__builtin_popcount(a),128);
    a/=b; // a = 2^64 - 1
    EXPECT_EQ(a,M);
}

TEST(Uint,Modulo2){
    uint128_t a;
    constexpr uint64_t M = std::numeric_limits<uint64_t>::max();
    a.set(); // a = 2^128 -1
    uint128_t b = M;
    ++b; // b = 2^64
    EXPECT_EQ(__builtin_popcount(a),128);
    a%=b; // a = 2^64 - 1
    EXPECT_EQ(a,M);
    a.set(); // a = 2^128 -1
    b = M;
    a%=b;
    EXPECT_EQ(a,0ULL);
}

}
}

