# ScanMapConf
Windows向けのキーマップツール．
※現在開発中のため未完成の個所があります．

## 概要
ScanMapConfはWindows向けのソフトで、キーボードのキー割り当てを設定する．
キーを別のキーに割り当てるだけではなく、キーの無効化や、任意のキー番号(スキャンコード)に割り当てることができる．
このツールを用いることで、`Caps Lock`の無効化や`Ctrl`への変換を行うことができる．

キーの割り当て情報はWindowsのレジストリキー`Scancode Map`に保存される．
`Scancode Map`の説明は[こちら](https://learn.microsoft.com/ja-jp/windows-hardware/drivers/hid/keyboard-and-mouse-class-drivers#scan-code-mapper-for-keyboards)に記述されている．

## ビルド方法
### MSYS2 (MINGW64)
バージョンが13以上の`gcc`とバージョンが3.20以上の`cmake`を用意する．具体的に追加するパッケージは次のとおり．

```
mingw-w64-x86_64-gcc, pacman -S mingw-w64-x86_64-cmake
```

GUIライブラリとしてQt6を用いているため以下のパッケージを追加する．

```
mingw-w64-x86_64-qt6-base
```

以上のパッケージをインストールした状態でMINGW64環境のシェルを開き、以下のとおりにコマンドを実行する．

```
cd /tmp
git clone https://github.com/Y-T10/ScancodeConfig.git
cd ScancodeConfig
cmake -S ./ -B build
cmake --build build
```

ビルドが完了すると、`ScancodeConfig`ディレクトリ内に`bin`ディレクトリが作成されており、その中にプログラムが生成されている．