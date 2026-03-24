#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace fileforge {

struct RenameEntry {
    std::filesystem::path original;
    std::filesystem::path renamed;
    bool conflict = false;
};

// Applies the default rename rule to a filename:
// lowercase the stem, replace spaces with dashes, preserve extension.
std::string apply_default_rule(const std::string& filename);

// Generate a rename plan for all regular files in a directory.
std::vector<RenameEntry> plan_renames(const std::filesystem::path& directory);

// Execute a rename plan. Returns the number of files renamed.
int execute_renames(const std::vector<RenameEntry>& plan);

} // namespace fileforge
