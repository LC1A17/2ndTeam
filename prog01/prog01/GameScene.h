#pragma once

#include "SafeDelete.h"
#include "DirectXCommon.h"
#include <DirectXMath.h>
#include "Input.h"
#include "Sprite.h"
#include "Object3d.h"
#include "DebugText.h"
#include "Audio.h"
#include "Sound.h"
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
		Title, Game, Clear, GameOver
	};

public://メンバ関数
	GameScene();//コンストクラタ
	~GameScene();//デストラクタ
	void Initialize(DirectXCommon* dxCommon, Input* input, Sound* sound);//初期化
	void Update();//毎フレーム処理
	void Draw();//描画
	void StartCameraMove(); //最初のカメラの動き

private: // メンバ変数
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	Sound* sound = nullptr;
	DebugText debugText;

	// ゲームシーン用
	Sprite* sprite = nullptr;
	Sprite* gamestart = nullptr;
	Sprite* titleBack = nullptr;//タイトル
	Sprite* gameBack = nullptr;//ゲーム
	Sprite* endBack = nullptr;//エンド
	Sprite* enemyHp = nullptr;
	Sprite* enemyHpFlame = nullptr;
	Sprite* playerHp = nullptr;
	Sprite* playerHpFlame = nullptr;
	Sprite* gameover = nullptr;
	Sprite* gameBack2 = nullptr;
	Sprite* clear = nullptr;
	Sprite* takendamage = nullptr;
	Sprite* num0[6] = { nullptr };
	Sprite* num1[6] = { nullptr };
	Sprite* num2[6] = { nullptr };
	Sprite* num3[6] = { nullptr };
	Sprite* num4[6] = { nullptr };
	Sprite* num5[6] = { nullptr };
	Sprite* num6[6] = { nullptr };
	Sprite* num7[6] = { nullptr };
	Sprite* num8[6] = { nullptr };
	Sprite* num9[6] = { nullptr };
	Model* modelFighter = nullptr;

	Object3d* baseObj = nullptr;//フィールド
	Object3d* playerObj = nullptr;//プレイヤー
	Object3d* enemyObj = nullptr;//エネミー
	Object3d* eAtkObj = nullptr;//エネミー
	Object3d* eArmObj = nullptr;//エネミーの腕
	Object3d* pBulletObj[255] = { nullptr };//弾
	Object3d* eBulletObj[255] = { nullptr };//弾
	Object3d* eBulletObj2[255] = { nullptr };//弾
	Object3d* wallObj[24] = { nullptr };//弾

	ParticleManager* particleMan = nullptr;
	ParticleManager* playerParticleMan = nullptr;
	ParticleManager* enemyParticleMan = nullptr;

	Model* modelFighter1 = nullptr;

	int sceneNum = Title;//Title, Game, Clear, GameOverで管理
	int circle = 2;//プレイヤーのいる円周の位置。1が最低値で数が大きい方が外側
	int maxCircle = 3;//現在の円周の最大数

	XMFLOAT3 basePos = { 0,0,5 };//土台の座標
	XMFLOAT3 baseScale = { 230, 230, 230 };//土台のスケール
	XMFLOAT3 wallPos[24];//壁の座標
	XMFLOAT3 wallRota[24];//壁の向き
	XMFLOAT3 wallScale[24];//壁のスケール
	bool isWall[24] = { false };//壁が出ているか
	int wallHP[24] = { 10 };

	float playerHP = 10;//プレイヤーの体力
	float maxPlayerHP = 10;//プレイヤーの最大体力
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
	float pBullDamage[255] = { 0 };
	float speed = 2.0f;
	float accel = 1.5f;//加速の倍率
	int pDamageInterval = 50;//敵の被弾時の無敵時間

	float enemyHP = 100;//敵の体力
	float maxEnemyHP = 100;//敵の最大体力
	int eDamageInterval = 50;//敵の被弾時の無敵時間
	XMFLOAT3 ePos = { 0, 0, 0 };//敵の座標
	XMFLOAT3 eRot = { 0, 180, 0 };
	XMFLOAT3 eScale = { 10, 10, 10 };//敵の大きさ
	int eAttackInterval = 0;//敵の行動の間隔
	int wallCount = 0;
	XMFLOAT3 eBullPos[255];//敵の弾の座標
	XMFLOAT3 eBullScale[255];//敵の弾の大きさ
	bool eBull[255] = { false };//敵の弾が画面上に出ているかどうか
	float eBullSpeedX[255], eBullSpeedY[255];//敵の弾の挙動用
	float eBullX[255], eBullY[255], eBullXY[255];//敵の弾の挙動用

	XMFLOAT3 eBullPos2[255];//敵の弾の座標
	XMFLOAT3 eBullScale2[255];//敵の弾の大きさ
	bool eBull2[255] = { false };//敵の弾が画面上に出ているかどうか
	float eBullSpeed2X[255], eBullSpeed2Y[255];//敵の弾の挙動用
	float eBull2X[255], eBull2Y[255], eBull2XY[255];//敵の弾の挙動用

	XMFLOAT3 eArmPos[5];//腕の座標
	XMFLOAT3 eArmRot[5];//腕の傾き
	XMFLOAT3 eArmScale[5];//腕の大きさ

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
	float angleX = 0.0f;
	float angleZ = 0.0f;
	float eAngle = 0.0f;

	bool shakeFlag = false;
	int shakeTimer = 0;
	int attenuation = 0;

	XMVECTOR hoge;

	Camera* camera = nullptr;
	XMFLOAT3 fixedCamera;
	XMFLOAT3 fixed = camera->GetEye();
	XMVECTOR cameraMove = { 0, 80, 140 };
	bool cameraMoveCount[14];
	float cameraRad = 0.0f;
	float cameraAngle = 90.0f;

	float i = 1.0f;
	bool hit = false;

	int logoCount = 0;//タイトル点滅カウント
	bool isLoad = false;//ロード
	int loadCount = 0;//ロード時間
	int endCount = 0;//ゲームオーバー後の入力待ち時間
	int enemyMove[2] = { 0, 0 };//行動判定用
	int moveCount = 0;//行動した回数
	bool isLaser = true;//レーザー使用可能か
	bool isBarrier = true;//ダメージレーン使用可能か
	bool isAttack = true;//攻撃が終わったか
	bool laserAttack = false;//レーザー発動中か
	int laserCount = 0;//レーザーの時間
	bool wallCreate = false;//壁生成中か
	bool isArm = false;//腕攻撃中か
	bool eArm[5] = { false };//腕の表示
	int eArmCount = 0;//腕の動作用
	int iceCount = 0;
	float PI = 3.1415926;
	int bulletCount = 0;

	int clearTime = 0;

	float bulletAngle[16] = {};

	bool isSound[4] = { false };
	int SoundCount[4] = { 0 };

	bool isEnemyAtk = true;
};