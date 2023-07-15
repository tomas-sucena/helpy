#include <chrono>
#include <iostream>

#include "manager/manager.h"

int main(int argc, char **argv) {
    auto start = std::chrono::high_resolution_clock::now();

    Helpy::Manager::run("../cli");

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "\nExecution time: " << duration << "ms" << std::endl;
}
