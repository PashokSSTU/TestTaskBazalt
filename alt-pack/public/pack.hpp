#pragma once
#define EXPORT __attribute__((__visibility__("default")))

#include <curl/curl.h>
#include <json/json.h>
#include <memory>
#include <string>

namespace alt
{
// RAII обёртка для инициализации и освобождения глобальных данных библиотеки
// libcurl
class EXPORT curl_setup
{
public:
    curl_setup();
    ~curl_setup();
};

// Класс для управления запросами
class EXPORT rest
{
public:
    rest();

    // Отправка запроса
    void send_request(const std::string& method);

    // Установка строковой переменной для записи ответа
    void set_request_output(std::string& output);

    // Включение progress-bar при получении ответа
    void set_progress_bar();

private:
    CURL* m_curl;
    CURLcode m_res;

private:
    static constexpr const char* ALT_API = "https://rdb.altlinux.org/api";
    static constexpr const char* REST_EXPORT =
        "/export/branch_binary_packages/";
};

// Класс для обработки результатов из базы данных
class EXPORT package_list
{
public:
    package_list(const std::vector<std::string>& names)
        : m_branches_names(names)
    {
        Json::CharReaderBuilder builder;
        m_reader_ptr.reset(builder.newCharReader());
    }

    // Парсинг json-данных о пакетах
    void parse(const std::string& first_branch_pkgs,
               const std::string& second_branch_pkgs);

    // Сравнение информации и пакетах
    std::string compare();

private:
    Json::Value m_first_branch;
    Json::Value m_second_branch;
    std::unique_ptr<Json::CharReader> m_reader_ptr;
    std::vector<std::string> m_branches_names;
};
}