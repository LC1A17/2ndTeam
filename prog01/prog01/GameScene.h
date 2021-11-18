#pragma once

#include "SafeDelete.h"
#include "DirectXCommon.h"
#include <DirectXMath.h>
#include "Input.h"
#include "Sprite.h"
#include "Object3d.h"
#include "DebugText.h"
#include "Audio.h"
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
		Title, Game, End
	};

public://�����o�֐�
	GameScene();//�R���X�g�N���^
	~GameScene();//�f�X�g���N�^
	void Initialize(DirectXCommon* dxCommon, Input* input, Audio* audio);//������
	void Update();//���t���[������
	void Draw();//�`��
	void StartCameraMove(); //�ŏ��̃J�����̓���

private: // �����o�ϐ�
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	Audio* audio = nullptr;
	DebugText debugText;

	// �Q�[���V�[���p
	Sprite* sprite = nullptr;
	Sprite* titleBack = nullptr;//�^�C�g��
	Sprite* gameBack = nullptr;//�Q�[��
	Sprite* endBack = nullptr;//�G���h
	Model* modelFighter = nullptr;

	Object3d* baseObj = nullptr;//�t�B�[���h
	Object3d* playerObj = nullptr;//�v���C���[
	Object3d* enemyObj = nullptr;//�G�l�~�[
	Object3d* eArmObj = nullptr;//�G�l�~�[�̘r
	Object3d* pBulletObj[255] = { nullptr };//�e
	Object3d* eBulletObj[255] = { nullptr };//�e
	Object3d* wallObj[30] = { nullptr };//�e

	ParticleManager* particleMan = nullptr;

	Model* modelFighter1 = nullptr;

	int sceneNum = Game;//Title, Game, End�ŊǗ�
	int circle = 2;//�v���C���[�̂���~���̈ʒu�B1���Œ�l�Ő����傫�������O��
	int maxCircle = 3;//���݂̉~���̍ő吔

	XMFLOAT3 basePos = { 0,0,5 };//�y��̍��W
	XMFLOAT3 baseScale = { 200, 200, 200 };//�y��̃X�P�[��
	XMFLOAT3 wallPos[30];//�ǂ̍��W
	XMFLOAT3 wallRota[30];//�ǂ̌���
	XMFLOAT3 wallScale[30];//�ǂ̃X�P�[��
	bool isWall[30] = { false };//�ǂ��o�Ă��邩

	int playerHP = 100;//�v���C���[�̗̑�
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
	float speed = 2.0f;
	float accel = 1.5f;//�����̔{��
	int pDamageInterval = 50;//�G�̔�e���̖��G����

	int enemyHP = 10;//�G�̗̑�
	int eDamageInterval = 50;//�G�̔�e���̖��G����
	XMFLOAT3 ePos = { 0, 0, 0 };//�G�̍��W
	XMFLOAT3 eScale = { 10, 10, 10 };//�G�̑傫��
	int eAttackInterval = 0;//�G�̍s���̊Ԋu
	int wallCount = 0;
	XMFLOAT3 eBullPos[255];//�G�̒e�̍��W
	XMFLOAT3 eBullScale[255];//�G�̒e�̑傫��
	bool eBull[255] = { false };//�G�̒e����ʏ�ɏo�Ă��邩�ǂ���
	float eBullSpeedX[255], eBullSpeedY[255];//�G�̒e�̋����p
	float eBullX[255], eBullY[255], eBullXY[255];//�G�̒e�̋����p

	XMFLOAT3 eArmPos = { 0, 0, 0 };//�r�̍��W
	XMFLOAT3 eArmRot = { 0, 0, 0 };//�r�̌X��
	XMFLOAT3 eArmScale = { 60, 60, 60 };//�r�̑傫��
	bool eArm = false;//�r�̕\��
	int eArmCount = 0;//�r�̓���p

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
};