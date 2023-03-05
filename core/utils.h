#pragma once

#include <string_view>
#include <charconv>

#include "app_error.h"

template <typename T>
T SVToType(std::string_view str) {
    T converted;
    auto result = std::from_chars(str.data(), str.data() + str.size(), converted);
    if (result.ec == std::errc::invalid_argument) {
        throw AppError(AppError::FilterArgumentCastError);
    }

    return converted;
}
