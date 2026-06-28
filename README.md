# ZUIKI USB Host for Raspberry Pi Pico / Pico 2

USB Host機能を利用して ZUIKI製ワンハンドルマスコン を Raspberry Pi Pico / Pico 2 に接続し、HIDレポートを取得・解析するサンプルプロジェクトです。

OLED(SSD1306)によるステータス表示、USB CDCによるデバッグ出力にも対応しています。

## 構成

RP2350-USB-A(Pico2代替) + Pico SDK 2.2.0 + Pico-PIO-USB
Windows11で開発しています。

## 目的

- USB-C: CDCシリアルログ出力
- USB-A: PIO USB Host
- ZuikiマスコンなどのHIDレポートをUSB-Cシリアルへ表示

## 置き場所

推奨:

```text
C:\Users\user\zuiki_usb_cdc_host
```

## ビルド
Visual Studio Codeでプロジェクトを開きます。

Ctrl + Shift + B

または

mkdir build
cd build

cmake ..

cmake --build .

生成されるファイル

build/zuiki_usb_pio_host.uf2

## 書き込み

BOOTSELを押しながらRP2350-USB-AをPCに接続し、生成されたUF2をコピーしてください。

## シリアルモニタ

USB-CをPCへ接続したまま、次のいずれかで開きます。

- VS Code Serial Monitor
- Arduino IDE シリアルモニタ
- Tera Term

速度は 115200bps でOKです。

## 確認

USB-AにZuikiマスコンを接続します。

表示例:

```text

USB CONNECTED
VID xxxx
PID xxxx
LEN xx
```

などの表示がされます。

## 注意

USB-AのD+がGPIO12、D-がGPIO13である前提です。
違う配線の場合はPico-PIO-USB側のピン設定変更が必要です。

## Third-party software

This project uses:

- TinyUSB (MIT License)
- Pico-PIO-USB (MIT License)
- Raspberry Pi Pico SDK (BSD-3-Clause)

## その他

This project is an unofficial USB HID implementation.
This project is not affiliated with ZUIKI.
