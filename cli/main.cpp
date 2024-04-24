#include <CLI/CLI.hpp>
#include <alt-pack/pack.hpp>
#include <fstream>
#include <stdexcept>
#include <stdio.h>
#include <vector>

int main(int argc, char* argv[])
{
    try
    {
        // Парсинг ключей командной строки
        CLI::App app{"Utility for comparing packages in alt-linux branches"};
        app.set_help_flag("-h,--help", "Usage help");

        std::vector<std::string> branches_name;
        app.add_option("-b,--branch", branches_name, "Branches name");

        CLI11_PARSE(app, argc, argv);

        if (app.count("-h") > 0 || app.count("--help") > 0)
        {
            app.help();
            return 0;
        }

        if (branches_name.size() != 2)
            throw std::runtime_error("Invalid branch names");

        // Инициализация библиотеки libcurl
        alt::curl_setup curl;

        alt::rest rest;

        std::string output;
        rest.set_request_output(output);
        rest.set_progress_bar();

        rest.send_request(branches_name[0]);
        rest.send_request(branches_name[1]);

        alt::package_list package_list;
        package_list.parse(output);

        printf("%u", package_list.get_root()["length"].asInt());
    }
    catch (const std::runtime_error& e)
    {
        printf("%s\n", e.what());
        return -1;
    }

    return 0;
}