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
    package_list();
    void parse(const std::string& package_data);
    Json::Value get_root();

private:
    Json::Value m_value;
    std::unique_ptr<Json::CharReader> m_reader_ptr;
};
}