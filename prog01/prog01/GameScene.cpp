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

	//�w�i�X�v���C�g����
	titleBack = Sprite::Create(2, { 0.0f,0.0f });
	titleBack->SetSize({ WinApp::window_width, WinApp::window_height });
	titleBack->SetPosition({ 0.0f,0.0f });

	gameBack = Sprite::Create(3, { 0.0f,0.0f });
	gameBack->SetSize({ WinApp::window_width, WinApp::window_height });
	gameBack->SetPosition({ 0.0f,0.0f });

	endBack = Sprite::Create(4, { 0.0f,0.0f });
	endBack->SetSize({ WinApp::window_width, WinApp::window_height });
	endBack->SetPosition({ 0.0f,0.0f });

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

	//��
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

	//�T�E���h�Đ�
	//audio->PlayWave("Resources/Alarm01.wav");

	for (int i = 0; i < _countof(cameraMoveCount); i++)
	{
		cameraMoveCount[i] = true;
	}

	camera->Update();
}

//�X�V����
void GameScene::Update()
{
	hoge.m128_f32[0] = circle;
	std::ostringstream spherestr;
	spherestr << "Sphere:("
		<< std::fixed << std::setprecision(2)
		<< hoge.m128_f32[0] << ")";

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

		//���͏���
		//�f�o�b�O�p�BR�L�[�ŃG���h
		if (input->TriggerKey(DIK_R) && !cameraMoveCount[13])
		{
			sceneNum = End;
		}

		//�����ړ�
		if (input->TriggerKey(DIK_UP) && !cameraMoveCount[13])
		{
			//�����ֈړ�
			//��ԓ����ɂ��Ȃ��Ȃ�ړ�
			if (circle > 1)
			{
				circle--;
			}
		}

		else if (input->TriggerKey(DIK_DOWN) && !cameraMoveCount[13])
		{
			//�O���ֈړ�
			//��ԊO���ɂ��Ȃ��Ȃ�ړ�
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

		//�~������ړ�
		if (input->PushKey(DIK_LEFT) && !cameraMoveCount[13])
		{
			//�����v���Ɉړ�
			//LSHIFT�������Ă��鎞�͉���
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
			//���v���Ɉړ�
			//LSHIFT�������Ă��鎞�͉���
			if (input->PushKey(DIK_LSHIFT))
			{
				angle += speed * accel;
			}

			else
			{
				angle += speed;
			}
		}

		//�e�𔭎�
		if (input->PushKey(DIK_SPACE) && !cameraMoveCount[13])
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

		//�X�V����
		pBullInterval++;
		eDamageInterval++;
		eAttackInterval++;

		//�ǂ̐�
		for (int i = 0; i < 30; i++)
		{
			if (isWall[i] == true)
			{
				wallCount++;
			}
		}

		//�v���C���[�̒e
		for (int i = 0; i < 255; i++)
		{
			//�e�̋���
			if (pBull[i] == true)
			{
				pBullPos[i].x += pBullSpeedX[i];
				pBullPos[i].z += pBullSpeedY[i];
				bulletObj[i]->SetPosition({ pBullPos[i] });
			}

			/*//�ǂƂ̔���
			if ( 1 )
			{
				pBullPos[i] = { 1000, 1000, 1000 };
				bulletObj[i]->SetPosition({ pBullPos[i] });
				pBull[i] = false;
			}
			*/

			//�G�Ƃ̔���
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

			//�{�X�̋���
			if (eAttackInterval >= 50)
			{
				//�v���C���[�̈ʒu���Q��
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

		//�{�X�̗̑͂�0�ɂȂ�����I��
		if (enemyHP <= 0)
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
			circle = 2;//�v���C���[�̂���~���̈ʒu�B1���Œ�l�Ő����傫�������O��
			maxCircle = 3;//���݂̉~���̍ő吔
			playerHP = 100;//�v���C���[�̗̑�
			pPos = { 0, 0, 95 };//�v���C���[�̍��W
			pRot = { 0, 0, 0 };//�v���C���[�̌X��
			pBullInterval = 30;
			speed = 2.0f;
			enemyHP = 10;//�G�̗̑�
			ePos = { 0, 0, 0 };//�G�̍��W
			eDamageInterval = 50;//�G�̔�e���̖��G����
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

	if (sceneNum == Title)
	{
		titleBack->Draw();//�w�i�X�v���C�g�`��
	}

	else if (sceneNum == Game)
	{
		gameBack->Draw();//�w�i�X�v���C�g�`��
	}

	else if (sceneNum == End)
	{
		endBack->Draw();//�w�i�X�v���C�g�`��
	}

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