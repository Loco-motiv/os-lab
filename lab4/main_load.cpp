#include "libs.h"
#include <iostream>
#include <dlfcn.h> 

int impl = 0; 
void *library = NULL;
float (*e)(int); 
float (*square)(float, float); 
 
 
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
 
    e = (float(*)(int))dlsym(library, "e"); 
    square = (float(*)(float, float))dlsym(library, "square"); 
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



#include <iostream> 
#include <dlfcn.h> 
 
typedef double (*EFunction)(int); 
typedef int* (*SortFunction)(int, int*); 
 
EFunction E = nullptr; 
SortFunction Sort = nullptr; 
void* libraryHandle = nullptr; 
 
void load_lib(const char* file) { 
    libraryHandle = dlopen(file, RTLD_LAZY); 
    if (!libraryHandle) { 
        throw  std::runtime_error(dlerror()); 
    } 
 
    E = (EFunction) dlsym(libraryHandle, "e"); 
    if (!E) { 
        throw  std::runtime_error(dlerror()); 
    } 
    Sort = (SortFunction) dlsym(libraryHandle, "square"); 
    if (!Sort) { 
        throw  std::runtime_error(dlerror()); 
    } 
} 
 
int main() { 
    load_lib("libimpl1.so"); 
 
    int current_impl{1}; 
    int n; 
 
    while (std::cin >> n) { 
        if (n == 0) { 
            dlclose(libraryHandle); 
            if (current_impl == 1) { 
                load_lib("libimpl2.so"); 
                current_impl = 2; 
            } else { 
                load_lib("libimpl1.so"); 
                current_impl = 1; 
            } 
 
        } else if (n == 1) { 
            int x; 
            std::cin >> x; 
            double res = E(x); 
            std::cout << res << std::endl; 
        } else if (n == 2) { 
            int size; 
            std::cin >> size; 
 
            int* arr = new int[size]; 
            for (int i{0}; i < size; ++i) { 
                std::cin >> arr[i]; 
            } 
 
            arr = Sort(size, arr); 
 
            for (int i{0}; i < size; ++i) { 
                std::cout << arr[i] << " "; 
            } 
            std::cout << std::endl; 
 
            delete[] arr; 
        } else { 
            break; 
        } 
    } 
    dlclose(libraryHandle); 
 
    return 0; 
}



#include <stdio.h> 
#include <stdlib.h> 
 
 
#define crash_on(boolean) if (boolean) { fprintf(stderr, "Error at file %s, line %d\n", __FILE__, __LINE__); exit(1); } 
#define handle(i) crash_on(i == -1) 
 
static int impl = 0; 
static void *library = NULL; 
 
static int (*prime_count)(int, int); 
static char* (*to_base)(long); 
 
 
void load_lib() { 
    if (impl == 0) { 
        library = dlopen("./libimpl1.so", RTLD_LAZY); 
    } else { 
        library = dlopen("./libimpl2.so", RTLD_LAZY); 
    } 
    crash_on(library == NULL); 
 
    prime_count = (int(*)(int, int))dlsym(library, "prime_count"); 
    to_base = (char*(*)(long))dlsym(library, "to_base"); 
} 
 
 
int main() { 
    int run = 1; 
 
    load_lib(); 
 
    while (run) { 
        int command = 0; 
        printf( 
            "Usage:\n" 
            "\t\b-1 - exit\n" 
            "\t0 - change implementation\n" 
            "\t1 - count primes\n" 
            "\t2 - rebase a number\n" 
            "Command: " 
        ); 
        scanf("%d", &command); 
 
        if (command == -1) { 
            run = 0; 
        } else if (command == 0) { 
            // change implementation 
            impl = 1 - impl; 
            dlclose(library); 
            load_lib(); 
        } else if (command == 1) { 
            // prime 
            int a = 0, b = 0; 
            printf( 
                "Input A and B to find out how many primes are in [A, B]:\n" 
                "\tA: " 
            ); 
            scanf("%d", &a); 
            printf("\tB: "); 
            scanf("%d", &b); 
 
            int result = 0; 
 
            result = prime_count(a, b); 
 
            printf("There are %d primes in [%d, %d]!\n", result, a, b); 
        } else if (command == 2) { 
            // rebase 
            int n = 0; 
            printf("Input a number you want to rebase to base %d: ", impl + 2); 
            scanf("%d", &n); 
 
            char* result = NULL; 
 
            result = to_base(n); 
 
            printf("The %d in base %d is %s!\n", n, impl + 2, result); 
            free(result); 
        } 
    } 
 
 
    return 0; 
}