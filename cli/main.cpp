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

        // Создание класса-запроса
        alt::rest rest;

        // Строка с ответом
        std::string output;

        // Установим, что ответ запишем в переменную output
        rest.set_request_output(output);

        // Включение progress-bar при получении ответа
        rest.set_progress_bar();

        // Вызов метода из REST API
        rest.send_request("/export/branch_binary_packages/p9");
    }
    catch (const std::runtime_error& e)
    {
        printf("%s", e.what());
    }

    return 0;
}