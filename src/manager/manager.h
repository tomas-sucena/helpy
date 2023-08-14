#ifndef HELPY_MANAGER_H
#define HELPY_MANAGER_H

#include <string>

namespace Helpy {
    class Manager {
    /* METHODS */
    private:
        static void formatPath(std::string &path);
        static void writeHelpyfileTemplate(const std::string &path);

    public:
        static void init(std::string path);
        static void run(std::string path, std::string filename);
    };
}

#endif //HELPY_MANAGER_H
