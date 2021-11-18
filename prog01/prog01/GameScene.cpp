#include "GameScene.h"
#include <cassert>
#include <sstream>
#include <iomanip>

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
void GameScene::Initialize(DirectXCommon* dxCommon, Input* input, Audio* audio)
{
	//nullptrチェック
	assert(dxCommon);
	assert(input);
	assert(audio);

	this->dxCommon = dxCommon;
	this->input = input;
	this->audio = audio;

	//デバッグテキスト用テクスチャ読み込み
	if (!Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png"))
	{
		assert(0);
	}

	//デバッグテキスト初期化
	debugText.Initialize(debugTextTexNumber);

	//テクスチャ読み込み
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

	//背景スプライト生成
	titleBack = Sprite::Create(2, { 0.0f,0.0f });
	titleBack->SetSize({ WinApp::window_width, WinApp::window_height });
	titleBack->SetPosition({ 0.0f,0.0f });

	gameBack = Sprite::Create(3, { 0.0f,0.0f });
	gameBack->SetSize({ WinApp::window_width, WinApp::window_height });
	gameBack->SetPosition({ 0.0f,0.0f });

	endBack = Sprite::Create(4, { 0.0f,0.0f });
	endBack->SetSize({ WinApp::window_width, WinApp::window_height });
	endBack->SetPosition({ 0.0f,0.0f });

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
	enemyObj->SetScale({ eScale });

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
		eBullScale[i] = { 20, 20, 20 };
		eBulletObj[i] = Object3d::Create();
		eBulletObj[i]->SetModel(modelFighter);
		eBulletObj[i]->SetPosition({ pBullPos[i] });
		eBulletObj[i]->SetScale({ pBullScale[i] });
	}

	//壁
	modelFighter = modelFighter->CreateFromObject("wall");

	for (int i = 0; i < 30; i++)
	{
		wallPos[i] = { 1000, 1000, 1000 };
		wallRota[i] = { 0, 0, 0 };
		wallScale[i] = { 20, 10, 5 };
		wallObj[i] = Object3d::Create();
		wallObj[i]->SetModel(modelFighter);
		wallObj[i]->SetPosition({ wallPos[i] });
		wallObj[i]->SetRotation({ wallRota[i] });
		wallObj[i]->SetScale({ wallScale[i] });
	}

	wallPos[26] = { 0, 0, 155 };
	wallPos[27] = { 0, 0, -155 };
	wallPos[28] = { 155, 0, 0};
	wallPos[29] = { -155, 0, 0};
	wallRota[28] = { 0, 90, 0};
	wallRota[29] = { 0, 90, 0 };
	wallObj[26]->SetPosition({ wallPos[26] });
	wallObj[27]->SetPosition({ wallPos[27] });
	wallObj[28]->SetPosition({ wallPos[28] });
	wallObj[29]->SetPosition({ wallPos[29] });
	wallObj[28]->SetRotation({ wallRota[28] });
	wallObj[29]->SetRotation({ wallRota[29] });
	isWall[26] = true;
	isWall[27] = true;
	isWall[28] = true;
	isWall[29] = true;

	//腕
	modelFighter = modelFighter->CreateFromObject("enemyArm");
	eArmObj = Object3d::Create();
	eArmObj->SetModel(modelFighter);
	eArmObj->SetPosition({ eArmPos });
	eArmObj->SetRotation({ eArmRot });
	eArmObj->SetScale({ eArmScale });

	particleMan = ParticleManager::Create();

	//サウンド再生
	//audio->PlayWave("Resources/Alarm01.wav");

	for (int i = 0; i < _countof(cameraMoveCount); i++)
	{
		cameraMoveCount[i] = true;
	}

	camera->Update();
}

//更新処理
void GameScene::Update()
{
	hoge.m128_f32[0] = circle;
	std::ostringstream spherestr;
	spherestr << "Sphere:("
		<< std::fixed << std::setprecision(2)
		<< hoge.m128_f32[0] << ")";

	//1:タイトル画面
	if (sceneNum == Title)
	{
		//スペースを押すと開始
		if (input->TriggerKey(DIK_SPACE))
		{
			sceneNum = Game;
		}

		debugText.Print("Title", 0, 0, 1.0f);
	}

	//2:ゲーム画面
	else if (sceneNum == Game)
	{
		rad = angle * 3.14f / 180.0f;

		aroundX = cos(rad) * len / i;
		aroundZ = sin(rad) * len / i;

		pPos.x = posX + aroundX;
		pPos.z = posZ + aroundZ;

		if (!cameraMoveCount[13])
		{
			playerObj->SetPosition({ pPos });

			fixedCamera.x = cos(rad) * len * 2.4f;
			fixedCamera.y = fixed.y;
			fixedCamera.z = sin(rad) * len * 2.4f;

			camera->SetEye(fixedCamera);
		}

		if (hit)
		{
			i -= 0.01f;
			if (i <= 0.8f && hit)
			{
				hit = false;
			}
		}
		else if (!hit && i <= 1.0f)
		{
			i += 0.05f;
		}

		//入力処理
		//軸を移動
		if ((input->TriggerKey(DIK_UP) || input->PushPadStickUp()) && !cameraMoveCount[13] && !hit && !isDive)
		{
			//内側へ移動
			//一番内側にいないなら移動
			if (circle > 1)
			{
				direction = false;
				isDive = true;
				diveMove[0] = true;
			}
		}

		if ((input->TriggerKey(DIK_DOWN) || input->PushPadStickDown()) && !cameraMoveCount[13] && !hit && !isDive)
		{
			//外側へ移動
			//一番外側にいないなら移動
			if (circle < maxCircle)
			{
				direction = true;
				isDive = true;
				diveMove[0] = true;
			}
		}

		if (circle == 1)
		{
			len = 60.0f;
		}

		if (circle == 2)
		{
			len = 120.0f;
		}

		if (circle == 3)
		{
			len = 180.0f;
		}

		//円周上を移動
		if ((input->PushKey(DIK_LEFT) || input->PushPadStickLeft()) && !cameraMoveCount[13] && !hit && !isDive)
		{
			//反時計回りに移動
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

		if ((input->PushKey(DIK_RIGHT) || input->PushPadStickRight()) && !cameraMoveCount[13] && !hit && !isDive)
		{
			//時計回りに移動
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

		rad = angle * 3.14159265359f / 180.0f;

		aroundX = cos(rad) * len / i;
		aroundZ = sin(rad) * len / i;

		pPos = playerObj->GetPosition();

		pPos.x = aroundX;
		pPos.z = aroundZ;
		fixedCamera.x = cos(rad) * len * 2;
		fixedCamera.y = fixed.y;
		fixedCamera.z = sin(rad) * len * 2;



		if (!cameraMoveCount[13])
		{
			playerObj->SetBillboard(true);
			playerObj->SetRotation({ 0,180,0 });
			playerObj->SetPosition(pPos);
			camera->SetEye(fixedCamera);
			camera->Update();
		}

		//弾を発射
		if ((input->PushKey(DIK_SPACE) || input->PushPadKey(BUTTON_RIGHT_SHOULDER)) && !cameraMoveCount[13] && !hit && !isDive)
		{
			if (pBullInterval >= 30)
			{
				//画面上に存在しない弾を一つ選んで自機の位置にセット
				for (int i = 0; i < 255; i++)
				{
					if (pBull[i] == false)
					{
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

		if (!cameraMoveCount[13])
		{
			//更新処理
			pBullInterval++;
			pDamageInterval++;
			eDamageInterval++;
			eAttackInterval++;

			//壁の数
			for (int i = 0; i < 25; i++)
			{
				if (isWall[i] == true)
				{
					wallCount++;
				}
			}

			//壁との判定
			if (hit)
			{
				for (int i = 0; i < 30; i++)
				{
					if (isWall[i] == true)
					{
						if (wallPos[i].x - 16 < pPos.x + 11.5 && pPos.x - 11.5 < wallPos[i].x + 16 && wallPos[i].z - 10.5 < pPos.z + 12 && pPos.z - 12 < wallPos[i].z + 10.5)
						{
							playerHP -= 5;
							isWall[i] = false;
						}
					}
				}
			}

			//潜る
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
				if (pBull[i] == true)
				{
					pBullPos[i].x += pBullSpeedX[i];
					pBullPos[i].z += pBullSpeedY[i];
					pBulletObj[i]->SetPosition({ pBullPos[i] });
				}

				//壁との判定
				for (int j = 0; j < 30; j++)
				{
					if (isWall[j] == true)
					{
						if (wallPos[j].x - 16 < pBullPos[i].x + 5 && pBullPos[i].x - 5 < wallPos[j].x + 16 && wallPos[j].z - 10.5 < pBullPos[i].z + 5 && pBullPos[i].z - 5 < wallPos[j].z + 10.5)
						{
							pBullPos[i] = { 1000, 1000, 1000 };
							pBulletObj[i]->SetPosition({ pBullPos[i] });
							pBull[i] = false;
						}
					}
				}

				//敵との判定
				float a = pBullPos[i].x - ePos.x;
				float b = pBullPos[i].z - ePos.z;
				float c = sqrt(a * a + b * b);

				if (c <= 10)
				{
					enemyHP--;
					pBullPos[i] = { 1000, 1000, 1000 };
					pBulletObj[i]->SetPosition({ pBullPos[i] });
					pBull[i] = false;
					eDamageInterval = 0;
				}

				//画面外に出た弾をfalseにする
				if (pBullPos[i].x <= -1000 || pBullPos[i].x >= 1000 || pBullPos[i].z <= -1000 || pBullPos[i].z >= 1000)
				{
					pBullPos[i] = { 1000, 1000, 1000 };
					pBulletObj[i]->SetPosition({ pBullPos[i] });
					pBull[i] = false;
				}
			}

			//敵の弾
			for (int i = 0; i < 255; i++)
			{
				//弾の挙動
				if (eBull[i] == true)
				{
					eBullPos[i].x += eBullSpeedX[i];
					eBullPos[i].z += eBullSpeedY[i];
					eBulletObj[i]->SetPosition({ eBullPos[i] });
				}

				//壁との判定
				for (int j = 0; j < 30; j++)
				{
					if (isWall[j] == true)
					{
						if (wallPos[j].x - 16 < eBullPos[i].x + 5 && eBullPos[i].x - 5 < wallPos[j].x + 16 && wallPos[j].z - 10.5 < eBullPos[i].z + 5 && eBullPos[i].z - 5 < wallPos[j].z + 10.5)
						{
							eBullPos[i] = { 1000, 1000, 1000 };
							eBulletObj[i]->SetPosition({ eBullPos[i] });
							eBull[i] = false;
						}
					}
				}

				//プレイヤーとの判定
				if (pDamageInterval >= 100 && isDive == false)
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

				//画面外に出た弾をfalseにする
				if (eBullPos[i].x <= -1000 || eBullPos[i].x >= 1000 || eBullPos[i].z <= -1000 || eBullPos[i].z >= 1000)
				{
					eBullPos[i] = { 1000, 1000, 1000 };
					eBulletObj[i]->SetPosition({ eBullPos[i] });
					eBull[i] = false;
				}
			}

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

				else if (circle == 2)
				{
					if (wallCount < 1)
					{
						for (int i = 0; i < 30; i++)
						{
							if (isWall[i] == false)
							{
								wallPos[i] = pPos;
								wallPos[i].z += 40;
								isWall[i] = true;
								wallObj[i]->SetPosition({ wallPos[i] });
								break;
							}
						}
					}
				}

				else if (circle == 3)
				{
					//画面上に存在しない弾を一つ選んで敵の位置にセット
					for (int i = 0; i < 255; i++)
					{
						if (eBull[i] == false)
						{
							eBullPos[i] = ePos;
							eBullPos[i].y += 20;
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

				else
				{

				}

				eAttackInterval = 0;
			}

			//プレイヤーの体力が0になったら終了
			if (playerHP <= 0)
			{
				sceneNum = End;
			}

			//ボスの体力が0になったら終了
			if (enemyHP <= 0)
			{
				sceneNum = End;
			}
		}

		debugText.Print("Game", 0, 0, 1.0f);
	}

	//3:リザルト画面
	else if (sceneNum == End)
	{
		//スペースを押すとタイトルに戻る
		if (input->TriggerKey(DIK_SPACE))
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

		debugText.Print("End", 0, 0, 1.0f);
	}

	debugText.Print(spherestr.str(), 50, 180, 1.0f);

	baseObj->Update();
	playerObj->Update();
	enemyObj->Update();
	eArmObj->Update();

	for (int i = 0; i < 255; i++)
	{
		pBulletObj[i]->Update();
		eBulletObj[i]->Update();
	}

	for (int i = 0; i < 30; i++)
	{
		wallObj[i]->Update();
	}

	if (cameraMoveCount[13])
	{
		StartCameraMove();
	}

	camera->Update();

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
		particleMan->Add(60, pos, vel, acc, 10.0f, 0.0f, color, { 0,0,0,0 });
	}

	particleMan->Update();
}

//描画処理
void GameScene::Draw()
{
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();//コマンドリストの取得

#pragma region 背景スプライト描画

	Sprite::PreDraw(dxCommon->GetCommandList());//背景スプライト描画前処理

	if (sceneNum == Title)
	{
		titleBack->Draw();//背景スプライト描画
	}

	else if (sceneNum == Game)
	{
		gameBack->Draw();//背景スプライト描画
	}

	else if (sceneNum == End)
	{
		endBack->Draw();//背景スプライト描画
	}

	Sprite::PostDraw();//スプライト描画後処理
	dxCommon->ClearDepthBuffer();//深度バッファクリア

#pragma endregion 背景スプライト描画

#pragma region 3Dオブジェクト描画

	Object3d::PreDraw(dxCommon->GetCommandList());//3Dオブジェクト描画前処理

	//3Dオブジェクトの描画
	if (sceneNum == Title)
	{

	}

	else if (sceneNum == Game)
	{
		baseObj->Draw();
		playerObj->Draw();
		enemyObj->Draw();

		if (eArm == true)
		{
			eArmObj->Draw();
		}

		for (int i = 0; i < 255; i++)
		{
			if (pBull[i] == true)
			{
				pBulletObj[i]->Draw();
			}

			if (eBull[i] == true)
			{
				eBulletObj[i]->Draw();
			}
		}

		for (int i = 0; i < 30; i++)
		{
			if (isWall[i] == true)
			{
				wallObj[i]->Draw();
			}
		}
	}

	else if (sceneNum == End)
	{

	}

	//object3d1->Draw();
	Object3d::PostDraw();//3Dオブジェクト描画後処理

#pragma endregion 3Dオブジェクト描画

#pragma region パーティクル

	ParticleManager::PreDraw(dxCommon->GetCommandList());
	particleMan->Draw();
	ParticleManager::PostDraw();

#pragma endregion パーティクル

#pragma region 前景スプライト描画

	Sprite::PreDraw(dxCommon->GetCommandList());//前景スプライト描画前処理
	debugText.DrawAll(dxCommon->GetCommandList());//デバッグテキストの描画
	Sprite::PostDraw();//スプライト描画後処理

#pragma endregion 前景スプライト描画
}

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