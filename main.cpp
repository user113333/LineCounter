#include <dirent.h>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sys/stat.h>

long lines = 0;
long files = 0;
bool error = false;

long FileLines(std::string path) {
    std::ifstream file(path);
    return std::count(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>(), '\n');
}

void FindFiles(std::string path) {
    DIR* dir;
    struct dirent* ent;
    if ((dir = opendir(path.c_str())) != nullptr) {
        while ((ent = readdir(dir)) != nullptr) {
            if (std::string(ent->d_name) == ".." || std::string(ent->d_name) == "." || std::string(ent->d_name) == "LC")
                continue;

            std::string filePath = path + std::string(ent->d_name);

            struct stat s;
            if (stat(filePath.c_str(), &s) == 0) {
                if (s.st_mode & S_IFDIR) {
                    FindFiles(filePath + "/");
                    continue;
                }
            } else {
                std::cout << std::endl << "ERROR: Cant read path " << filePath << std::endl;
                error = true;
                return;
            }

            long fileLines = FileLines(path + ent->d_name);
            lines += fileLines;
            files++;
            std::cout << fileLines << "\t" << path + ent->d_name << std::endl;
        }

        closedir(dir);
    } else {
        perror("");
        std::cout << "EXIT_FAILURE";
    }
}

int main() {
    FindFiles("./");

    if (error)
        return EXIT_FAILURE;

    std::cout << std::endl << lines << " lines of code in " << files << " files" << std::endl << std::endl;
    return EXIT_SUCCESS;
}
