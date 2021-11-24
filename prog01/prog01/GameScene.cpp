#include "GameScene.h"
#include <cassert>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <math.h>

using namespace std;
using namespace DirectX;

//コンストラクタ
GameScene::GameScene()
{

}

//デストラクタ
GameScene::~GameScene()
{
	safe_delete(sprite);
	safe_delete(baseObj);
	safe_delete(modelFighter);
	safe_delete(particleMan);
}

//初期化処理
void GameScene::Initialize(DirectXCommon* dxCommon, Input* input, Sound* sound)
{
	//nullptrチェック
	assert(dxCommon);
	assert(input);
	assert(sound);

	this->dxCommon = dxCommon;
	this->input = input;
	this->sound = sound;

	//デバッグテキスト用テクスチャ読み込み
	if (!Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png"))
	{
		assert(0);
	}

	//デバッグテキスト初期化
	debugText.Initialize(debugTextTexNumber);

	srand(time(NULL));//乱数初期化

	//テクスチャ読み込み
	if (!Sprite::LoadTexture(1, L"Resources/gamestart.png"))
	{
		assert(0);
	}

	if (!Sprite::LoadTexture(2, L"Resources/title.png"))
	{
		assert(0);
	}

	if (!Sprite::LoadTexture(3, L"Resources/game.png"))
	{
		assert(0);
	}

	if (!Sprite::LoadTexture(4, L"Resources/end.png"))
	{
		assert(0);
	}

	if (!Sprite::LoadTexture(5, L"Resources/enemyHp.png"))
	{
		assert(0);
	}

	if (!Sprite::LoadTexture(6, L"Resources/enemyHpFlame.png"))
	{
		assert(0);
	}

	if (!Sprite::LoadTexture(7, L"Resources/playerHp.png"))
	{
		assert(0);
	}

	if (!Sprite::LoadTexture(8, L"Resources/playerHpFlame.png"))
	{
		assert(0);
	}

	if (!Sprite::LoadTexture(9, L"Resources/gameover.png"))
	{
		assert(0);
	}

	if (!Sprite::LoadTexture(10, L"Resources/game2.png"))
	{
		assert(0);
	}

	if (!Sprite::LoadTexture(11, L"Resources/clear.png"))
	{
		assert(0);
	}

	if (!Sprite::LoadTexture(12, L"Resources/takendamage.png"))
	{
		assert(0);
	}

	if (!Sprite::LoadTexture(13, L"Resources/0.png"))
	{
		assert(0);
	}

	if (!Sprite::LoadTexture(14, L"Resources/1.png"))
	{
		assert(0);
	}

	if (!Sprite::LoadTexture(15, L"Resources/2.png"))
	{
		assert(0);
	}

	if (!Sprite::LoadTexture(16, L"Resources/3.png"))
	{
		assert(0);
	}

	if (!Sprite::LoadTexture(17, L"Resources/4.png"))
	{
		assert(0);
	}

	if (!Sprite::LoadTexture(18, L"Resources/5.png"))
	{
		assert(0);
	}

	if (!Sprite::LoadTexture(19, L"Resources/6.png"))
	{
		assert(0);
	}

	if (!Sprite::LoadTexture(20, L"Resources/7.png"))
	{
		assert(0);
	}

	if (!Sprite::LoadTexture(21, L"Resources/8.png"))
	{
		assert(0);
	}

	if (!Sprite::LoadTexture(22, L"Resources/9.png"))
	{
		assert(0);
	}

	//背景スプライト生成
	gamestart = Sprite::Create(1, { 0.0f,0.0f });
	gamestart->SetSize({ 830, 46 });
	gamestart->SetPosition({ 225.0f, WinApp::window_height - 184 });

	titleBack = Sprite::Create(2, { 0.0f,0.0f });
	titleBack->SetSize({ 875, 255 });
	titleBack->SetPosition({ 202.5f, 100.0f });

	gameBack = Sprite::Create(3, { 0.0f,0.0f });
	gameBack->SetSize({ WinApp::window_width, WinApp::window_height });
	gameBack->SetPosition({ 0.0f,0.0f });

	endBack = Sprite::Create(4, { 0.0f,0.0f });
	endBack->SetSize({ WinApp::window_width, WinApp::window_height });
	endBack->SetPosition({ 0.0f,0.0f });

	enemyHp = Sprite::Create(5, { 0.0f,0.0f });
	enemyHp->SetSize({ 872, 28 });
	enemyHp->SetPosition({ 212.0f,34.0f });

	enemyHpFlame = Sprite::Create(6, { 0.0f,0.0f });
	enemyHpFlame->SetSize({ 896, 51 });
	enemyHpFlame->SetPosition({ 200.0f,20.0f });

	playerHp = Sprite::Create(7, { 0.0f,0.0f });
	playerHp->SetSize({ 326, 28 });
	playerHp->SetPosition({ 22.0f, WinApp::window_height - 56 });

	playerHpFlame = Sprite::Create(8, { 0.0f,0.0f });
	playerHpFlame->SetSize({ 330, 36 });
	playerHpFlame->SetPosition({ 20.0f, WinApp::window_height - 60 });

	gameover = Sprite::Create(9, { 0.0f,0.0f });
	gameover->SetSize({ WinApp::window_width, WinApp::window_height });
	gameover->SetPosition({ 0.0f,0.0f });

	gameBack2 = Sprite::Create(10, { 0.0f,0.0f });
	gameBack2->SetSize({ WinApp::window_width, WinApp::window_height });
	gameBack2->SetPosition({ 0.0f,0.0f });

	clear = Sprite::Create(11, { 0.0f,0.0f });
	clear->SetSize({ 300.5, 45.5 });
	clear->SetPosition({ 0.0f, 300.0f });

	takendamage = Sprite::Create(12, { 0.0f,0.0f });
	takendamage->SetSize({ 413, 56 });
	takendamage->SetPosition({ 0.0f, 400.0f });

	for (int i = 0; i < 6; i++)
	{
		num0[i] = Sprite::Create(13, { 0.0f,0.0f });
		num0[i]->SetSize({ 17, 22 });
		num0[i]->SetPosition({ 0.0f,0.0f });

		num1[i] = Sprite::Create(14, { 0.0f,0.0f });
		num1[i]->SetSize({ 17, 22 });
		num1[i]->SetPosition({ 0.0f,0.0f });

		num2[i] = Sprite::Create(15, { 0.0f,0.0f });
		num2[i]->SetSize({ 17, 22 });
		num2[i]->SetPosition({ 0.0f,0.0f });

		num3[i] = Sprite::Create(16, { 0.0f,0.0f });
		num3[i]->SetSize({ 17, 22 });
		num3[i]->SetPosition({ 0.0f,0.0f });

		num4[i] = Sprite::Create(17, { 0.0f,0.0f });
		num4[i]->SetSize({ 17, 22 });
		num4[i]->SetPosition({ 0.0f,0.0f });

		num5[i] = Sprite::Create(18, { 0.0f,0.0f });
		num5[i]->SetSize({ 17, 22 });
		num5[i]->SetPosition({ 0.0f,0.0f });

		num6[i] = Sprite::Create(19, { 0.0f,0.0f });
		num6[i]->SetSize({ 17, 22 });
		num6[i]->SetPosition({ 0.0f,0.0f });

		num7[i] = Sprite::Create(20, { 0.0f,0.0f });
		num7[i]->SetSize({ 17, 22 });
		num7[i]->SetPosition({ 0.0f,0.0f });

		num8[i] = Sprite::Create(21, { 0.0f,0.0f });
		num8[i]->SetSize({ 17, 22 });
		num8[i]->SetPosition({ 0.0f,0.0f });

		num9[i] = Sprite::Create(22, { 0.0f,0.0f });
		num9[i]->SetSize({ 17, 22 });
		num9[i]->SetPosition({ 0.0f,0.0f });
	}

	//3Dオブジェクト生成

	//ベース
	modelFighter = modelFighter->CreateFromObject("base");
	baseObj = Object3d::Create();
	baseObj->SetModel(modelFighter);
	baseObj->SetScale({ baseScale });

	//プレイヤー
	modelFighter = modelFighter->CreateFromObject("player");
	playerObj = Object3d::Create();
	playerObj->SetModel(modelFighter);
	playerObj->SetPosition({ pPos });
	playerObj->SetScale({ pScale });

	//敵
	modelFighter = modelFighter->CreateFromObject("enemy");
	enemyObj = Object3d::Create();
	enemyObj->SetModel(modelFighter);
	enemyObj->SetPosition({ ePos });
	enemyObj->SetRotation({ eRot });
	enemyObj->SetScale({ eScale });

	//敵
	modelFighter = modelFighter->CreateFromObject("enemyArmAttack");
	eAtkObj = Object3d::Create();
	eAtkObj->SetModel(modelFighter);
	eAtkObj->SetPosition({ ePos });
	eAtkObj->SetRotation({ eRot });
	eAtkObj->SetScale({ eScale });

	//弾
	modelFighter = modelFighter->CreateFromObject("pBullet");

	for (int i = 0; i < 255; i++)
	{
		pOldPos[i] = { 1000, 1000, 1000 };
		pBullPos[i] = { 1000, 1000, 1000 };
		pBullScale[i] = { 10, 10, 10 };
		pBulletObj[i] = Object3d::Create();
		pBulletObj[i]->SetModel(modelFighter);
		pBulletObj[i]->SetPosition({ pBullPos[i] });
		pBulletObj[i]->SetScale({ pBullScale[i] });
	}

	modelFighter = modelFighter->CreateFromObject("eBullet");

	for (int i = 0; i < 255; i++)
	{
		eBullPos[i] = { 1000, 1000, 1000 };
		eBullScale[i] = { 10, 10, 10 };
		eBulletObj[i] = Object3d::Create();
		eBulletObj[i]->SetModel(modelFighter);
		eBulletObj[i]->SetPosition({ eBullPos[i] });
		eBulletObj[i]->SetScale({ eBullScale[i] });
	}

	for (int i = 0; i < 255; i++)
	{
		eBullPos2[i] = { 1000, 1000, 1000 };
		eBullScale2[i] = { 10, 10, 10 };
		eBulletObj2[i] = Object3d::Create();
		eBulletObj2[i]->SetModel(modelFighter);
		eBulletObj2[i]->SetPosition({ eBullPos2[i] });
		eBulletObj2[i]->SetScale({ eBullScale2[i] });
	}

	//腕
	for (int i = 0; i < 5; i++)
	{
		eArmPos[i] = { 0, 0, 0 };
		eArmRot[i] = { 0, 0, 0 };
		eArmScale[i] = { 10, 10, 10 };
		eArmObj = Object3d::Create();
		eArmObj->SetModel(modelFighter);
		eArmObj->SetPosition({ eArmPos[i] });
		eArmObj->SetRotation({ eArmRot[i] });
		eArmObj->SetScale({ eArmScale[i] });
	}

	//壁
	modelFighter = modelFighter->CreateFromObject("wall");

	wallPos[0] = { 0, 0, 95 };
	wallPos[1] = { 0, 0, -95 };
	wallPos[2] = { 95, 0, 0 };
	wallPos[3] = { -95, 0, 0 };
	wallPos[4] = { 0, 0, 155 };
	wallPos[5] = { 0, 0, -155 };
	wallPos[6] = { 155, 0, 0};
	wallPos[7] = { -155, 0, 0};
	wallPos[8] = { 0, 0, 215 };
	wallPos[9] = { 0, 0, -215 };
	wallPos[10] = { 215, 0, 0 };
	wallPos[11] = { -215, 0, 0 };
	wallPos[12] = { 60, 0, 60 };
	wallPos[13] = { 60, 0, -60 };
	wallPos[14] = { -60, 0, -60 };
	wallPos[15] = { -60, 0, 60 };
	wallPos[16] = { 100, 0, 90 };
	wallPos[17] = { 100, 0, -100 };
	wallPos[18] = { -100, 0, -100 };
	wallPos[19] = { -100, 0, 100 };
	wallPos[20] = { 140, 0, 140 };
	wallPos[21] = { 140, 0, -140 };
	wallPos[22] = { -140, 0, -140 };
	wallPos[23] = { -140, 0, 140 };

	wallRota[0] = { 0, 0, 0 };
	wallRota[1] = { 0, 0, 0 };
	wallRota[2] = { 0, 90, 0};
	wallRota[3] = { 0, 90, 0 };
	wallRota[4] = { 0, 0, 0 };
	wallRota[5] = { 0, 0, 0 };
	wallRota[6] = { 0, 90, 0 };
	wallRota[7] = { 0, 90, 0 };
	wallRota[8] = { 0, 0, 0 };
	wallRota[9] = { 0, 0, 0 };
	wallRota[10] = { 0, 90, 0 };
	wallRota[11] = { 0, 90, 0 };
	wallRota[12] = { 0, 45, 0 };
	wallRota[13] = { 0, -45, 0 };
	wallRota[14] = { 0, 45, 0 };
	wallRota[15] = { 0, -45, 0 };
	wallRota[16] = { 0, 45, 0 };
	wallRota[17] = { 0, -45, 0 };
	wallRota[18] = { 0, 45, 0 };
	wallRota[19] = { 0, -45, 0 };
	wallRota[20] = { 0, 45, 0 };
	wallRota[21] = { 0, -45, 0 };
	wallRota[22] = { 0, 45, 0 };
	wallRota[23] = { 0, -45, 0 };

	for (int i = 0; i < 24; i++)
	{
		wallScale[i] = { 20, 10, 5 };
		wallObj[i] = Object3d::Create();
		wallObj[i]->SetModel(modelFighter);
		wallObj[i]->SetPosition({ wallPos[i] });
		wallObj[i]->SetRotation({ wallRota[i] });
		wallObj[i]->SetScale({ wallScale[i] });
	}

	particleMan = ParticleManager::Create();//パーティクル生成
	playerParticleMan = ParticleManager::Create();//パーティクル生成
	enemyParticleMan = ParticleManager::Create();//パーティクル生成

	for (int i = 0; i < _countof(bulletAngle); i++)
	{
		bulletAngle[i] = 62 * i * (PI * PI);
	}

	Sound::SoundData soundData1 = sound->SoundLoadWave("Resources/audio/title.wav");
	Sound::SoundData soundData2 = sound->SoundLoadWave("Resources/audio/game.wav");
	Sound::SoundData soundData3 = sound->SoundLoadWave("Resources/audio/clear.wav");
	Sound::SoundData soundData4 = sound->SoundLoadWave("Resources/audio/gameOver.wav");
	//sound->SoundPlayWave(soundData1);//サウンド再生

	//カメラ
	for (int i = 0; i < _countof(cameraMoveCount); i++)
	{
		cameraMoveCount[i] = true;
	}

	camera->Update();
}

//更新処理
void GameScene::Update()
{
	//1:タイトル画面
	if (sceneNum == Title)
	{
		//タイトル点滅カウント
		logoCount++;

		if (logoCount == 40)
		{
			logoCount = 0;
		}

		//SPACEorSTARTを押すとロード開始
		if ((input->TriggerKey(DIK_SPACE) || input->TriggerPadKey(BUTTON_START)) && !isLoad)
		{
			loadCount = 0;
			isLoad = true;
		}

		//ロード
		if (isLoad)
		{
			loadCount++;
		}

		//ロード終了
		if (loadCount >= 20)
		{
			loadCount = 0;
			isLoad = false;

			//壁をランダムに5つ生成（被らなくなるまでループ）
			while (wallCount <= 4)
			{
				int w = rand() % 24;
				wallHP[w] = 10;
				isWall[w] = true;

				wallCount = 0;

				//出現している壁の数をカウント
				for (int i = 0; i < 24; i++)
				{
					if (isWall[i])
					{
						wallCount++;
					}
				}
			}

			sceneNum = Game;
		}
	}

	//2:ゲーム画面
	else if (sceneNum == Game)
	{
		//↓入力処理ここから↓

		//開始時カメラ中と被弾時と潜行時は入力を受け付けない
		if (!cameraMoveCount[13] && !hit && !isDive)
		{
			//内側へ潜行
			if (input->TriggerKey(DIK_UP) || input->TriggerPadKey(BUTTON_B))
			{
				//一番内側にいないなら潜行開始
				if (circle > 1)
				{
					direction = false;
					isDive = true;
					diveMove[0] = true;
				}
			}

			//外側へ潜行
			if (input->TriggerKey(DIK_DOWN) || input->TriggerPadKey(BUTTON_A))
			{
				//一番外側にいないなら潜行開始
				if (circle < maxCircle)
				{
					direction = true;
					isDive = true;
					diveMove[0] = true;
				}
			}

			//時計回りに地上を移動
			if (input->PushKey(DIK_LEFT) || input->PushPadStickLeft())
			{
				//LSHIFTを押している時は加速
				if (input->PushKey(DIK_LSHIFT) || input->TriggerPadKey(BUTTON_LEFT_SHOULDER))
				{
					angle -= speed * accel;
				}

				else
				{
					angle -= speed;
				}
			}

			//反時計回りに地上を移動
			if (input->PushKey(DIK_RIGHT) || input->PushPadStickRight())
			{
				//LSHIFTを押している時は加速
				if (input->PushKey(DIK_LSHIFT) || input->TriggerPadKey(BUTTON_LEFT_SHOULDER))
				{
					angle += speed * accel;
				}

				else
				{
					angle += speed;
				}
			}

			//弾を発射
			if (input->PushKey(DIK_SPACE) || input->PushPadKey(BUTTON_RIGHT_SHOULDER))
			{
				if (pBullInterval >= 30)
				{
					//画面上に存在しない弾を一つ選んで自機の位置にセット
					for (int i = 0; i < 255; i++)
					{
						//ボスの位置を捕捉
						if (!pBull[i])
						{
							if (circle == 1)
							{
								pBullDamage[i] = 3;
							}

							else if (circle == 2)
							{
								pBullDamage[i] = 2;
							}

							else
							{
								pBullDamage[i] = 1;
							}

							pOldPos[i] = pPos;
							pBullPos[i] = pPos;
							pBullPos[i].y += 20;
							pBulletObj[i]->SetPosition({ pBullPos[i] });
							pBullX[i] = ePos.x - pOldPos[i].x;
							pBullY[i] = ePos.z - pOldPos[i].z;
							pBullXY[i] = sqrt(pBullX[i] * pBullX[i] + pBullY[i] * pBullY[i]);
							pBullSpeedX[i] = pBullX[i] / pBullXY[i] * 10;
							pBullSpeedY[i] = pBullY[i] / pBullXY[i] * 10;
							pBull[i] = true;
							pBullInterval = 0;
							break;
						}
					}
				}
			}
		}

		//↑入力処理ここまで↑

		//↓更新処理ここから↓
		pBullInterval++;//プレイヤーの弾の発射間隔
		pDamageInterval++;//プレイヤーの被弾後の無敵時間
		len = 60.0f * circle;//現在の円周の位置
		playerHp->SetSize({ (326 / maxPlayerHP) * playerHP, 28 });//プレイヤーのHPバー
		enemyHp->SetSize({ (872 / maxEnemyHP) * enemyHP, 28 });//ボスのHPバー

		//出現している壁の数をカウント
		wallCount = 0;

		for (int i = 0; i < 24; i++)
		{
			if (isWall[i])
			{
				wallCount++;
			}
		}

		//プレイヤーの潜行
		if (isDive)
		{
			if (diveMove[0])
			{
				if (pPos.y >= -44)
				{
					pPos.y -= 4;
				}

				else
				{
					diveMove[0] = false;
					diveMove[1] = true;
				}
			}

			if (diveMove[1])
			{
				if (direction)
				{
					circle++;
				}

				else
				{
					circle--;
				}

				diveMove[1] = false;
				diveMove[2] = true;
			}

			if (diveMove[2])
			{
				if (pPos.y <= 0)
				{
					pPos.y += 4;
				}

				else
				{
					diveMove[2] = false;
					isDive = false;
				}
			}
		}

		//プレイヤーの弾
		for (int i = 0; i < 255; i++)
		{
			//弾の挙動
			if (pBull[i])
			{
				pBullPos[i].x += pBullSpeedX[i];
				pBullPos[i].z += pBullSpeedY[i];
				pBulletObj[i]->SetPosition({ pBullPos[i] });
			}

			//壁との判定
			for (int j = 0; j < 24; j++)
			{
				if (isWall[j])
				{
					if (wallPos[j].x - 16 < pBullPos[i].x + 5 && pBullPos[i].x - 5 < wallPos[j].x + 16 && wallPos[j].z - 10.5 < pBullPos[i].z + 5 && pBullPos[i].z - 5 < wallPos[j].z + 10.5)
					{
						wallHP[j]--;
						pBullPos[i] = { 1000, 1000, 1000 };
						pBulletObj[i]->SetPosition({ pBullPos[i] });
						pBull[i] = false;
					}
				}
			}

			//ボスとの判定
			float a = pBullPos[i].x - ePos.x;
			float b = pBullPos[i].z - ePos.z;
			float c = sqrt(a * a + b * b);

			if (c <= 10)
			{
				enemyHP -= pBullDamage[i];
				pBullPos[i] = { 1000, 1000, 1000 };
				pBulletObj[i]->SetPosition({ pBullPos[i] });
				pBull[i] = false;
				eDamageInterval = 0;
			}

			//範囲外に出た弾をfalseにする
			if (pBullPos[i].x <= -1000 || pBullPos[i].x >= 1000 || pBullPos[i].z <= -1000 || pBullPos[i].z >= 1000)
			{
				pBullPos[i] = { 1000, 1000, 1000 };
				pBulletObj[i]->SetPosition({ pBullPos[i] });
				pBull[i] = false;
			}
		}

		//プレイヤーのノックバック
		if (hit)
		{
			i -= 0.01f;

			if (i <= 0.8f && hit)
			{
				hit = false;
			}
		}

		//プレイヤーと壁の衝突判定
		if (hit)
		{
			for (int i = 0; i < 24; i++)
			{
				if (isWall[i])
				{
					if (wallPos[i].x - 16 < pPos.x + 11.5 && pPos.x - 11.5 < wallPos[i].x + 16 && wallPos[i].z - 10.5 < pPos.z + 12 && pPos.z - 12 < wallPos[i].z + 10.5)
					{
						playerHP -= 5;
						isWall[i] = false;
						shakeFlag = true;
					}
				}
			}
		}

		else if (!hit && i <= 1.0f)
		{
			i += 0.05f;
		}

		//ボスの弾
		for (int i = 0; i < 255; i++)
		{
			//弾の挙動
			if (eBull[i])
			{
				eBullPos[i].x += eBullSpeedX[i];
				eBullPos[i].z += eBullSpeedY[i];
				eBulletObj[i]->SetPosition({ eBullPos[i] });
			}

			if (eBull2[i])
			{
				eBullPos2[i].x += eBullSpeed2X[i];
				eBullPos2[i].z += eBullSpeed2Y[i];
				eBulletObj2[i]->SetPosition({ eBullPos2[i] });
			}

			//壁との判定
			for (int j = 0; j < 24; j++)
			{
				if (isWall[j])
				{
					if (wallPos[j].x - 16 < eBullPos[i].x + 5 && eBullPos[i].x - 5 < wallPos[j].x + 16 && wallPos[j].z - 10.5 < eBullPos[i].z + 5 && eBullPos[i].z - 5 < wallPos[j].z + 10.5)
					{
						wallHP[j]--;
						eBullPos[i] = { 1000, 1000, 1000 };
						eBulletObj[i]->SetPosition({ eBullPos[i] });
						eBull[i] = false;
					}

					if (wallPos[j].x - 16 < eBullPos2[i].x + 5 && eBullPos2[i].x - 5 < wallPos[j].x + 16 && wallPos[j].z - 10.5 < eBullPos2[i].z + 5 && eBullPos2[i].z - 5 < wallPos[j].z + 10.5)
					{
						wallHP[j]--;
						eBullPos2[i] = { 1000, 1000, 1000 };
						eBulletObj2[i]->SetPosition({ eBullPos2[i] });
						eBull2[i] = false;
					}
				}
			}

			//プレイヤーとの判定
			if (pDamageInterval >= 100 && !isDive)
			{
				float a = eBullPos[i].x - pPos.x;
				float b = eBullPos[i].z - pPos.z;
				float c = sqrt(a * a + b * b);

				if (c <= 10)
				{
					playerHP--;
					eBullPos[i] = { 1000, 1000, 1000 };
					eBulletObj[i]->SetPosition({ eBullPos[i] });
					eBull[i] = false;
					hit = true;
					pDamageInterval = 0;
				}
			}

			//プレイヤーとの判定
			if (pDamageInterval >= 100 && !isDive)
			{
				float a = eBullPos2[i].x - pPos.x;
				float b = eBullPos2[i].z - pPos.z;
				float c = sqrt(a * a + b * b);

				if (c <= 10)
				{
					playerHP--;
					eBullPos2[i] = { 1000, 1000, 1000 };
					eBulletObj2[i]->SetPosition({ eBullPos2[i] });
					eBull2[i] = false;
					hit = true;
					pDamageInterval = 0;
				}
			}

			//範囲外に出た弾をfalseにする
			if (eBullPos[i].x <= -1000 || eBullPos[i].x >= 1000 || eBullPos[i].z <= -1000 || eBullPos[i].z >= 1000)
			{
				eBullPos[i] = { 1000, 1000, 1000 };
				eBulletObj[i]->SetPosition({ eBullPos[i] });
				eBull[i] = false;
			}

			//範囲外に出た弾をfalseにする
			if (eBullPos2[i].x <= -1000 || eBullPos2[i].x >= 1000 || eBullPos2[i].z <= -1000 || eBullPos2[i].z >= 1000)
			{
				eBullSpeed2Y[i] = 0;
				eBullSpeed2X[i] = 0;
				eBullPos2[i] = { 1000, 1000, 1000 };
				eBulletObj2[i]->SetPosition({ eBullPos2[i] });
				eBull2[i] = false;
			}
		}

		for (int i = 0; i < 24; i++)
		{
			if (wallHP[i] <= 0)
			{
				isWall[i] = false;
			}
		}

		//↓この辺よくわからないからコメントアウト書いて↓
		rad = angle * 3.14f / 180.0f;
		aroundX = cos(rad) * len / i;
		aroundZ = sin(rad) * len / i;
		pPos.x = posX + aroundX;
		pPos.z = posZ + aroundZ;

		if (!cameraMoveCount[13] && !shakeFlag)
		{
			playerObj->SetPosition({ pPos });

			fixedCamera.x = cos(rad) * len * 2.4f;
			fixedCamera.y = fixed.y;
			fixedCamera.z = sin(rad) * len * 2.4f;

			camera->SetEye(fixedCamera);
		}

		rad = angle * 3.14159265359f / 180.0f;

		aroundX = cos(rad) * len / i;
		aroundZ = sin(rad) * len / i;

		pPos = playerObj->GetPosition();

		pPos.x = aroundX;
		pPos.z = aroundZ;
		fixedCamera.x = cos(rad) * len * 2;
		fixedCamera.y = fixed.y;
		fixedCamera.z = sin(rad) * len * 2;

		if (!cameraMoveCount[13] && !shakeFlag)
		{
			playerObj->SetBillboard(true);
			playerObj->SetRotation({ 0,180,0 });
			playerObj->SetPosition(pPos);
			camera->SetEye(fixedCamera);
			camera->Update();
		}

		//↑この辺よくわからないからコメントアウト書いて↑

		//開始時カメラ
		if (cameraMoveCount[13])
		{
			StartCameraMove();
			camera->Update();
		}

		//開始時カメラが終わるまで更新しない
		if (!cameraMoveCount[13])
		{
			clearTime++;

			if (isAttack)
			{
				eAttackInterval++;//ボスの攻撃間隔
			}

			//ボスの行動パターン（発狂前）
			if (enemyHP > maxEnemyHP / 2)
			{
				//攻撃間隔チェック
				if (eAttackInterval >= 60)
				{
					eAttackInterval = 0;//攻撃間隔リセット
					moveCount++;

					//大技が溜まっていないとき
					if (moveCount <= 7)
					{
						enemyMove[0] = rand() % 80 + 1;//行動パターンを決定

						//1:自機狙いの弾を発射
						if (enemyMove[0] <= 30)
						{
							enemyMove[1] = rand() % 100 + 1;//行動パターンを決定

							//0:冰気錬成
							if (enemyMove[1] <= 20)
							{
								wallCreate = true;
							}

							isLaser = true;
							angleX = pPos.x - ePos.x;
							angleZ = pPos.z - ePos.z;

							eAngle = atan2(angleX, angleZ);

							enemyObj->SetRotation({ 0, XMConvertToDegrees(eAngle) - 180, 0 });
							eAtkObj->SetRotation({ 0, XMConvertToDegrees(eAngle) - 180, 0 });

							//画面上に存在しない弾を一つ選んでボスの位置にセット
							for (int i = 0; i < 255; i++)
							{
								if (!eBull[i])
								{
									eBullPos[i] = ePos;
									eBullPos[i].y += 35;
									eBulletObj[i]->SetPosition({ eBullPos[i] });
									eBullX[i] = pPos.x - ePos.x;
									eBullY[i] = pPos.z - ePos.z;
									eBullXY[i] = sqrt(eBullX[i] * eBullX[i] + eBullY[i] * eBullY[i]);
									eBullSpeedX[i] = eBullX[i] / eBullXY[i] * 10;
									eBullSpeedY[i] = eBullY[i] / eBullXY[i] * 10;
									eBull[i] = true;
									break;
								}
							}
						}

						//2:16方向に大きめの弾を2発発射
						else if (31 <= enemyMove[0] && enemyMove[0] <= 55)
						{
							enemyMove[1] = rand() % 100 + 1;//行動パターンを決定

							//0:冰気錬成
							if (enemyMove[1] <= 20)
							{
								wallCreate = true;
							}

							isLaser = true;
							bulletCount = 0;
							angleX = pPos.x - ePos.x;
							angleZ = pPos.z - ePos.z;

							eAngle = atan2(angleX, angleZ);

							enemyObj->SetRotation({ 0, XMConvertToDegrees(eAngle) - 180, 0 });
							eAtkObj->SetRotation({ 0, XMConvertToDegrees(eAngle) - 180, 0 });

							for (int i = 0; i < 16; i++)
							{
								for (int j = 0; j < 255; j++)
								{
									if (!eBull2[j])
									{
										eBull2[j] = true;
										eBullPos2[j] = ePos;
										eBulletObj2[j]->SetPosition({ eBullPos2[j] });
										eBullSpeed2Y[j] = sin(62.5 * i * (PI * PI)) * 2;
										eBullSpeed2X[j] = cos(62.5 * i * (PI * PI)) * 2;
										break;
									}
								}
							}
						}

						//3:（半）時計回りに1周するレーザー状の弾幕
						else if (56 <= enemyMove[0] && enemyMove[0] <= 80)
						{
							enemyMove[1] = rand() % 100 + 1;//行動パターンを決定

							//0:冰気錬成
							if (enemyMove[1] <= 20)
							{
								wallCreate = true;
							}

							//レーザー使用可能ならレーザー
							if (isLaser)
							{
								laserAttack = true;
								isLaser = false;
								isAttack = false;
							}

							//使えないなら自機狙い弾
							else
							{
								angleX = pPos.x - ePos.x;
								angleZ = pPos.z - ePos.z;

								eAngle = atan2(angleX, angleZ);

								enemyObj->SetRotation({ 0, XMConvertToDegrees(eAngle) - 180, 0 });

								isLaser = true;
								angleX = pPos.x - ePos.x;
								angleZ = pPos.z - ePos.z;

								eAngle = atan2(angleX, angleZ);

								enemyObj->SetRotation({ 0, XMConvertToDegrees(eAngle) - 180, 0 });
								eAtkObj->SetRotation({ 0, XMConvertToDegrees(eAngle) - 180, 0 });

								//画面上に存在しない弾を一つ選んでボスの位置にセット
								for (int i = 0; i < 255; i++)
								{
									if (!eBull[i])
									{
										eBullPos[i] = ePos;
										eBullPos[i].y += 35;
										eBulletObj[i]->SetPosition({ eBullPos[i] });
										eBullX[i] = pPos.x - ePos.x;
										eBullY[i] = pPos.z - ePos.z;
										eBullXY[i] = sqrt(eBullX[i] * eBullX[i] + eBullY[i] * eBullY[i]);
										eBullSpeedX[i] = eBullX[i] / eBullXY[i] * 10;
										eBullSpeedY[i] = eBullY[i] / eBullXY[i] * 10;
										eBull[i] = true;
										break;
									}
								}
							}
						}

						//4:ダメージレーンを展開
						else if (81 <= enemyMove[0] && enemyMove[0] <= 100)
						{
							enemyMove[1] = rand() % 100 + 1;//行動パターンを決定

							//0:冰気錬成
							if (enemyMove[1] <= 20)
							{
								wallCreate = true;
							}

							isLaser = true;

							//ダメージレーン使用可能なら展開
							if (isLaser)
							{

							}

							//使えないなら16方向弾
							else
							{
								
							}
						}
					}

					//5:巨大な腕で押し潰す
					else
					{
						moveCount = 0;
						eArmCount = 0;
						iceCount = 0;
						isArm = true;
						isAttack = false;

						for (int i = 0; i < 5; i++)
						{
							eArmPos[i] = { 0, 60, 0 };
							eArmScale[i] = { 0, 0, 0 };
							eArmObj->SetPosition({ eArmPos[i] });
							eArmObj->SetScale({ eArmScale[i] });
							eArm[i] = false;
						}
					}
				}
			}

			//ボスの行動パターン（発狂後）
			else
			{
				//攻撃間隔チェック
				if (eAttackInterval >= 40)
				{
					//大技チェック
					if (moveCount <= 3)
					{

					}
				}
			}

			//冰気錬成
			if (wallCreate)
			{
				int w = rand() % 24;

				if (isWall[w])
				{
					while (isWall[w])
					{
						w = rand() % 24;
					}
				}

				if (wallCount < 10)
				{
					wallHP[w] = 10;
					wallPos[w].y = -32;
					isWall[w] = true;
					wallObj[w]->SetPosition({ wallPos[w] });
				}

				wallCreate = false;
			}

			//壁を生やす
			for (int i = 0; i < 24; i++)
			{
				if (isWall[i])
				{
					if (wallPos[i].y < 0)
					{
						wallPos[i].y += 4;
						wallObj[i]->SetPosition({ wallPos[i] });
					}
				}
			}

			//レーザー攻撃
			if (laserAttack)
			{
				laserAttack = false;
				isAttack = true;
			}

			//腕攻撃
			if (isArm)
			{
				eArmCount++;

				if (eArmCount >= 30)
				{
					for (int i = 0; i < 5; i++)
					{
						if (!eArm[i])
						{
							eArmPos[i] = pPos;
							eArmPos[i].y = 120;
							eArmObj->SetPosition({ eArmPos[i] });
							eArm[i] = true;
							eArmCount = 0;
							iceCount++;
							break;
						}
					}
				}

				if (iceCount >= 3)
				{
					eArmCount = 0;
					iceCount = 0;
					isArm = false;
					isAttack = true;
				}
			}

			for (int i = 0; i < 5; i++)
			{
				if (eArm[i])
				{
					if (eArmScale[i].x <= 20)
					{
						eArmScale[i].x += 2;
						eArmScale[i].y += 2;
						eArmScale[i].z += 2;
						eArmObj->SetScale({ eArmScale[i] });
					}

					else
					{
						if (eArmPos[i].y >= 0)
						{
							eArmPos[i].y -= 6;
							eArmObj->SetPosition({ eArmPos[i] });
						}
					}
				}
			}

			/*
			//腕
			if (eArm == true)
			{
				eArmCount++;

				if (eArmPos.y >= -300)
				{
					eArmPos.y -= 16;
					eArmObj->SetPosition({ eArmPos });
				}

				if (eArmCount >= 90)
				{
					eArmCount = 0;
					eArm = false;
				}
			}

			//ボスの挙動
			if (eAttackInterval >= 50)
			{
				//プレイヤーの位置を参照
				if (circle == 1)
				{
					if (eArm == false)
					{
						eArmPos = pPos;
						eArmPos.x += 120;
						eArm = true;
						eArmObj->SetPosition({ eArmPos });
					}
				}
			}
			*/
		}

		//ボスの体力が0になったらクリア
		if (enemyHP <= 0)
		{
			sceneNum = Clear;
		}

		//プレイヤーの体力が0になったらゲームオーバー
		if (playerHP <= 0 && enemyHP > 0)
		{
			sceneNum = GameOver;
		}

		//↑更新処理ここまで↑
	}

	//3:リザルト画面
	else if (sceneNum == Clear)
	{
		endCount++;//入力待ちカウント

		//一定フレーム経ったら入力可能
		if (endCount >= 120)
		{
			//タイトル点滅カウント
			logoCount++;

			if (logoCount == 40)
			{
				logoCount = 0;
			}

			//SPACEorSTARTを押すとロード開始
			if ((input->TriggerKey(DIK_SPACE) || input->TriggerPadKey(BUTTON_START)) && !isLoad)
			{
				loadCount = 0;
				isLoad = true;
			}
		}

		//ロード
		if (isLoad)
		{
			loadCount++;
		}

		//各種初期化を行いタイトルへ
		if (loadCount >= 20)
		{
			endCount = 0;
			loadCount = 0;
			isLoad = false;
			sceneNum = Title;
		}

		/*
		//スペースを押すとタイトルに戻る
		if (input->TriggerKey(DIK_SPACE) || input->TriggerPadKey(BUTTON_START))
		{
			for (int i = 0; i < _countof(cameraMoveCount); i++)
			{
				cameraMoveCount[i] = true;
			}

			for (int i = 0; i < 30; i++)
			{
				isWall[i] = false;
			}

			for (int i = 0; i < 255; i++)
			{
				pBull[i] = false;
			}
			
			cameraMove = { 0, 80, 140 };
			circle = 2;//プレイヤーのいる円周の位置。1が最低値で数が大きい方が外側
			maxCircle = 3;//現在の円周の最大数
			playerHP = 100;//プレイヤーの体力
			pPos = { 0, 0, 120 };//プレイヤーの座標
			pRot = { 0, 0, 0 };//プレイヤーの傾き
			pBullInterval = 30;
			speed = 2.0f;
			enemyHP = 10;//敵の体力
			ePos = { 0, 0, 0 };//敵の座標
			eDamageInterval = 50;//敵の被弾時の無敵時間
			angle = 90.0f;
			len = 60.0f;
			aroundX = 0.0f;
			aroundZ = 0.0f;
			sceneNum = Title;

			playerObj->SetPosition({ pPos });
			playerObj->SetRotation({ pPos });

		}
		*/
	}

	//4:ゲームオーバー画面
	else
	{
		endCount++;//入力待ちカウント

		//一定フレーム経ったら入力可能
		if (endCount >= 120)
		{
			//タイトル点滅カウント
			logoCount++;

			if (logoCount == 40)
			{
				logoCount = 0;
			}

			//SPACEorSTARTを押すとロード開始
			if ((input->TriggerKey(DIK_SPACE) || input->TriggerPadKey(BUTTON_START)) && !isLoad)
			{
				loadCount = 0;
				isLoad = true;
			}
		}

		//ロード
		if (isLoad)
		{
			loadCount++;
		}

		//各種初期化を行いタイトルへ
		if (loadCount >= 20)
		{
			endCount = 0;
			loadCount = 0;
			isLoad = false;
			sceneNum = Title;
		}
	}

	baseObj->Update();
	playerObj->Update();

	if (isEnemyAtk)
	{
		eAtkObj->Update();
	}

	else
	{
		enemyObj->Update();
	}

	eArmObj->Update();

	for (int i = 0; i < 255; i++)
	{
		pBulletObj[i]->Update();
		eBulletObj[i]->Update();
		eBulletObj2[i]->Update();
	}

	for (int i = 0; i < 24; i++)
	{
		wallObj[i]->Update();
	}

	
	XMFLOAT3 i = fixedCamera;

	//シェイク
	if (shakeFlag)
	{
		shakeTimer++;
	}

	if (shakeTimer > 0)
	{
		i.x = ((rand() + 80) % (91 - attenuation) - 5);
		i.y = ((rand() + 80) % (91 - attenuation) - 5);
	}

	if (shakeTimer >= attenuation * 60)
	{
		attenuation += 1;
	}

	else if (attenuation >= 100)
	{
		shakeTimer = 0;
		attenuation = 0;
		shakeFlag = false;
	}

	camera->SetEye(i);
	camera->Update();
	

	//パーティクル
	if (circle == 3)
	{
		playerParticleMan->Update();
		enemyParticleMan->EaseInUpdate(ePos);
	}

	if (sceneNum == Game)
	{
		for (int i = 0; i < 10; i++)
		{
			//X,Y,Z全て[-5.0,+5.0]でランダムに分布
			const float md_pos = 10.0f;
			XMFLOAT3 pos{};
			pos.x = ((float)rand() / RAND_MAX * md_pos - md_pos / 2.0f) + pPos.x;
			pos.y = ((float)rand() / RAND_MAX * md_pos - md_pos / 2.0f) + pPos.y;
			pos.z = ((float)rand() / RAND_MAX * md_pos - md_pos / 2.0f) + pPos.z;
			//X,Y,Z全て[-0.05,+0.05]でランダムに分布
			const float md_vel = 0.1f;
			XMFLOAT3 vel{};
			vel.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
			vel.y = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
			vel.z = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
			//重力に見立ててYのみ[-0.001f,0]でランダムに分布
			XMFLOAT3 acc{};
			const float md_acc = 0.001f;
			acc.y = -(float)rand() / RAND_MAX * md_acc;
			//色をランダムに分布
			XMFLOAT4 color;
			color.w = 1;
			color.x = (float)rand() / RAND_MAX * 1;
			color.y = (float)rand() / RAND_MAX * 1;
			color.z = (float)rand() / RAND_MAX * 1;
			//追加
			playerParticleMan->Add(60, pos, vel, acc, 10.0f, 0.0f, color, { 0,0,0,0 });
		}

		playerParticleMan->Update();
		enemyParticleMan->EaseInUpdate(ePos);

		for (int i = 0; i < 1; i++)
		{
			//X,Y,Z全て[-5.0,+5.0]でランダムに分布
			const float md_pos = 10.0f;
			//敵のパーティクルの発生範囲
			XMFLOAT3 ePos{};
			ePos.x = ((float)rand() / RAND_MAX * md_pos - md_pos / 2.0f);
			ePos.y = ((float)rand() / RAND_MAX * md_pos - md_pos / 2.0f);
			ePos.z = ((float)rand() / RAND_MAX * md_pos - md_pos / 2.0f);
			//X,Y,Z全て[-0.05,+0.05]でランダムに分布
			const float md_vel = 0.1f;
			XMFLOAT3 vel{};
			vel.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
			vel.y = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
			vel.z = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
			//重力に見立ててYのみ[-0.001f,0]でランダムに分布
			XMFLOAT3 acc{};
			const float md_acc = 0.001f;
			acc.y = -(float)rand() / RAND_MAX * md_acc;
			//色をランダムに分布
			XMFLOAT4 color;
			color.w = 1;
			color.x = (float)rand() / RAND_MAX * 1;
			color.y = (float)rand() / RAND_MAX * 1;
			color.z = (float)rand() / RAND_MAX * 1;
			//追加
			enemyParticleMan->Add(60, ePos, vel, acc, 10.0f, 0.0f, color, { 0,0,0,0 });
		}

		playerParticleMan->Update();
		enemyParticleMan->EaseInUpdate(ePos);
	}
}

//描画処理
void GameScene::Draw()
{
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();//コマンドリストの取得

#pragma region 背景スプライト描画

	Sprite::PreDraw(dxCommon->GetCommandList());//背景スプライト描画前処理

	//1:タイトル画面
	if (sceneNum == Title)
	{
		gameBack->Draw();//背景
	}

	//2:ゲーム画面
	else if (sceneNum == Game)
	{
		gameBack2->Draw();//背景
	}

	//3:リザルト画面
	else if (sceneNum == Clear)
	{
		gameBack2->Draw();//背景
		endBack->Draw();//背景
	}

	//4:ゲームオーバー画面
	else
	{
		gameBack2->Draw();//背景
		gameover->Draw();//背景
	}

	Sprite::PostDraw();//スプライト描画後処理
	dxCommon->ClearDepthBuffer();//深度バッファクリア

#pragma endregion 背景スプライト描画

#pragma region 3Dオブジェクト描画

	Object3d::PreDraw(dxCommon->GetCommandList());//3Dオブジェクト描画前処理

	//1:タイトル画面
	if (sceneNum == Title)
	{

	}

	//2:ゲーム画面
	else if (sceneNum == Game)
	{
		baseObj->Draw();
		playerObj->Draw();

		if (isEnemyAtk)
		{
			eAtkObj->Draw();
		}

		else
		{
			enemyObj->Draw();
		}

		for (int i = 0; i < 5; i++)
		{
			if (eArm[i] == true)
			{
				eArmObj->Draw();
			}
		}

		for (int i = 0; i < 255; i++)
		{
			if (pBull[i])
			{
				pBulletObj[i]->Draw();
			}

			if (eBull[i])
			{
				eBulletObj[i]->Draw();
			}

			if (eBull2[i])
			{
 				eBulletObj2[i]->Draw();
			}
		}

		for (int i = 0; i < 24; i++)
		{
			if (isWall[i] == true)
			{
				wallObj[i]->Draw();
			}
		}
	}

	//3:リザルト画面
	else if (sceneNum == Clear)
	{

	}

	//4:ゲームオーバー画面
	else
	{

	}

	//object3d1->Draw();
	Object3d::PostDraw();//3Dオブジェクト描画後処理

#pragma endregion 3Dオブジェクト描画

#pragma region パーティクル

	ParticleManager::PreDraw(dxCommon->GetCommandList());

	//1:タイトル画面
	if (sceneNum == Title)
	{

	}

	//2:ゲーム画面
	else if (sceneNum == Game)
	{
		playerParticleMan->Draw();
		enemyParticleMan->Draw();
		//particleMan->Draw();
	}

	//3:リザルト画面
	else if (sceneNum == Clear)
	{

	}

	//4:ゲームオーバー画面
	else
	{

	}

	ParticleManager::PostDraw();

#pragma endregion パーティクル

#pragma region 前景スプライト描画

	Sprite::PreDraw(dxCommon->GetCommandList());//前景スプライト描画前処理
	debugText.DrawAll(dxCommon->GetCommandList());//デバッグテキストの描画

	//1:タイトル画面
	if (sceneNum == Title)
	{
		titleBack->Draw();//タイトルロゴ

		//ゲームスタート（点滅あり）
		if (logoCount <= 20)
		{
			gamestart->Draw();//背景スプライト描画
		}
	}

	//2:ゲーム画面
	else if (sceneNum == Game)
	{
		if (!cameraMoveCount[13])
		{
			playerHpFlame->Draw();
			playerHp->Draw();
			enemyHpFlame->Draw();
			enemyHp->Draw();
		}
	}

	//3:リザルト画面
	else if (sceneNum == Clear)
	{
		clear->Draw();//背景スプライト描画
		takendamage->Draw();//背景スプライト描画

		for (int i = 0; i < 6; i++)
		{
			num0[i]->Draw();
			num1[i]->Draw();
			num2[i]->Draw();
			num3[i]->Draw();
			num4[i]->Draw();
			num5[i]->Draw();
			num6[i]->Draw();
			num7[i]->Draw();
			num8[i]->Draw();
			num9[i]->Draw();
		}

		//ゲームスタート（点滅あり）
		if (logoCount <= 20 && endCount >= 120)
		{
			gamestart->Draw();//背景スプライト描画
		}
	}

	//4:ゲームオーバー画面
	else
	{
		//ゲームスタート（点滅あり）
		if (logoCount <= 20 && endCount >= 120)
		{
			gamestart->Draw();//背景スプライト描画
		}
	}

	Sprite::PostDraw();//スプライト描画後処理

#pragma endregion 前景スプライト描画
}

//開始時カメラ
void GameScene::StartCameraMove()
{
	if (cameraMove.m128_f32[0] >= -140 && cameraMoveCount[0])
	{
		cameraMove.m128_f32[0] -= 1;

		if (cameraMove.m128_f32[0] <= -140)
		{
			cameraMoveCount[0] = false;
		}
	}
	else if (cameraMove.m128_f32[0] <= 0 && cameraMoveCount[3])
	{
		cameraMove.m128_f32[0] += 1;

		if (cameraMove.m128_f32[0] >= 0)
		{
			cameraMoveCount[3] = false;
		}
	}
	else if (cameraMove.m128_f32[0] <= 140 && cameraMoveCount[6])
	{
		cameraMove.m128_f32[0] += 1;

		if (cameraMove.m128_f32[0] >= 140)
		{
			cameraMoveCount[6] = false;
		}
	}
	else if (cameraMove.m128_f32[0] >= 0 && cameraMoveCount[9])
	{
		cameraMove.m128_f32[0] -= 1;

		if (cameraMove.m128_f32[0] <= 0)
		{
			cameraMoveCount[9] = false;
		}
	}

	if (cameraMove.m128_f32[1] <= 105 && cameraMoveCount[1])
	{
		cameraMove.m128_f32[1] += 0.2f;

		if (cameraMove.m128_f32[1] >= 105)
		{
			cameraMoveCount[1] = false;
		}
	}
	else if (cameraMove.m128_f32[1] <= 130 && cameraMoveCount[4])
	{
		cameraMove.m128_f32[1] += 0.2f;

		if (cameraMove.m128_f32[1] >= 130)
		{
			cameraMoveCount[4] = false;
		}
	}
	else if (cameraMove.m128_f32[1] <= 155 && cameraMoveCount[7])
	{
		cameraMove.m128_f32[1] += 0.2f;

		if (cameraMove.m128_f32[1] >= 155)
		{
			cameraMoveCount[7] = false;
		}
	}
	else if (cameraMove.m128_f32[1] <= 180 && cameraMoveCount[10])
	{
		cameraMove.m128_f32[1] += 0.2f;

		if (cameraMove.m128_f32[1] >= 180)
		{
			cameraMoveCount[10] = false;
		}
	}
	else if (cameraMove.m128_f32[1] >= 80 && cameraMoveCount[12])
	{
		cameraMove.m128_f32[1] -= 2;

		if (cameraMove.m128_f32[1] <= 80)
		{
			cameraMoveCount[12] = false;
		}
	}

	if (cameraMove.m128_f32[2] >= 0 && cameraMoveCount[2])
	{
		cameraMove.m128_f32[2] -= 1;

		if (cameraMove.m128_f32[2] <= 0)
		{
			cameraMoveCount[2] = false;
		}
	}
	else if (cameraMove.m128_f32[2] >= -140 && cameraMoveCount[5])
	{
		cameraMove.m128_f32[2] -= 1;

		if (cameraMove.m128_f32[2] <= -140)
		{
			cameraMoveCount[5] = false;
		}
	}
	else if (cameraMove.m128_f32[2] <= 0 && cameraMoveCount[8])
	{
		cameraMove.m128_f32[2] += 1;

		if (cameraMove.m128_f32[2] >= 0)
		{
			cameraMoveCount[8] = false;
		}
	}
	else if (cameraMove.m128_f32[2] <= 40 && cameraMoveCount[11])
	{
		cameraMove.m128_f32[2] += 0.15;

		if (cameraMove.m128_f32[2] >= 20)
		{
			cameraMoveCount[11] = false;
		}
	}
	else if (cameraMove.m128_f32[2] <= sin(rad) * len * 2.1f && cameraMoveCount[13])
	{
		cameraMove.m128_f32[2] += 2;

		if (cameraMove.m128_f32[2] >= sin(rad) * len * 2.1f)
		{
			cameraMove.m128_f32[2] = sin(rad) * len * 2.1f;
			cameraMoveCount[13] = false;
		}
	}

	camera->SetEye({ cameraMove.m128_f32[0], cameraMove.m128_f32[1], cameraMove.m128_f32[2] });
}