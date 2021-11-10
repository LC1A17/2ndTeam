#include "GameScene.h"
#include <cassert>
#include <sstream>
#include <iomanip>

using namespace DirectX;

//�R���X�g���N�^
GameScene::GameScene()
{

}

//�f�X�g���N�^
GameScene::~GameScene()
{
	safe_delete(sprite);
	safe_delete(baseObj);
	safe_delete(modelFighter);
	safe_delete(particleMan);
}

//����������
void GameScene::Initialize(DirectXCommon* dxCommon, Input* input, Audio* audio)
{
	//nullptr�`�F�b�N
	assert(dxCommon);
	assert(input);
	assert(audio);

	this->dxCommon = dxCommon;
	this->input = input;
	this->audio = audio;

	//�f�o�b�O�e�L�X�g�p�e�N�X�`���ǂݍ���
	if (!Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png"))
	{
		assert(0);
	}
	
	//�f�o�b�O�e�L�X�g������
	debugText.Initialize(debugTextTexNumber);

	//�e�N�X�`���ǂݍ���
	if (!Sprite::LoadTexture(1, L"Resources/APEX_01.png"))
	{
		assert(0);
	}

	//�w�i�X�v���C�g����
	sprite = Sprite::Create(1, { 0.0f,0.0f });
	sprite->SetSize({ 100.0f,100.0f });
	sprite->SetPosition({ 100.0f,100.0f });

	//3D�I�u�W�F�N�g����

	//�x�[�X
	modelFighter = modelFighter->CreateFromObject("base");
	baseObj = Object3d::Create();
	baseObj->SetModel(modelFighter);
	baseObj->SetScale({ 100,100,100 });

	//�v���C���[
	modelFighter = modelFighter->CreateFromObject("player");
	playerObj = Object3d::Create();
	playerObj->SetModel(modelFighter);
	playerObj->SetPosition({ pPos });
	playerObj->SetScale({ pScale });

	//�G
	modelFighter = modelFighter->CreateFromObject("enemy");
	enemyObj = Object3d::Create();
	enemyObj->SetModel(modelFighter);
	enemyObj->SetPosition({ ePos });
	enemyObj->SetScale({ eScale });

	//�e
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

	//�T�E���h�Đ�
	//audio->PlayWave("Resources/Alarm01.wav");
}

//�X�V����
void GameScene::Update()
{
	//1:�^�C�g�����
	if (sceneNum == Title)
	{
		//�X�y�[�X�������ƊJ�n
		if (input->TriggerKey(DIK_SPACE))
		{
			sceneNum = Game;
		}

		debugText.Print("Title", 0, 0, 1.0f);
	}

	//2:�Q�[�����
	else if (sceneNum == Game)
	{
		//���͏���
		//���̉~�Ɉړ�
		if (input->PushKey(DIK_UP))
		{
			//�����ֈړ�
			//��ԓ����ɂ��Ȃ��Ȃ�ړ�
			if (circle > 1)
			{
				circle--;
			}
		}

		else if (input->PushKey(DIK_DOWN))
		{
			//�O���ֈړ�
			//��ԊO���ɂ��Ȃ��Ȃ�ړ�
			if (circle < maxCircle)
			{
				circle++;
			}

		}

		//�~������ړ�
		if (input->PushKey(DIK_LEFT))
		{
			//���v���Ɉړ�

		}

		else if (input->PushKey(DIK_RIGHT))
		{
			//�����v���Ɉړ�

		}

		//�e�𔭎�
		if (input->PushKey(DIK_SPACE))
		{
			if (pBullInterval >= 30)
			{
				//��ʏ�ɑ��݂��Ȃ��e����I��Ŏ��@�̈ʒu�ɃZ�b�g
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

		//�X�V����
		pBullInterval++;
		eDamageInterval++;

		//�v���C���[�̒e
		for (int i = 0; i < 255; i++)
		{
			//�e�̋���
			if (pBull[i] == true)
			{
				pBullPos[i].x += 2;
				bulletObj[i]->SetPosition({ pBullPos[i] });
			}

			//�ǂƂ̔���
			if ( 1 )
			{
				pBullPos[i] = { 1000, 1000, 1000 };
				bulletObj[i]->SetPosition({ pBullPos[i] });
				pBull[i] = false;
			}

			//�G�Ƃ̔���
			if (eDamageInterval >= 50)
			{
				enemyHP--;
				pBullPos[i] = { 1000, 1000, 1000 };
				bulletObj[i]->SetPosition({ pBullPos[i] });
				pBull[i] = false;
				eDamageInterval = 0;
			}

			//��ʊO�ɏo���e��false�ɂ���
			if (pBullPos[i].x <= -200 || pBullPos[i].x >= 200 || pBullPos[i].z <= -200 || pBullPos[i].z >= 200)
			{
				pBullPos[i] = { 1000, 1000, 1000 };
				bulletObj[i]->SetPosition({ pBullPos[i] });
				pBull[i] = false;
			}
		}

		//�v���C���[�̗̑͂�0�ɂȂ�����I��
		if (playerHP <= 0)
		{
			sceneNum = End;
		}

		debugText.Print("Game", 0, 0, 1.0f);
	}

	//3:���U���g���
	else if (sceneNum == End)
	{
		//�X�y�[�X�������ƃ^�C�g���ɖ߂�
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
	*/

	//particleMan->Update();
}

//�`�揈��
void GameScene::Draw()
{
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();//�R�}���h���X�g�̎擾

#pragma region �w�i�X�v���C�g�`��

	Sprite::PreDraw(dxCommon->GetCommandList());//�w�i�X�v���C�g�`��O����
	//sprite->Draw();//�w�i�X�v���C�g�`��
	Sprite::PostDraw();//�X�v���C�g�`��㏈��
	dxCommon->ClearDepthBuffer();//�[�x�o�b�t�@�N���A

#pragma endregion �w�i�X�v���C�g�`��

#pragma region 3D�I�u�W�F�N�g�`��

	Object3d::PreDraw(dxCommon->GetCommandList());//3D�I�u�W�F�N�g�`��O����
	
	//3D�I�u�W�F�N�g�̕`��
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
	Object3d::PostDraw();//3D�I�u�W�F�N�g�`��㏈��

#pragma endregion 3D�I�u�W�F�N�g�`��

#pragma region �p�[�e�B�N��

	ParticleManager::PreDraw(dxCommon->GetCommandList());
	//particleMan->Draw();
	ParticleManager::PostDraw();

#pragma endregion �p�[�e�B�N��

#pragma region �O�i�X�v���C�g�`��

	Sprite::PreDraw(dxCommon->GetCommandList());//�O�i�X�v���C�g�`��O����
	debugText.DrawAll(dxCommon->GetCommandList());//�f�o�b�O�e�L�X�g�̕`��
	Sprite::PostDraw();//�X�v���C�g�`��㏈��

#pragma endregion �O�i�X�v���C�g�`��
}