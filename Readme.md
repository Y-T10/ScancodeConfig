# ScancodeConf
SDL製のWindows向けのキーマップツール．
※現在開発中のため未完成の部分やバグがあります．

## 概要
ScancodeConfはWindows内部にキーボードのキー割当を設定します．
この割当はレジストリのキー`Scancode Map`が持ち、設定後に再起動をすることで有効になります．
ユーザはこのアプリを使うことで、キー入力の置換や無効化を行うことができます．

[SharpKeys](https://github.com/randyrants/sharpkeys)や[Change Key](https://satoshi3.sakura.ne.jp/f_soft/dw_win.htm)
などの既存のアプリは、キー割当先のキー名を特定のキーボードレイアウトに沿って表示するようにしています．
また、レジストリ書き込みに必要となる管理者権限を起動時に必要とするため、
割当の内容確認や変更などの権限を必要としない場合でも権限を持ってしまいます．

これに対してScancodeConfは、割当先のキー名をユーザがWindowsに指定したキーボードレイアウトに従って表示します．
また起動時に管理者権限を要求せず、レジストリキーへの値書き込みにて権限を要求します．
そのため、既存のアプリが抱えていた問題を解消しながらその機能を実現しています．

## `Scancode Map`について
`Scancode Map`の説明は下記のドキュメントに書かれています．
- ["Configuration of keyboard and mouse class drivers", Scan code mapper for keyboards](https://learn.microsoft.com/ja-jp/windows-hardware/drivers/hid/keyboard-and-mouse-class-drivers#scan-code-mapper-for-keyboards)

また設定時に使用する値の説明は下記のドキュメントに書かれています．
- ["Keyboard Input Overview", Scan Codes](https://learn.microsoft.com/en-us/windows/win32/inputdev/about-keyboard-input#scan-codes)

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
現在GUIのインターフェイスライブラリを変更しているため、安定したビルド方法が確立されていません．
参考として、これまでに行ってきたビルド方法を記載しておきます．

### MSYS2 (MINGW64)
このアプリは[MSYS2](https://www.msys2.org/)の`MINGW64`環境にて開発されています．
そのため、これらのセットアップを事前に完了させておく必要があります．

まず`MINGW64`環境のMSYS2シェルを開き、下記のコマンドにて必要なツールやライブラリをインストールして下さい．
```
pacman -S mingw-w64-x86_64-fontconfig \
          mingw-w64-x86_64-cmake \
          mingw-w64-x86_64-boost \
          mingw-w64-x86_64-SDL2 \
          mingw-w64-x86_64-gcc \
```
インストール後、このファイルがあるディレクトリにて下記のコマンドを実行し、プログラムをビルドしてください．
```
cmake -S ./ -B build
cmake --build build
```
ビルド完了後、ディレクトリ下のbinに下記のプログラム生成されていることを確認してください．
これらがあればビルド完了です．

- `AppScancodeConf-sdl`: GUIプログラム
- `AppScanMapWriter`: レジストリキー`Scancode Map`への書き込みを行うプログラム．

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
