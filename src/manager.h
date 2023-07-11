#ifndef HELPY_MANAGER_H
#define HELPY_MANAGER_H

#include <string>

namespace Helpy {
    class Manager {
        static void writeHelpyfileTemplate(const std::string &path);

    public:
        static void init(std::string path, const std::string &dirname = "helpy");
    };
}

#endif //HELPY_MANAGER_H
