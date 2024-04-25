#include <CLI/CLI.hpp>
#include <alt-pack/pack.hpp>
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

        if (branches_name.empty())
        {
            printf("branch_cli: missing branch names\n%s", app.help().c_str());
            return 0;
        }
        else if (branches_name.size() != 2)
            throw std::runtime_error("Invalid branch names");

        // Инициализация библиотеки libcurl
        alt::curl_setup curl;

        alt::rest rest;
        rest.set_progress_bar();

        std::string branch1_pkgs, branch2_pkgs;
        rest.set_request_output(branch1_pkgs);
        rest.send_request(branches_name[0]);

        rest.set_request_output(branch2_pkgs);
        rest.send_request(branches_name[1]);

        alt::package_list first_package_list, second_package_list;
        first_package_list.parse(branch1_pkgs);
        second_package_list.parse(branch2_pkgs);
    }
    catch (const std::runtime_error& e)
    {
        printf("%s\n", e.what());
        return -1;
    }

    return 0;
}