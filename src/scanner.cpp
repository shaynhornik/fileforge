#include "fileforge/scanner.h"

#include <algorithm>

namespace fileforge {

std::vector<std::filesystem::path> scan_directory(const std::filesystem::path& directory) {
    std::vector<std::filesystem::path> files;

    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (entry.is_regular_file()) {
            files.push_back(entry.path());
        }
    }

    std::sort(files.begin(), files.end());
    return files;
}

} // namespace fileforge
