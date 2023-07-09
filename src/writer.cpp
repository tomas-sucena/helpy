#include <fstream>

#include "writer.h"

namespace Helpy {
    Writer::Writer(const std::string &path) : file(path) {}

    void Writer::write() {
        file << "Hey!";
    }
}
