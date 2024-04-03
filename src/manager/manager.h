#ifndef HELPY_MANAGER_H
#define HELPY_MANAGER_H

#include <string>

namespace Helpy {
    class Manager {
    /* METHODS */
    private:
        static std::string readInput(const std::string &instr);
        static void formatDirname(std::string &path);
        static bool createDirectory(std::string &path);
        static void writeHelpyfileTemplate(const std::string &path);

    public:
        static void init(std::string outputDir);
        static void run(std::string path, std::string outputDir);
    };
}

#endif //HELPY_MANAGER_H
