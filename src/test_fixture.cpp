#include "fileforge/test_fixture.h"

#include <array>
#include <filesystem>
#include <fstream>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

namespace fs = std::filesystem;

namespace fileforge {
namespace {

std::string random_token(std::mt19937& rng, int min_len = 4, int max_len = 10) {
    static constexpr char chars[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::uniform_int_distribution<int> len_dist(min_len, max_len);
    std::uniform_int_distribution<int> char_dist(0, static_cast<int>(sizeof(chars) - 2));

    std::string out;
    out.reserve(static_cast<std::size_t>(max_len));
    int len = len_dist(rng);
    for (int i = 0; i < len; ++i) {
        out.push_back(chars[char_dist(rng)]);
    }
    return out;
}

std::string category_for_extension(const std::string& ext) {
    static const std::unordered_map<std::string, std::string> mapping = {
        {".txt", "documents"},
        {".md", "documents"},
        {".pdf", "documents"},
        {".csv", "data"},
        {".json", "data"},
        {".png", "images"},
        {".jpg", "images"},
        {".jpeg", "images"},
        {".gif", "images"},
        {".mp3", "audio"},
        {".wav", "audio"},
        {".cpp", "code"},
        {".h", "code"},
        {".py", "code"},
        {".zip", "archives"},
        {".tar", "archives"},
    };

    auto it = mapping.find(ext);
    if (it != mapping.end()) {
        return it->second;
    }
    return "misc";
}

void write_text_file(const fs::path& path, const std::string& content) {
    fs::create_directories(path.parent_path());
    std::ofstream out(path, std::ios::binary);
    out << content;
}

} // namespace

void generate_fixture(const fs::path& root, const FixtureOptions& options) {
    fs::remove_all(root);
    fs::create_directories(root);

    std::mt19937 rng(options.seed);
    const std::array<std::string, 14> extensions = {
        ".txt", ".md", ".pdf", ".csv", ".json", ".png", ".jpg",
        ".gif", ".mp3", ".wav", ".cpp", ".py", ".zip", ".bin"
    };

    std::uniform_int_distribution<int> ext_dist(0, static_cast<int>(extensions.size() - 1));

    for (int i = 0; i < options.top_level_files; ++i) {
        std::string base = random_token(rng) + " " + random_token(rng);
        fs::path file = root / (base + extensions[ext_dist(rng)]);
        write_text_file(file, "top-level fixture file\n");
    }

    for (int dir_index = 0; dir_index < options.nested_directories; ++dir_index) {
        fs::path subdir = root / ("mixed folder " + std::to_string(dir_index + 1));
        fs::create_directories(subdir);

        for (int file_index = 0; file_index < options.nested_files_per_directory; ++file_index) {
            std::string base = random_token(rng) + "_" + random_token(rng);
            fs::path file = subdir / (base + extensions[ext_dist(rng)]);
            write_text_file(file, "nested fixture file\n");
        }
    }
}

int sort_directory_by_extension(const fs::path& root) {
    if (!fs::exists(root)) {
        return 0;
    }

    std::vector<fs::path> files;
    for (const auto& entry : fs::recursive_directory_iterator(root)) {
        if (entry.is_regular_file()) {
            files.push_back(entry.path());
        }
    }

    int moved = 0;
    for (const auto& file : files) {
        fs::path rel = fs::relative(file, root);
        if (!rel.empty() && rel.begin() != rel.end()) {
            const std::string first = (*rel.begin()).string();
            if (first == "sorted") {
                continue;
            }
        }

        std::string category = category_for_extension(file.extension().string());
        fs::path destination_dir = root / "sorted" / category;
        fs::create_directories(destination_dir);

        fs::path destination = destination_dir / file.filename();
        int suffix = 1;
        while (fs::exists(destination)) {
            destination = destination_dir /
                (file.stem().string() + "-" + std::to_string(suffix++) + file.extension().string());
        }

        fs::rename(file, destination);
        ++moved;
    }

    return moved;
}

} // namespace fileforge
