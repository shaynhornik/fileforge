#pragma once

#include <filesystem>

namespace fileforge {

struct FixtureOptions {
    int seed = 1337;
    int top_level_files = 12;
    int nested_directories = 4;
    int nested_files_per_directory = 5;
};

// Create a messy, repeatable directory tree for testing rename/sort behavior.
void generate_fixture(const std::filesystem::path& root, const FixtureOptions& options = {});

// Sort files from a messy directory tree into categorized folders by extension.
// Returns the number of files moved.
int sort_directory_by_extension(const std::filesystem::path& root);

} // namespace fileforge
