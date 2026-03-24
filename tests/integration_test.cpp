#include "fileforge/test_fixture.h"

#include <filesystem>
#include <iostream>
#include <map>
#include <set>
#include <string>

namespace fs = std::filesystem;

int main() {
    const fs::path root = fs::temp_directory_path() / "fileforge-fixture-test";
    fileforge::generate_fixture(root);

    const int moved = fileforge::sort_directory_by_extension(root);
    if (moved <= 0) {
        std::cerr << "Expected files to be moved, but moved=" << moved << "\n";
        return 1;
    }

    const std::map<std::string, std::set<std::string>> allowed = {
        {"documents", {".txt", ".md", ".pdf"}},
        {"data", {".csv", ".json"}},
        {"images", {".png", ".jpg", ".jpeg", ".gif"}},
        {"audio", {".mp3", ".wav"}},
        {"code", {".cpp", ".h", ".py"}},
        {"archives", {".zip", ".tar"}},
        {"misc", {".bin"}},
    };

    for (const auto& entry : fs::recursive_directory_iterator(root / "sorted")) {
        if (!entry.is_regular_file()) {
            continue;
        }

        fs::path rel = fs::relative(entry.path(), root / "sorted");
        auto it = rel.begin();
        if (it == rel.end()) {
            std::cerr << "Unexpected empty relative path for sorted file\n";
            return 1;
        }

        const std::string category = (*it).string();
        const std::string extension = entry.path().extension().string();
        auto allowed_it = allowed.find(category);
        if (allowed_it == allowed.end()) {
            std::cerr << "Unexpected category: " << category << "\n";
            return 1;
        }
        if (!allowed_it->second.count(extension)) {
            std::cerr << "File with extension " << extension << " landed in wrong category " << category << "\n";
            return 1;
        }
    }

    std::cout << "Integration test passed. Moved " << moved << " files into expected categories.\n";
    fs::remove_all(root);
    return 0;
}
