#include "Input.h"
#include <cassert>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

bool Input::Initialize(HINSTANCE hInstance, HWND hwnd)
{
	HRESULT result = S_FALSE;

	//�������i��x�����s�������j
	result = DirectInput8Create
	(
		hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr
	);
	if (FAILED(result))
	{
		assert(0);
		return result;
	}

	//�L�[�{�[�h�f�o�C�X�̐���
	result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);
	if (FAILED(result))
	{
		assert(0);
		return result;
	}

	//���̓f�[�^�`���̃Z�b�g
	result = devkeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
	{
		assert(0);
		return result;
	}

	//�r�����䃌�x���̃Z�b�g
	result = devkeyboard->SetCooperativeLevel
	(
		hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY
	);
	if (FAILED(result))
	{
		assert(0);
		return result;
	}

	//�������i��x�����s�������j
	result = DirectInput8Create
	(
		hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinputPad, nullptr
	);
	if (FAILED(result))
	{
		assert(0);
		return result;
	}

	// �f�o�C�X�̗�
	if (FAILED(dinputPad->EnumDevices(DI8DEVTYPE_GAMEPAD, DeviceFindCallBack, &parameter, DIEDFL_ATTACHEDONLY)))
	{
		assert(0);
		return result;
	}

	result = dinputPad->CreateDevice(GUID_Joystick, &devGamePad, NULL);
	if (FAILED(result))
	{
		padFlag = false;
	}

	if (padFlag == true)
	{
		// �f�o�C�X�̃t�H�[�}�b�g�̐ݒ�
		result = devGamePad->SetDataFormat(&c_dfDIJoystick);
		if (FAILED(result))
		{
			assert(0);
			return result;
		}

		// �����[�h���Βl���[�h�Ƃ��Đݒ�
		//ZeroMemory(&diprop, sizeof(diprop));
		diprop.diph.dwSize = sizeof(diprop);
		diprop.diph.dwHeaderSize = sizeof(diprop.diph);
		diprop.diph.dwHow = DIPH_DEVICE;
		diprop.diph.dwObj = 0;
		diprop.dwData = DIPROPAXISMODE_ABS;	// ��Βl���[�h�̎w��(DIPROPAXISMODE_REL�ɂ����瑊�Βl)
		// �����[�h��ύX
		result = devGamePad->SetProperty(DIPROP_AXISMODE, &diprop.diph);
		if (FAILED(result))
		{
			assert(0);
			return result;
		}

		// X���̒l�͈̔͐ݒ�
		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.diph.dwObj = DIJOFS_X;
		diprg.lMin = -1000;
		diprg.lMax = 1000;
		result = devGamePad->SetProperty(DIPROP_RANGE, &diprg.diph);
		if (FAILED(result))
		{
			assert(0);
			return result;
		}

		// Y���̒l�͈̔͐ݒ�
		diprg.diph.dwObj = DIJOFS_Y;
		result = devGamePad->SetProperty(DIPROP_RANGE, &diprg.diph);
		if (FAILED(result))
		{
			assert(0);
			return result;
		}

		result = devGamePad->SetCooperativeLevel(hwnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
		if (FAILED(result))
		{
			assert(0);
			return result;
		}
	}

	return true;
}

void Input::Update()
{
	HRESULT result;

	//�L�[�{�[�h���̎擾�J�n
	result = devkeyboard->Acquire();
	// �O��̃L�[���͂�ۑ�
	memcpy(keyPre, key, sizeof(key));
	//�S�L�[�̓��͏�Ԃ��擾����
	result = devkeyboard->GetDeviceState(sizeof(key), key);

	if (padFlag == true)
	{
		// ����J�n
		result = devGamePad->Acquire();
		// �O��̃L�[���͂�ۑ�
		memcpy(&padDataPre, &padData, sizeof(padData));
		// �Q�[���p�b�h�̓��͏��擾
		result = devGamePad->GetDeviceState(sizeof(padData), &padData);
	}
}

bool Input::PushKey(BYTE keyNumber)
{
	// �ُ�Ȉ��������o
	assert(0 <= keyNumber && keyNumber <= 256);

	// 0�łȂ���Ή����Ă���
	if (key[keyNumber])
	{
		return true;
	}

	// �����Ă��Ȃ�
	return false;
}

bool Input::TriggerKey(BYTE keyNumber)
{
	// �ُ�Ȉ��������o
	assert(0 <= keyNumber && keyNumber <= 256);

	// �O��0�ŁA����0�łȂ���΃g���K�[
	if (!keyPre[keyNumber] && key[keyNumber])
	{
		return true;
	}

	// �g���K�[�łȂ�
	return false;
}

BOOL CALLBACK Input::DeviceFindCallBack(LPCDIDEVICEINSTANCE ipddi, LPVOID pvRef)
{
	return DIENUM_CONTINUE;
}

bool Input::PushPadStickUp()
{
	if (padData.lY < -angle && padData.lX < angle * 4.3f &&
		padData.lY < -angle && padData.lX > -angle * 4.2f)
	{
		return true;
	}

	return false;
}

bool Input::PushPadStickDown()
{
	if (padData.lY > angle && padData.lX < angle * 4.2f &&
		padData.lY > angle && padData.lX > -angle * 4.2f)
	{
		return true;
	}

	return false;
}

bool Input::PushPadStickRight()
{
	if (padData.lX > angle && padData.lY > -angle * 4.3f &&
		padData.lX > angle && padData.lY < angle * 4.2f)
	{
		return true;
	}

	return false;
}

bool Input::PushPadStickLeft()
{
	if (padData.lX < -angle && padData.lY < angle * 4.2f &&
		padData.lX < -angle && padData.lY > -angle * 4.2f)
	{
		return true;
	}

	return false;
}

bool Input::PushPadKey(PadKey keyNumber)
{
	// 0�łȂ���Ή����Ă���
	if (padData.rgbButtons[keyNumber])
	{
		return true;
	}

	// �����Ă��Ȃ�
	return false;
}

bool Input::TriggerPadKey(PadKey keyNumber)
{
	// �O��0�ŁA����0�łȂ���΃g���K�[
	if (!padDataPre.rgbButtons[keyNumber] && padData.rgbButtons[keyNumber])
	{
		return true;
	}

	// �g���K�[�łȂ�
	return false;
}