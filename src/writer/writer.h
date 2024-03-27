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
        void writeHeaderGuards();
        void writeIncludes();

        // header
        void writeMethodsDeclaration();
        void writeClass();

        // source
        void writeMacros();
        void writeKeywordMaps();
        void writeUserMethods();
        void writeHelpyMethods();

        void writeUtils();
        void writeHeader();
        void writeSource();

    public:
        void execute();
    };
}

#endif //HELPY_WRITER_H
