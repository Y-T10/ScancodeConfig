# ScanMapConf
Windows向けのキーマップツール．
※現在開発中のため未完成の個所があります．

## 概要
ScanMapConfはWindows向けのソフトで、キーボードのキー割り当てを設定する．
キーを別のキーに割り当てるだけではなく、キーの無効化や、任意のキー番号(スキャンコード)に割り当てることができる．
このツールを用いることで、`Caps Lock`の無効化や`Ctrl`への変換を行うことができる．

キーの割り当て情報はWindowsのレジストリキー`Scancode Map`に保存される．
`Scancode Map`の説明は[こちら](https://learn.microsoft.com/ja-jp/windows-hardware/drivers/hid/keyboard-and-mouse-class-drivers#scan-code-mapper-for-keyboards)に記述されている．