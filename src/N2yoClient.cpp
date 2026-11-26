#include "N2yoClient.hpp"

#include <cmath>
#include <cstdlib>
#include <locale>
#include <memory>
#include <sstream>
#include <stdexcept>

#include <curl/curl.h>
#include <nlohmann/json.hpp>

namespace {
std::string setting(const char* name) {
    const char* value = std::getenv(name);
    if (!value || !*value) {
        throw std::runtime_error(std::string("Set ") + name);
    }
    return value;
}

double coordinate(const char* name, double minimum, double maximum) {
    std::istringstream input(setting(name));
    input.imbue(std::locale::classic());
    double value;
    if (!(input >> value) || !(input >> std::ws).eof() ||
        !std::isfinite(value) || value < minimum || value > maximum) {
        throw std::runtime_error(std::string("Invalid ") + name);
    }
    return value;
}

std::size_t receive(char* data, std::size_t size, std::size_t count, void* output) {
    try {
        static_cast<std::string*>(output)->append(data, size * count);
        return size * count;
    } catch (...) {
        return 0;
    }
}

void fetchPasses(int id, const char* name, const std::string& observer,
                 const std::string& apiKey, std::time_t now, std::vector<Pass>& passes) {
    std::unique_ptr<CURL, decltype(&curl_easy_cleanup)> curl(curl_easy_init(), curl_easy_cleanup);
    if (!curl) {
        throw std::runtime_error("Could not initialize curl");
    }
    std::unique_ptr<char, decltype(&curl_free)> key(
        curl_easy_escape(curl.get(), apiKey.c_str(), 0), curl_free);
    if (!key) {
        throw std::runtime_error("Could not encode the API key");
    }
    const auto url = "https://api.n2yo.com/rest/v1/satellite/radiopasses/" +
                     std::to_string(id) + "/" + observer + "/1/40/&apiKey=" + key.get();
    std::string response;
    curl_easy_setopt(curl.get(), CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl.get(), CURLOPT_WRITEFUNCTION, receive);
    curl_easy_setopt(curl.get(), CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl.get(), CURLOPT_TIMEOUT, 30L);
    curl_easy_setopt(curl.get(), CURLOPT_FAILONERROR, 1L);

    const auto result = curl_easy_perform(curl.get());
    if (result != CURLE_OK) {
        throw std::runtime_error(std::string(name) + ": " + curl_easy_strerror(result));
    }

    const auto json = nlohmann::json::parse(response, nullptr, false);
    if (!json.is_object() || !json.contains("passes") || !json["passes"].is_array()) {
        throw std::runtime_error(std::string(name) + ": invalid N2YO response");
    }
    try {
        for (const auto& entry : json["passes"]) {
            const auto start = entry.at("startUTC").get<std::time_t>();
            const auto end = entry.at("endUTC").get<std::time_t>();
            const auto elevation = entry.at("maxEl").get<double>();
            if (start >= now && std::difftime(start, now) < 86400 && elevation > 40) {
                passes.push_back({name, start, end, elevation});
            }
        }
    } catch (const nlohmann::json::exception&) {
        throw std::runtime_error(std::string(name) + ": invalid N2YO pass data");
    }
}
} // namespace

std::vector<Pass> fetchMeteorPasses() {
    const auto apiKey = setting("N2YO_API_KEY");
    std::ostringstream observer;
    observer.imbue(std::locale::classic());
    observer.precision(10);
    const char* altitude = std::getenv("N2YO_ALTITUDE");
    observer << coordinate("N2YO_LATITUDE", -90, 90) << '/'
             << coordinate("N2YO_LONGITUDE", -180, 180) << '/'
             << (altitude && *altitude ? coordinate("N2YO_ALTITUDE", -12000, 100000) : 0);

    const auto now = std::time(nullptr);
    std::vector<Pass> passes;
    fetchPasses(57166, "METEOR M2-3", observer.str(), apiKey, now, passes);
    fetchPasses(59051, "METEOR M2-4", observer.str(), apiKey, now, passes);
    return passes;
}
