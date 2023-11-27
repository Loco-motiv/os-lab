#include <iostream> 
#include <unistd.h> 
#include <pthread.h>
#include <vector>
#include <chrono>
#include <algorithm>

using namespace std;

struct arg {
    int tid;
    int thread_count;
    vector<vector<int>> *matrix;
    vector<vector<int>> *result;
    int rows_m;
    int columns_m;
    int rows_w;
    int columns_w;
    int rep;
    pthread_barrier_t *barrier;
};

void rotatematrix(vector<vector<int>> &matrix){
    vector<vector<int>> result(matrix[0].size());
    for(long unsigned int i = 0; i < matrix[0].size(); ++i){
        for (long unsigned int j = 0; j < matrix.size(); ++j){
            result[i].push_back(matrix[j][i]);
        }
    }
    matrix = result;
}

void* solve(void* args){
    arg* l_args = (arg*)args;
    int winc =  l_args->columns_w * l_args->rows_w;
    vector<int> win(winc);
    int edge_r = l_args->rows_w / 2;
    int edge_c = l_args->columns_w / 2;
    for(int r = 0; r < l_args->rep; ++r){
        for(int x = edge_r + l_args->tid; x < l_args->rows_m - edge_r; x += l_args->thread_count){
            for(int y = edge_c; y < l_args->columns_m - edge_c; ++y){
                for (int i = 0, f_x = 0; f_x < l_args->rows_w; ++f_x){
                    for (int f_y = 0; f_y < l_args->columns_w; ++f_y){
                        win[i++] = (*l_args->matrix)[x + f_x - edge_r][y + f_y - edge_c];
                    }
                }
                sort(win.begin(), win.end());
                if (winc % 2 == 1){
                    (*l_args->result)[x][y] = win[winc / 2];
                }
                else{
                    (*l_args->result)[x][y] = (win[(winc / 2) - 1] + win[(winc / 2)]) / 2;
                }
            }
        }
        pthread_barrier_wait(l_args->barrier);
        if (l_args->tid == 0){
            *l_args->matrix = *l_args->result;
        }
        pthread_barrier_wait(l_args->barrier);
    }
    return NULL;
}

int main(int argc, char* argv[]) { 

    if (argc != 2){
        cout << "Требуется количество потоков" << endl;
        return 1;
    }
    // int thread_count = atoi(argv[1]);
    // if (thread_count < 1){
    //     cout << "Количество потоков не может быть меньше 1" << endl;
    //     return 1; 
    // }

    char *filename = NULL;
    char c; 
    int len = 1; 
    filename = (char*) malloc(sizeof(char)); 
    // cout << "Введите имя файла: " << endl;
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
    
    FILE *file = fopen(filename, "w");
    if (file == NULL) {  
        perror("Ошибка открытия файла");  
        exit(1);  
    }
    
    int rows_m, columns_m, rows_w, columns_w, rep;

    // cout << "Введите размеры матрицы(количество строк/количество стобцов)" << endl;
    cin >> rows_m >> columns_m;
    vector<vector<int>> source(rows_m, vector<int>(columns_m));

    // cout << "Введите матрицу" << endl;
    for (int i = 0; i < rows_m; ++i){
        for (int y = 0; y < columns_m; ++y){
            cin >> source[i][y];
        }
    }

    // cout << "Введите размеры окна(количество строк/количество стобцов)" << endl;
    cin >> rows_w >> columns_w;

    int flag_if_rotated = 0;
    if(rows_m < columns_m){
        flag_if_rotated = 1;
        rotatematrix(source);
        swap(rows_w, columns_w);
        swap(rows_m, columns_m);
    }

    // cout << "Введите количество применений фильтра" << endl;
    cin >> rep;

    vector<int> thread_count = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 16, 24, 32, 42, 48, 54, 60, 64, 128, 256, 512, 1024};

    for (int idd : thread_count){
    vector<pthread_t> tid(idd);

    pthread_barrier_t barrier;
    pthread_barrier_init(&barrier, NULL, idd);

 
    auto begin{chrono::steady_clock::now()}; 

    vector<arg> Arg(idd);
    vector<vector<int>> result = source;

    for (int i = 0; i < idd; ++i){
        Arg[i].barrier = &barrier;
        Arg[i].tid = i;
        Arg[i].thread_count = idd;
        Arg[i].rows_w = rows_w;
        Arg[i].rows_m = rows_m;
        Arg[i].rep = rep;
        Arg[i].matrix = &source;
        Arg[i].result = &result;
        Arg[i].columns_w = columns_w;
        Arg[i].columns_m = columns_m;
        pthread_create(&tid[i], NULL, solve, (void *)&Arg[i]);
    }

    for (int i = 0; i < idd; ++i){
        pthread_join(tid[i], NULL);
    }
    
    auto end{chrono::steady_clock::now()}; 
    auto elapsed_ms{chrono::duration_cast<chrono::milliseconds>(end - begin)}; 
    cout << idd << " " << elapsed_ms.count() << endl;
    source = *Arg[0].matrix;
    }


    if(flag_if_rotated){
        rotatematrix(source);
        swap(rows_m, columns_m);
    }

    for (int i = 0; i < rows_m; ++i){
        for (int y = 0; y < columns_m; ++y){
            fprintf(file, "%d ", source[i][y]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
    return 0;
}