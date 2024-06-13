#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <iomanip>
#include <string>
#include <windows.h>

using namespace std;

void keyGen(vector<unsigned char>& key);

vector<unsigned char> AddRoundKey(vector<unsigned char>& first, vector<unsigned char>& second);

void SubBytes(vector<unsigned char>& state);

void ShiftRows(vector<unsigned char>& state);

unsigned char gmul(unsigned char a, unsigned char b);

vector<unsigned char> generate_mult_by_2();

vector<unsigned char> generate_mult_by_3();

vector<unsigned char> generate_mult_by_14();

vector<unsigned char> generate_mult_by_9();

vector<unsigned char> generate_mult_by_13();

vector<unsigned char> generate_mult_by_11();

void MixColumns(vector<unsigned char>& state);

void KeyExpansion(vector<unsigned char>& key, vector<vector<unsigned char>>& roundKeys);

void InvShiftRows(vector<unsigned char>& state);

void InvSubBytes(vector<unsigned char>& state);

void InvMixColumns(vector<unsigned char>& state);

void blocksGenerator(string& text, vector<vector<vector<unsigned char>>>& block);

vector<vector<unsigned char>> AESencrypt(vector<vector<unsigned char>>& block, vector<vector<unsigned char>>& roundKeys, vector<unsigned char> key);

vector<vector<unsigned char>> AESdecrypt(vector<vector<unsigned char>>& block, vector<vector<unsigned char>>& roundKeys, vector<unsigned char> key);