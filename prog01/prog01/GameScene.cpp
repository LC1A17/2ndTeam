#include "GameScene.h"
#include <cassert>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <ctime>

using namespace std;
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

	srand(time(NULL));//����������

	//�e�N�X�`���ǂݍ���
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

	//�w�i�X�v���C�g����
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

	//3D�I�u�W�F�N�g����

	//�x�[�X
	modelFighter = modelFighter->CreateFromObject("base");
	baseObj = Object3d::Create();
	baseObj->SetModel(modelFighter);
	baseObj->SetScale({ baseScale });

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
	enemyObj->SetRotation({ eRot });
	enemyObj->SetScale({ eScale });

	//�e
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

	//�r
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

	//��
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

	particleMan = ParticleManager::Create();//�p�[�e�B�N������
	playerParticleMan = ParticleManager::Create();//�p�[�e�B�N������
	enemyParticleMan = ParticleManager::Create();//�p�[�e�B�N������

	//audio->PlayWave("Resources/title.wav");//�T�E���h�Đ�

	//�J����
	for (int i = 0; i < _countof(cameraMoveCount); i++)
	{
		cameraMoveCount[i] = true;
	}

	camera->Update();
}

//�X�V����
void GameScene::Update()
{
	//1:�^�C�g�����
	if (sceneNum == Title)
	{
		//�^�C�g���_�ŃJ�E���g
		logoCount++;

		if (logoCount == 40)
		{
			logoCount = 0;
		}

		//SPACEorSTART�������ƃ��[�h�J�n
		if ((input->TriggerKey(DIK_SPACE) || input->TriggerPadKey(BUTTON_START)) && !isLoad)
		{
			loadCount = 0;
			isLoad = true;
		}

		//���[�h
		if (isLoad)
		{
			loadCount++;
		}

		//���[�h�I��
		if (loadCount >= 20)
		{
			loadCount = 0;
			isLoad = false;

			//�ǂ������_����5�����i���Ȃ��Ȃ�܂Ń��[�v�j
			while (wallCount <= 4)
			{
				int w = rand() % 24;
				isWall[w] = true;

				wallCount = 0;

				//�o�����Ă���ǂ̐����J�E���g
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

	//2:�Q�[�����
	else if (sceneNum == Game)
	{
		//�����͏����������火

		//�J�n���J�������Ɣ�e���Ɛ��s���͓��͂��󂯕t���Ȃ�
		if (!cameraMoveCount[13] && !hit && !isDive)
		{
			//�����֐��s
			if (input->TriggerKey(DIK_UP) || input->TriggerPadKey(BUTTON_B))
			{
				//��ԓ����ɂ��Ȃ��Ȃ���s�J�n
				if (circle > 1)
				{
					direction = false;
					isDive = true;
					diveMove[0] = true;
				}
			}

			//�O���֐��s
			if (input->TriggerKey(DIK_DOWN) || input->TriggerPadKey(BUTTON_A))
			{
				//��ԊO���ɂ��Ȃ��Ȃ���s�J�n
				if (circle < maxCircle)
				{
					direction = true;
					isDive = true;
					diveMove[0] = true;
				}
			}

			//���v���ɒn����ړ�
			if (input->PushKey(DIK_LEFT) || input->PushPadStickLeft())
			{
				//LSHIFT�������Ă��鎞�͉���
				if (input->PushKey(DIK_LSHIFT) || input->TriggerPadKey(BUTTON_LEFT_SHOULDER))
				{
					angle -= speed * accel;
				}

				else
				{
					angle -= speed;
				}
			}

			//�����v���ɒn����ړ�
			if (input->PushKey(DIK_RIGHT) || input->PushPadStickRight())
			{
				//LSHIFT�������Ă��鎞�͉���
				if (input->PushKey(DIK_LSHIFT) || input->TriggerPadKey(BUTTON_LEFT_SHOULDER))
				{
					angle += speed * accel;
				}

				else
				{
					angle += speed;
				}
			}

			//�e�𔭎�
			if (input->PushKey(DIK_SPACE) || input->PushPadKey(BUTTON_RIGHT_SHOULDER))
			{
				if (pBullInterval >= 30)
				{
					//��ʏ�ɑ��݂��Ȃ��e����I��Ŏ��@�̈ʒu�ɃZ�b�g
					for (int i = 0; i < 255; i++)
					{
						//�{�X�̈ʒu��ߑ�
						if (!pBull[i])
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
		}

		//�����͏��������܂Ł�

		//���X�V�����������火
		pBullInterval++;//�v���C���[�̒e�̔��ˊԊu
		pDamageInterval++;//�v���C���[�̔�e��̖��G����
		len = 60.0f * circle;//���݂̉~���̈ʒu
		playerHp->SetSize({ (326 / maxPlayerHP) * playerHP, 28 });//�v���C���[��HP�o�[
		enemyHp->SetSize({ (872 / maxEnemyHP) * enemyHP, 28 });//�{�X��HP�o�[

		//�o�����Ă���ǂ̐����J�E���g
		wallCount = 0;

		for (int i = 0; i < 24; i++)
		{
			if (isWall[i])
			{
				wallCount++;
			}
		}

		//�v���C���[�̐��s
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

		//�v���C���[�̒e
		for (int i = 0; i < 255; i++)
		{
			//�e�̋���
			if (pBull[i])
			{
				pBullPos[i].x += pBullSpeedX[i];
				pBullPos[i].z += pBullSpeedY[i];
				pBulletObj[i]->SetPosition({ pBullPos[i] });
			}

			//�ǂƂ̔���
			for (int j = 0; j < 24; j++)
			{
				if (isWall[j])
				{
					if (wallPos[j].x - 16 < pBullPos[i].x + 5 && pBullPos[i].x - 5 < wallPos[j].x + 16 && wallPos[j].z - 10.5 < pBullPos[i].z + 5 && pBullPos[i].z - 5 < wallPos[j].z + 10.5)
					{
						pBullPos[i] = { 1000, 1000, 1000 };
						pBulletObj[i]->SetPosition({ pBullPos[i] });
						pBull[i] = false;
					}
				}
			}

			//�{�X�Ƃ̔���
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

			//�͈͊O�ɏo���e��false�ɂ���
			if (pBullPos[i].x <= -1000 || pBullPos[i].x >= 1000 || pBullPos[i].z <= -1000 || pBullPos[i].z >= 1000)
			{
				pBullPos[i] = { 1000, 1000, 1000 };
				pBulletObj[i]->SetPosition({ pBullPos[i] });
				pBull[i] = false;
			}
		}

		//�v���C���[�̃m�b�N�o�b�N
		if (hit)
		{
			i -= 0.01f;

			if (i <= 0.8f && hit)
			{
				hit = false;
			}
		}

		//�v���C���[�ƕǂ̏Փ˔���
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
					}
				}
			}
		}

		else if (!hit && i <= 1.0f)
		{
			i += 0.05f;
		}

		//�{�X�̒e
		for (int i = 0; i < 255; i++)
		{
			//�e�̋���
			if (eBull[i])
			{
				eBullPos[i].x += eBullSpeedX[i];
				eBullPos[i].z += eBullSpeedY[i];
				eBulletObj[i]->SetPosition({ eBullPos[i] });
			}

			//�ǂƂ̔���
			for (int j = 0; j < 24; j++)
			{
				if (isWall[j])
				{
					if (wallPos[j].x - 16 < eBullPos[i].x + 5 && eBullPos[i].x - 5 < wallPos[j].x + 16 && wallPos[j].z - 10.5 < eBullPos[i].z + 5 && eBullPos[i].z - 5 < wallPos[j].z + 10.5)
					{
						eBullPos[i] = { 1000, 1000, 1000 };
						eBulletObj[i]->SetPosition({ eBullPos[i] });
						eBull[i] = false;
					}
				}
			}

			//�v���C���[�Ƃ̔���
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

			//�͈͊O�ɏo���e��false�ɂ���
			if (eBullPos[i].x <= -1000 || eBullPos[i].x >= 1000 || eBullPos[i].z <= -1000 || eBullPos[i].z >= 1000)
			{
				eBullPos[i] = { 1000, 1000, 1000 };
				eBulletObj[i]->SetPosition({ eBullPos[i] });
				eBull[i] = false;
			}
		}

		//�����̕ӂ悭�킩��Ȃ�����R�����g�A�E�g�����ā�
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
		//�����̕ӂ悭�킩��Ȃ�����R�����g�A�E�g�����ā�

		//�J�n���J�������I���܂ōX�V���Ȃ�
		if (!cameraMoveCount[13])
		{
			if (isAttack)
			{
				enemyObj->SetBillboard(false);
				eAttackInterval++;//�{�X�̍U���Ԋu
			}

			//�{�X�̍s���p�^�[���i�����O�j
			if (enemyHP > maxEnemyHP / 2)
			{
				//�U���Ԋu�`�F�b�N
				if (eAttackInterval >= 60)
				{
					eAttackInterval = 0;//�U���Ԋu���Z�b�g
					moveCount++;

					//��Z�����܂��Ă��Ȃ��Ƃ�
					if (moveCount <= 7)
					{
						enemyMove[0] = rand() % 80 + 1;//�s���p�^�[��������

						//1:���@�_���̒e�𔭎�
						if (enemyMove[0] <= 30)
						{
							enemyMove[1] = rand() % 100 + 1;//�s���p�^�[��������

							//0:�u�C�B��
							if (enemyMove[1] <= 20)
							{
								wallCreate = true;
							}

							isLaser = true;
							enemyObj->SetBillboard(true);
							enemyObj->SetRotation({ 0, 0, 0 });

							//��ʏ�ɑ��݂��Ȃ��e����I��Ń{�X�̈ʒu�ɃZ�b�g
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

						//2:16�����ɑ傫�߂̒e��2������
						else if (31 <= enemyMove[0] && enemyMove[0] <= 55)
						{
							enemyMove[1] = rand() % 100 + 1;//�s���p�^�[��������

							//0:�u�C�B��
							if (enemyMove[1] <= 20)
							{
								wallCreate = true;
							}

							isLaser = true;
						}

						//3:�i���j���v����1�����郌�[�U�[��̒e��
						else if (56 <= enemyMove[0] && enemyMove[0] <= 80)
						{
							enemyMove[1] = rand() % 100 + 1;//�s���p�^�[��������

							//0:�u�C�B��
							if (enemyMove[1] <= 20)
							{
								wallCreate = true;
							}

							//���[�U�[�g�p�\�Ȃ烌�[�U�[
							if (isLaser)
							{
								laserAttack = true;
								isLaser = false;
								isAttack = false;
							}

							//�g���Ȃ��Ȃ玩�@�_���e
							else
							{
								enemyObj->SetBillboard(true);
								enemyObj->SetRotation({ 0, 0, 0 });
								isLaser = true;

								//��ʏ�ɑ��݂��Ȃ��e����I��Ń{�X�̈ʒu�ɃZ�b�g
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

						//4:�_���[�W���[����W�J
						else if (81 <= enemyMove[0] && enemyMove[0] <= 100)
						{
							enemyMove[1] = rand() % 100 + 1;//�s���p�^�[��������

							//0:�u�C�B��
							if (enemyMove[1] <= 20)
							{
								wallCreate = true;
							}

							isLaser = true;

							//�_���[�W���[���g�p�\�Ȃ�W�J
							if (isLaser)
							{

							}

							//�g���Ȃ��Ȃ�16�����e
							else
							{
								
							}
						}
					}

					//5:����Șr�ŉ����ׂ�
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

			//�{�X�̍s���p�^�[���i������j
			else
			{
				//�U���Ԋu�`�F�b�N
				if (eAttackInterval >= 40)
				{
					//��Z�`�F�b�N
					if (moveCount <= 3)
					{

					}
				}
			}

			//�u�C�B��
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
					wallPos[w].y = -32;
					isWall[w] = true;
					wallObj[w]->SetPosition({ wallPos[w] });
				}

				wallCreate = false;
			}

			//�ǂ𐶂₷
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

			//���[�U�[�U��
			if (laserAttack)
			{
				laserAttack = false;
				isAttack = true;
			}

			//�r�U��
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
			//�r
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

			//�{�X�̋���
			if (eAttackInterval >= 50)
			{
				//�v���C���[�̈ʒu���Q��
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

		//�{�X�̗̑͂�0�ɂȂ�����N���A
		if (enemyHP <= 0)
		{
			sceneNum = Clear;
		}

		//�v���C���[�̗̑͂�0�ɂȂ�����Q�[���I�[�o�[
		if (playerHP <= 0 && enemyHP > 0)
		{
			sceneNum = GameOver;
		}

		//���X�V���������܂Ł�
	}

	//3:���U���g���
	else if (sceneNum == Clear)
	{
		endCount++;//���͑҂��J�E���g

		//���t���[���o��������͉\
		if (endCount >= 120)
		{
			//�^�C�g���_�ŃJ�E���g
			logoCount++;

			if (logoCount == 40)
			{
				logoCount = 0;
			}

			//SPACEorSTART�������ƃ��[�h�J�n
			if ((input->TriggerKey(DIK_SPACE) || input->TriggerPadKey(BUTTON_START)) && !isLoad)
			{
				loadCount = 0;
				isLoad = true;
			}
		}

		//���[�h
		if (isLoad)
		{
			loadCount++;
		}

		//�e�평�������s���^�C�g����
		if (loadCount >= 20)
		{
			endCount = 0;
			loadCount = 0;
			isLoad = false;
			sceneNum = Title;
		}

		/*
		//�X�y�[�X�������ƃ^�C�g���ɖ߂�
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
			circle = 2;//�v���C���[�̂���~���̈ʒu�B1���Œ�l�Ő����傫�������O��
			maxCircle = 3;//���݂̉~���̍ő吔
			playerHP = 100;//�v���C���[�̗̑�
			pPos = { 0, 0, 120 };//�v���C���[�̍��W
			pRot = { 0, 0, 0 };//�v���C���[�̌X��
			pBullInterval = 30;
			speed = 2.0f;
			enemyHP = 10;//�G�̗̑�
			ePos = { 0, 0, 0 };//�G�̍��W
			eDamageInterval = 50;//�G�̔�e���̖��G����
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

	//4:�Q�[���I�[�o�[���
	else
	{
		endCount++;//���͑҂��J�E���g

		//���t���[���o��������͉\
		if (endCount >= 120)
		{
			//�^�C�g���_�ŃJ�E���g
			logoCount++;

			if (logoCount == 40)
			{
				logoCount = 0;
			}

			//SPACEorSTART�������ƃ��[�h�J�n
			if ((input->TriggerKey(DIK_SPACE) || input->TriggerPadKey(BUTTON_START)) && !isLoad)
			{
				loadCount = 0;
				isLoad = true;
			}
		}

		//���[�h
		if (isLoad)
		{
			loadCount++;
		}

		//�e�평�������s���^�C�g����
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
	enemyObj->Update();
	eArmObj->Update();

	for (int i = 0; i < 255; i++)
	{
		pBulletObj[i]->Update();
		eBulletObj[i]->Update();
	}

	for (int i = 0; i < 24; i++)
	{
		wallObj[i]->Update();
	}

	if (cameraMoveCount[13])
	{
		StartCameraMove();
	}

	camera->Update();

	if (sceneNum == Game)
	{
		for (int i = 0; i < 10; i++)
		{
			//X,Y,Z�S��[-5.0,+5.0]�Ń����_���ɕ��z
			const float md_pos = 10.0f;
			XMFLOAT3 pos{};
			pos.x = ((float)rand() / RAND_MAX * md_pos - md_pos / 2.0f) + pPos.x;
			pos.y = ((float)rand() / RAND_MAX * md_pos - md_pos / 2.0f) + pPos.y;
			pos.z = ((float)rand() / RAND_MAX * md_pos - md_pos / 2.0f) + pPos.z;
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
			playerParticleMan->Add(60, pos, vel, acc, 10.0f, 0.0f, color, { 0,0,0,0 });
		}

		playerParticleMan->Update();
		enemyParticleMan->EaseInUpdate(ePos);

		for (int i = 0; i < 1; i++)
		{
			//X,Y,Z�S��[-5.0,+5.0]�Ń����_���ɕ��z
			const float md_pos = 10.0f;
			//�G�̃p�[�e�B�N���̔����͈�
			XMFLOAT3 ePos{};
			ePos.x = ((float)rand() / RAND_MAX * md_pos - md_pos / 2.0f);
			ePos.y = ((float)rand() / RAND_MAX * md_pos - md_pos / 2.0f);
			ePos.z = ((float)rand() / RAND_MAX * md_pos - md_pos / 2.0f);
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
			enemyParticleMan->Add(60, ePos, vel, acc, 10.0f, 0.0f, color, { 0,0,0,0 });
		}

		playerParticleMan->Update();
		enemyParticleMan->EaseInUpdate(ePos);
	}
}

//�`�揈��
void GameScene::Draw()
{
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();//�R�}���h���X�g�̎擾

#pragma region �w�i�X�v���C�g�`��

	Sprite::PreDraw(dxCommon->GetCommandList());//�w�i�X�v���C�g�`��O����

	//1:�^�C�g�����
	if (sceneNum == Title)
	{
		gameBack->Draw();//�w�i
	}

	//2:�Q�[�����
	else if (sceneNum == Game)
	{
		gameBack->Draw();//�w�i
	}

	//3:���U���g���
	else if (sceneNum == Clear)
	{
		endBack->Draw();//�w�i
	}

	//4:�Q�[���I�[�o�[���
	else
	{
		endBack->Draw();//�w�i
	}

	Sprite::PostDraw();//�X�v���C�g�`��㏈��
	dxCommon->ClearDepthBuffer();//�[�x�o�b�t�@�N���A

#pragma endregion �w�i�X�v���C�g�`��

#pragma region 3D�I�u�W�F�N�g�`��

	Object3d::PreDraw(dxCommon->GetCommandList());//3D�I�u�W�F�N�g�`��O����

	//1:�^�C�g�����
	if (sceneNum == Title)
	{

	}

	//2:�Q�[�����
	else if (sceneNum == Game)
	{
		baseObj->Draw();
		playerObj->Draw();
		enemyObj->Draw();

		for (int i = 0; i < 5; i++)
		{
			if (eArm[i] == true)
			{
				eArmObj->Draw();
			}
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

		for (int i = 0; i < 24; i++)
		{
			if (isWall[i] == true)
			{
				wallObj[i]->Draw();
			}
		}
	}

	//3:���U���g���
	else if (sceneNum == Clear)
	{

	}

	//4:�Q�[���I�[�o�[���
	else
	{

	}

	//object3d1->Draw();
	Object3d::PostDraw();//3D�I�u�W�F�N�g�`��㏈��

#pragma endregion 3D�I�u�W�F�N�g�`��

#pragma region �p�[�e�B�N��

	ParticleManager::PreDraw(dxCommon->GetCommandList());

	//1:�^�C�g�����
	if (sceneNum == Title)
	{

	}

	//2:�Q�[�����
	else if (sceneNum == Game)
	{
		playerParticleMan->Draw();
		enemyParticleMan->Draw();
		//particleMan->Draw();
	}

	//3:���U���g���
	else if (sceneNum == Clear)
	{

	}

	//4:�Q�[���I�[�o�[���
	else
	{

	}

	ParticleManager::PostDraw();

#pragma endregion �p�[�e�B�N��

#pragma region �O�i�X�v���C�g�`��

	Sprite::PreDraw(dxCommon->GetCommandList());//�O�i�X�v���C�g�`��O����
	debugText.DrawAll(dxCommon->GetCommandList());//�f�o�b�O�e�L�X�g�̕`��

	//1:�^�C�g�����
	if (sceneNum == Title)
	{
		titleBack->Draw();//�^�C�g�����S

		//�Q�[���X�^�[�g�i�_�ł���j
		if (logoCount <= 20)
		{
			gamestart->Draw();//�w�i�X�v���C�g�`��
		}
	}

	//2:�Q�[�����
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

	//3:���U���g���
	else if (sceneNum == Clear)
	{
		//�Q�[���X�^�[�g�i�_�ł���j
		if (logoCount <= 20 && endCount >= 120)
		{
			gamestart->Draw();//�w�i�X�v���C�g�`��
		}
	}

	//4:�Q�[���I�[�o�[���
	else
	{
		//�Q�[���X�^�[�g�i�_�ł���j
		if (logoCount <= 20 && endCount >= 120)
		{
			gamestart->Draw();//�w�i�X�v���C�g�`��
		}
	}

	Sprite::PostDraw();//�X�v���C�g�`��㏈��

#pragma endregion �O�i�X�v���C�g�`��
}

//�J�n���J����
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