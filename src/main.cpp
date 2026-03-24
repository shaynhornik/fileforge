#include "fileforge/rename_rule.h"

#include <cstring>
#include <filesystem>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

static void print_usage(const char* program) {
    std::cerr << "Usage: " << program << " rename <directory> [--apply]\n"
              << "\n"
              << "Commands:\n"
              << "  rename    Preview or apply batch renames in a directory.\n"
              << "            Rule: lowercase stem, replace spaces/underscores with dashes,\n"
              << "            preserve extension.\n"
              << "\n"
              << "Options:\n"
              << "  --apply   Actually rename files (default is dry-run).\n";
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        print_usage(argv[0]);
        return 1;
    }

    std::string command = argv[1];
    if (command != "rename") {
        std::cerr << "Error: unknown command '" << command << "'\n";
        print_usage(argv[0]);
        return 1;
    }

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
