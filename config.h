/*********************************************************************
 *  BocchiTwin v1.0 Alpha 
 *  Alat ini ditujukan untuk tujuan pengujian dan edukasi. Harap gunakan alat ini hanya untuk keperluan pen-testing pada jaringan milik sendiri atau jaringan yang telah Anda dapatkan izin untuk diuji. Segala konsekuensi dari penggunaan alat ini menjadi tanggung jawab pengguna, jadi Gunakan dengan risiko Anda sendiri.
 *********************************************************************/

#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// ================== CONFIG ==================
const char* PROJECT_NAME    = "Bocchi twin";
const char* VERSION         = "v1.0 Alpha";
const char* AP_SSID         = "Bocchi Note 13 5G";
const char* AP_PASSWORD     = "Bocchi12";

IPAddress AP_IP(192, 168, 4, 1);
IPAddress AP_SUBNET(255, 255, 255, 0);

const uint8_t DNS_PORT = 53;

#endif