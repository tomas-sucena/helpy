#ifndef HELPY_WRITER_H
#define HELPY_WRITER_H

#include <fstream>

#include "../parser/parser.h"
#include "../utils/command.hpp"

namespace Helpy {
    class Writer {
        std::ofstream header, source;
        ParserInfo info;

    /* CONSTRUCTOR */
    public:
        Writer(const std::string &path, ParserInfo info);
    };
}

#endif //HELPY_WRITER_H
