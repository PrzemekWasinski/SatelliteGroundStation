#include "CsvHandler.hpp"
#include "N2yoClient.hpp"

#include <algorithm>
#include <exception>
#include <iostream>

int main() {
    try {
        auto passes = fetchMeteorPasses();
        std::sort(passes.begin(), passes.end(), [](const Pass& a, const Pass& b) {
            return a.start < b.start;
        });
        const std::filesystem::path output = "data/satellite_passes.csv";
        writePasses(output, passes);
        std::cout << "Wrote " << passes.size() << " passes to " << output.string() << '\n';
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
