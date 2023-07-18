#include <stdexcept>

#include "manager/manager.h"

int main(int argc, char *argv[]) {
    if (argc < 2) return 0;
    std::string command(argv[1]);

    if (command == "init")
        Helpy::Manager::init((argc < 3) ? "" : argv[2], (argc < 4) ? "helpy" : argv[3]);
    else if (command == "run")
        Helpy::Manager::run((argc < 3) ? "" : argv[2], (argc < 4) ? "Helpyfile" : argv[3]);
    else
        throw std::runtime_error("Error: Undefined command '" + command + "'!");

    return 0;
}
