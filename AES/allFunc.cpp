#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <iomanip>
#include <string>
#include <windows.h>

using namespace std;

// Функция для генерации ключа
void keyGen(vector<unsigned char>& key) {
    minstd_rand gen(random_device{}()); // Генератор случайных чисел
    uniform_int_distribution<int> numbers(0, 9); // Распределение случайных чисел от 0 до 9 (кодировка ASCII)
    for (int i = 1; i <= 16; i++) {
        key.push_back('0' + numbers(gen)); // Добавляем случайный символ в ключ
    }
}

// Функция для выполнения побитового сложения состояния с раундовым ключом
vector<unsigned char> AddRoundKey(vector<unsigned char>& first, vector<unsigned char>& second) {
    vector<unsigned char> temp;
    for (int i = 0; i < 4; ++i) {
        temp.push_back(first[i] ^ second[i]); // XOR операция для каждого байта
    }
    return temp;
}

// Функция для выполнения подстановки байтов с использованием S-box
void SubBytes(vector<unsigned char>& state) {
    vector<unsigned char> Sbox = {
       0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
       0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
       0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
       0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
       0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
       0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
       0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
       0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
       0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
       0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
       0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
       0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
       0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
       0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
       0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
       0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
    };

    vector<unsigned char> temp;
    for (auto i : state) {
        temp.push_back(Sbox[i]); // Замена байта на соответствующее значение из S-box
    }
    state = temp;
}

// Функция для выполнения сдвига строк
void ShiftRows(vector<unsigned char>& state) {
    vector<unsigned char> temp(state.size());
    for (int i = 1; i < state.size(); i++) {
        temp[i - 1] = state[i]; // Сдвиг элементов влево
    }
    temp[state.size() - 1] = state[0]; // Последний элемент становится первым

    state = temp;
}

// Функция для умножения двух байтов в поле Галуа
unsigned char gmul(unsigned char a, unsigned char b) {
    unsigned char result = 0;
    unsigned char hi_bit_set;
    for (unsigned char i = 0; i < 8; i++) {
        if (b & 1) { // Если младший бит b равен 1
            result ^= a; // Выполняем XOR текущего результата с переменной a
        }
        hi_bit_set = a & 0x80; // Проверяем старший бит переменной a перед сдвигом
        a <<= 1; // Сдвигаем переменную a влево на 1 бит
        if (hi_bit_set) { // Если старший бит a был 1
            a ^= 0x1b; // Выполняем XOR переменной a с модулем AES
        }
        b >>= 1; // Сдвигаем переменную b вправо на 1 бит
    }
    return result;
}

// Генерация таблицы умножения на 2
vector<unsigned char> generate_mult_by_2() {
    vector<unsigned char> mult_by_2(256);
    for (int i = 0; i < 256; ++i) {
        mult_by_2[i] = gmul(i, 2); // Умножаем каждый байт на 2
    }
    return mult_by_2;
}

// Генерация таблицы умножения на 3
vector<unsigned char> generate_mult_by_3() {
    vector<unsigned char> mult_by_3(256);
    for (int i = 0; i < 256; ++i) {
        mult_by_3[i] = gmul(i, 3); // Умножаем каждый байт на 3
    }
    return mult_by_3;
}

// Функция для выполнения операции MixColumns
void MixColumns(vector<unsigned char>& state) {
    vector<unsigned char> mult_by_2 = generate_mult_by_2();
    vector<unsigned char> mult_by_3 = generate_mult_by_3();

    vector<unsigned char> temp(state.size());
    temp[0] = mult_by_2[state[0]] ^ mult_by_3[state[1]] ^ state[2] ^ state[3];
    temp[1] = state[0] ^ mult_by_2[state[1]] ^ mult_by_3[state[2]] ^ state[3];
    temp[2] = state[0] ^ state[1] ^ mult_by_2[state[2]] ^ mult_by_3[state[3]];
    temp[3] = mult_by_3[state[0]] ^ state[1] ^ state[2] ^ mult_by_2[state[3]];

    state = temp;
}

// Функция шифрования данных
void encrypt(vector<unsigned char>& data, vector<unsigned char>& key) {
    vector<unsigned char> state = AddRoundKey(data, key); // Начальная операция AddRoundKey

    for (int i = 0; i < 9; ++i) {
        SubBytes(state); // Замена байтов
        ShiftRows(state); // Сдвиг строк
        MixColumns(state); // Перемешивание столбцов
        state = AddRoundKey(state, key); // Добавление раундового ключа
    }

    SubBytes(state); // Замена байтов в последнем раунде
    ShiftRows(state); // Сдвиг строк
    state = AddRoundKey(state, key); // Последняя операция AddRoundKey

    data = state;
}

int main() {
    vector<unsigned char> data = { 0x32, 0x88, 0x31, 0xe0 }; // Пример данных для шифрования
    vector<unsigned char> key;
    keyGen(key); // Генерация ключа

    cout << "Original data: ";
    for (auto d : data) {
        cout << hex << setw(2) << setfill('0') << static_cast<int>(d) << " ";
    }
    cout << endl;

    encrypt(data, key); // Шифрование данных

    cout << "Encrypted data: ";
    for (auto d : data) {
        cout << hex << setw(2) << setfill('0') << static_cast<int>(d) << " ";
    }
    cout << endl;

    return 0;
}
