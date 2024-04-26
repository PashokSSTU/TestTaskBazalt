#include <algorithm>
#include <sstream>
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

    for (size_t i = 0; i < std::max(parts1.size(), parts2.size()); ++i)
    {
        std::transform(parts1[i].begin(), parts1[i].end(), parts1[i].begin(),
                       ::tolower);
        std::transform(parts2[i].begin(), parts2[i].end(), parts2[i].begin(),
                       ::tolower);

        if (parts1[i] < parts2[i])
            return comparison_result::LESS;
        if (parts1[i] > parts2[i])
            return comparison_result::GREATER;
    }

    return comparison_result::EQUAL;
}
}
}