
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <iomanip>
#include <string>
#include <windows.h>
#include "header.h"

// Используем пространство имен std
using namespace std;

int main() {
    // Устанавливаем кодировку консоли для ввода и вывода
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    
    string text; // Строка для хранения введенного текста
    cout << "Input text for cipher >> ";
    getline(cin, text); // Считываем текст пользователя

    // Создаем трехмерный вектор для хранения блоков текста
    vector<vector<vector<unsigned char>>> block;
    blocksGenerator(text, block); // Генерируем блоки текста
    cout << "----------------------------------------------";
    cout << "\nThe encryption block: \n";
    // Выводим сгенерированные блоки
    for (auto i : block) {
        for (auto j : i) {
            for (auto k : j) {
                cout << setw(4) << k << " ";
            }
            cout << endl;
        }
    }
    cout << "\n----------------------------------------------" << endl;

    // Создаем вектор для хранения ключа
    vector<unsigned char> key;
    keyGen(key); // Генерируем ключ
    cout << "128-bit master key: "; // Выводим сгенерированный ключ
    for (auto i : key) {
        cout << i;
    }
    cout << "\n----------------------------------------------\n";

    // Создаем вектор для хранения раундовых ключей
    vector<vector<unsigned char>> roundKeys;
    KeyExpansion(key, roundKeys); // Расширяем ключи
    cout << "Generated keys: \n";
    // Выводим сгенерированные раундовые ключи
    for (auto i : roundKeys) {
        for (auto j : i) {
            cout << hex << setw(4) << static_cast<int>(j) << " ";
        }
        cout << endl;
    }
    cout << "\n----------------------------------------------" << endl;

    // Инициализируем матрицы для шифрования
    vector<vector<unsigned char>> PREV(4, vector<unsigned char>(4, 0));
    vector<vector<unsigned char>> DEFOLT = PREV;
    vector<vector<unsigned char>> TEK;
    vector<vector<vector<unsigned char>>> ECRYPT;

    // Шифруем блоки
    for (int i = 0; i < block.size(); i++) {
        vector<vector<unsigned char>> res(4, vector<unsigned char>(4, 0));
        vector<vector<unsigned char>> B = block[i];
        TEK = AESencrypt(PREV, roundKeys, key);

        // XOR текущего блока с предыдущим
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                res[i][j] = (TEK[i][j] ^ B[i][j]);
            }
        }
        ECRYPT.push_back(res);

        PREV = TEK;
    }

    // Выводим зашифрованные блоки
    cout << "The final cipher after encryphion:\n" << endl;
    for (auto t : ECRYPT) {
        for (auto i : t) {
            for (auto j : i) {
                cout << setw(4) << j << " ";
            }
            cout << endl;
        }
    }

    cout << "\n----------------------------------------------\n";

    PREV = DEFOLT;
    vector<vector<vector<unsigned char>>> DECRYPT;

    // Расшифровываем блоки
    for (int i = 0; i < ECRYPT.size(); ++i) {
        vector<vector<unsigned char>> res(4, vector<unsigned char>(4, 0));
        vector<vector<unsigned char>> B = ECRYPT[i];

        TEK = AESencrypt(PREV, roundKeys, key);

        // XOR зашифрованного блока с текущим ключом
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                res[i][j] = (B[i][j] ^ TEK[i][j]);
            }
        }
        DECRYPT.push_back(res);
        PREV = TEK;
    }

    vector<vector<vector<unsigned char>>> DECRYPT_1;

    // Второй этап расшифровки
    for (int i = 0; i < ECRYPT.size(); ++i) {
        vector<vector<unsigned char>> res(4, vector<unsigned char>(4, 0));
        vector<vector<unsigned char>> B = ECRYPT[i];

        TEK = AESdecrypt(B, roundKeys, key);

        // Сохранение расшифрованного блока
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                res[i][j] = TEK[i][j];
            }
        }
        DECRYPT_1.push_back(res);
    }

    // Выводим расшифрованное сообщение
    cout << "Message after decryption:\n" << endl;
    for (int i = 0; i < DECRYPT.size(); i++) {
        vector<vector<unsigned char>>& decrypt = DECRYPT[i];
        for (int j = 0; j < decrypt.size(); j++) {
            for (int k = 0; k < decrypt[j].size(); k++) {
                cout << decrypt[k][j];
            }
        }
    }
    cout << "\n----------------------------------------------\n" << endl;

    return 0;
}
