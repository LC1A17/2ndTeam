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
#include "Camera.h"

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
	using XMVECTOR = DirectX::XMVECTOR;

private: // 静的メンバ変数
	static const int debugTextTexNumber = 0;

public:
	//SceneNum用
	enum Scene
	{
		Title, Game, End
	};

public://メンバ関数
	GameScene();//コンストクラタ
	~GameScene();//デストラクタ
	void Initialize(DirectXCommon* dxCommon, Input* input, Audio* audio);//初期化
	void Update();//毎フレーム処理
	void Draw();//描画
	void StartCameraMove(); //最初のカメラの動き

private: // メンバ変数
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	Audio* audio = nullptr;
	DebugText debugText;

	// ゲームシーン用
	Sprite* sprite = nullptr;
	Sprite* titleBack = nullptr;//タイトル
	Sprite* gameBack = nullptr;//ゲーム
	Sprite* endBack = nullptr;//エンド
	Model* modelFighter = nullptr;

	Object3d* baseObj = nullptr;//フィールド
	Object3d* playerObj = nullptr;//プレイヤー
	Object3d* enemyObj = nullptr;//エネミー
	Object3d* eArmObj = nullptr;//エネミーの腕
	Object3d* pBulletObj[255] = { nullptr };//弾
	Object3d* eBulletObj[255] = { nullptr };//弾
	Object3d* wallObj[30] = { nullptr };//弾

	ParticleManager* playerParticleMan = nullptr;
	ParticleManager* enemyParticleMan = nullptr;

	Model* modelFighter1 = nullptr;

	int sceneNum = Game;//Title, Game, Endで管理
	int circle = 2;//プレイヤーのいる円周の位置。1が最低値で数が大きい方が外側
	int maxCircle = 3;//現在の円周の最大数

	XMFLOAT3 basePos = { 0,0,5 };//土台の座標
	XMFLOAT3 baseScale = { 200, 200, 200 };//土台のスケール
	XMFLOAT3 wallPos[30];//壁の座標
	XMFLOAT3 wallRota[30];//壁の向き
	XMFLOAT3 wallScale[30];//壁のスケール
	bool isWall[30] = { false };//壁が出ているか

	int playerHP = 100;//プレイヤーの体力
	XMFLOAT3 pPos = { 0, 0, 120 };//プレイヤーの座標
	XMFLOAT3 pRot = { 0, 0, 0 };//プレイヤーの傾き
	XMFLOAT3 pScale = { 5, 5, 5 };//プレイヤーの大きさ
	XMFLOAT3 pOldPos[255];//プレイヤーの座標
	XMFLOAT3 pBullPos[255];//プレイヤーの弾の座標
	XMFLOAT3 pBullScale[255];//プレイヤーの弾の大きさ
	int pBullInterval = 30;
	bool pBull[255] = { false };//プレイヤーの弾が画面上に出ているかどうか
	float pBullSpeedX[255], pBullSpeedY[255];//プレイヤーの弾の挙動用
	float pBullX[255], pBullY[255], pBullXY[255];//プレイヤーの弾の挙動用
	float speed = 2.0f;
	float accel = 1.5f;//加速の倍率
	int pDamageInterval = 50;//敵の被弾時の無敵時間

	int enemyHP = 10;//敵の体力
	int eDamageInterval = 50;//敵の被弾時の無敵時間
	XMFLOAT3 ePos = { 0, 0, 0 };//敵の座標
	XMFLOAT3 eScale = { 10, 10, 10 };//敵の大きさ
	int eAttackInterval = 0;//敵の行動の間隔
	int wallCount = 0;
	XMFLOAT3 eBullPos[255];//敵の弾の座標
	XMFLOAT3 eBullScale[255];//敵の弾の大きさ
	bool eBull[255] = { false };//敵の弾が画面上に出ているかどうか
	float eBullSpeedX[255], eBullSpeedY[255];//敵の弾の挙動用
	float eBullX[255], eBullY[255], eBullXY[255];//敵の弾の挙動用

	XMFLOAT3 eArmPos = { 0, 0, 0 };//腕の座標
	XMFLOAT3 eArmRot = { 0, 0, 0 };//腕の傾き
	XMFLOAT3 eArmScale = { 60, 60, 60 };//腕の大きさ
	bool eArm = false;//腕の表示
	int eArmCount = 0;//腕の動作用

	bool isDive = false;//潜る
	bool direction = false;//潜るときの進行方向
	bool diveMove[3] = { false };//潜った後の移動

	float posX = 0.0f;
	float posZ = 0.0f;

	float rad = 0.0f;
	float angle = 90.0f;
	float len = 60.0f;
	float aroundX = 0.0f;
	float aroundZ = 0.0f;

	XMVECTOR hoge;

	Camera* camera = nullptr;
	XMFLOAT3 fixedCamera;
	XMFLOAT3 fixed = camera->GetEye();
	XMVECTOR cameraMove = { 0, 80, 140 };
	bool cameraMoveCount[14];
	float cameraRad = 0.0f;
	float cameraAngle = 90.0f;

	float i =1.0f;
	bool hit = false;
};