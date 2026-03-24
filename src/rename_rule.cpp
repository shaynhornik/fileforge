#include "fileforge/rename_rule.h"
#include "fileforge/scanner.h"

#include <algorithm>
#include <cctype>
#include <set>

namespace fileforge {

std::string apply_default_rule(const std::string& filename) {
    std::filesystem::path p(filename);
    std::string stem = p.stem().string();
    std::string ext = p.extension().string();

    std::string result;
    result.reserve(stem.size());

    for (char c : stem) {
        if (c == ' ' || c == '_') {
            result += '-';
        } else {
            result += static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        }
    }

    return result + ext;
}

std::vector<RenameEntry> plan_renames(const std::filesystem::path& directory) {
    auto files = scan_directory(directory);
    std::vector<RenameEntry> plan;
    std::set<std::string> seen;

    for (const auto& file : files) {
        std::string original_name = file.filename().string();
        std::string new_name = apply_default_rule(original_name);

        RenameEntry entry;
        entry.original = file;
        entry.renamed = file.parent_path() / new_name;
        entry.conflict = !seen.insert(new_name).second;

        if (original_name != new_name) {
            plan.push_back(entry);
        }
    }

    return plan;
}

int execute_renames(const std::vector<RenameEntry>& plan) {
    int count = 0;
    for (const auto& entry : plan) {
        if (entry.conflict) {
            continue;
        }
        std::filesystem::rename(entry.original, entry.renamed);
        ++count;
    }
    return count;
}

} // namespace fileforge
