#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <iomanip>
#include <string>
#include <windows.h>

using namespace std;

// Прототип функции для генерации ключа шифрования
void keyGen(vector<unsigned char>& key);

// Прототип функции для добавления раундового ключа к состоянию
vector<unsigned char> AddRoundKey(vector<unsigned char>& first, vector<unsigned char>& second);

// Прототип функции для подстановки байтов (S-box)
void SubBytes(vector<unsigned char>& state);

// Прототип функции для циклического сдвига строк в состоянии
void ShiftRows(vector<unsigned char>& state);

// Прототип функции для умножения байтов в поле Галуа
unsigned char gmul(unsigned char a, unsigned char b);

// Прототип функции для генерации таблицы умножения на 2
vector<unsigned char> generate_mult_by_2();

// Прототип функции для генерации таблицы умножения на 3
vector<unsigned char> generate_mult_by_3();

// Прототип функции для генерации таблицы умножения на 14
vector<unsigned char> generate_mult_by_14();

// Прототип функции для генерации таблицы умножения на 9
vector<unsigned char> generate_mult_by_9();

// Прототип функции для генерации таблицы умножения на 13
vector<unsigned char> generate_mult_by_13();

// Прототип функции для генерации таблицы умножения на 11
vector<unsigned char> generate_mult_by_11();

// Прототип функции для перемешивания столбцов состояния
void MixColumns(vector<unsigned char>& state);

// Прототип функции для расширения ключа шифрования
void KeyExpansion(vector<unsigned char>& key, vector<vector<unsigned char>>& roundKeys);

// Прототип функции для инверсного циклического сдвига строк в состоянии
void InvShiftRows(vector<unsigned char>& state);

// Прототип функции для инверсной подстановки байтов (S-box)
void InvSubBytes(vector<unsigned char>& state);

// Прототип функции для инверсного перемешивания столбцов состояния
void InvMixColumns(vector<unsigned char>& state);

// Прототип функции для генерации блоков текста
void blocksGenerator(string& text, vector<vector<vector<unsigned char>>>& block);

// Прототип функции для шифрования блока с использованием AES
vector<vector<unsigned char>> AESencrypt(vector<vector<unsigned char>>& block, vector<vector<unsigned char>>& roundKeys, vector<unsigned char> key);

// Прототип функции для расшифровки блока с использованием AES
vector<vector<unsigned char>> AESdecrypt(vector<vector<unsigned char>>& block, vector<vector<unsigned char>>& roundKeys, vector<unsigned char> key);
