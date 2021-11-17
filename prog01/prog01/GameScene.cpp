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
	baseObj->SetScale({ 100,100,100 });

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
	modelFighter = modelFighter->CreateFromObject("bullet");

	for (int i = 0; i < 255; i++)
	{
		pOldPos[i] = { 1000, 1000, 1000 };
		pBullPos[i] = { 1000, 1000, 1000 };
		pBullScale[i] = { 10, 10, 10 };
		bulletObj[i] = Object3d::Create();
		bulletObj[i]->SetModel(modelFighter);
		bulletObj[i]->SetPosition({ pBullPos[i] });
		bulletObj[i]->SetScale({ pBullScale[i] });
	}

	//壁
	modelFighter = modelFighter->CreateFromObject("wall");

	for (int i = 0; i < 30; i++)
	{
		wallPos[i] = { 1000, 1000, 1000 };
		wallRota[i] = { 0, 0, 0 };
		wallScale[i] = { 10, 10, 10 };
		wallObj[i] = Object3d::Create();
		wallObj[i]->SetModel(modelFighter);
		wallObj[i]->SetPosition({ wallPos[i] });
		wallObj[i]->SetRotation({ wallRota[i] });
		wallObj[i]->SetScale({ wallScale[i] });
	}

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

		aroundX = cos(rad) * len;
		aroundZ = sin(rad) * len;

		pPos.x = posX + aroundX;
		pPos.z = posZ + aroundZ;

		if (!cameraMoveCount[13])
		{
			playerObj->SetPosition({ pPos });

			fixedCamera.x = cos(rad) * len * 2.1f;
			fixedCamera.y = fixed.y;
			fixedCamera.z = sin(rad) * len * 2.1f;

			camera->SetEye(fixedCamera);
		}

		//入力処理
		//デバッグ用。Rキーでエンド
		if (input->TriggerKey(DIK_R) && !cameraMoveCount[13])
		{
			sceneNum = End;
		}

		//軸を移動
		if (input->TriggerKey(DIK_UP) && !cameraMoveCount[13])
		{
			//内側へ移動
			//一番内側にいないなら移動
			if (circle > 1)
			{
				circle--;
			}
		}

		else if (input->TriggerKey(DIK_DOWN) && !cameraMoveCount[13])
		{
			//外側へ移動
			//一番外側にいないなら移動
			if (circle < maxCircle)
			{
				circle++;
			}
		}

		if (circle == 1)
		{
			len = 30.0f;
			speed = 2.5f;
		}

		if (circle == 2)
		{
			len = 60.0f;
			speed = 2.0f;
		}

		if (circle == 3)
		{
			len = 90.0f;
			speed = 1.5f;
		}

		//円周上を移動
		if (input->PushKey(DIK_LEFT) && !cameraMoveCount[13])
		{
			//反時計回りに移動
			//LSHIFTを押している時は加速
			if (input->PushKey(DIK_LSHIFT))
			{
				angle -= speed * accel;
			}

			else
			{
				angle -= speed;
			}
		}

		if (input->PushKey(DIK_RIGHT) && !cameraMoveCount[13])
		{
			//時計回りに移動
			//LSHIFTを押している時は加速
			if (input->PushKey(DIK_LSHIFT))
			{
				angle += speed * accel;
			}

			else
			{
				angle += speed;
			}
		}

		//弾を発射
		if (input->PushKey(DIK_SPACE) && !cameraMoveCount[13])
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
						bulletObj[i]->SetPosition({ pBullPos[i] });
						pBullX[i] = ePos.x - pOldPos[i].x;
						pBullY[i] = ePos.z - pOldPos[i].z;
						pBullXY[i] = sqrt(pBullX[i] * pBullX[i] + pBullY[i] * pBullY[i]);
						pBullSpeedX[i] = pBullX[i] / pBullXY[i] * 2;
						pBullSpeedY[i] = pBullY[i] / pBullXY[i] * 2;
						pBull[i] = true;
						pBullInterval = 0;
						break;
					}
				}
			}
		}

		//更新処理
		pBullInterval++;
		eDamageInterval++;
		eAttackInterval++;

		//壁の数
		for (int i = 0; i < 30; i++)
		{
			if (isWall[i] == true)
			{
				wallCount++;
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
				bulletObj[i]->SetPosition({ pBullPos[i] });
			}

			/*//壁との判定
			if ( 1 )
			{
				pBullPos[i] = { 1000, 1000, 1000 };
				bulletObj[i]->SetPosition({ pBullPos[i] });
				pBull[i] = false;
			}
			*/

			//敵との判定
			if (eDamageInterval >= 50)
			{
				float a = pBullPos[i].x - ePos.x;
				float b = pBullPos[i].z - ePos.z;
				float c = sqrt(a * a + b * b);

				if (c <= 10)
				{
					enemyHP--;
					pBullPos[i] = { 1000, 1000, 1000 };
					bulletObj[i]->SetPosition({ pBullPos[i] });
					pBull[i] = false;
					eDamageInterval = 0;
				}
			}

			//ボスの挙動
			if (eAttackInterval >= 50)
			{
				//プレイヤーの位置を参照
				if (circle == 1)
				{

				}

				else if (circle == 2)
				{
					if (wallCount <= 10)
					{
						for (int i = 0; i < 30; i++)
						{
							if (isWall[i] == false)
							{
								wallPos[i] = { 30, 0, 0 };
								isWall[i] = true;
								wallObj[i]->SetPosition({ wallPos[i] });
								break;
							}
						}
					}
				}

				else if (circle == 3)
				{

				}

				else
				{

				}

				eAttackInterval = 0;
			}

			//画面外に出た弾をfalseにする
			if (pBullPos[i].x <= -200 || pBullPos[i].x >= 200 || pBullPos[i].z <= -200 || pBullPos[i].z >= 200)
			{
				pBullPos[i] = { 1000, 1000, 1000 };
				bulletObj[i]->SetPosition({ pBullPos[i] });
				pBull[i] = false;
			}
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

			for (int i = 0; i < 255; i++)
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
			pPos = { 0, 0, 95 };//プレイヤーの座標
			pRot = { 0, 0, 0 };//プレイヤーの傾き
			pBullInterval = 30;
			speed = 2.0f;
			enemyHP = 10;//敵の体力
			ePos = { 0, 0, 0 };//敵の座標
			eDamageInterval = 50;//敵の被弾時の無敵時間
			angle = 90.0f;
			len = 60.0f;
			sceneNum = Title;
		}

		debugText.Print("End", 0, 0, 1.0f);
	}

	debugText.Print(spherestr.str(), 50, 180, 1.0f);

	baseObj->Update();
	playerObj->Update();
	enemyObj->Update();

	for (int i = 0; i < 255; i++)
	{
		bulletObj[i]->Update();
		wallObj[i]->Update();
	}

	if (cameraMoveCount[13])
	{
		StartCameraMove();
	}

	camera->Update();

	/*
	for (int i = 0; i < 10; i++)
	{
		//X,Y,Z全て[-5.0,+5.0]でランダムに分布
		const float md_pos = 10.0f;
		XMFLOAT3 pos{};
		pos.x = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;
		pos.y = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;
		pos.z = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;
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
		particleMan->Add(60, pos, vel, acc, 1.0f, 0.0f, color);
	}
	*/

	//particleMan->Update();
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

		for (int i = 0; i < 255; i++)
		{
			if (pBull[i] == true)
			{
				bulletObj[i]->Draw();
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
	//particleMan->Draw();
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