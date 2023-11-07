#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
  
int main(int argc, char *argv[]) {    
    FILE *file = fopen(argv[1], "w");  
    if (file == NULL) {  
        perror("Ошибка открытия файла");  
        exit(1);  
    }  
  
    int number;  
    while (read(0, &number, sizeof(int)) > 0) {  
        int count = number;  
        int *numbers = malloc(count * sizeof(int));  
        if (numbers == NULL) {  
            perror("Ошибка выделения памяти");  
            exit(1);  
        }  
        for (int i = 0; i < count; i++) {  
            read(0, &number, sizeof(int));  
            numbers[i] = number;  
        }  
  
        if (count < 2) {  
            fprintf(stderr, "Недостаточное количество чисел в команде\n");  
            number = -2;  
            write(1, &number, sizeof(int));  
            continue;  
        }  
        int result = numbers[0];  
        for (int i = 1; i < count; i++) {  
            if (numbers[i] == 0) {  
                number = -1;  
                write(1, &number, sizeof(int));  
                exit(1);  
            }  
            result /= numbers[i];  
        }  
  
        fprintf(file, "%d\n", result);  
        write(1, &result, sizeof(int));  
  
        free(numbers);  
    }  
  
    fclose(file);  
    exit(0);  
}