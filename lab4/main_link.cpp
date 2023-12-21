#include "libs.h"
#include <iostream>

void menu()
{
    std::cout << "Enter number for command: " << std::endl;
    std::cout << "1. e" << std::endl;
    std::cout << "2. square" << std::endl;
    std::cout << "3. exit" << std::endl;
}

int main()
{
    int flag;
    menu();
    std::cin >> flag;
    while (flag != 3)
    {
        if (flag == 1)
        {
            int x;
            std::cout << "Enter x: " << std::endl;
            std::cin >> x;
            std::cout << "e = " << e(x) << std::endl;
        }
        else if (flag == 2)
        {
            int a, b;
            std::cout << "Enter a and b: " << std::endl;
            std::cin >> a >> b;
            std::cout << "Square of triangle = " << square(a, b) << std::endl;
        }
        else
        {
            throw std::logic_error("Wrong value");
        }
        menu();
        std::cin >> flag;
    }
    return 0;
}