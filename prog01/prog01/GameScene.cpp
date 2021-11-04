#include "GameScene.h"
#include <cassert>

GameScene::GameScene()
{

}

GameScene::~GameScene()
{
	safe_delete(sprite);
	safe_delete(object3d);
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
	modelFighter = modelFighter->CreateFromObject("cube");
	// 3Dオブジェクト生成
	object3d = Object3d::Create();
	// 3Dオブジェクトにモデルを割り当てる
	object3d->SetModel(modelFighter);


	//.objの名前を指定してモデルを読み込む
	modelFighter1 = modelFighter1->CreateFromObject("ground");
	// 3Dオブジェクト生成
	object3d1 = Object3d::Create();
	// 3Dオブジェクトにモデルを割り当てる
	object3d1->SetModel(modelFighter1);

	particleMan = ParticleManager::Create();

	//サウンド再生
	audio->PlayWave("Resources/Alarm01.wav");
}

void GameScene::Update()
{
	//キーが押されているときの処理
	if (input->TriggerKey(DIK_0))
	{
		OutputDebugStringA("Hit 0\n");
	}

	//X座標、Y座標を指定して表示
	debugText.Print("Hello,DirectX!!", 200, 100, 1.0f);
	//X座標、Y座標、縮尺を指定して表示
	debugText.Print("Nihon Kogakuin", 200, 200, 2.0f);

	// オブジェクト移動
	if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN) || input->PushKey(DIK_RIGHT) || input->PushKey(DIK_LEFT))
	{
		// 現在の座標を取得
		XMFLOAT3 rotation = object3d->GetRotation();

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
		object3d->SetRotation(rotation);
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

	object3d->SetEye({ 0,85,-100 });

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

	object3d1->SetPosition({ 0,-10.0f,0 });

	object3d->Update();
	object3d1->Update();
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
	sprite->Draw();
	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon->ClearDepthBuffer();
#pragma endregion 背景スプライト描画
#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Object3d::PreDraw(dxCommon->GetCommandList());
	// 3Dオブクジェクトの描画
	object3d->Draw();
	object3d1->Draw();
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