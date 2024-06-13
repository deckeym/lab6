#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <iomanip>
#include <string>
#include <windows.h>
#include "header.h"

using namespace std;

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    string text;
    cout << "Input text for cipher >> ";
    getline(cin, text);

    vector<vector<vector<unsigned char>>> block;   //генерация матрицы для работы
    blocksGenerator(text, block);
    cout << "----------------------------------------------";
    cout << "\nThe encryption block: \n";
    for (auto i : block) {
        for (auto j : i) {
            for (auto k : j) {
                cout << setw(4) << k << " ";
            }
            cout << endl;
        }
    }
    cout << "\n----------------------------------------------" << endl;

    vector<unsigned char> key;
    keyGen(key);
    cout << "128-bit master key: "; //генерируем рандомный ключ 
    for (auto i : key) {
        cout << i;
    }
    cout << "\n----------------------------------------------\n";

    vector<vector<unsigned char>> roundKeys;
    KeyExpansion(key, roundKeys); // расширение ключей
    cout << "Generated keys: \n";
    for (auto i : roundKeys) {
        for (auto j : i) {
            cout << hex << setw(4) << static_cast<int>(j) << " ";

        }
        cout << endl;
    }
    cout << "\n----------------------------------------------" << endl;

    vector<vector<unsigned char>> PREV(4, vector<unsigned char>(4, 0));
    vector<vector<unsigned char>> DEFOLT = PREV;
    vector<vector<unsigned char>> TEK;
    vector<vector<vector<unsigned char>>> ECRYPT;

    for (int i = 0; i < block.size(); i++) {

        vector<vector<unsigned char>> res(4, vector<unsigned char>(4, 0));
        vector<vector<unsigned char>> B = block[i];
        TEK = AESencrypt(PREV, roundKeys, key);

        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                res[i][j] = (TEK[i][j] ^ B[i][j]);

            }
        }
        ECRYPT.push_back(res);

        PREV = TEK;
    }

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

    for (int i = 0; i < ECRYPT.size(); ++i) {
        vector<vector<unsigned char>> res(4, vector<unsigned char>(4, 0));
        vector<vector<unsigned char>> B = ECRYPT[i];

        TEK = AESencrypt(PREV, roundKeys, key);

        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                res[i][j] = (B[i][j] ^ TEK[i][j]);
            }
        }
        DECRYPT.push_back(res);
        PREV = TEK;
    }

    vector<vector<vector<unsigned char>>> DECRYPT_1;

    for (int i = 0; i < ECRYPT.size(); ++i) {
        vector<vector<unsigned char>> res(4, vector<unsigned char>(4, 0));
        vector<vector<unsigned char>> B = ECRYPT[i];

        TEK = AESdecrypt(B, roundKeys, key);

        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                res[i][j] = TEK[i][j];
            }
        }
        DECRYPT_1.push_back(res);
    }

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