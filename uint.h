#ifndef CSL_UINT_H
#define CSL_UINT_H

#include <gmp.h>
#include <type_traits>

namespace csl {

template<unsigned int x>
class basic_uint;

template<unsigned int x>
mp_bitcnt_t __builtin_popcount(const basic_uint<x> &);

template<unsigned int x>
class basic_uint {
    //friendship with all basic_uint
    template<unsigned int y> friend
    class basic_uint;

    typedef std::size_t size_t;
    typedef unsigned int iint;
    typedef mp_limb_t word_t;
    static constexpr size_t n_words = (2 << x);
    word_t data[n_words]; // little endian
    static word_t buffer_data[2*n_words];
    static constexpr size_t n_bytes = n_words * sizeof(word_t);
public:
    static constexpr size_t n_bits = n_words * sizeof(word_t) * 8;

    //constructors
    basic_uint(); // empty constructor

    template<class UintT>
    basic_uint(const UintT &); // primitive, copy constructor

    basic_uint(const basic_uint &); //basic_uint, copy constructor

    template<unsigned int y, typename std::enable_if<y < x>::type * = nullptr>
    basic_uint(const basic_uint<y> &); //smaller basic_uint, widening copy constructor
    template<unsigned int y, typename std::enable_if<x < y>::type * = nullptr>
    explicit basic_uint(const basic_uint<y> &); //smaller basic_uint, narrowing copy constructor

    //arithmetic operators

    template<class UintT>
    basic_uint& operator+=(const UintT&);
    basic_uint& operator+=(const basic_uint&);
    template<unsigned int y, typename std::enable_if<y < x>::type * = nullptr>
    basic_uint& operator+=(const basic_uint<y> &); //smaller basic_uint, widening
    template<unsigned int y, typename std::enable_if<x < y>::type * = nullptr>
    basic_uint& operator+=(const basic_uint<y> &); //bigger basic_uint, narrowing

    template<class UintT>
    basic_uint& operator-=(const UintT&);
    basic_uint& operator-=(const basic_uint&);
    template<unsigned int y, typename std::enable_if<y < x>::type * = nullptr>
    basic_uint& operator-=(const basic_uint<y> &); //smaller basic_uint, widening
    template<unsigned int y, typename std::enable_if<x < y>::type * = nullptr>
    basic_uint& operator-=(const basic_uint<y> &); //bigger basic_uint, narrowing

    template<class UintT>
    basic_uint& operator*=(const UintT&);
    basic_uint& operator*=(const basic_uint&);
    template<unsigned int y, typename std::enable_if<y < x>::type * = nullptr>
    basic_uint& operator*=(const basic_uint<y> &); //smaller basic_uint, widening
    template<unsigned int y, typename std::enable_if<x < y>::type * = nullptr>
    basic_uint& operator*=(const basic_uint<y> &); //bigger basic_uint, narrowing

    /*
    template<class UintT>
    basic_uint& operator/=(const UintT&);
    basic_uint& operator/=(const basic_uint&);
    template<unsigned int y, typename std::enable_if<y < x>::type * = nullptr>
    basic_uint& operator/=(const basic_uint<y> &); //smaller basic_uint, widening
    template<unsigned int y, typename std::enable_if<x < y>::type * = nullptr>
    basic_uint& operator/=(const basic_uint<y> &); //bigger basic_uint, narrowing
    */

    /*
    template<class UintT>
    basic_uint& operator%=(const UintT&);
    basic_uint& operator%=(const basic_uint&);
    template<unsigned int y, typename std::enable_if<y < x>::type * = nullptr>
    basic_uint& operator%=(const basic_uint<y> &); //smaller basic_uint, widening
    template<unsigned int y, typename std::enable_if<x < y>::type * = nullptr>
    basic_uint& operator%=(const basic_uint<y> &); //bigger basic_uint, narrowing
    */

    /*
    template<class UintT>
    basic_uint& operator&=(const UintT&);
    basic_uint& operator&=(const basic_uint&);
    template<unsigned int y, typename std::enable_if<y < x>::type * = nullptr>
    basic_uint& operator&=(const basic_uint<y> &); //smaller basic_uint, widening
    template<unsigned int y, typename std::enable_if<x < y>::type * = nullptr>
    basic_uint& operator&=(const basic_uint<y> &); //bigger basic_uint, narrowing
    */

    /*
    template<class UintT>
    basic_uint& operator|=(const UintT&);
    basic_uint& operator|=(const basic_uint&);
    template<unsigned int y, typename std::enable_if<y < x>::type * = nullptr>
    basic_uint& operator|=(const basic_uint<y> &); //smaller basic_uint, widening
    template<unsigned int y, typename std::enable_if<x < y>::type * = nullptr>
    basic_uint& operator|=(const basic_uint<y> &); //bigger basic_uint, narrowing
    */


    /*
    template<class UintT>
    basic_uint& operator^=(const UintT&);
    basic_uint& operator^=(const basic_uint&);
    template<unsigned int y, typename std::enable_if<y < x>::type * = nullptr>
    basic_uint& operator^=(const basic_uint<y> &); //smaller basic_uint, widening
    template<unsigned int y, typename std::enable_if<x < y>::type * = nullptr>
    basic_uint& operator^=(const basic_uint<y> &); //bigger basic_uint, narrowing
    */

    /*
    basic_uint& operator<<=(int);
    basic_uint& operator>>=(int);
    */

    basic_uint& operator++(int);
    basic_uint& operator--(int);
    basic_uint& operator++();
    basic_uint& operator--();

    //friend iint



    //comparison operators
    template<class UintT>
    bool operator==(const UintT &) const; // primitive
    bool operator==(const basic_uint&) const; //basic_uint
    template<unsigned int y, typename std::enable_if<y < x>::type * = nullptr>
    bool operator==(const basic_uint<y> &) const; //smaller basic_uint, widening
    template<unsigned int y, typename std::enable_if<x < y>::type * = nullptr>
    bool operator==(const basic_uint<y> &) const; //bigger basic_uint, widening

    //destructor
    ~basic_uint() = default;

    //friend functions
    friend mp_bitcnt_t __builtin_popcount<x>(const basic_uint&);
};

template<unsigned int x>
basic_uint<x>::basic_uint() {
    memset(data, 0, n_bytes);
}

template<unsigned int x>
template<class UintT>
basic_uint<x>::basic_uint(const UintT &v) {
    static_assert(std::is_integral<UintT>::value, "Need an integral type!");
    data[0] = v;
    memset(data + 1, 0, n_bytes - sizeof(word_t));
}

template<unsigned int x>
basic_uint<x>::basic_uint(const basic_uint &v) {
    memcpy(data, v.data, n_bytes);
}

template<unsigned int x>
basic_uint<x> &basic_uint<x>::operator+=(const basic_uint &v) {
    mpn_add_n(data,data,v.data,n_words);
    return *this;
}

template<unsigned int x>
template<class UintT>
basic_uint<x> &basic_uint<x>::operator+=(const UintT &v) {
    static_assert(std::is_integral<UintT>::value, "Need an integral type!");
    static_assert(std::is_unsigned<UintT>::value, "Need an unsigned type!");
    mpn_add_1(data,data,n_words,v);
    return *this;
}

template<unsigned int x>
template<unsigned int y, typename std::enable_if<y < x>::type *>
basic_uint<x> &basic_uint<x>::operator+=(const basic_uint<y> &v) {
    mpn_add(data,data,n_words,v.data,basic_uint<y>::n_words);
    return *this;
}

template<unsigned int x>
template<unsigned int y, typename std::enable_if<x < y>::type *>
basic_uint<x> &basic_uint<x>::operator+=(const basic_uint<y> &v) {
    mpn_add_n(data,data,v.data,n_words); //overflow
    return *this;
}


template<unsigned int x>
basic_uint<x> &basic_uint<x>::operator-=(const basic_uint &v) {
    mpn_sub_n(data,data,v.data,n_words);
    return *this;
}

template<unsigned int x>
template<class UintT>
basic_uint<x> &basic_uint<x>::operator-=(const UintT &v) {
    static_assert(std::is_integral<UintT>::value, "Need an integral type!");
    static_assert(std::is_unsigned<UintT>::value, "Need an unsigned type!");
    mpn_sub_1(data,data,n_words,v);
    return *this;
}

template<unsigned int x>
template<unsigned int y, typename std::enable_if<y < x>::type *>
basic_uint<x> &basic_uint<x>::operator-=(const basic_uint<y> &v) {
    mpn_sub(data,data,n_words,v.data,basic_uint<y>::n_words);
    return *this;
}

template<unsigned int x>
template<unsigned int y, typename std::enable_if<x < y>::type *>
basic_uint<x> &basic_uint<x>::operator-=(const basic_uint<y> &v) {
    mpn_sub_n(data,data,v.data,n_words); //overflow
    return *this;
}

template<unsigned int x>
basic_uint<x> &basic_uint<x>::operator*=(const basic_uint &v) {
    mpn_mul_n(buffer_data,data,v.data,n_words);
    memcpy(data,buffer_data,n_words);
    return *this;
}

template<unsigned int x>
template<class UintT>
basic_uint<x> &basic_uint<x>::operator*=(const UintT &v) {
    static_assert(std::is_integral<UintT>::value, "Need an integral type!");
    static_assert(std::is_unsigned<UintT>::value, "Need an unsigned type!");
    mpn_mul_1(data,data,n_words,v);
    return *this;
}

template<unsigned int x>
template<unsigned int y, typename std::enable_if<y < x>::type *>
basic_uint<x> &basic_uint<x>::operator*=(const basic_uint<y> &v) {
    mpn_mul(buffer_data,data,n_words,v.data,basic_uint<y>::n_words);
    memcpy(data,buffer_data,n_words);
    return *this;
}

template<unsigned int x>
template<unsigned int y, typename std::enable_if<x < y>::type *>
basic_uint<x> &basic_uint<x>::operator*=(const basic_uint<y> &v) {
    mpn_mul_n(buffer_data,data,v.data,n_words); //overflow
    memcpy(data,buffer_data,n_words);
    return *this;
}

template<unsigned int x>
template<class UintT>
bool basic_uint<x>::operator==(const UintT &v) const {
    static_assert(std::is_integral<UintT>::value, "Need an integral type!");
    static_assert(std::is_unsigned<UintT>::value, "Need an unsigned type!");
    if(data[0]!=v)return false;
    for(iint i=1;i<n_words;++i)if(data[i])return false;
    return true;
}

template<unsigned int x>
bool basic_uint<x>::operator==(const basic_uint &v) const {
    for(iint i=0;i<n_words;++i)if(data[i]!=v.data[i])return false;
    return true;
}

template<unsigned int x>
basic_uint<x> &basic_uint<x>::operator++() {
    ++(*data);
    iint i=0;
    while(data[i]==0){
        ++i;
        if(i==n_words)return *this;
        ++data[i];
    }
    return *this;
}

template<unsigned int x>
basic_uint<x> &basic_uint<x>::operator--() {
    iint i=0;
    while(data[i]==0){
        --data[i];
        ++i;
        if(i==n_words)return *this;
    }
    --data[i];
    return *this;
}

template<unsigned int x>
template<unsigned int y, typename std::enable_if<y < x>::type *>
bool basic_uint<x>::operator==(const basic_uint<y> &v) const {
    for(iint i=0;i<basic_uint<y>::n_words;++i)if(data[i]!=v.data[i])return false;
    for(iint i=basic_uint<y>::n_words;i<n_words;++i)if(data[i])return false;
    return true;
}

template<unsigned int x>
template<unsigned int y, typename std::enable_if<x < y>::type *>
bool basic_uint<x>::operator==(const basic_uint<y> &v) const {
    for(iint i=0;i<n_words;++i)if(data[i]!=v.data[i])return false;
    for(iint i=n_words;i<basic_uint<y>::n_words;++i)if(v.data[i])return false;
    return true;
}

template<unsigned int x>
template<unsigned int y, typename std::enable_if<y < x>::type *>
basic_uint<x>::basic_uint(const basic_uint<y> &v) {
    memcpy(data, v.data, basic_uint<y>::n_bytes);
    memset(data + basic_uint<y>::n_words, 0, n_bytes - basic_uint<y>::n_bytes);
}

template<unsigned int x>
template<unsigned int y, typename std::enable_if<x < y>::type *>
basic_uint<x>::basic_uint(const basic_uint<y> &v) {
    memcpy(data, v.data, n_bytes);
}

template<unsigned int x>
mp_bitcnt_t __builtin_popcount(const basic_uint<x> &v){
    return mpn_popcount(v.data, basic_uint<x>::n_words);
}

// useful type definitions
typedef basic_uint<0> uint128_t;
typedef basic_uint<1> uint256_t;
typedef basic_uint<2> uint512_t;
typedef basic_uint<3> uint1024_t;
typedef basic_uint<4> uint2048_t;
typedef basic_uint<5> uint4096_t;
typedef basic_uint<6> uint8192_t;
typedef basic_uint<7> uint16384_t;
typedef basic_uint<8> uint32768_t;
typedef basic_uint<9> uint65536_t;
typedef basic_uint<10> uint131072_t;
typedef basic_uint<11> uint262144_t;

}

#endif //CSL_UINT_H
