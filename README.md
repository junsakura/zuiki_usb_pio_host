# zuiki_pio_usb_host

RP2350-USB-A + Pico SDK 2.2.0 + Pico-PIO-USB 用のテスト構成です。
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

PowerShellで:

```powershell
cd C:\Users\user\zuiki_usb_cdc_host
Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass
.\scripts\build.ps1
```

成功すると:

```text
build\zuiki_usb_cdc_host.uf2
```

ができます。

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
USB mounted: addr=1 VID=xxxx PID=xxxx
HID mounted
HID report addr=1 inst=0 len=... : xx xx xx ...
```

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
