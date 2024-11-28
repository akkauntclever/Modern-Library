//
//  main.cpp
//  modern_library
//
//  Created by Иван Трифонов on 17.09.2024.
//  Copyright © 2024 Иван Трифонов. All rights reserved.
//

#include "Library.hpp"

int main(int argc, const char * argv[]) {
    #if defined(_WIN32)
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    #endif
    setlocale(LC_ALL, "rus");
    std::cout << "\nСистема для учета книг в библиотеке\nModern Library" << std::endl;
    Library library;
    try {
        if (argc == 1) {
            library.one_argument();
        }
        if (argc == 2) {
            library.two_arguments(argv[1]);
        }
        if (argc > 2) {
            throw (char*)"Открытие нескольких систем для учета книг в библиотеке не поддерживается.";
        }
        commands();
        while (true) {
            library.terminal();
        }
    } catch (char* str) {
        std::cout << str << std::endl;
        return 0;
    }
    return 0;
}
