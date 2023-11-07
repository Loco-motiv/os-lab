#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/wait.h> 
#include <string.h>
 
int main() { 
    int pipe1[2], pipe2[2]; 
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) { 
        perror("Ошибка создания канала"); 
        exit(1); 
    } 
 
    char *filename = NULL;
    char c; 
    int len = 1; 
    filename = (char*) malloc(sizeof(char)); 
    printf("Введите имя файла: \n"); 
    while((c = getchar()) != '\n') { 
        filename[len - 1] = c; 
        len++;
        if (len > 255) {
            perror("Ошибка создания файла"); 
            exit(1); 
        } 
        filename = (char*) realloc(filename, len); 
    } 
    filename[len - 1] = '\0';
    
    pid_t pid = fork(); 
    if (pid == -1) { 
        perror("Ошибка создания процесса"); 
        exit(1); 
    } 
 
    if (pid == 0) { // Дочерний процесс 
        close(pipe1[1]); 
        close(pipe2[0]); 
 
        dup2(pipe1[0], 0); 
 
        dup2(pipe2[1], 1); 
 
        execl("child.out", "child.out", filename, NULL); 
 
        perror("Ошибка запуска программы дочернего процесса"); 
        exit(1); 
    } 
    else { // Родительский процесс 
        close(pipe1[0]); 
        close(pipe2[1]); 
 
        int number;  
        while(1){
            printf("Введите команду: \n");  
            int count = 1;
            int num; 
            char ch; 
            int *array = NULL;
            array = (int*) malloc(sizeof(int)); 
            while (1) { 
                if (scanf("%d", &num) == EOF){
                    exit(0);
                }  
                array[count - 1] = num;     
                scanf("%c", &ch);  
                if (ch == '\n') {  
                    break; 
                } 
                count++;
                array = (int*) realloc(array, count); 
            } 
            write(pipe1[1], &count, sizeof(int));    
            write(pipe1[1], array, sizeof(int) * count);
            read(pipe2[0], &number, sizeof(int));  
            printf("Данные из дочернего процесса: %d\n", number);  

            if (number == -1) {  
                printf("Деление на ноль! Завершение работы.\n");  
                exit(1);  
            }  
            if (number == -2) {  
                printf("Недостаточное количество чисел в команде\n");  
                exit(1);  
            }  
        }
    }  
}