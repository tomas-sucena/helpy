//
// Created by tosul on 15/07/2023.
//

#include "writer.h"

namespace Helpy {
    Writer::Writer(const std::string &path, ParserInfo info) : info(std::move(info)) {
        header = std::ofstream(path + this->info.name + ".h");
        source = std::ofstream(path + this->info.name + ".cpp");
    }
}