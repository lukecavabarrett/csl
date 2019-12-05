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
    static constexpr size_t n_words = x;
    word_t data[n_words]; // little endian
    static word_t buffer_data[2 * n_words];
    static constexpr size_t n_bytes = n_words * sizeof(word_t);
public:
    static constexpr size_t n_bits = n_words * sizeof(word_t) * 8;

public:

    inline void reset();
    inline void reset(size_t b);
    inline void set();

public:

    //constructors
    basic_uint(); // empty constructor

    basic_uint(const word_t); // primitive, copy constructor

    basic_uint(const basic_uint &); //basic_uint, copy constructor

    template<unsigned int y, typename std::enable_if<y < x>::type * = nullptr>
    basic_uint(const basic_uint<y> &); //smaller basic_uint, widening copy constructor
    template<unsigned int y, typename std::enable_if<x < y>::type * = nullptr>
    explicit basic_uint(const basic_uint<y> &); //smaller basic_uint, narrowing copy constructor

    //arithmetic operators

    basic_uint &operator+=(const word_t);
    basic_uint &operator+=(const basic_uint &);
    template<unsigned int y, typename std::enable_if<y < x>::type * = nullptr>
    basic_uint &operator+=(const basic_uint<y> &); //smaller basic_uint, widening
    template<unsigned int y, typename std::enable_if<x < y>::type * = nullptr>
    basic_uint &operator+=(const basic_uint<y> &); //bigger basic_uint, narrowing

    basic_uint &operator-=(const word_t);
    basic_uint &operator-=(const basic_uint &);
    template<unsigned int y, typename std::enable_if<y < x>::type * = nullptr>
    basic_uint &operator-=(const basic_uint<y> &); //smaller basic_uint, widening
    template<unsigned int y, typename std::enable_if<x < y>::type * = nullptr>
    basic_uint &operator-=(const basic_uint<y> &); //bigger basic_uint, narrowing

    basic_uint &operator*=(const word_t);
    basic_uint &operator*=(const basic_uint &);
    template<unsigned int y, typename std::enable_if<y < x>::type * = nullptr>
    basic_uint &operator*=(const basic_uint<y> &); //smaller basic_uint, widening
    template<unsigned int y, typename std::enable_if<x < y>::type * = nullptr>
    basic_uint &operator*=(const basic_uint<y> &); //bigger basic_uint, narrowing

    basic_uint &operator/=(const word_t);
    template<unsigned int y>
    basic_uint &operator/=(const basic_uint<y> &);

    basic_uint &operator%=(const word_t);
    template<unsigned int y>
    basic_uint &operator%=(const basic_uint<y> &);

    basic_uint &operator&=(const word_t);
    basic_uint &operator&=(const basic_uint &);
    template<unsigned int y, typename std::enable_if<y < x>::type * = nullptr>
    basic_uint &operator&=(const basic_uint<y> &); //smaller basic_uint, widening
    template<unsigned int y, typename std::enable_if<x < y>::type * = nullptr>
    basic_uint &operator&=(const basic_uint<y> &); //bigger basic_uint, narrowing

    basic_uint &operator|=(const word_t);
    basic_uint &operator|=(const basic_uint &);
    template<unsigned int y, typename std::enable_if<y < x>::type * = nullptr>
    basic_uint &operator|=(const basic_uint<y> &); //smaller basic_uint, widening
    template<unsigned int y, typename std::enable_if<x < y>::type * = nullptr>
    basic_uint &operator|=(const basic_uint<y> &); //bigger basic_uint, narrowing

    basic_uint &operator^=(const word_t);
    basic_uint &operator^=(const basic_uint &);
    template<unsigned int y, typename std::enable_if<y < x>::type * = nullptr>
    basic_uint &operator^=(const basic_uint<y> &); //smaller basic_uint, widening
    template<unsigned int y, typename std::enable_if<x < y>::type * = nullptr>
    basic_uint &operator^=(const basic_uint<y> &); //bigger basic_uint, narrowing


    basic_uint &operator<<=(const word_t);
    basic_uint &operator>>=(const word_t);


    basic_uint operator++(int);
    basic_uint operator--(int);
    basic_uint &operator++();
    basic_uint &operator--();


    //comparison operators
    bool operator==(const word_t) const; // primitive
    bool operator==(const basic_uint &) const; //basic_uint
    template<unsigned int y, typename std::enable_if<y < x>::type * = nullptr>
    bool operator==(const basic_uint<y> &) const; //smaller basic_uint, widening
    template<unsigned int y, typename std::enable_if<x < y>::type * = nullptr>
    bool operator==(const basic_uint<y> &) const; //bigger basic_uint, widening

    //destructor
    ~basic_uint() = default;

    //friend functions
    friend mp_bitcnt_t __builtin_popcount<x>(const basic_uint &);
};

template<unsigned int x>
typename basic_uint<x>::word_t basic_uint<x>::buffer_data[2 * n_words];

template<unsigned int x>
inline void basic_uint<x>::reset() {
    memset(data, 0, n_bytes);
}

template<unsigned int x>
inline void basic_uint<x>::reset(size_t b) {
    memset(data + b, 0, (n_words - b) * sizeof(word_t));
}

template<unsigned int x>
inline void basic_uint<x>::set() {
    memset(data, 255, n_bytes);
}

template<unsigned int x>
basic_uint<x>::basic_uint() {
    reset();
}

template<unsigned int x>
basic_uint<x>::basic_uint(const word_t v) {
    data[0] = v;
    memset(data + 1, 0, n_bytes - sizeof(word_t));
}

template<unsigned int x>
basic_uint<x>::basic_uint(const basic_uint &v) {
    memcpy(data, v.data, n_bytes);
}

template<unsigned int x>
basic_uint<x> &basic_uint<x>::operator+=(const basic_uint &v) {
    mpn_add_n(data, data, v.data, n_words);
    return *this;
}

template<unsigned int x>
basic_uint<x> &basic_uint<x>::operator+=(const word_t v) {
    mpn_add_1(data, data, n_words, v);
    return *this;
}

template<unsigned int x>
template<unsigned int y, typename std::enable_if<y < x>::type *>
basic_uint<x> &basic_uint<x>::operator+=(const basic_uint<y> &v) {
    mpn_add(data, data, n_words, v.data, basic_uint<y>::n_words);
    return *this;
}

template<unsigned int x>
template<unsigned int y, typename std::enable_if<x < y>::type *>
basic_uint<x> &basic_uint<x>::operator+=(const basic_uint<y> &v) {
    mpn_add_n(data, data, v.data, n_words); //overflow
    return *this;
}


template<unsigned int x>
basic_uint<x> &basic_uint<x>::operator-=(const basic_uint &v) {
    mpn_sub_n(data, data, v.data, n_words);
    return *this;
}

template<unsigned int x>
basic_uint<x> &basic_uint<x>::operator-=(const word_t v) {
    mpn_sub_1(data, data, n_words, v);
    return *this;
}

template<unsigned int x>
template<unsigned int y, typename std::enable_if<y < x>::type *>
basic_uint<x> &basic_uint<x>::operator-=(const basic_uint<y> &v) {
    mpn_sub(data, data, n_words, v.data, basic_uint<y>::n_words);
    return *this;
}

template<unsigned int x>
template<unsigned int y, typename std::enable_if<x < y>::type *>
basic_uint<x> &basic_uint<x>::operator-=(const basic_uint<y> &v) {
    mpn_sub_n(data, data, v.data, n_words); //overflow
    return *this;
}

template<unsigned int x>
basic_uint<x> &basic_uint<x>::operator*=(const basic_uint &v) {
    mpn_mul_n(buffer_data, data, v.data, n_words);
    memcpy(data, buffer_data, n_bytes);
    return *this;
}

template<unsigned int x>
basic_uint<x> &basic_uint<x>::operator*=(const word_t v) {
    mpn_mul_1(data, data, n_words, v);
    return *this;
}

template<unsigned int x>
template<unsigned int y, typename std::enable_if<y < x>::type *>
basic_uint<x> &basic_uint<x>::operator*=(const basic_uint<y> &v) {
    mpn_mul(buffer_data, data, n_words, v.data, basic_uint<y>::n_words);
    memcpy(data, buffer_data, n_bytes);
    return *this;
}

template<unsigned int x>
template<unsigned int y, typename std::enable_if<x < y>::type *>
basic_uint<x> &basic_uint<x>::operator*=(const basic_uint<y> &v) {
    mpn_mul_n(buffer_data, data, v.data, n_words); //overflow
    memcpy(data, buffer_data, n_bytes);
    return *this;
}

template<unsigned int x>
template<unsigned int y>
basic_uint<x> &basic_uint<x>::operator/=(const basic_uint<y> &v) {
    // most significant bit of v must be non zero
    size_t vn_words = basic_uint<y>::n_words;
    while (vn_words && v.data[vn_words - 1] == 0)--vn_words;
    if (vn_words == 0)vn_words /= vn_words;
    if (vn_words > n_words) {
        reset();
        return *this;
    }
    mpn_tdiv_qr(buffer_data, data, 0, data, n_words, v.data, vn_words);
    memcpy(data, buffer_data, (n_words - vn_words + 1) * sizeof(word_t));
    // TODO: discover whether the following clean is necessary
    // memset(data + n_words - vn_words + 1, 0, (vn_words - 1) * sizeof(word_t));
    return *this;
}

template<unsigned int x>
basic_uint<x> &basic_uint<x>::operator/=(const word_t v) {
    mpn_divrem_1(data, 0, data, n_words, v);
    return *this;
}

template<unsigned int x>
template<unsigned int y>
basic_uint<x> &basic_uint<x>::operator%=(const basic_uint<y> &v) {
    // most significant bit of v must be non zero
    size_t vn_words = basic_uint<y>::n_words;
    while (vn_words && v.data[vn_words - 1] == 0)--vn_words;
    if (vn_words == 0)vn_words /= vn_words;
    if (vn_words > n_words)return *this;
    mpn_tdiv_qr(buffer_data, data, 0, data, n_words, v.data, vn_words);
    memset(data + vn_words, 0, (n_words - vn_words) * sizeof(word_t));
    return *this;
}

template<unsigned int x>
basic_uint<x> &basic_uint<x>::operator%=(const word_t v) {
    mpn_divrem_1(data, 0, data, n_words, v);
    return *this;
}


template<unsigned int x>
basic_uint<x> &basic_uint<x>::operator&=(const basic_uint &v) {
    mpn_and_n(data, data, v.data, n_words);
    return *this;
}

template<unsigned int x>
basic_uint<x> &basic_uint<x>::operator&=(const word_t v) {
    *data &= v;
    reset(1);
    return *this;
}

template<unsigned int x>
template<unsigned int y, typename std::enable_if<y < x>::type *>
basic_uint<x> &basic_uint<x>::operator&=(const basic_uint<y> &v) {
    mpn_and_n(data, data, v.data, basic_uint<y>::n_words);
    reset(basic_uint<y>::n_words);
    return *this;
}

template<unsigned int x>
template<unsigned int y, typename std::enable_if<x < y>::type *>
basic_uint<x> &basic_uint<x>::operator&=(const basic_uint<y> &v) {
    mpn_and_n(data, data, v.data, n_words);
    return *this;
}

template<unsigned int x>
basic_uint<x> &basic_uint<x>::operator|=(const basic_uint &v) {
    mpn_ior_n(data, data, v.data, n_words);
    return *this;
}

template<unsigned int x>
basic_uint<x> &basic_uint<x>::operator|=(const word_t v) {
    *data |= v;
    return *this;
}

template<unsigned int x>
template<unsigned int y, typename std::enable_if<y < x>::type *>
basic_uint<x> &basic_uint<x>::operator|=(const basic_uint<y> &v) {
    mpn_ior_n(data, data, v.data, basic_uint<y>::n_words);
    return *this;
}

template<unsigned int x>
template<unsigned int y, typename std::enable_if<x < y>::type *>
basic_uint<x> &basic_uint<x>::operator|=(const basic_uint<y> &v) {
    mpn_ior_n(data, data, v.data, n_words);
    return *this;
}

template<unsigned int x>
basic_uint<x> &basic_uint<x>::operator^=(const basic_uint &v) {
    mpn_xor_n(data, data, v.data, n_words);
    return *this;
}

template<unsigned int x>
basic_uint<x> &basic_uint<x>::operator^=(const word_t v) {
    *data ^= v;
    return *this;
}

template<unsigned int x>
template<unsigned int y, typename std::enable_if<y < x>::type *>
basic_uint<x> &basic_uint<x>::operator^=(const basic_uint<y> &v) {
    mpn_xor_n(data, data, v.data, basic_uint<y>::n_words);
    return *this;
}

template<unsigned int x>
template<unsigned int y, typename std::enable_if<x < y>::type *>
basic_uint<x> &basic_uint<x>::operator^=(const basic_uint<y> &v) {
    mpn_xor_n(data, data, v.data, n_words);
    return *this;
}

template<unsigned int x>
basic_uint<x> &basic_uint<x>::operator<<=(const word_t v) {
    const iint wskip = v / mp_bits_per_limb;
    mpn_lshift(data + wskip, data, n_words - wskip, v % mp_bits_per_limb);
    mpn_zero(data, wskip);
    return *this;
}

template<unsigned int x>
basic_uint<x> &basic_uint<x>::operator>>=(const word_t v) {
    const iint wskip = v / mp_bits_per_limb;
    mpn_rshift(data, data + wskip, n_words - wskip, v);
    mpn_zero(data + n_words - wskip, wskip);
    return *this;
}

template<unsigned int x>
bool basic_uint<x>::operator==(const word_t v) const {
    return *data == v && mpn_zero_p(data + 1, n_words - 1);
}

template<unsigned int x>
bool basic_uint<x>::operator==(const basic_uint &v) const {
    return !mpn_cmp(data,v.data,n_words);
}

template<unsigned int x>
basic_uint<x> &basic_uint<x>::operator++() {
    mpn_add_1(data, data, n_words, 1);
    return *this;
}

template<unsigned int x>
basic_uint<x> &basic_uint<x>::operator--() {
    mpn_sub_1(data, data, n_words, 1);
    return *this;
}

template<unsigned int x>
basic_uint<x> basic_uint<x>::operator++(int) {
    basic_uint r(*this);
    ++(*this);
    return r;
}

template<unsigned int x>
basic_uint<x> basic_uint<x>::operator--(int) {
    basic_uint r(*this);
    --(*this);
    return r;
}

template<unsigned int x>
template<unsigned int y, typename std::enable_if<y < x>::type *>
bool basic_uint<x>::operator==(const basic_uint<y> &v) const {
    return !mpn_cmp(data, v.data, basic_uint<y>::n_words) &&
           mpn_zero_p(data + basic_uint<y>::n_words, n_words - basic_uint<y>::n_words);
}

template<unsigned int x>
template<unsigned int y, typename std::enable_if<x < y>::type *>
bool basic_uint<x>::operator==(const basic_uint<y> &v) const {
    return !mpn_cmp(data, v.data, n_words) && mpn_zero_p(v.data + n_words, basic_uint<y>::n_words - n_words);
}

template<unsigned int x>
template<unsigned int y, typename std::enable_if<y < x>::type *>
basic_uint<x>::basic_uint(const basic_uint<y> &v) {
    mpn_copyi(data, v.data, basic_uint<y>::n_words);
    mpn_zero(data + basic_uint<y>::n_words, n_words - basic_uint<y>::n_words);
}

template<unsigned int x>
template<unsigned int y, typename std::enable_if<x < y>::type *>
basic_uint<x>::basic_uint(const basic_uint<y> &v) {
    mpn_copyi(data, v.data, n_words);
}

template<unsigned int x>
mp_bitcnt_t __builtin_popcount(const basic_uint<x> &v) {
    return mpn_popcount(v.data, basic_uint<x>::n_words);
}

// useful type definitions
typedef basic_uint<2> uint128_t;
typedef basic_uint<4> uint256_t;
typedef basic_uint<8> uint512_t;
typedef basic_uint<16> uint1024_t;
typedef basic_uint<32> uint2048_t;

}

#endif //CSL_UINT_H
