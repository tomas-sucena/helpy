#include <iostream>

#include "src/writer.h"

void init(std::string path, const std::string &filename = "Helpyfile") {
    std::string path_(std::move(path));

    if (!path_.empty() && path_.back() != '/')
        path_.push_back('/');

    Helpy::Writer writer(path_ + filename);
    writer.write();
}

int main(int argc, char **argv) {
    init("..");
}
