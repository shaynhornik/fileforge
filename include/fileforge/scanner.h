#pragma once

#include <filesystem>
#include <vector>

namespace fileforge {

// Collect all regular files in a directory (non-recursive).
std::vector<std::filesystem::path> scan_directory(const std::filesystem::path& directory);

} // namespace fileforge
