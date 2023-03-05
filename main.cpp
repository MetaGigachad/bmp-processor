#include <exception>
#include <iostream>

#include "app.h"

int main(int argc, const char* argv[]) {
    App app(argc, argv);

    try {
        app.Run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unexpected error occurred!" << std::endl;
    }

    return 0;
}


