#ifndef HELPY_WRITER_H
#define HELPY_WRITER_H

#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

#include "../parser/parser.h"
#include "../utils/command.hpp"

#define uMap std::unordered_map

namespace Helpy {
    class Writer {
        ParserInfo info;
        std::ofstream header, source, utils;
        std::vector<uMap<std::string, int>> maps;

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
        void writeHelpyMethods();

        void writeHeader();
        void writeSource();
        void writeUtils();

    public:
        void execute();
    };
}

#endif //HELPY_WRITER_H
