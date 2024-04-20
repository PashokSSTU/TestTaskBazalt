#include <alt-pack/pack.hpp>
#include <stdexcept>
#include <stdio.h>

int main(int argc, char* argv[])
{
    try
    {
        // Инициализация библиотеки libcurl
        alt::curl_setup curl;
    }
    catch (const std::runtime_error& e)
    {
        printf("%s", e.what());
    }

    return 0;
}