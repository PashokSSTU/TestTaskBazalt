#include <stdexcept>
#include <string>

#include "../public/pack.hpp"

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
    curl_easy_setopt(m_curl, CURLOPT_URL, std::string(ALT_API) + method);
    m_res = curl_easy_perform(m_curl);

    if (m_res != CURLE_OK)
        throw std::runtime_error(
            std::string("Curl request error code: ") + std::to_string(m_res) +
            "\nError description:\n" + curl_easy_strerror(m_res));
}
}