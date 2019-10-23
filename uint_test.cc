
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

    //262144
    EXPECT_EQ(uint262144_t::n_bits,262144);
    EXPECT_EQ(sizeof(uint262144_t),32768);
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
}

}
}

