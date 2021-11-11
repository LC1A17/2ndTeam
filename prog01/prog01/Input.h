#pragma once

#include <Windows.h>
#include <wrl.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

class Input
{
private:
	//namespace省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // サブクラス
	static enum PadKey
	{
		BUTTON_A, // Aボタン
		BUTTON_B, // Bボタン
		BUTTON_X, // Xボタン
		BUTTON_Y, // Yボタン
		BUTTON_LEFT_SHOULDER, // LBボタン
		BUTTON_RIGHT_SHOULDER, // RBボタン
		BUTTON_START, // STARTボタン
		BUTTON_BACK, // BACKボタン
		BUTTON_LEFT_THUMB, // 左スティック押し込み
		BUTTON_RIGHT_THUMB, // 右スティック押し込み
		BUTTON_DPAD_UP, // デジタル方向ボタン上
		BUTTON_DPAD_DOWN, // デジタル方向ボタン下
		BUTTON_DPAD_LEFT, // デジタル方向ボタン左
		BUTTON_DPAD_RIGHT // デジタル方向ボタン右
	};

public: //メンバ関数
	//初期化
	bool Initialize(HINSTANCE hInstance, HWND hwnd);
	//更新
	void Update();
	// キーの押下をチェック
	bool PushKey(BYTE keyNumber);
	// キーのトリガーをチェック
	bool TriggerKey(BYTE keyNumber);

	static BOOL CALLBACK DeviceFindCallBack(LPCDIDEVICEINSTANCE ipddi, LPVOID pvRef);

	//ゲームパッドのスティック
	bool PushPadStickUp();
	bool PushPadStickDown();
	bool PushPadStickRight();
	bool PushPadStickLeft();

	//ゲームパッドのキーの押下をチェック
	bool PushPadKey(PadKey keyNumber);
	// キーのトリガーをチェック
	bool TriggerPadKey(PadKey keyNumber);

private: //メンバ変数
	//キーボード
	ComPtr<IDirectInput8> dinput;
	ComPtr<IDirectInputDevice8> devkeyboard;
	BYTE key[256] = {};
	BYTE keyPre[256] = {};

	//ゲームパッド
	ComPtr<IDirectInput8> dinputPad;
	ComPtr<IDirectInputDevice8> devGamePad;
	LPVOID parameter;
	// 軸モードを絶対値モードとして設定
	DIPROPDWORD diprop;
	// 値の範囲設定
	DIPROPRANGE diprg;
	// 無反応範囲
	float angle = 200;
	// 入力情報
	DIJOYSTATE padData;
	DIJOYSTATE padDataPre;
	//接続確認
	bool padFlag = true;
};