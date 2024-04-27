#include <algorithm>
#include <functional>
#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>

namespace
{
// Функция разбивает строку версии на компоненты
std::vector<std::string> split_version(const std::string& version)
{
    std::vector<std::string> parts;
    std::string part;
    char prev_symbol = version[0];
    for (char symbol : version)
    {
        // Если текущий символ отличается от предыдущего и один из них - цифра,
        // или если текущий символ цифра, а предыдущий - буква,
        // то добавляем новый компонент версии
        if ((isdigit(symbol) && !isdigit(prev_symbol)) ||
            (!isdigit(symbol) && isdigit(prev_symbol)))
        {
            parts.push_back(part);
            part.clear();
        }
        part += symbol;
        prev_symbol = symbol;
    }
    parts.push_back(part); // Добавляем последний компонент
    return parts;
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

// Функция сравнения версий
int compare_versions(const std::string& v1, const std::string& v2)
{
    std::vector<std::string> parts1 = split_version(v1);
    std::vector<std::string> parts2 = split_version(v2);

    size_t maxLength = std::max(parts1.size(), parts2.size());
    for (size_t i = 0; i < maxLength; ++i)
    {
        std::string part1 = (i < parts1.size()) ? parts1[i] : "0";
        std::string part2 = (i < parts2.size()) ? parts2[i] : "0";

        // Преобразуем компоненты в числа, если это возможно
        int num1 = 0, num2 = 0;
        bool is_num1 = true, is_num2 = true;

        try
        {
            num1 = std::stoi(part1);
        }
        catch (...)
        {
            is_num1 = false;
        }

        try
        {
            num2 = std::stoi(part2);
        }
        catch (...)
        {
            is_num2 = false;
        }

        // Сравниваем числа или строки
        if (is_num1 && is_num2)
        {
            if (num1 < num2)
                return comparison_result::LESS;
            if (num1 > num2)
                return comparison_result::GREATER;
        }
        else
        {
            if (part1 < part2)
                return comparison_result::LESS;
            if (part1 > part2)
                return comparison_result::GREATER;
        }
    }

    return comparison_result::EQUAL;
}
}
}
