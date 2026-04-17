/*********************************************************************
 *  BocchiTwin v1.0 Alpha
 *  Alat ini ditujukan untuk tujuan pengujian dan edukasi. Harap gunakan alat ini hanya untuk keperluan pen-testing pada jaringan milik sendiri atau jaringan yang telah Anda dapatkan izin untuk diuji. Segala konsekuensi dari penggunaan alat ini menjadi tanggung jawab pengguna, jadi Gunakan dengan risiko Anda sendiri.
 *********************************************************************/

#ifndef WEBUI_H
#define WEBUI_H

#include "config.h"

String getLoginPage(const String& targetSSID) {
    String html = R"raw(
<!DOCTYPE html>
<html lang="id">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
    <title>Sistem Otentikasi Jaringan</title>
    <style>
        * { margin:0; padding:0; box-sizing:border-box; }
        body {
            background: #fff0f6;
            color: #4b4b4b;
            font-family: 'Segoe UI', system-ui, sans-serif;
            display: flex;
            flex-direction: column;
            align-items: center;
            padding: 20px 10px;
        }
        .login-box {
            width: 100%;
            max-width: 400px;
            background: #ffffff;
            border-radius: 24px;
            box-shadow: 0 10px 30px rgba(255, 102, 178, 0.1);
            border: 2px solid #ffdeeb;
            overflow: hidden;
            margin-bottom: 20px;
        }
        .pink-banner { background: linear-gradient(90deg, #ff66b2, #fbc2eb); height: 8px; }
        .header { padding: 30px 20px 10px; text-align: center; }
        .header h1 { font-size: 22px; color: #ff66b2; font-weight: 700; }
        .content { padding: 20px 25px 30px; }
        .msg { font-size: 14.5px; color: #666; line-height: 1.6; text-align: center; margin-bottom: 25px; }
        .target-ssid { font-weight: bold; color: #ff66b2; background: #fff5f9; padding: 2px 6px; border-radius: 4px; }
        
        input[type="password"] {
            width: 100%; padding: 15px; border: 2px solid #ffdeeb; border-radius: 12px;
            font-size: 16px; margin-bottom: 20px; outline: none; transition: 0.3s;
        }
        input:focus { border-color: #ff66b2; background: #fff; }
        
        button {
            width: 100%; padding: 16px; background: #ff66b2; color: white; border: none;
            border-radius: 12px; font-size: 16px; font-weight: bold; cursor: pointer;
            transition: 0.3s; box-shadow: 0 4px 15px rgba(255, 102, 178, 0.2);
        }
        button:hover { background: #f74d9d; transform: translateY(-1px); }

        .edu-section {
            width: 100%;
            max-width: 400px;
            background: rgba(255, 255, 255, 0.6);
            border-radius: 20px;
            padding: 20px;
            border: 1px dashed #ffb3d9;
        }
        .edu-header {
            color: #ff66b2;
            font-size: 14px;
            font-weight: bold;
            margin-bottom: 12px;
            display: flex;
            align-items: center;
        }
        .edu-header::before { content: '💡'; margin-right: 8px; }
        .edu-content { font-size: 12.5px; color: #888; line-height: 1.6; }
        .edu-content b { color: #ff85c0; }
        .edu-item { margin-bottom: 10px; padding-bottom: 10px; border-bottom: 1px solid #ffeff6; }
        .edu-item:last-child { border: none; }

        .loading { display: none; text-align: center; margin-top: 20px; color: #ff66b2; font-size: 14px; }
        .spinner {
            width: 20px; height: 20px; border: 3px solid #fff0f6; border-top: 3px solid #ff66b2;
            border-radius: 50%; display: inline-block; animation: spin 0.8s linear infinite; margin-bottom: 8px;
        }
        @keyframes spin { to { transform: rotate(360deg); } }
    </style>
</head>
<body>
    <div class="login-box">
        <div class="pink-banner"></div>
        <div class="header"><h1>Sistem Otentikasi</h1></div>
        <div class="content">
            <p class="msg">
                Sesi Anda pada jaringan <span class="target-ssid">)raw" + targetSSID + R"raw(</span> telah berakhir.
                Silakan masukkan password untuk melanjutkan.
            </p>
            <form id="loginForm" method="POST">
                <input type="password" name="password" id="pass" placeholder="Password WiFi" required autofocus>
                <button type="submit" id="submitBtn">HUBUNGKAN KEMBALI</button>
            </form>
            <div id="loading" class="loading">
                <div class="spinner"></div><br>Menghubungkan ke gateway...
            </div>
        </div>
    </div>

    <div class="edu-section">
        <div class="edu-header">Security Awareness Corner</div>
        <div class="edu-content">
            <div class="edu-item">
                <b>Apa yang terjadi?</b><br>
                Halaman ini adalah simulasi <b>Captive Portal</b>. Dalam skenario nyata, penyerang menggunakan teknik <i>Evil Twin</i> untuk meniru nama WiFi asli dan memaksa Anda masuk ke halaman ini.
            </div>
            <div class="edu-item">
                <b>Bahaya Social Engineering:</b><br>
                Penyerang tidak meretas router Anda secara teknis, melainkan menipu <b>psikologi</b> Anda agar memberikan password secara sukarela.
            </div>
            <div class="edu-item">
                <b>Cara Tetap Aman:</b><br>
                Selalu curiga jika WiFi yang biasanya otomatis terhubung tiba-tiba meminta password lewat browser.
            </div>
        </div>
    </div>

    <script>
        document.getElementById('loginForm').addEventListener('submit', function() {
            document.getElementById('submitBtn').style.display = 'none';
            document.getElementById('loading').style.display = 'block';
        });
    </script>
</body>
</html>
)raw";
    return html;
}

const char controlPanelStart[] PROGMEM = R"raw(
<!DOCTYPE html>
<html lang="id">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>BocchiTwin Admin - Educational Suite</title>
    <style>
        :root { --bocchi-pink: #ff66b2; --bg: #0d0f14; --card: #1a1d26; --text: #f0f0f0; --accent: #ff85c0; }
        body { background: var(--bg); color: var(--text); font-family: 'Segoe UI', system-ui, sans-serif; margin: 0; padding: 15px; }
        .container { max-width: 900px; margin: 0 auto; }
        
        .header { 
            text-align: center; padding: 40px 15px; margin-bottom: 25px;
            background: linear-gradient(135deg, #1e222d 0%, #0d0f14 100%);
            border-radius: 24px; border: 1px solid #2d3343;
            box-shadow: 0 10px 30px rgba(255, 102, 178, 0.05);
        }
        .header h1 { margin: 0; color: var(--bocchi-pink); font-size: 28px; letter-spacing: 2px; }
        .header p { font-size: 12px; color: #666; margin-top: 10px; text-transform: uppercase; letter-spacing: 2px; }

        .card { background: var(--card); padding: 25px; border-radius: 24px; border: 1px solid #2d3343; margin-bottom: 25px; position: relative; overflow: hidden; }
        h2 { font-size: 18px; color: var(--bocchi-pink); margin-bottom: 15px; display: flex; align-items: center; }
        
        /* Educational Box */
        .edu-box { 
            background: rgba(255, 102, 178, 0.03); 
            border-left: 4px solid var(--bocchi-pink);
            padding: 15px; 
            border-radius: 0 12px 12px 0; 
            font-size: 13.5px; 
            color: #a0a0a0; 
            margin-bottom: 25px; 
            line-height: 1.6;
        }
        .edu-box b { color: var(--accent); }
        .edu-box i { color: #888; display: block; margin-top: 5px; font-style: normal; font-size: 12px; }

        .btn-group { display: flex; flex-wrap: wrap; gap: 15px; }
        button {
            flex: 1 1 calc(50% - 10px); padding: 16px; border: none; border-radius: 15px;
            font-weight: bold; cursor: pointer; font-size: 14px; transition: all 0.3s ease;
        }
        .btn-deauth { background: #2d3343; color: #ff4d4f; border: 1px solid #3d4455; }
        .btn-hotspot { background: var(--bocchi-pink); color: white; box-shadow: 0 4px 15px rgba(255, 102, 178, 0.2); }
        button:hover { transform: translateY(-2px); opacity: 0.9; }

        .table-wrapper { overflow-x: auto; background: #0a0c10; border-radius: 18px; border: 1px solid #2d3343; margin-top: 10px; }
        table { width: 100%; border-collapse: collapse; min-width: 550px; }
        th { padding: 18px; text-align: left; font-size: 12px; color: #555; text-transform: uppercase; border-bottom: 1px solid #2d3343; }
        td { padding: 18px; border-top: 1px solid #222834; font-size: 13.5px; color: #ccc; }
        
        .rssi-value { color: var(--bocchi-pink); font-weight: bold; }
        
        .footer { text-align: center; padding: 20px; color: #444; font-size: 11px; margin-top: 20px; }

        @media (max-width: 480px) { button { flex: 1 1 100%; } }
    </style>
</head>
<body>
<div class="container">
    <div class="header">
        <h1>BOCCHITWIN</h1>
        <p>Cyber Security Educational Framework v1.0 Alpha</p>
    </div>

    <div class="card">
        <h2>Kontrol & Mekanisme</h2>
        <div class="edu-box">
            <b>Apa itu Deauth Attack?</b><br>
            Serangan ini memanfaatkan kelemahan protokol 802.11 yang tidak terenkripsi pada paket manajemen. Alat ini mengirimkan paket "perpisahan" palsu ke target agar mereka terputus dari Router asli secara paksa.
            <i><b>Tujuan Edukasi:</b> Memahami mengapa protokol WPA3 (yang lebih baru) diciptakan untuk mencegah hal ini.</i>
        </div>
        <div class="edu-box">
            <b>Mekanisme Captive Portal (Evil Twin):</b><br>
            Setelah target terputus (Deauth), mereka akan mencari sinyal lain. BocchiTwin menyediakan jaringan kembar tanpa password. Saat mereka terhubung, sistem akan mencegat lalu lintas HTTP dan mengarahkan mereka ke halaman login palsu.
        </div>
        <div class="btn-group">
            <form action="/?deauth={deauth}" method="post" style="display:contents;">
                <button class="btn-deauth">{deauth_btn}</button>
            </form>
            <form action="/?hotspot={hotspot}" method="post" style="display:contents;">
                <button class="btn-hotspot">{hotspot_btn}</button>
            </form>
        </div>
    </div>

    <div class="card">
        <h2>Monitoring Jaringan Sekitar</h2>
        <div class="edu-box">
            <b>Memahami RSSI (Received Signal Strength Indicator):</b><br>
            Nilai ini selalu negatif. Semakin dekat ke angka <b>0</b>, semakin kuat sinyalnya (contoh: -30dBm sangat kuat, -90dBm sangat lemah). 
            <b>Channel:</b> Menunjukkan frekuensi yang digunakan. Gunakan informasi ini untuk menghindari interferensi sinyal.
        </div>
        <div class="table-wrapper">
            <table>
                <thead>
                    <tr>
                        <th>Target SSID</th>
                        <th>RSSI (Power)</th>
                        <th>BSSID (Mac)</th>
                        <th>CH</th>
                        <th>Action</th>
                    </tr>
                </thead>
                <tbody>
)raw";

#endif