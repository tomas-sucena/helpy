#include <cstring>

#include "manager/manager.h"
#include "utils/utils.hpp"

int main(int argc, char *argv[]) {
    if (argc < 2) return 0;

    if (!strcmp(argv[1], "init"))
        Helpy::Manager::init((argc < 3) ? "helpy" : argv[2]);
    else if (!strcmp(argv[1], "run"))
        Helpy::Manager::run((argc < 3) ? "" : argv[2], (argc < 4) ? "Helpyfile" : argv[3]);
    else
        Helpy::Utils::printError((std::string) "Undefined command '" + argv[1] + "'!");

    return 0;
}
