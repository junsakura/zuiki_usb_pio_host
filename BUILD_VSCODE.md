# VS Code build notes

この版では、USB-A 側の PIO USB Host 処理は残したまま、Pico-PIO-USB の取得方法だけを修正しています。

## ビルド手順

1. 古い `build` フォルダを削除します。
2. VS Code でこのフォルダを開きます。
3. `Ctrl + Shift + B` を実行します。

最初に `Init Pico-PIO-USB dependency` タスクが走ります。これは次の処理です。

```powershell
cd $env:USERPROFILE/.pico-sdk/sdk/2.2.0/lib/tinyusb
python tools/get_deps.py rp2040
```

以前のような

```powershell
git submodule update --init --recursive hw/mcu/raspberry_pi/Pico-PIO-USB
```

は使いません。Pico SDK 2.2.0 では、その pathspec が TinyUSB 側の git に存在しないため失敗します。

## 書き込み

ビルド後、`build/zuiki_usb_pio_host.uf2` ができます。

BOOTSEL ボタンを押しながら Pico を接続し、`RPI-RP2` ドライブへ UF2 をドラッグ＆ドロップしてください。
または VS Code の `Run Project` タスクを使います。

## USB-A 側

`src/usb_host.c` の PIO USB Host 初期化は維持しています。

```c
pio_usb_configuration_t pio_cfg = PIO_USB_DEFAULT_CONFIG;
pio_cfg.pin_dp = 12;
tuh_configure(1, TUH_CFGID_RPI_PIO_USB_CONFIGURATION, &pio_cfg);
tusb_init();
```

GPIO12 を USB-A の D+ として使う前提です。
