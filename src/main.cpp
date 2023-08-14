#include <cstring>
#include <stdexcept>

#include "manager/manager.h"

int main(int argc, char *argv[]) {
    if (argc < 2) return 0;

    if (!strcmp(argv[1], "init"))
        Helpy::Manager::init((argc < 3) ? "helpy" : argv[2]);
    else if (!strcmp(argv[1], "run"))
        Helpy::Manager::run((argc < 3) ? "" : argv[2], (argc < 4) ? "Helpyfile" : argv[3]);
    else {
        std::string command(argv[1]);
        throw std::runtime_error("Error: Undefined command '" + command + "'!");
    }

    return 0;
}
