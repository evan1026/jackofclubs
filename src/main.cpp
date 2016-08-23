#include <iostream>

#include "Game.h"

int doMain(){

    Game g;
    g.run();

    return 0;
}

int main() {
    try {
        return doMain();
    } catch (std::exception& e) {
        std::cout << std::endl;
        std::cout << "***************************************" << std::endl;
        std::cout << "* Exception uncaught. Must terminate. *" << std::endl;
        std::cout << "*      Exception details follow       *" << std::endl;
        std::cout << "***************************************" << std::endl;
        std::cout << e.what() << std::endl;
        std::cout << "If you're seeing this in production, please screenshot and send to the developer." << std::endl;
        return 1;
    }
}

