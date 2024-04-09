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

## 使用しているライブラリのライセンス

- doctest
The MIT License (MIT)

Copyright (c) 2016-2023 Viktor Kirilov

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
