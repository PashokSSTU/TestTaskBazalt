#include <CLI/CLI.hpp>
#include <alt-pack/pack.hpp>
#include <filesystem>
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

        std::vector<std::string> branches_names;
        app.add_option("-b,--branch", branches_names, "Branches name");

        std::filesystem::path filepath;
        app.add_option("-o,--output", filepath,
                       "Filepath to .json file with result");

        CLI11_PARSE(app, argc, argv);

        // Валидация аргументов с именами веток
        if (branches_names.empty())
        {
            printf("branch_cli: missing branch names\n%s", app.help().c_str());
            return 0;
        }
        else if (branches_names.size() != 2)
            throw std::runtime_error("Invalid branch names");

        // Валидация пути к результирующему файлу
        if (filepath.empty())
            throw std::runtime_error("Output file name not specified");

        // Инициализация библиотеки libcurl
        alt::curl_setup curl;

        // Настройка curl и вызов методов из REST_API
        alt::rest rest;
        rest.set_progress_bar();

        std::string branch1_pkgs, branch2_pkgs;
        rest.set_request_output(branch1_pkgs);
        rest.send_request(branches_names[0]);

        rest.set_request_output(branch2_pkgs);
        rest.send_request(branches_names[1]);

        // Загрузка информации о пакетах в класс для сравнения
        alt::package_list package_list(branches_names);
        package_list.parse(branch1_pkgs, branch2_pkgs);

        // Получение результата и его загрузка в выходной файл
        std::filesystem::create_directory(filepath.parent_path());
        std::ofstream result(filepath);
        result << package_list.compare();
    }
    catch (const std::runtime_error& e)
    {
        printf("%s\n", e.what());
        return -1;
    }

    return 0;
}