# DIY7K
Raspberry Pi Pico (RP2040)を使用した７鍵コントローラー

自分用なのでコードは雑です。多分更新もしません。

## Usage

PicoのBOOTSELを押しながらPCに接続し、firmware/rp2040/buildにあるuf2ファイルを書き込みます。

自衛のため、VID/PIDは仮のものになっています。
変更したい方はArduino-Pico環境でコンパイルしてください。

GPIOのアサインについてはconfig.hをご覧ください。

### Settings

START + SELECT + １鍵を３秒以上長押ししてから離すと、設定モードに入ることができます。

１鍵： 鍵盤LEDモード変更
２鍵： スクラッチLEDモード変更
３鍵： キーボードデバウンスタイム変更 
４鍵： スクラッチ感度変更（値が小さいほど感度が高い）
５鍵： なし
６鍵： スクラッチLEDカラー変更
７鍵： スクラッチLED明るさ変更

START(長押し): EEPROMに設定を保存
SELECT(長押し): 初期値に設定をリセット

## LICENSES
このプロジェクトはMITライセンスです。詳細はLICENSE.txtをご覧ください。
The project is licensed under the MIT License - see the LICENSE.txt file for details.

このプロジェクトは、サードパーティのコードを含んでいます。それらのライセンスについては、NOTICE.txtをご覧ください。
This project includes third-party software. Please see the NOTICE.txt file for their respective licenses.
