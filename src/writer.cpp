#include <fstream>

#include "writer.h"

#define LINE_BREAK std::endl << std::endl

namespace Helpy {
    Writer::Writer(const std::string &path) : file(path) {}

    void Writer::writeName() {
        file << "/*\n"
             << "This will be the name of your class. It will also be used to name the generated code files.\n"
             << "By default, the class is named Helpy and the code files are named helpy.h and helpy.cpp.\n"
             << "\n"
             << "Note: The naming conventions used are CamelCase (class) and snake_case (code files).\n"
             << "*/\n";

        file << "NAME: Helpy" << LINE_BREAK;
    }

    void Writer::write() {
        writeName();
    }
}
