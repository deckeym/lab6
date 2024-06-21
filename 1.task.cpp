#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

void Eratosthenes(vector<int>& PrimeNums, int k) {  //решето Эратосфена
    for (int i = 2; i < k; i++) { // Заполняем вектор числами от 2 до 50, потому что 1 и 0 - не простые
        PrimeNums.push_back(i);
    }

    for (int i = 0; i <= sqrt(PrimeNums.size()); i++) {
        int j = i + 1;
        while (j < PrimeNums.size()) {
            if (PrimeNums[j] % PrimeNums[i] == 0) {
                for (int k = j; k < PrimeNums.size() - 1; k++) { //если чисто составное
                    PrimeNums[k] = PrimeNums[k + 1]; //сдвигаем влево
                }
                PrimeNums.pop_back(); //и удаляем последний элемент
            }
            else {
                j++; //если простое, идем дальше
            }
        }
    }
}

void findMaxFrequentPrime(const vector<int>& primeNums, vector<int>& counter) {
    int maxFreq = 0;
    int maxFreqPrime = 0;
    // maxFreq максимальная частота встречаемости
    // maxFreqPrime храним значение простого числа
    for (int num : primeNums) {
        if (counter[num] > maxFreq) {
            maxFreq = counter[num];
            maxFreqPrime = num;
        }
    }
    cout << "Число " << maxFreqPrime << " встречается " << maxFreq << " раз" << endl;
}

void Input(int& M, int& N){
    cout << "\nВведите число M: ";
    cin >> M;
    while (M <= 0){
        cout << "Введены неправильные данные, попробуйте еще раз: ";
        cin >> M;
    }
    cout << "\nВведите число N: ";
    cin >> N;
    while (N <= 0){
        cout << "Введены неправильные данные, попробуйте еще раз: ";
        cin >> N;
    }
    cout << endl;
}

// ostream& operator<< (ostream& os, const vector <int>& differences) {
//     for (size_t i = 0; i < differences.size(); ++i){
//         os << differences[i] << ' ';
//     }
//     return os;
// }

vector<int> findSaddlePoints(const vector<vector<int>>& new_array) {
    vector<int> saddlePoints;

    int M = new_array.size(); // Получаем количество строк
    int N = new_array[0].size(); // Получаем количество столбцов

    // Находим минимумы в строках
    vector<int> rowMin(M, INT_MAX);
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            rowMin[i] = min(rowMin[i], new_array[i][j]);
        }
    }

    // Находим максимумы в столбцах
    vector<int> colMax(N, INT_MIN);
    for (int j = 0; j < N; ++j) {
        for (int i = 0; i < M; ++i) {
            colMax[j] = max(colMax[j], new_array[i][j]);
        }
    }

    // Проверяем, являются ли найденные минимумы в строках и максимумы в столбцах седловыми точками
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            if (new_array[i][j] == rowMin[i] && new_array[i][j] == colMax[j]) {
                saddlePoints.push_back(new_array[i][j]);
            }
        }
    }

    return saddlePoints;
}

int main() {
    system("chcp 65001");
    // пункт 1: определяем матрицу и заполняем случайными числами
    minstd_rand gen(std::random_device{}());
    uniform_int_distribution<> dis(0, 50);
    int M = 5, N = 5;
    int matrix[M][N];
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i][j] = dis(gen);
        }
    }
    vector<int> primeNums; // с помощью решета Эратосфена определяем 
    int k = 50;
    Eratosthenes(primeNums, k); // простые числа в диапазоне от 2 до 50
    int maxPrime = *max_element(primeNums.begin(), primeNums.end());
    vector<int> counter(maxPrime + 1, 0);
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            for (int num : primeNums) {
                if (matrix[i][j] == num) {
                    counter[num]++;
                }
            }
        }
    }
    for (int i = 0; i < M; i++){
        for(int j = 0; j < N; j++){
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;

    findMaxFrequentPrime(primeNums, counter);

    // 2 пункт
    // M - строка, N - столбец
    // i - строка, j - столбец
    Input(M, N); // Вводим данные с консоли
    int new_matrix[M][N];
    for (int i = 0; i < M; i++){
        for(int j = 0; j < N; j++){
            if (i == j){
                new_matrix[i][j] = 000;
            } else if(i > j){
                new_matrix[i][j] = 100 + j;
            } else {
                new_matrix[i][j] = 100 + N - j - 1;
            }
        }
    }
    for (int i = 0; i < M; i++){
        for(int j = 0; j < N; j++){
            if (new_matrix[i][j] == 0){
                cout << "000" << " ";
            } else {
                cout << new_matrix[i][j] << " ";
            }
        }
        cout << endl;
    }

    // 3 пункт
    k = 500; // Размер матрицы
    Eratosthenes(primeNums, k); // обновим вектор простых чисел
    vector<int> unique_nums(primeNums.begin(), primeNums.begin() + M * N); // Берем первые M*N уникальных простых чисел
    shuffle(unique_nums.begin(), unique_nums.end(), gen); // Перемешиваем числа

    // Определение и заполнение матрицы new_array
    vector<vector<int>> new_array(M, vector<int>(N));
    int idx = 0;
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            new_array[i][j] = unique_nums[idx++];
        }
    }

    for (int i = 0; i < M; i++){
        for(int j = 0; j < N; j++){
            cout << new_array[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;

    // Поиск седловых точек в матрице
    vector<int> saddlePoints = findSaddlePoints(new_array);
    // Вывод результатов
    if (saddlePoints.empty()) {
        cout << "\nВ матрице нет седловых точек." << endl;
    } else {
        cout << "\nНайденные седловые точки:" << endl;
        for (int point : saddlePoints) {
            cout << point << " ";
        }
        cout << endl;
    }

    return 0;
}
