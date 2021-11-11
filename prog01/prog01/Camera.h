#pragma once

#include <DirectXMath.h>
#include "Object3d.h"
#include "ParticleManager.h"

class Camera
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // 静的メンバ関数
	// 視点座標の取得
	static const XMFLOAT3& GetEye() { return eye; }
	// 視点座標の設定
	static void SetEye(XMFLOAT3 eye) { Camera::eye = eye; }
	// 注視点座標の取得
	static const XMFLOAT3& GetTarget() { return target; }
	// 注視点座標の設定
	static void SetTarget(XMFLOAT3 target) { Camera::target = target; }
	// ベクトルの取得
	static const XMFLOAT3& GetUp() { return up; }
	// ベクトルの設定
	static void SetUp(XMFLOAT3 up) { Camera::up = up; }
	// ベクトルによる移動
	static void CameraMoveVector(XMFLOAT3 move);
	static void CameraMoveEyeVector(XMFLOAT3 move);

private: // 静的メンバ変数
	// 視点座標
	static XMFLOAT3 eye;
	// 注視点座標
	static XMFLOAT3 target;
	// 上方向ベクトル
	static XMFLOAT3 up;

public: // メンバ関数
	// 毎フレーム処理
	void Update();

private: // メンバ変数
	Object3d* object3d = nullptr;
	ParticleManager* particleManager = nullptr;
};

