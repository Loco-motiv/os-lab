#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
  
int main(int argc, char *argv[]) {    
    FILE *file = fopen(argv[1], "w");  
    if (file == NULL) {  
        perror("Ошибка открытия файла");  
        exit(1);  
    }  
    
    int fd = shm_open("/shd", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("Ошибка при открытии shared memory файла");
        return 1;
    }

    struct stat st;
    if (fstat(fd, &st) == -1) {
        perror("Ошибка при получении информации о файле");
        return 1;
    }
    int count = st.st_size;

    int *map = (int*)mmap(0, count * sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) {
        close(fd);
        perror("Ошибка мапинига");
        exit(1);
    }


    int result = map[0];  
    for (int i = 1; i < count/sizeof(int); i++) {  
        if (map[i] == 0) {            
            map[0] += 1;
            exit(1);  
        }  
        result /= map[i];  
    }  

    fprintf(file, "%d\n", result);  
    fclose(file);
    map[0] -= 1;
    if (munmap(map, count * sizeof(int)) == -1) {
        perror("Ошибка анмапинга");
    }
    exit(0);  
}