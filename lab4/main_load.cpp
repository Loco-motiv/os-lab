#include "libs.h"
#include <iostream>
#include <dlfcn.h> 

int impl = 0; 
void *library = NULL;
typedef float (*EFunc)(int);
typedef float* (*SquareFunc)(float, float);
 
void load_lib() { 
    if (impl == 0) { 
        library = dlopen("./liblib1.so", RTLD_LAZY); 
    } else { 
        library = dlopen("./liblib2.so", RTLD_LAZY);
    } 
    if (library == NULL){
        std::cout << "Error with library handler" << std::endl;
        exit(1);
    } 
    EFunc e = (EFunc)dlsym(library, "e"); 
    SquareFunc square = (SquareFunc)dlsym(library, "square"); 
    if (!square) {
        std::cerr << "Cannot load symbol square: " << dlerror() << std::endl;
        dlclose(library);
        exit(1);
    }
    if (!e) {
        std::cerr << "Cannot load symbol e: " << dlerror() << std::endl;
        dlclose(library);
        exit(1);
    }
} 
 


void menu()
{
    std::cout << "Enter number for command: " << std::endl;
    std::cout << "0. Switch implementation" << std::endl;
    std::cout << "1. e" << std::endl;
    std::cout << "2. Square" << std::endl;
    std::cout << "3. Exit" << std::endl;
}

int main()
{
    int flag;
    load_lib();
    menu();
    std::cin >> flag;
    while (flag != 3)
    {
        if (flag == 0){
            impl = 1 - impl; 
            dlclose(library); 
            load_lib(); 
        }
        else if (flag == 1)
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