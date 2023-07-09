//
// Created by tosul on 10/07/2023.
//

#ifndef HELPY_WRITER_H
#define HELPY_WRITER_H

#include <fstream>
#include <string>

namespace Helpy {
    class Writer {
        std::ofstream file;

    /* CONSTRUCTOR */
    public:
        explicit Writer(const std::string &path);

    /* METHODS */
    private:
        static void writeName();

    public:
        void write();
    };
}

#endif //HELPY_WRITER_H
