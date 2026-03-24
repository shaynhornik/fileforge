#include "fileforge/rename_rule.h"
#include "fileforge/test_fixture.h"

#include <cstring>
#include <filesystem>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

static void print_usage(const char* program) {
    std::cerr << "Usage: " << program << " <command> [options]\n"
              << "\n"
              << "Commands:\n"
              << "  rename <directory> [--apply]\n"
              << "      Preview or apply batch renames in a directory.\n"
              << "      Rule: lowercase stem, replace spaces/underscores with dashes,\n"
              << "      preserve extension.\n"
              << "\n"
              << "  fixture <directory> [--seed N] [--top-level-files N] [--nested-directories N] [--nested-files-per-directory N]\n"
              << "      Generate a repeatable messy directory tree for testing.\n"
              << "\n"
              << "  sort <directory>\n"
              << "      Sort files recursively into sorted/<category>/ folders by extension.\n";
}

static bool parse_int_arg(const char* value, int& out) {
    try {
        out = std::stoi(value);
        return true;
    } catch (...) {
        return false;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        print_usage(argv[0]);
        return 1;
    }

    std::string command = argv[1];

    if (command == "rename") {
        fs::path directory = argv[2];
        if (!fs::is_directory(directory)) {
            std::cerr << "Error: '" << directory.string() << "' is not a directory.\n";
            return 1;
        }

        bool apply = false;
        for (int i = 3; i < argc; ++i) {
            if (std::strcmp(argv[i], "--apply") == 0) {
                apply = true;
            } else {
                std::cerr << "Error: unknown option '" << argv[i] << "'\n";
                return 1;
            }
        }

        auto plan = fileforge::plan_renames(directory);

        if (plan.empty()) {
            std::cout << "No renames needed — all filenames already match the rule.\n";
            return 0;
        }

        std::cout << (apply ? "[APPLY]" : "[DRY-RUN]")
                  << " Batch rename in: " << fs::canonical(directory).string()
                  << "\n\n";

        for (const auto& entry : plan) {
            std::string tag;
            if (entry.conflict) {
                tag = " [CONFLICT — skipped]";
            }
            std::cout << "  " << entry.original.filename().string()
                      << "  ->  " << entry.renamed.filename().string()
                      << tag << "\n";
        }

        if (!apply) {
            std::cout << "\n" << plan.size() << " file(s) would be renamed. "
                      << "Run with --apply to execute.\n";
            return 0;
        }

        int renamed = fileforge::execute_renames(plan);
        std::cout << "\n" << renamed << " file(s) renamed.\n";
        return 0;
    }

    if (command == "fixture") {
        fs::path directory = argv[2];
        fileforge::FixtureOptions options;

        for (int i = 3; i < argc; ++i) {
            const std::string arg = argv[i];
            if (i + 1 >= argc) {
                std::cerr << "Error: missing value for option '" << arg << "'\n";
                return 1;
            }

            int value = 0;
            if (!parse_int_arg(argv[i + 1], value)) {
                std::cerr << "Error: invalid numeric value '" << argv[i + 1] << "' for option '" << arg << "'\n";
                return 1;
            }

            if (arg == "--seed") {
                options.seed = value;
            } else if (arg == "--top-level-files") {
                options.top_level_files = value;
            } else if (arg == "--nested-directories") {
                options.nested_directories = value;
            } else if (arg == "--nested-files-per-directory") {
                options.nested_files_per_directory = value;
            } else {
                std::cerr << "Error: unknown option '" << arg << "'\n";
                return 1;
            }
            ++i;
        }

        fileforge::generate_fixture(directory, options);
        std::cout << "Generated fixture at: " << directory.string() << "\n";
        std::cout << "Seed: " << options.seed
                  << ", top-level files: " << options.top_level_files
                  << ", nested directories: " << options.nested_directories
                  << ", files per nested directory: " << options.nested_files_per_directory
                  << "\n";
        return 0;
    }

    if (command == "sort") {
        fs::path directory = argv[2];
        if (!fs::is_directory(directory)) {
            std::cerr << "Error: '" << directory.string() << "' is not a directory.\n";
            return 1;
        }

        const int moved = fileforge::sort_directory_by_extension(directory);
        std::cout << "Moved " << moved << " file(s) into sorted category folders under: "
                  << fs::canonical(directory).string() << "\n";
        return 0;
    }

    std::cerr << "Error: unknown command '" << command << "'\n";
    print_usage(argv[0]);
    return 1;
}
