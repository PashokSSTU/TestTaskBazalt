#include <boost/functional/hash.hpp>
#include <functional>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <thread>
#include <tuple>
#include <unordered_map>

#include "../public/pack.hpp"
#include "version.hpp"

namespace
{
using pkg_info_t = std::tuple<std::string, std::string>;

// Callback для записи ответа на запрос в строку
size_t write_callback(void* contents, size_t size, size_t nmemb,
                      std::string* buffer)
{
    size_t total_size = size * nmemb;
    buffer->append((char*)contents, total_size);
    return total_size;
}

// Callback для вывода прогресса завершения получения ответа
int progress_callback(void* clientp, double dltotal, double dlnow,
                      double ultotal, double ulnow)
{
    if (dltotal <= 0)
        return 0;

    if (dlnow == dltotal)
    {
        printf("\n");
        return 0;
    }

    double progress = (double)dlnow / (double)dltotal * 100.0;

    // Определяем количество символов '=' и ' ' для progress bar
    int num_equals = static_cast<int>(progress / 2);
    int num_spaces = 50 - num_equals;

    // Создаем строку для progress bar
    std::string progress_bar = std::string("[") + std::string(num_equals, '=') +
                               ">" + std::string(num_spaces, ' ') + "] ";

    // Выводим progress bar
    printf("\r%s%.1f%%\r", progress_bar.c_str(), progress);
    fflush(stdout);

    return 0;
}

void compare_with_branch(const Json::Value original,
                         const Json::Value comparable, Json::Value& packages,
                         bool compare_versions)
{
    std::unordered_map<pkg_info_t, Json::Value, boost::hash<pkg_info_t>>
        packages_table;

    for (const auto& package : comparable["packages"])
        packages_table[{package["name"].asString(),
                        package["arch"].asString()}] = package;

    for (const auto& package : original["packages"])
    {
        auto it = packages_table.find(
            {package["name"].asString(), package["arch"].asString()});
        if (it == packages_table.end())
        {
            if (!compare_versions)
                packages.append(package);
        }
        else if (compare_versions &&
                 alt::internal::is_release(package["version"].asString()))
        {
            bool is_greater =
                (!alt::internal::is_release(it->second["version"].asString()))
                    ? true
                    : false;

            is_greater = (alt::internal::compare_versions(
                              package["version"].asString(),
                              it->second["version"].asString()) ==
                          alt::internal::comparison_result::GREATER)
                             ? true
                             : false;

            if (is_greater)
                packages.append(package);
        }
    }
}
}

namespace alt
{
curl_setup::curl_setup()
{
    if (curl_global_init(CURL_GLOBAL_ALL))
    {
        throw std::runtime_error("Error initializing libcurl");
    }
}

curl_setup::~curl_setup() { curl_global_cleanup(); }

rest::rest() { m_curl = curl_easy_init(); }

void rest::send_request(const std::string& method)
{
    size_t branch_pos = method.find_last_of('/');
    printf("Get package list from branch %s\n",
           method.substr(branch_pos + 1).c_str());

    curl_easy_setopt(
        m_curl, CURLOPT_URL,
        (std::string(ALT_API) + std::string(REST_EXPORT) + method).c_str());
    m_res = curl_easy_perform(m_curl);

    if (m_res != CURLE_OK)
        throw std::runtime_error(
            std::string("Curl request error code: ") + std::to_string(m_res) +
            "\nError description:\n" + curl_easy_strerror(m_res));
}

void rest::set_request_output(std::string& output)
{
    curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &output);
}

void rest::set_progress_bar()
{
    curl_easy_setopt(m_curl, CURLOPT_NOPROGRESS, false);
    curl_easy_setopt(m_curl, CURLOPT_PROGRESSFUNCTION, progress_callback);
}

void package_list::parse(const std::string& first_branch_pkgs,
                         const std::string& second_branch_pkgs)
{
    std::string errors;
    if (!m_reader_ptr->parse(&first_branch_pkgs.front(),
                             &first_branch_pkgs.back(), &m_first_branch,
                             &errors) ||
        !m_reader_ptr->parse(&second_branch_pkgs.front(),
                             &second_branch_pkgs.back(), &m_second_branch,
                             &errors))
    {
        throw std::runtime_error(errors);
    }
}

std::string package_list::compare()
{
    Json::Value root;
    Json::Value branch1(Json::arrayValue);
    Json::Value branch2(Json::arrayValue);
    Json::Value releases(Json::arrayValue);

    std::thread th1(compare_with_branch, m_first_branch, m_second_branch,
                    std::ref(branch1), false);
    std::thread th2(compare_with_branch, m_second_branch, m_first_branch,
                    std::ref(branch2), false);
    std::thread th3(compare_with_branch, m_first_branch, m_second_branch,
                    std::ref(releases), true);

    th1.join();
    th2.join();
    th3.join();

    const std::string missing_from_second_branch =
        "Missing from " + m_branches_names[1];
    const std::string missing_from_first_branch =
        "Missing from " + m_branches_names[0];

    root["branches"][missing_from_second_branch] = branch1;
    root["branches"][missing_from_first_branch] = branch2;
    root["branches"]["releases"] = releases;

    return root.toStyledString();
}
}