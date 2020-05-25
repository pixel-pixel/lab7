#include <iostream>
#include <fstream>

using namespace std;

//Однозв'язний граф
//Вершини графа - індекси(починаючи від нуля)
//При об'єднанні вершин графу(наприклад переміщуємо вершину 3 до вершини 5),
//то вершина 3 зникає, а індекси пересуваються:
//вершина 4 тепер стає вершиною 3, 5->4, 6->5 і т.д.
//Якщо граф має 3 вершини(0, 1, 2) і ви з'єднуєте наприклад вершину 1 із вершиною 5,
//то розмір графу автоматично збвльшується до 6 (адже індексація з нуля: 5+1 = 6)
//При видалені ребере розмір зменшуватись не буде.

struct Graph{
    int size;
    int matrix_size = 2;
    bool **matrix = new bool* [matrix_size];

    Graph(int size){
        for(int i = 0; i < matrix_size; i++){
            matrix[i] = new bool [matrix_size];
        }
        clear();

        this->size = size;
        if(size > matrix_size) change_matrix_size(size / 2 * 3);
    }

    void change_matrix_size(int new_size){
        bool **new_matrix = new bool* [new_size];

        for(int i = 0; i < new_size; i++){
            new_matrix[i] = new bool [new_size];

            for(int j = 0; j < new_size; j++){
                if(i < size && j < size) new_matrix[i][j] = matrix[i][j];
                else new_matrix[i][j] = false;
            }
        }
        swap(matrix, new_matrix);

        for(int i = 0; i < size; i++){
            delete [] new_matrix[i];
        }

        matrix_size = new_size;
    }

    void add_edge(int from, int to){
        int max;
        if(from > to) max = from;
        else max = to;

        if(max+1 > matrix_size){
            change_matrix_size((max+1)/2*3);
            size = max+1;
        }

        matrix[from][to] = true;
    }

    void del_edge(int from, int to){
        if(from < size && to < size){
            matrix[from][to] = false;
        }
    }

    void merge(int old, int in){
        for(int i = 0; i < size; i++){
            if(i != old){
                if(matrix[i][old]) matrix[i][in] = true;
                if(matrix[old][i]) matrix[in][i] = true;
            }
        }

        if(old == size-1){
            for(int i = 0; i < size; i++){
                matrix[i][old] = false;
            }
            for(int j = 0; j < size; j++){
                matrix[old][j] = false;
            }
        } else {
            for (int i = old; i < size - 1; i++) {
                for (int j = 0; j < size; j++) {
                    matrix[i][j] = matrix[i + 1][j];
                }
            }
            for (int i = 0; i < size; i++) {
                for (int j = old; j < size - 1; j++) {
                    matrix[i][j] = matrix[i][j + 1];
                }
            }
        }
        size--;
    }

    int* matrix_to_arr(){
        int arr_size = 0;
        for(int i = 0; i < size; i++){
            for(int j = 0; j < size; j++){
                if(matrix[i][j]) arr_size++;
            }
        }

        ++arr_size *=2;

        int * arr = new int[arr_size];
        int index = 0;
        arr[index++] = arr_size;
        arr[index++] = size;

        for(int i = 0; i < size; i++){
            for(int j = 0; j < size; j++){
                if(matrix[i][j]) {
                    arr[index++] = i;
                    arr[index++] = j;
                }
            }
        }
        return arr;
    }

    void arr_to_matrix(int *arr){
        clear();
        int n = arr[0];
        for(int i = 2; i < n; i+=2){
            add_edge(arr[i], arr[i+1]);
        }
        delete [] arr;
    }

    void write_matrix(){
        ofstream out;
        out.open("/home/andrew/CLionProjects/mod2_lab6_grph/file", ios::binary);
        int *arr = matrix_to_arr();
        int n = arr[0];
        if(out.is_open()) {
            out.write((char*)&(*arr), sizeof(int)*n);
            out.close();
        }
        delete [] arr;
    }

    void read_matrix(){
        ifstream in;
        in.open("/home/andrew/CLionProjects/mod2_lab6_grph/file", ios::binary);
        if(in.is_open()){
            in.seekg(0, ios::end);
            int n = in.tellg() / 4;

            in.seekg(0, ios::beg);

            int *arr = new int[n];
            in.read((char*)&(*arr), sizeof(int) * n);
            arr_to_matrix(arr);
            in.close();
        }
    }

    void clear(){
        for(int i = 0; i < size; i++){
            for(int j = 0; j < size; j++){
                matrix[i][j] = false;
            }
        }
    }

    void print(){
        for(int j = 0; j < size; j++){
            for(int i = 0; i < size; i++){
                cout << matrix[i][j] << "\t";
            }
            cout << endl;
        }
        cout << endl;
    }
};
//--------------Лаба-№6--^-----------------------------

//Перевід матриці графа в допоміжну матрицю для сортування
void graph_matrix_to_in_matrix(bool** graph_matrix, int** in_matrix, int matrix_size){
    for(int i = 0; i < matrix_size; i++){
        in_matrix[i][0] = i;
        in_matrix[i][1] = 0;
    }

    for(int j = 0; j < matrix_size; j++){
        for(int i = 0; i < matrix_size; i++){
            in_matrix[j][1] += graph_matrix[i][j];
        }
    }
}
//Навпаки - допоміжну в матрицю графа
bool** in_matrix_to_graph_matrix(int** in_matrix, bool** graph_matrix, int matrix_size){
    bool** result_matrix = new bool* [matrix_size];
    for(int i = 0; i < matrix_size; i++){
        result_matrix[i] = new bool [matrix_size];
        for(int j = 0; j < matrix_size; j++){
            result_matrix[i][j] = false;
        }
    }

    for(int i = 0; i < matrix_size; i++){
        for(int j = 0; j < matrix_size; j++){
            if(graph_matrix[i][j]){
                result_matrix[in_matrix[i][0]][in_matrix[j][0]] = true;
            }
        }
    }
    return result_matrix;
}

//----------------Сортування--\/------------------------
void insertion_sort(int** in_matrix, int matrix_size){
    for(int i = 1; i<matrix_size; i++)
        for(int j = i; j > 0 && in_matrix[j-1][1] > in_matrix[j][1]; j--){
            swap(in_matrix[j-1][0], in_matrix[j][0]);
            swap(in_matrix[j-1][1], in_matrix[j][1]);
        }
}

void bubble_sort(int** in_matrix, int matrix_size){
    int prob = 0;

    for(int k = 1; k < matrix_size; k++){
        for(int i = 0; i < matrix_size - k; i++){
            if(in_matrix[i][1] > in_matrix[i+1][1]){
                prob = in_matrix[i][0];
                in_matrix[i][0] = in_matrix[i+1][0];
                in_matrix[i+1][0] = prob;

                prob = in_matrix[i][1];
                in_matrix[i][1] = in_matrix[i+1][1];
                in_matrix[i+1][1] = prob;
            }
        }
    }
}

void shaker_sort(int **in_matrix, int matrix_size){
    bool to_right = true;
    int left = 1, right = 1;

    while (right + left != matrix_size){
        if(to_right){
            for(int i = 0; i < matrix_size - right; i++){
                if(in_matrix[i][1] > in_matrix[i+1][1]){
                    swap(in_matrix[i][0], in_matrix[i+1][0]);
                    swap(in_matrix[i][1], in_matrix[i+1][1]);
                }
            }
            right++;
            to_right = false;
        }else{
            for(int i = matrix_size - left - 1; i > left; i--){
                if(in_matrix[i][1] < in_matrix[i-1][1]){
                    swap(in_matrix[i][0], in_matrix[i-1][0]);
                    swap(in_matrix[i][1], in_matrix[i-1][1]);
                }
            }
            left++;
            to_right = true;
        }
    }
}

void quick_sort(int **in_matrix, int left, int right) {
    int i = left, j = right;
    int tmp;
    int pivot = in_matrix[(left + right) / 2][1];

    while (i <= j) {
        while (in_matrix[i][1] < pivot)
            i++;

        while (in_matrix[j][1] > pivot)
            j--;

        if (i <= j) {
            swap(in_matrix[i][0], in_matrix[j][0]);
            swap(in_matrix[i][1], in_matrix[j][1]);

            i++;
            j--;
        }
    }
    if (left < j) quick_sort(in_matrix, left, j);
    if (i < right) quick_sort(in_matrix, i, right);
}
//допоміжний метод для сортування злиттям
void merge(int **in_matrix, int low, int high, int mid){
    int i, j, k, c[50][2];
    i = low;
    k = low;
    j = mid + 1;
    while (i <= mid && j <= high) {
        if (in_matrix[i][1] < in_matrix[j][1]) {
            c[k][0] = in_matrix[i][0];
            c[k][1] = in_matrix[i][1];
            k++;
            i++;
        }
        else  {
            c[k][0] = in_matrix[j][0];
            c[k][1] = in_matrix[j][1];
            k++;
            j++;
        }
    }
    while (i <= mid) {
        c[k][0] = in_matrix[i][0];
        c[k][1] = in_matrix[i][1];
        k++;
        i++;
    }
    while (j <= high) {
        c[k][0] = in_matrix[j][0];
        c[k][1] = in_matrix[j][1];
        k++;
        j++;
    }
    for (i = low; i < k; i++)  {
        in_matrix[i][0] = c[i][0];
        in_matrix[i][1] = c[i][1];
    }
}
void merge_sort(int **in_matrix, int low, int high){
    int mid;
    if (low < high){
        mid=(low+high)/2;
        merge_sort(in_matrix, low, mid);
        merge_sort(in_matrix, mid + 1, high);
        merge(in_matrix, low, high, mid);
    }
}

void shell_sort (int** in_matrix, int matrix_size){
    int step, i, j;
    int temp[2];

    for (step = matrix_size / 2; step > 0; step /= 2)
        for (i = step; i < matrix_size; i++)
            for (j = i - step; j >= 0 && in_matrix[j][1] > in_matrix[j + step][1]; j -= step){
                temp[0] = in_matrix[j][0];
                temp[1] = in_matrix[j][1];
                in_matrix[j][0] = in_matrix[j + step][0];
                in_matrix[j][1] = in_matrix[j + step][1];
                in_matrix[j + step][0] = temp[0];
                in_matrix[j + step][1] = temp[1];
            }
}

int main() {
    Graph gr(2);
    gr.add_edge(0, 3);
    gr.add_edge(3, 2);
    gr.add_edge(2, 2);
    gr.write_matrix();
    gr.read_matrix();
    gr.print();
    //Створюємо допоміжний масив для простішого сортування
    int** in_matrix = new int* [gr.size];
    for(int i = 0; i < gr.matrix_size; i++){
        in_matrix[i] = new int[2];
    }
    //Вносимо в допоміжний масив кількість взідних ребер кожної вершини
    graph_matrix_to_in_matrix(gr.matrix, in_matrix, gr.size);
    //Сортуємо допоміжний масив
    merge_sort(in_matrix, 0, gr.size-1);
    //Переводимо допоміжний масив в матриццю графа
    gr.matrix = in_matrix_to_graph_matrix(in_matrix, gr.matrix, gr.size);

    gr.print();
}
