#ifndef CSL_UINT_H
#define CSL_UINT_H

namespace csl {

template<unsigned int x>
class basic_uint {
    //friendship with all basic_uint
    template<unsigned int y> friend
    class basic_uint;

    typedef std::size_t size_t;
    typedef uint64_t word_t;
    static constexpr size_t n_words = (2 << x);
    word_t data[n_words]{}; // little endian
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

    //destructor
    ~basic_uint() = default;
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


typedef basic_uint<0> uint128_t;
typedef basic_uint<1> uint256_t;
typedef basic_uint<2> uint512_t;
typedef basic_uint<3> uint1024_t;
typedef basic_uint<4> uint2048_t;
typedef basic_uint<5> uint4096_t;

}

#endif //CSL_UINT_H
