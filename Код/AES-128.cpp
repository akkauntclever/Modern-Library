//
//  AES-128.cpp
//  modern_library
//
//  Created by Трифонов Иван on 22.11.2024.
//  Copyright © 2024 Трифонов Иван. All rights reserved.
//

#include "Library.hpp"

const int Nb = 4;  // Количество столбцов состояния
const int Nk = 4;  // Размер ключа
const int Nr = 10; // Количество раундов

// Таблицы и функции, используемые в AES
// Предопределённый S-box для AES
static const uint8_t SBox[256] = {
    0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
    0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
    0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
    0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
    0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
    0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
    0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
    0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
    0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
    0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
    0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
    0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
    0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
    0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
    0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
    0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
};
// Обратный S-box для AES
static const uint8_t InvSBox[256] = {
    0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB,
    0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87, 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB,
    0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D, 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E,
    0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25,
    0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92,
    0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84,
    0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06,
    0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B,
    0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA, 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73,
    0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85, 0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E,
    0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B,
    0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4,
    0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F,
    0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF,
    0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0, 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26, 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D
};
const uint8_t Rcon[11] = {0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36};

uint8_t gf_mul(uint8_t a, uint8_t b) {
    uint8_t result = 0;      // Результат умножения
    uint8_t primitive = 0x1b; // Примитивный многочлен x^8 + x^4 + x^3 + x + 1
    for (int i = 0; i < 8; i++) {
        // Если текущий бит b равен 1, добавляем текущую a к результату
        if (b & 1) {
            result ^= a;
        }
        // Проверяем, есть ли старший бит в a (нужен для редукции)
        uint8_t carry = a & 0x80;
        a <<= 1; // Сдвигаем a влево на 1 бит
        // Если был старший бит, выполняем редукцию
        if (carry) {
            a ^= primitive;
        }
        b >>= 1; // Сдвигаем b вправо на 1 бит
    }
    return result;
}
// Функция SubBytes
void SubBytes(uint8_t state[4][4]) {
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            state[row][col] = SBox[state[row][col]];
        }
    }
}
// Функция InvSubBytes
void InvSubBytes(uint8_t state[4][4]) {
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            state[row][col] = InvSBox[state[row][col]];
        }
    }
}

void ShiftRows(uint8_t state[4][4]) {
    uint8_t temp;
    // Сдвиг второй строки на 1 байт влево
    temp = state[1][0];
    state[1][0] = state[1][1];
    state[1][1] = state[1][2];
    state[1][2] = state[1][3];
    state[1][3] = temp;
    // Сдвиг третьей строки на 2 байта влево
    temp = state[2][0];
    state[2][0] = state[2][2];
    state[2][2] = temp;
    temp = state[2][1];
    state[2][1] = state[2][3];
    state[2][3] = temp;
    // Сдвиг четвёртой строки на 3 байта влево
    temp = state[3][3];
    state[3][3] = state[3][2];
    state[3][2] = state[3][1];
    state[3][1] = state[3][0];
    state[3][0] = temp;
}

void InvShiftRows(uint8_t state[4][4]) {
    uint8_t temp;
    // Сдвиг второй строки на 1 байт вправо
    temp = state[1][3];
    state[1][3] = state[1][2];
    state[1][2] = state[1][1];
    state[1][1] = state[1][0];
    state[1][0] = temp;
    // Сдвиг третьей строки на 2 байта вправо
    temp = state[2][0];
    state[2][0] = state[2][2];
    state[2][2] = temp;
    temp = state[2][1];
    state[2][1] = state[2][3];
    state[2][3] = temp;
    // Сдвиг четвёртой строки на 3 байта вправо
    temp = state[3][0];
    state[3][0] = state[3][1];
    state[3][1] = state[3][2];
    state[3][2] = state[3][3];
    state[3][3] = temp;
}
// Преобразование MixColumns
void MixColumns(uint8_t state[4][4]) {
    uint8_t temp[4];
    for (int col = 0; col < 4; ++col) {
        temp[0] = gf_mul(state[0][col], 0x02) ^ gf_mul(state[1][col], 0x03) ^ state[2][col] ^ state[3][col];
        temp[1] = state[0][col] ^ gf_mul(state[1][col], 0x02) ^ gf_mul(state[2][col], 0x03) ^ state[3][col];
        temp[2] = state[0][col] ^ state[1][col] ^ gf_mul(state[2][col], 0x02) ^ gf_mul(state[3][col], 0x03);
        temp[3] = gf_mul(state[0][col], 0x03) ^ state[1][col] ^ state[2][col] ^ gf_mul(state[3][col], 0x02);
        // Копируем результат обратно в столбец состояния
        for (int row = 0; row < 4; ++row) {
            state[row][col] = temp[row];
        }
    }
}

void InvMixColumns(uint8_t state[4][4]) {
    uint8_t temp[4];
    for (int col = 0; col < 4; ++col) {
        temp[0] = gf_mul(state[0][col], 0x0e) ^
        gf_mul(state[1][col], 0x0b) ^
        gf_mul(state[2][col], 0x0d) ^
        gf_mul(state[3][col], 0x09);
        temp[1] = gf_mul(state[0][col], 0x09) ^
        gf_mul(state[1][col], 0x0e) ^
        gf_mul(state[2][col], 0x0b) ^
        gf_mul(state[3][col], 0x0d);
        temp[2] = gf_mul(state[0][col], 0x0d) ^
        gf_mul(state[1][col], 0x09) ^
        gf_mul(state[2][col], 0x0e) ^
        gf_mul(state[3][col], 0x0b);
        temp[3] = gf_mul(state[0][col], 0x0b) ^
        gf_mul(state[1][col], 0x0d) ^
        gf_mul(state[2][col], 0x09) ^
        gf_mul(state[3][col], 0x0e);
        // Копируем результат обратно в столбец состояния
        for (int row = 0; row < 4; ++row) {
            state[row][col] = temp[row];
        }
    }
}

void AddRoundKey(uint8_t state[4][4], const uint8_t roundKey[16]) {
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            state[row][col] ^= roundKey[col * 4 + row];
        }
    }
}
// Подстановка байтов с использованием S-box
uint8_t SubByte(uint8_t byte) {
    return SBox[byte];
}
// Функция KeyExpansion
void KeyExpansion(const uint8_t key[16], uint8_t expandedKeys[176]) {
    // Копируем исходный ключ в первые 16 байт expandedKeys
    for (int i = 0; i < Nk * 4; ++i) {
        expandedKeys[i] = key[i];
    }
    uint8_t temp[4];
    int currentSize = Nk * 4;
    while (currentSize < (Nr + 1) * Nb * 4) {
        // Сохраняем последние 4 байта предыдущего блока ключей
        for (int i = 0; i < 4; ++i) {
            temp[i] = expandedKeys[currentSize - 4 + i];
        }
        // Если текущая позиция кратна Nk, применяем преобразования
        if (currentSize / 4 % Nk == 0) {
            // RotWord: циклический сдвиг влево
            uint8_t t = temp[0];
            temp[0] = temp[1];
            temp[1] = temp[2];
            temp[2] = temp[3];
            temp[3] = t;
            // SubWord: замена байтов через S-box
            for (int i = 0; i < 4; ++i) {
                temp[i] = SubByte(temp[i]);
            }
            // Добавление раундовой константы Rcon
            temp[0] ^= Rcon[(currentSize / 4 / Nk) - 1];
        }
        // Записываем текущие 4 байта в expandedKeys
        for (int i = 0; i < 4; ++i) {
            expandedKeys[currentSize] = expandedKeys[currentSize - Nk * 4] ^ temp[i];
            currentSize++;
        }
    }
}

// Функция для добавления паддинга PKCS#7
void addPadding(std::vector<uint8_t>& data) {
    size_t paddingLength = 16 - (data.size() % 16);
    data.insert(data.end(), paddingLength, static_cast<uint8_t>(paddingLength));
}

// Функция для удаления паддинга PKCS#7
void removePadding(std::vector<uint8_t>& data) {
    if (data.empty()) return;
    uint8_t paddingLength = data.back();
    if (paddingLength > 0 && paddingLength <= 16)
        data.resize(data.size() - paddingLength);
}

// Шифрование одного блока
void AES_encryptBlock(const uint8_t input[16], uint8_t output[16], const uint8_t key[16]) {
    uint8_t state[4][Nb];
    uint8_t roundKeys[(Nr + 1) * Nb * 4];
    KeyExpansion(key, roundKeys);
    for (int i = 0; i < 16; ++i) {
         state[i % 4][i / 4] = input[i];
    }
    AddRoundKey(state, roundKeys);
    for (int round = 1; round < Nr; ++round) {
        SubBytes(state);
        ShiftRows(state);
        MixColumns(state);
        AddRoundKey(state, roundKeys + round * 4 * Nb);
    }
    SubBytes(state);
    ShiftRows(state);
    AddRoundKey(state, roundKeys + Nr * 4 * Nb);
    for (int i = 0; i < 16; ++i) {
        output[i] = state[i % 4][i / 4];
    }
}

// Дешифрование одного блока
void AES_decryptBlock(const uint8_t input[16], uint8_t output[16], const uint8_t key[16]) {
    uint8_t state[4][Nb];
    uint8_t roundKeys[(Nr + 1) * Nb * 4];
    KeyExpansion(key, roundKeys);
    for (int i = 0; i < 16; ++i) {
        state[i % 4][i / 4] = input[i];
    }
    AddRoundKey(state, roundKeys + Nr * 4 * Nb);
    for (int round = Nr - 1; round > 0; --round) {
        InvShiftRows(state);
        InvSubBytes(state);
        AddRoundKey(state, roundKeys + round * 4 * Nb);
        InvMixColumns(state);
    }
    InvShiftRows(state);
    InvSubBytes(state);
    AddRoundKey(state, roundKeys);
    for (int i = 0; i < 16; ++i) {
        output[i] = state[i % 4][i / 4];
    }
}
// Шифрование строки
std::string AES_encryptString(const std::string& plaintext, const uint8_t key[16]) {
    std::vector<uint8_t> data(plaintext.begin(), plaintext.end());
    addPadding(data);
    std::vector<uint8_t> ciphertext(data.size());
    uint8_t block[16], encryptedBlock[16];
    for (size_t i = 0; i < data.size(); i += 16) {
        std::memcpy(block, &data[i], 16);
        AES_encryptBlock(block, encryptedBlock, key);
        std::memcpy(&ciphertext[i], encryptedBlock, 16);
    }
    return std::string(ciphertext.begin(), ciphertext.end());
}
// Дешифрование строки
std::string AES_decryptString(const std::string& ciphertext, const uint8_t key[16]) {
    std::vector<uint8_t> data(ciphertext.begin(), ciphertext.end());
    std::vector<uint8_t> plaintext(data.size());
    uint8_t block[16], decryptedBlock[16];
    for (size_t i = 0; i < data.size(); i += 16) {
        std::memcpy(block, &data[i], 16);
        AES_decryptBlock(block, decryptedBlock, key);
        std::memcpy(&plaintext[i], decryptedBlock, 16);
    }
    removePadding(plaintext);
    return std::string(plaintext.begin(), plaintext.end());
}
