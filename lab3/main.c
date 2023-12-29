#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/wait.h> 
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
 
int main() { 
    char *filename = NULL;
    char c; 
    int len = 1;        
    filename = (char*) malloc(sizeof(char)); 
    //printf("Введите имя файла: \n"); 
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
    
    int *array = NULL;
    array = (int*) malloc(sizeof(int)); 
    int count = 1;

    //while(1){
        //printf("Введите команду: \n");  
    int num; 
    char ch; 
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
    

    int fd = shm_open("/shd", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("Ошибка при открытии shared memory файла");
        return 1;
    }
    ftruncate(fd, count * sizeof(int));

    int *map = (int*)mmap(0, count * sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) {
        close(fd);
        perror("Ошибка мапинига");
        exit(1);
    }

    memcpy(map, array, count * sizeof(int));

    pid_t pid = fork(); 
    if (pid == -1) { 
        perror("Ошибка создания процесса"); 
        exit(1); 
    } 

    int mem = map[0];

    if (pid == 0) { // Дочерний процесс 
        execl("child.out", "child.out", filename, NULL); 
        perror("Ошибка запуска программы дочернего процесса"); 
        exit(1); 
    } 
    else { // Родительский процесс 
        while(1){  
            if (mem < map[0]) {  
                printf("Деление на ноль! Завершение работы.\n");  
                exit(1);  
            }  
            else if (mem > map[0]){
                exit(0);
            }
        }
    }  
}
