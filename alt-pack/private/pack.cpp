#include <stdexcept>
#include <stdio.h>
#include <string>

#include "../public/pack.hpp"

namespace
{
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

package_list::package_list()
{
    Json::CharReaderBuilder builder;
    m_reader_ptr.reset(builder.newCharReader());
}

void package_list::parse(const std::string& package_data)
{
    std::string errors;
    if (!m_reader_ptr->parse(&package_data.front(), &package_data.back(),
                             &m_value, &errors))
    {
        throw std::runtime_error(errors);
    }
}

Json::Value package_list::get_root() { return m_value; }
}