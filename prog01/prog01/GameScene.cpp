#include "GameScene.h"
#include <cassert>

GameScene::GameScene()
{

}

GameScene::~GameScene()
{
	safe_delete(sprite);
	safe_delete(baseObj);
	safe_delete(modelFighter);
	safe_delete(particleMan);
}

void GameScene::Initialize(DirectXCommon* dxCommon, Input* input, Audio* audio)
{
	// nullptrチェック
	assert(dxCommon);
	assert(input);
	assert(audio);

	this->dxCommon = dxCommon;
	this->input = input;
	this->audio = audio;

	// デバッグテキスト用テクスチャ読み込み
	if (!Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png")) {
		assert(0);
	}
	// デバッグテキスト初期化
	debugText.Initialize(debugTextTexNumber);

	// テクスチャ読み込み
	if (!Sprite::LoadTexture(1, L"Resources/APEX_01.png")) {
		assert(0);
	}

	// 背景スプライト生成
	sprite = Sprite::Create(1, { 0.0f,0.0f });
	sprite->SetSize({ 100.0f,100.0f });
	sprite->SetPosition({ 100.0f,100.0f });

	//.objの名前を指定してモデルを読み込む
	//modelFighter = modelFighter->CreateFromObject("cube");
	modelFighter = modelFighter->CreateFromObject("base");
	// 3Dオブジェクト生成
	baseObj = Object3d::Create();
	// 3Dオブジェクトにモデルを割り当てる
	baseObj->SetModel(modelFighter);
	baseObj->SetScale({ 100,100,100 });

	modelFighter = modelFighter->CreateFromObject("player");
	//3Dオブジェクト生成
	playerObj = Object3d::Create();
	
	//3Dオブジェクトにモデルを割り当てる
	playerObj->SetModel(modelFighter);
	playerObj->SetPosition({ pPos });
	playerObj->SetScale({ pScale });

	modelFighter = modelFighter->CreateFromObject("enemy");
	// 3Dオブジェクト生成
	enemyObj = Object3d::Create();
	// 3Dオブジェクトにモデルを割り当てる
	enemyObj->SetModel(modelFighter);
	enemyObj->SetPosition({ ePos });
	enemyObj->SetScale({ eScale });

	for (int i = 0; i < 255; i++)
	{
		pBullPos[i] = { 1000, 1000, 1000 };
		pBullScale[i] = { 10, 10, 10 };

		modelFighter = modelFighter->CreateFromObject("bullet");
		// 3Dオブジェクト生成
		bulletObj[i] = Object3d::Create();
		// 3Dオブジェクトにモデルを割り当てる
		bulletObj[i]->SetModel(modelFighter);
		bulletObj[i]->SetPosition({ pBullPos[i] });
		bulletObj[i]->SetScale({ pBullScale[i] });
	}

	particleMan = ParticleManager::Create();

	//サウンド再生
	audio->PlayWave("Resources/Alarm01.wav");
}

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
	}

	//2:ゲーム画面
	else if (sceneNum == Game)
	{
		//入力処理
		//軸を移動
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
			//反時計回りに移動

		}

		else if (input->PushKey(DIK_RIGHT))
		{
			//時計回りに移動

		}

		//弾を発射
		if (input->PushKey(DIK_SPACE))
		{
			//画面上に存在しない弾を一つ選んで自機の位置にセット
			for (int i = 0; i < 255; i++)
			{
				if (pBull[i] == false)
				{
					bulletObj[i]->SetPosition({ pPos });
 					pBull[i] = true;
					break;
				}
			}
		}

		//更新処理
		//プレイヤーの体力が0になったら終了
		if (playerHP >= 0)
		{
			sceneNum = End;
		}
	}

	//3:リザルト画面
	else if (sceneNum == End)
	{
		//スペースを押すとタイトルに戻る
		if (input->TriggerKey(DIK_SPACE))
		{
			sceneNum = Title;
		}
	}

	//キーが押されているときの処理
	if (input->TriggerKey(DIK_0))
	{
		OutputDebugStringA("Hit 0\n");
	}

	//X座標、Y座標を指定して表示
	//debugText.Print("Hello,DirectX!!", 200, 100, 1.0f);
	//X座標、Y座標、縮尺を指定して表示
	//debugText.Print("Nihon Kogakuin", 200, 200, 2.0f);

	// オブジェクト移動
	if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN) || input->PushKey(DIK_RIGHT) || input->PushKey(DIK_LEFT))
	{
		// 現在の座標を取得
		XMFLOAT3 rotation = baseObj->GetRotation();

		// 移動後の座標を計算
		if (input->PushKey(DIK_UP))
		{
			rotation.y += 1.0f;
		}
		else if (input->PushKey(DIK_DOWN))
		{
			rotation.y -= 1.0f;
		}

		if (input->PushKey(DIK_RIGHT))
		{
			rotation.x += 1.0f;
		}
		else if (input->PushKey(DIK_LEFT))
		{
			rotation.x -= 1.0f;
		}

		// 座標の変更を反映
		baseObj->SetRotation(rotation);
	}

	// カメラ移動
	if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_D) || input->PushKey(DIK_A))
	{
		if (input->PushKey(DIK_W))
		{
			ParticleManager::CameraMoveEyeVector({ 0.0f,+1.0f,0.0f });
		}
		else if (input->PushKey(DIK_S))
		{
			ParticleManager::CameraMoveEyeVector({ 0.0f,-1.0f,0.0f });
		}

		if (input->PushKey(DIK_D))
		{
			ParticleManager::CameraMoveEyeVector({ +1.0f,0.0f,0.0f });
		}
		else if (input->PushKey(DIK_A))
		{
			ParticleManager::CameraMoveEyeVector({ -1.0f,0.0f,0.0f });
		}
	}

	baseObj->SetEye({ 0,180,1 });

	playerObj->SetEye({ 0,180,1 });
	enemyObj->SetEye({ 0,180,1 });

	for (int i = 0; i < 255; i++)
	{
		bulletObj[i]->SetEye({ 0,180,1 });
	}

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

	baseObj->Update();
	playerObj->Update();
	enemyObj->Update();

	for (int i = 0; i < 255; i++)
	{
		bulletObj[i]->Update();
	}

	particleMan->Update();
}

void GameScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(dxCommon->GetCommandList());
	// 背景スプライト描画
	//sprite->Draw();
	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon->ClearDepthBuffer();
#pragma endregion 背景スプライト描画

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Object3d::PreDraw(dxCommon->GetCommandList());
	// 3Dオブジェクトの描画
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

	//object3d1->Draw();
	// 3Dオブジェクト描画後処理
	Object3d::PostDraw();
#pragma endregion 3Dオブジェクト描画

#pragma region パーティクル
	ParticleManager::PreDraw(dxCommon->GetCommandList());
	//particleMan->Draw();
	ParticleManager::PostDraw();
#pragma endregion パーティクル

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(dxCommon->GetCommandList());
	// デバッグテキストの描画
	debugText.DrawAll(dxCommon->GetCommandList());
	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion 前景スプライト描画
}