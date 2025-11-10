# DIY7K
Raspberry Pi Pico (RP2040)を使用した７鍵コントローラー

自分用なのでコードは雑です。多分更新もしません。

## Usage

Arduino-IDEでuf2ファイルを生成して、Picoに書き込んでください。

依存関係については、以下のDependenciesをご覧ください。

自衛のため、VID/PIDは仮のものになっています。

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

## Dependencies

* **quadrature_encoder.pio (BSD-3-Clause)**
* Copyright (c) 2023 Raspberry Pi (Trading) Ltd.
* [https://github.com/raspberrypi/pico-examples/blob/master/pio/quadrature_encoder/quadrature_encoder.pio]

## LICENSES
このプロジェクトはMITライセンスです。詳細はLICENSEをご覧ください。

The project is licensed under the MIT License - see the LICENSE.txt file for details.

このプロジェクトは、サードパーティのコードやライブラリを参照しています。それらのライセンスについては、NOTICE.txtをご覧ください。

This project uses third-party libraries. Please see the NOTICE.txt file for their respective licenses.
