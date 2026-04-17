/*********************************************************************
 *  BocchiTwin v1.0 Alpha
 *  Alat ini ditujukan untuk tujuan pengujian dan edukasi. Harap gunakan alat ini hanya untuk keperluan pen-testing pada jaringan milik sendiri atau jaringan yang telah Anda dapatkan izin untuk diuji. Segala konsekuensi dari penggunaan alat ini menjadi tanggung jawab pengguna, jadi Gunakan dengan risiko Anda sendiri.
 *********************************************************************/

#ifndef NETWORKS_H
#define NETWORKS_H

typedef struct {
    String ssid;
    uint8_t channel;
    uint8_t bssid[6];
    int8_t  rssi;
} NetworkInfo;

#endif