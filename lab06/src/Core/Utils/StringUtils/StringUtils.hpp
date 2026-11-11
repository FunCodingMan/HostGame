#pragma once
#include <SFML/System/String.hpp>
#include <string>

namespace StringUtils {

    inline sf::String ToSfStringFromUtf8(const std::string& utf8Str) {
        return sf::String::fromUtf8(utf8Str.begin(), utf8Str.end());
    }

    inline std::string ToUtf8(const sf::String& str) {
        auto utf8 = str.toUtf8();
        return std::string(utf8.begin(), utf8.end());
    }

}