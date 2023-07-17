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

    /* METHODS */
    private:
        // header
        void writeHeaderIncludes();
        void writeMethodsDeclaration();
        void writeClass();

        // source
        void writeSourceIncludes();
        void writeKeywordMaps();
        void writeMethodsDefinition();

        void writeHeader();
        void writeSource();

    public:
        void execute();
    };
}

#endif //HELPY_WRITER_H
