/*********************************************************************
 *  BocchiTwin v1.0 Alpha 
 *  Alat ini ditujukan untuk tujuan pengujian dan edukasi. Harap gunakan alat ini hanya untuk keperluan pen-testing pada jaringan milik sendiri atau jaringan yang telah Anda dapatkan izin untuk diuji. Segala konsekuensi dari penggunaan alat ini menjadi tanggung jawab pengguna, jadi Gunakan dengan risiko Anda sendiri.
 *********************************************************************/

#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>

String macToString(const uint8_t* mac) {
    String str;
    for (int i = 0; i < 6; i++) {
        if (mac[i] < 0x10) str += '0';
        str += String(mac[i], HEX);
        if (i < 5) str += ':';
    }
    str.toUpperCase();
    return str;
}

void clearNetworkList(NetworkInfo* list, size_t size) {
    for (size_t i = 0; i < size; i++) {
        list[i] = NetworkInfo();
    }
}

#endif