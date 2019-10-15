
#include "gtest/gtest.h"
#include "csl/uint.h"

namespace csl{
namespace {

TEST(Uint,BitsNumber){
    EXPECT_EQ(uint128_t::n_bits,128);
    EXPECT_EQ(uint256_t::n_bits,256);
    EXPECT_EQ(uint512_t::n_bits,512);
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

}
}

