# ScanMapConf
Windows向けのキーマップツール．

## 概要
ScanMapConfはWindows向けのソフトで、キーボードのキー割り当てを設定する．
キーを別のキーに割り当てるだけではなく、キーの無効化や、任意のキー番号(スキャンコード)に割り当てることができる．
このツールを用いることで、`Caps Lock`の無効化や`Ctrl`への変換を行うことができる．

キーの割り当て情報はWindowsのレジストリキー`Scancode Map`に保存される．
`Scancode Map`の説明は[こちら](https://learn.microsoft.com/ja-jp/windows-hardware/drivers/hid/keyboard-and-mouse-class-drivers#scan-code-mapper-for-keyboards)に記述されている．

## プログラムについて
Qt版とSDL版があります．どちらも現在開発中のため、未完成の個所やバグがあります．

Qt版は試作段階のもので、GUIライブラリにQt6を用いています．
ライブラリのLGPLライセンスによって、実行時にQt6の共有ライブラリを必要とします．
現在は下記の機能を備えています．
- キー割当の追加や変更．
- 割当一覧を持つレジストリの読み書き．
- 一覧のインポートとエクスポート．

SDL版は本番段階のもので、GUIライブライのSDLを用いて作成されています．
このライブラリはQt6よりも緩いライセンスが適用されているため、コンパイル時に必要なライブラリをプログラムに組み込むことができます．
現在は下記の機能を備えています．
- 割当一覧を持つレジストリの読み書き．
- 一覧のインポートとエクスポート．

## ブランチについて
Qt版とSDL版は別々のブランチで管理されています．
前者は[`qt-version`](https://github.com/Y-T10/ScancodeConfig/tree/qt-version)にて、
後者は[`main`](https://github.com/Y-T10/ScancodeConfig/tree/main)にて管理されています．
これら2つの実装ではビルド方法やライブラリのライセンス表記などが異なるため、
これらを参照する際は各ブランチに移動してください．
Qt版の`Readme.md`は[こちら](/../qt-version/Readme.md)です．
以降、このドキュメントはSDL版について扱っていきます．

## ビルド方法
### MSYS2 (MINGW64)
現在GUIのインターフェイスライブラリを変更しているため、安定したビルド方法が確立されていません．

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

- CPM.cmake
MIT License

Copyright (c) 2019-2022 Lars Melchior and contributors

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

- ImGui
The MIT License (MIT)

Copyright (c) 2014-2024 Omar Cornut

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
