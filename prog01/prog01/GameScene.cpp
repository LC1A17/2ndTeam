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
	if (!Sprite::LoadTexture(1, L"Resources/APEX_01.png"))
	{
		assert(0);
	}

	//背景スプライト生成
	sprite = Sprite::Create(1, { 0.0f,0.0f });
	sprite->SetSize({ 100.0f,100.0f });
	sprite->SetPosition({ 100.0f,100.0f });

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
	for (int i = 0; i < 255; i++)
	{
		pOldPos[i] = { 1000, 1000, 1000 };
		pBullPos[i] = { 1000, 1000, 1000 };
		pBullScale[i] = { 10, 10, 10 };

		modelFighter = modelFighter->CreateFromObject("bullet");
		bulletObj[i] = Object3d::Create();
		bulletObj[i]->SetModel(modelFighter);
		bulletObj[i]->SetPosition({ pBullPos[i] });
		bulletObj[i]->SetScale({ pBullScale[i] });
	}

	particleMan = ParticleManager::Create();

	//サウンド再生
	//audio->PlayWave("Resources/Alarm01.wav");
}

//更新処理
void GameScene::Update()
{
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
		//入力処理
		//他の円に移動
		if (input->PushKey(DIK_UP))
		{
			//内側へ移動
			//一番内側にいないなら移動
			if (circle > 1)
			{
				circle--;
			}
		}

		else if (input->PushKey(DIK_DOWN))
		{
			//外側へ移動
			//一番外側にいないなら移動
			if (circle < maxCircle)
			{
				circle++;
			}

		}

		//円周上を移動
		if (input->PushKey(DIK_LEFT))
		{
			//時計回りに移動

		}

		else if (input->PushKey(DIK_RIGHT))
		{
			//反時計回りに移動

		}

		//弾を発射
		if (input->PushKey(DIK_SPACE))
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

		//プレイヤーの弾
		for (int i = 0; i < 255; i++)
		{
			//弾の挙動
			if (pBull[i] == true)
			{
				pBullPos[i].x += 2;
				bulletObj[i]->SetPosition({ pBullPos[i] });
			}

			//壁との判定
			if ( 1 )
			{
				pBullPos[i] = { 1000, 1000, 1000 };
				bulletObj[i]->SetPosition({ pBullPos[i] });
				pBull[i] = false;
			}

			//敵との判定
			if (eDamageInterval >= 50)
			{
				enemyHP--;
				pBullPos[i] = { 1000, 1000, 1000 };
				bulletObj[i]->SetPosition({ pBullPos[i] });
				pBull[i] = false;
				eDamageInterval = 0;
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

		debugText.Print("Game", 0, 0, 1.0f);
	}

	//3:リザルト画面
	else if (sceneNum == End)
	{
		//スペースを押すとタイトルに戻る
		if (input->TriggerKey(DIK_SPACE))
		{
			sceneNum = Title;
		}

		debugText.Print("End", 0, 0, 1.0f);
	}

	baseObj->SetEye({ 0,180,1 });
	playerObj->SetEye({ 0,180,1 });
	enemyObj->SetEye({ 0,180,1 });

	for (int i = 0; i < 255; i++)
	{
		bulletObj[i]->SetEye({ 0,180,1 });
	}

	baseObj->Update();
	playerObj->Update();
	enemyObj->Update();

	for (int i = 0; i < 255; i++)
	{
		bulletObj[i]->Update();
	}

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
	//sprite->Draw();//背景スプライト描画
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
	}

	else if (sceneNum -= End)
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