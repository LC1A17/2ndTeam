#pragma once

#include "SafeDelete.h"
#include "DirectXCommon.h"
#include <DirectXMath.h>
#include "Input.h"
#include "Sprite.h"
#include "Object3d.h"
#include "DebugText.h"
#include "Audio.h"
#include "Model.h"
#include "ParticleManager.h"

class GameScene
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private: // 静的メンバ変数
	static const int debugTextTexNumber = 0;

public:
	//SceneNum用
	enum Scene
	{
		Title, Game, End
	};

public: // メンバ関数
	// コンストクラタ
	GameScene();
	// デストラクタ
	~GameScene();
	// 初期化
	void Initialize(DirectXCommon* dxCommon, Input* input, Audio* audio);
	// 毎フレーム処理
	void Update();
	// 描画
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	Audio* audio = nullptr;
	DebugText debugText;

	// ゲームシーン用
	Sprite* sprite = nullptr;
	Model* modelFighter = nullptr;
	
	Object3d* baseObj = nullptr;//フィールド
	Object3d* playerObj = nullptr;//プレイヤー
	Object3d* enemyObj = nullptr;//エネミー
	Object3d* bulletObj[255] = { nullptr };//弾

	ParticleManager* particleMan = nullptr;

	Model* modelFighter1 = nullptr;

	int sceneNum = Game;//Title, Game, Endで管理
	int circle = 2;//プレイヤーのいる円周の位置。1が最低値で数が大きい方が外側
	int maxCircle = 3;//現在の円周の最大数

	int playerHP = 100;//プレイヤーの体力
	XMFLOAT3 pPos = { 100, 1, 0 };//プレイヤーの座標
	XMFLOAT3 pScale = { 10, 10, 10 };//プレイヤーの大きさ
	XMFLOAT3 pBullPos[255];//プレイヤーの弾の座標
	XMFLOAT3 pBullScale[255];//プレイヤーの弾の大きさ
	bool pBull[255] = { false };//プレイヤーの弾が画面上に出ているかどうか

	int enemyHP = 100;//敵の体力
	XMFLOAT3 ePos = { 0, 0, 0 };//敵の座標
	XMFLOAT3 eScale = { 5, 5, 5 };//敵の大きさ
};