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
	// nullptr�`�F�b�N
	assert(dxCommon);
	assert(input);
	assert(audio);

	this->dxCommon = dxCommon;
	this->input = input;
	this->audio = audio;

	// �f�o�b�O�e�L�X�g�p�e�N�X�`���ǂݍ���
	if (!Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png")) {
		assert(0);
	}
	// �f�o�b�O�e�L�X�g������
	debugText.Initialize(debugTextTexNumber);

	// �e�N�X�`���ǂݍ���
	if (!Sprite::LoadTexture(1, L"Resources/APEX_01.png")) {
		assert(0);
	}

	// �w�i�X�v���C�g����
	sprite = Sprite::Create(1, { 0.0f,0.0f });
	sprite->SetSize({ 100.0f,100.0f });
	sprite->SetPosition({ 100.0f,100.0f });

	//.obj�̖��O���w�肵�ă��f����ǂݍ���
	modelFighter = modelFighter->CreateFromObject("cube");
	// 3D�I�u�W�F�N�g����
	object3d = Object3d::Create();
	// 3D�I�u�W�F�N�g�Ƀ��f�������蓖�Ă�
	object3d->SetModel(modelFighter);


	//.obj�̖��O���w�肵�ă��f����ǂݍ���
	modelFighter1 = modelFighter1->CreateFromObject("ground");
	// 3D�I�u�W�F�N�g����
	object3d1 = Object3d::Create();
	// 3D�I�u�W�F�N�g�Ƀ��f�������蓖�Ă�
	object3d1->SetModel(modelFighter1);

	particleMan = ParticleManager::Create();

	//�T�E���h�Đ�
	audio->PlayWave("Resources/Alarm01.wav");
}

void GameScene::Update()
{
	//�L�[��������Ă���Ƃ��̏���
	if (input->TriggerKey(DIK_0))
	{
		OutputDebugStringA("Hit 0\n");
	}

	//X���W�AY���W���w�肵�ĕ\��
	debugText.Print("Hello,DirectX!!", 200, 100, 1.0f);
	//X���W�AY���W�A�k�ڂ��w�肵�ĕ\��
	debugText.Print("Nihon Kogakuin", 200, 200, 2.0f);

	// �I�u�W�F�N�g�ړ�
	if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN) || input->PushKey(DIK_RIGHT) || input->PushKey(DIK_LEFT))
	{
		// ���݂̍��W���擾
		XMFLOAT3 rotation = object3d->GetRotation();

		// �ړ���̍��W���v�Z
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

		// ���W�̕ύX�𔽉f
		object3d->SetRotation(rotation);
	}

	// �J�����ړ�
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
		//X,Y,Z�S��[-5.0,+5.0]�Ń����_���ɕ��z
		const float md_pos = 10.0f;
		XMFLOAT3 pos{};
		pos.x = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;
		pos.y = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;
		pos.z = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;
		//X,Y,Z�S��[-0.05,+0.05]�Ń����_���ɕ��z
		const float md_vel = 0.1f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		//�d�͂Ɍ����Ă�Y�̂�[-0.001f,0]�Ń����_���ɕ��z
		XMFLOAT3 acc{};
		const float md_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * md_acc;
		//�F�������_���ɕ��z
		XMFLOAT4 color;
		color.w = 1;
		color.x = (float)rand() / RAND_MAX * 1;
		color.y = (float)rand() / RAND_MAX * 1;
		color.z = (float)rand() / RAND_MAX * 1;

		//�ǉ�
		particleMan->Add(60, pos, vel, acc, 1.0f, 0.0f, color);
	}

	object3d1->SetPosition({ 0,-10.0f,0 });

	object3d->Update();
	object3d1->Update();
	particleMan->Update();
}

void GameScene::Draw()
{
	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();
#pragma region �w�i�X�v���C�g�`��
	// �w�i�X�v���C�g�`��O����
	Sprite::PreDraw(dxCommon->GetCommandList());
	// �w�i�X�v���C�g�`��
	sprite->Draw();
	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();
	// �[�x�o�b�t�@�N���A
	dxCommon->ClearDepthBuffer();
#pragma endregion �w�i�X�v���C�g�`��
#pragma region 3D�I�u�W�F�N�g�`��
	// 3D�I�u�W�F�N�g�`��O����
	Object3d::PreDraw(dxCommon->GetCommandList());
	// 3D�I�u�N�W�F�N�g�̕`��
	object3d->Draw();
	object3d1->Draw();
	// 3D�I�u�W�F�N�g�`��㏈��
	Object3d::PostDraw();
#pragma endregion 3D�I�u�W�F�N�g�`��
#pragma region �p�[�e�B�N��
	ParticleManager::PreDraw(dxCommon->GetCommandList());
	//particleMan->Draw();
	ParticleManager::PostDraw();
#pragma endregion �p�[�e�B�N��
#pragma region �O�i�X�v���C�g�`��
	// �O�i�X�v���C�g�`��O����
	Sprite::PreDraw(dxCommon->GetCommandList());
	// �f�o�b�O�e�L�X�g�̕`��
	debugText.DrawAll(dxCommon->GetCommandList());
	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();
#pragma endregion �O�i�X�v���C�g�`��
}