#include <iostream>
#include <filesystem>
#include <string>
#include <map>
#include <vector>

namespace fs = std::filesystem;

// Map of file extensions to folder names
const std::map<std::string, std::string> CATEGORIES = {
    {".jpg", "Images"}, {".jpeg", "Images"}, {".png", "Images"}, {".gif", "Images"},
    {".mp3", "Music"}, {".wav", "Music"}, {".flac", "Music"},
    {".mp4", "Videos"}, {".mov", "Videos"}, {".avi", "Videos"},
    {".pdf", "Documents"}, {".docx", "Documents"}, {".txt", "Documents"}, {".xlsx", "Documents"},
    {".zip", "Archives"}, {".rar", "Archives"}, {".7z", "Archives"},
    {".exe", "Programs"}, {".msi", "Programs"},
    {".cpp", "Code"}, {".h", "Code"}, {".py", "Code"}, {".java", "Code"}
};

void organize_files(const fs::path& directory) {
    int moved_files = 0;
    int skipped_files = 0;

    for (const auto& entry : fs::directory_iterator(directory)) {
        if (fs::is_regular_file(entry)) {
            const auto& path = entry.path();
            std::string extension = path.extension().string();
            
            // Get category or use "Other" for unknown types
            std::string category = "Other";
            if (CATEGORIES.find(extension) != CATEGORIES.end()) {
                category = CATEGORIES.at(extension);
            }

            // Create target directory if it doesn't exist
            fs::path target_dir = directory / category;
            if (!fs::exists(target_dir)) {
                fs::create_directories(target_dir);
            }

            // Move file to new location
            fs::path new_path = target_dir / path.filename();
            try {
                fs::rename(path, new_path);
                std::cout << "Moved: " << path.filename() << " -> " << category << "\n";
                moved_files++;
            } catch (const fs::filesystem_error& e) {
                std::cerr << "Error moving " << path.filename() << ": " << e.what() << "\n";
                skipped_files++;
            }
        }
    }

    std::cout << "\nOrganization complete!\n";
    std::cout << "Moved files: " << moved_files << "\n";
    std::cout << "Skipped files: " << skipped_files << "\n";
}

int main() {
    std::string path;
    std::cout << "Enter directory to organize (leave empty for current directory): ";
    std::getline(std::cin, path);

    if (path.empty()) {
        path = fs::current_path().string();
    }

    if (!fs::exists(path)) {
        std::cerr << "Error: Directory does not exist!\n";
        return 1;
    }

    organize_files(path);
    return 0;
}
