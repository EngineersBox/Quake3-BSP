#pragma once

#ifndef QUAKE_ARRAYUTILS_HPP
#define QUAKE_ARRAYUTILS_HPP

#include <string>
#include <iostream>
#include <sstream>
#include <string_view>

namespace ArrayUtils {
    template <typename Range, typename Value = typename Range::value_type>
    std::string join(Range const& elements, const char *const delimiter) {
        std::ostringstream os;
        auto b = begin(elements), e = end(elements);

        if (b != e) {
            std::copy(b, prev(e), std::ostream_iterator<Value>(os, delimiter));
            b = prev(e);
        }
        if (b != e) {
            os << *b;
        }

        return os.str();
    }

    template <size_t N>
    constexpr std::string_view stringView(const char (&literal)[N]) {
        return std::string_view(literal, N - 1);
    }

    template <size_t... N>
    constexpr std::array<std::string_view, sizeof...(N)> makeLiteralArray(const char (&... literals)[N]) {
        return {{stringView(literals)...}};
    }
}

#endif //QUAKE_ARRAYUTILS_HPP
