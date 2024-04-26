#include <algorithm>
#include <functional>
#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>

namespace
{
// Функция для разделения строки по разделителю
std::vector<std::string> split(const std::string& str, char delim)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream iss(str);
    while (std::getline(iss, token, delim))
    {
        tokens.push_back(token);
    }
    return tokens;
}
}

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

// Функция для определения, является ли версия release
bool is_release(const std::string& version)
{
    return version.find("alpha") == std::string::npos &&
           version.find("beta") == std::string::npos;
}

// Функция для сравнения версий
comparison_result compare_versions(const std::string& version1,
                                   const std::string& version2)
{
    std::vector<std::string> parts1 = split(version1, '.');
    std::vector<std::string> parts2 = split(version2, '.');

    auto to_lower = [](const std::string& str)
    {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(), ::tolower);
        return result;
    };

    std::transform(parts1.begin(), parts1.end(), parts1.begin(), to_lower);
    std::transform(parts2.begin(), parts2.end(), parts2.begin(), to_lower);

    for (size_t i = 0; i < std::max(parts1.size(), parts2.size()); ++i)
    {
        if (i < parts1.size() && i < parts2.size())
        {
            if (parts1[i] < parts2[i])
                return comparison_result::LESS;
            if (parts1[i] > parts2[i])
                return comparison_result::GREATER;
        }
        else if (i < parts1.size())
            return comparison_result::GREATER;
        else
            return comparison_result::LESS;
    }
    return comparison_result::EQUAL;
}
}
}
