#include <alt-pack/pack.hpp>
#include <fstream>
#include <stdexcept>
#include <stdio.h>

int main(int argc, char* argv[])
{
    try
    {
        // Инициализация библиотеки libcurl
        alt::curl_setup curl;

        alt::rest rest;

        std::string output;
        rest.set_request_output(output);
        rest.set_progress_bar();

        rest.send_request("/export/branch_binary_packages/p9");

        alt::package_list package_list;
        package_list.parse(output);

        printf("%u", package_list.get_root()["length"].asInt());
    }
    catch (const std::runtime_error& e)
    {
        printf("%s", e.what());
        return -1;
    }

    return 0;
}