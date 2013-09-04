#include "ui/CodeCancer.hpp"

#include <iostream>
#include <stdexcept>

int main(int argc, char** argv) {
    CodeCancer app;
    try {
        app.run(argc, argv);
    } catch (std::exception const& ex) {
        std::cerr << ex.what() << "\n";
        return 1;
    }

    return 0;
}
