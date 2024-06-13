#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>

using namespace std;

// Функция для выбора главного элемента в текущем столбце
int findPivot(double A[][4], int col, int startRow, int n) {
    double maxVal = fabs(A[startRow][col]);
    int maxRow = startRow;

    for (int i = startRow + 1; i < n; i++) {
        if (fabs(A[i][col]) > maxVal) {
            maxVal = fabs(A[i][col]);
            maxRow = i;
        }
    }

    return maxRow;
}

void gaussElimination(double A[][4], vector<double>& b, vector<double>& x, int n) {
    double temp;

    // Прямой ход
    cout << "Прямой ход:" << endl;
    for (int k = 0; k < n - 1; k++) {
        // Выбор ведущего элемента
        int maxRow = findPivot(A, k, k, n);

        // Прямой ход
        for (int i = k + 1; i < n; i++) {
            double factor = A[i][k] / A[k][k];
            for (int j = k; j < n; j++) {
                A[i][j] -= factor * A[k][j];
            }
            b[i] -= factor * b[k];
        }

        // Вывод промежуточных результатов
        cout << "Шаг " << k + 1 << ":" << endl;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cout << fixed << setprecision(3) << setw(10) << A[i][j];
            }
            cout << " | " << fixed << setprecision(3) << setw(10) << b[i] << endl;
        }
        cout << endl;
    }

    // Обратный ход
    cout << "\nОбратный ход:" << endl;
    x[n - 1] = b[n - 1] / A[n - 1][n - 1];
    for (int i = n - 2; i >= 0; i--) {
        double sum = b[i];
        for (int j = i + 1; j < n; j++) {
            sum -= A[i][j] * x[j];
        }
        x[i] = sum / A[i][i];
    }

    // Вывод конечной матрицы
    cout << "\nКонечная матрица:\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << fixed << setprecision(3) << setw(10) << A[i][j];
        }
        cout << " | " << fixed << setprecision(3) << setw(10) << b[i] << endl;
    }
}

// Преобразование системы Ах=b к виду х=Сх+f
void formCanonicalSystem(double A[][4], vector<double>& b, double C[][4], vector<double>& f, int n) {
    for (int i = 0; i < n; i++) {
        f[i] = b[i] / A[i][i];
        for (int j = 0; j < n; j++) {
            if (j != i) {
                C[i][j] = -A[i][j] / A[i][i];
            } else {
                C[i][j] = 0.0;
            }
        }
    }
}

// Метод простых итераций
void simpleIteration(double C[][4], vector<double>& f, vector<double>& x, int n, double epsilon) {
    vector<double> xNew(n);
    int k = 0;
    double maxDiff = 0.0;

    // Задаем начальное приближение
    for (int i = 0; i < n; i++) {
        x[i] = 0.0;
    }

    cout << "N" << setw(10) << "x1" << setw(10) << "x2" << setw(10) << "x3" << setw(10) << "x4" << setw(10) << "εn" << endl;

    do {
        // Вычисляем новое приближение
        for (int i = 0; i < n; i++) {
            double sum = f[i];
            for (int j = 0; j < n; j++) {
                sum += C[i][j] * x[j];
            }
            xNew[i] = sum;
        }

        // Проверяем условие остановки
        maxDiff = 0.0;
        for (int i = 0; i < n; i++) {
            if (fabs(xNew[i] - x[i]) > maxDiff) {
                maxDiff = fabs(xNew[i] - x[i]);
            }
            x[i] = xNew[i];
        }
        k++;

        // Вывод результатов в таблицу
        cout << k << fixed << setprecision(3) << setw(10) << x[0] << setw(10) << x[1] << setw(10) << x[2] << setw(10) << x[3] << setw(10) << maxDiff << endl;
    } while (maxDiff > epsilon);

    cout << "Число итераций: " << k << endl;

    // Вывод сообщения о сходимости или расходимости
    if (maxDiff <= epsilon) {
        cout << "Метод сходится." << endl;
    } else {
        cout << "Метод расходится." << endl;
    }
}

int main() {
    system("chcp 65001");
    double A[4][4] = {
        {0.89, -0.04, 0.21, -18.0},
        {0.25, -1.23, 0.12, -0.09},
        {-0.21, 0.12, 0.8,  -0.13},
        {0.15, -1.31, 0.06, -1.15}
    };
    vector<double> b = {-1.24, -1.15, 2.56, 0.89};
    vector<double> x(4, 0.0);
    double C[4][4] = {0.0};
    vector<double> f(4, 0.0);

    // Решение методом Гаусса
    cout << "Решение методом Гаусса:" << endl;
    gaussElimination(A, b, x, 4);
    cout << "x1 = " << fixed << setprecision(3) << x[0] << endl;
    cout << "x2 = " << fixed << setprecision(3) << x[1] << endl;
    cout << "x3 = " << fixed << setprecision(3) << x[2] << endl;
    cout << "x4 = " << fixed << setprecision(3) << x[3] << endl;

    // Преобразуем систему к каноническому виду
    formCanonicalSystem(A, b, C, f, 4);

    // Решение методом простых итераций
    cout << "\nРешение методом простых итераций:" << endl;
    simpleIteration(C, f, x, 4, 0.001);
    cout << "x1 = " << fixed << setprecision(3) << x[0] << endl;
    cout << "x2 = " << fixed << setprecision(3) << x[1] << endl;
    cout << "x3 = " << fixed << setprecision(3) << x[2] << endl;
    cout << "x4 = " << fixed << setprecision(3) << x[3] << endl;

    return 0;
}