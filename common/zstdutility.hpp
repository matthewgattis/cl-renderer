// https://stackoverflow.com/questions/57745122/how-to-compress-a-c-string-using-zstd
//
#ifndef ZSTD_UTILITY_H
#define ZSTD_UTILITY_H

#include <type_traits>
#include <string>

namespace zstd {
#include <zstd.h>
} // namespace zstd

static constexpr int kMinCLevel = 1;
static constexpr int kMaxCLevel = 22;
static constexpr int kDefaultCLevel = kMinCLevel;

template<typename ChType>
struct ZstdCompression final {
    using char_type = typename std::char_traits<ChType>::char_type;
    using ContainerType = std::basic_string<char_type>;

    inline auto operator()(const char_type* input, const size_t inputSize, const int level = kDefaultCLevel) {
        assert(0 < inputSize);
        assert(kMinCLevel <= level);
        assert(kMaxCLevel >= level);

        const size_t required = zstd::ZSTD_compressBound(inputSize);
        ContainerType block(required, 0x0);
        const size_t actual = zstd::ZSTD_compress(block.data(), block.size(), input, inputSize, level);
        assert(!zstd::ZSTD_isError(actual));
        block.erase(block.begin() + actual, block.end());

        return block;
    }

    template<typename Container, typename IteratorValueType = typename std::iterator_traits<typename Container::iterator>::value_type>
    inline auto operator()(const Container& input, const int level = kDefaultCLevel) -> std::enable_if_t<std::is_same_v<char_type, IteratorValueType>, ContainerType> {
        return this->operator()(input.data(), input.size(), level);
    }
};

using ZstdCompressor = ZstdCompression<char>;

template<typename ChType>
struct ZstdDeCompression final {
    using char_type = typename std::char_traits<ChType>::char_type;
    using ContainerType = std::basic_string<char_type>;

    inline auto operator()(const char_type* input, const size_t inputSize) {
        assert(0 < inputSize);

        const size_t size = zstd::ZSTD_getFrameContentSize(input, inputSize);
        assert(ZSTD_CONTENTSIZE_ERROR != size);
        assert(ZSTD_CONTENTSIZE_UNKNOWN != size);

        ContainerType block(size, 0x0);
        const size_t actual = zstd::ZSTD_decompress(block.data(), block.size(), input, inputSize);
        assert(!zstd::ZSTD_isError(actual));
        assert(actual == size);

        return block;
    }

    template<typename Container, typename IteratorValueType = typename std::iterator_traits<typename Container::iterator>::value_type>
    inline auto operator()(const Container& input) -> std::enable_if_t<std::is_same_v<char_type, IteratorValueType>, ContainerType> {
        return this->operator()(input.data(), input.size());
    }
};

using ZstdDeCompressor = ZstdDeCompression<char>;

#endif // ZSTD_UTILITY_H
