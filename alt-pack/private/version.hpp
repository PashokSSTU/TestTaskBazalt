#pragma once

#include <string>

namespace alt
{
namespace internal
{
enum comparison_result
{
    EQUAL,
    GREATER,
    LESS
};

// Функция для сравнения версий
comparison_result compare_versions(const std::string& version1,
                                   const std::string& version2);
}
}