#pragma once

#include "SafeDelete.h"
#include "DirectXCommon.h"
#include <DirectXMath.h>
#include "Input.h"
#include "Sprite.h"
#include "Object3d.h"
#include "DebugText.h"
#include "Audio.h"
#include "Sound.h"
#include "Model.h"
#include "ParticleManager.h"
#include "Camera.h"

class GameScene
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

private: // �ÓI�����o�ϐ�
	static const int debugTextTexNumber = 0;

public:
	//SceneNum�p
	enum Scene
	{
		Title, Game, Clear, GameOver
	};

public://�����o�֐�
	GameScene();//�R���X�g�N���^
	~GameScene();//�f�X�g���N�^
	void Initialize(DirectXCommon* dxCommon, Input* input, Sound* sound);//������
	void Update();//���t���[������
	void Draw();//�`��
	void StartCameraMove(); //�ŏ��̃J�����̓���

private: // �����o�ϐ�
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	Sound* sound = nullptr;
	DebugText debugText;

	// �Q�[���V�[���p
	Sprite* sprite = nullptr;
	Sprite* gamestart = nullptr;
	Sprite* titleBack = nullptr;//�^�C�g��
	Sprite* gameBack = nullptr;//�Q�[��
	Sprite* endBack = nullptr;//�G���h
	Sprite* enemyHp = nullptr;
	Sprite* enemyHpFlame = nullptr;
	Sprite* playerHp = nullptr;
	Sprite* playerHpFlame = nullptr;
	Sprite* gameover = nullptr;
	Sprite* gameBack2 = nullptr;
	Sprite* clear = nullptr;
	Sprite* takendamage = nullptr;
	Sprite* num0[6] = { nullptr };
	Sprite* num1[6] = { nullptr };
	Sprite* num2[6] = { nullptr };
	Sprite* num3[6] = { nullptr };
	Sprite* num4[6] = { nullptr };
	Sprite* num5[6] = { nullptr };
	Sprite* num6[6] = { nullptr };
	Sprite* num7[6] = { nullptr };
	Sprite* num8[6] = { nullptr };
	Sprite* num9[6] = { nullptr };
	Model* modelFighter = nullptr;

	Object3d* baseObj = nullptr;//�t�B�[���h
	Object3d* playerObj = nullptr;//�v���C���[
	Object3d* enemyObj = nullptr;//�G�l�~�[
	Object3d* eAtkObj = nullptr;//�G�l�~�[
	Object3d* eArmObj = nullptr;//�G�l�~�[�̘r
	Object3d* pBulletObj[255] = { nullptr };//�e
	Object3d* eBulletObj[255] = { nullptr };//�e
	Object3d* eBulletObj2[255] = { nullptr };//�e
	Object3d* wallObj[24] = { nullptr };//�e

	ParticleManager* particleMan = nullptr;
	ParticleManager* playerParticleMan = nullptr;
	ParticleManager* enemyParticleMan = nullptr;

	Model* modelFighter1 = nullptr;

	int sceneNum = Title;//Title, Game, Clear, GameOver�ŊǗ�
	int circle = 2;//�v���C���[�̂���~���̈ʒu�B1���Œ�l�Ő����傫�������O��
	int maxCircle = 3;//���݂̉~���̍ő吔

	XMFLOAT3 basePos = { 0,0,5 };//�y��̍��W
	XMFLOAT3 baseScale = { 230, 230, 230 };//�y��̃X�P�[��
	XMFLOAT3 wallPos[24];//�ǂ̍��W
	XMFLOAT3 wallRota[24];//�ǂ̌���
	XMFLOAT3 wallScale[24];//�ǂ̃X�P�[��
	bool isWall[24] = { false };//�ǂ��o�Ă��邩
	int wallHP[24] = { 10 };

	float playerHP = 10;//�v���C���[�̗̑�
	float maxPlayerHP = 10;//�v���C���[�̍ő�̗�
	XMFLOAT3 pPos = { 0, 0, 120 };//�v���C���[�̍��W
	XMFLOAT3 pRot = { 0, 0, 0 };//�v���C���[�̌X��
	XMFLOAT3 pScale = { 5, 5, 5 };//�v���C���[�̑傫��
	XMFLOAT3 pOldPos[255];//�v���C���[�̍��W
	XMFLOAT3 pBullPos[255];//�v���C���[�̒e�̍��W
	XMFLOAT3 pBullScale[255];//�v���C���[�̒e�̑傫��
	int pBullInterval = 30;
	bool pBull[255] = { false };//�v���C���[�̒e����ʏ�ɏo�Ă��邩�ǂ���
	float pBullSpeedX[255], pBullSpeedY[255];//�v���C���[�̒e�̋����p
	float pBullX[255], pBullY[255], pBullXY[255];//�v���C���[�̒e�̋����p
	float pBullDamage[255] = { 0 };
	float speed = 2.0f;
	float accel = 1.5f;//�����̔{��
	int pDamageInterval = 50;//�G�̔�e���̖��G����

	float enemyHP = 100;//�G�̗̑�
	float maxEnemyHP = 100;//�G�̍ő�̗�
	int eDamageInterval = 50;//�G�̔�e���̖��G����
	XMFLOAT3 ePos = { 0, 0, 0 };//�G�̍��W
	XMFLOAT3 eRot = { 0, 180, 0 };
	XMFLOAT3 eScale = { 10, 10, 10 };//�G�̑傫��
	int eAttackInterval = 0;//�G�̍s���̊Ԋu
	int wallCount = 0;
	XMFLOAT3 eBullPos[255];//�G�̒e�̍��W
	XMFLOAT3 eBullScale[255];//�G�̒e�̑傫��
	bool eBull[255] = { false };//�G�̒e����ʏ�ɏo�Ă��邩�ǂ���
	float eBullSpeedX[255], eBullSpeedY[255];//�G�̒e�̋����p
	float eBullX[255], eBullY[255], eBullXY[255];//�G�̒e�̋����p

	XMFLOAT3 eBullPos2[255];//�G�̒e�̍��W
	XMFLOAT3 eBullScale2[255];//�G�̒e�̑傫��
	bool eBull2[255] = { false };//�G�̒e����ʏ�ɏo�Ă��邩�ǂ���
	float eBullSpeed2X[255], eBullSpeed2Y[255];//�G�̒e�̋����p
	float eBull2X[255], eBull2Y[255], eBull2XY[255];//�G�̒e�̋����p

	XMFLOAT3 eArmPos[5];//�r�̍��W
	XMFLOAT3 eArmRot[5];//�r�̌X��
	XMFLOAT3 eArmScale[5];//�r�̑傫��

	bool isDive = false;//����
	bool direction = false;//����Ƃ��̐i�s����
	bool diveMove[3] = { false };//��������̈ړ�

	float posX = 0.0f;
	float posZ = 0.0f;
	float rad = 0.0f;
	float angle = 90.0f;
	float len = 60.0f;
	float aroundX = 0.0f;
	float aroundZ = 0.0f;
	float angleX = 0.0f;
	float angleZ = 0.0f;
	float eAngle = 0.0f;

	bool shakeFlag = false;
	int shakeTimer = 0;
	int attenuation = 0;

	XMVECTOR hoge;

	Camera* camera = nullptr;
	XMFLOAT3 fixedCamera;
	XMFLOAT3 fixed = camera->GetEye();
	XMVECTOR cameraMove = { 0, 80, 140 };
	bool cameraMoveCount[14];
	float cameraRad = 0.0f;
	float cameraAngle = 90.0f;

	float i = 1.0f;
	bool hit = false;

	int logoCount = 0;//�^�C�g���_�ŃJ�E���g
	bool isLoad = false;//���[�h
	int loadCount = 0;//���[�h����
	int endCount = 0;//�Q�[���I�[�o�[��̓��͑҂�����
	int enemyMove[2] = { 0, 0 };//�s������p
	int moveCount = 0;//�s��������
	bool isLaser = true;//���[�U�[�g�p�\��
	bool isBarrier = true;//�_���[�W���[���g�p�\��
	bool isAttack = true;//�U�����I�������
	bool laserAttack = false;//���[�U�[��������
	int laserCount = 0;//���[�U�[�̎���
	bool wallCreate = false;//�ǐ�������
	bool isArm = false;//�r�U������
	bool eArm[5] = { false };//�r�̕\��
	int eArmCount = 0;//�r�̓���p
	int iceCount = 0;
	float PI = 3.1415926;
	int bulletCount = 0;

	int clearTime = 0;

	float bulletAngle[16] = {};

	bool isSound[4] = { false };
	int SoundCount[4] = { 0 };

	bool isEnemyAtk = true;
};