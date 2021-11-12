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
	Object3d* bulletObj[255] = { nullptr };//�e
	Object3d* wallObj[255] = { nullptr };//�e

	ParticleManager* particleMan = nullptr;

	Model* modelFighter1 = nullptr;

	int sceneNum = Game;//Title, Game, End�ŊǗ�
	int circle = 2;//�v���C���[�̂���~���̈ʒu�B1���Œ�l�Ő����傫�������O��
	int maxCircle = 3;//���݂̉~���̍ő吔

	XMFLOAT3 basePos = { 0,0,5 };//�y��̍��W
	XMFLOAT3 baseScale = { 100,100,100 };//�y��̃X�P�[��
	XMFLOAT3 wallPos[255];//�ǂ̍��W
	XMFLOAT3 wallScale[255];//�ǂ̃X�P�[��
	bool isWall[255] = { false };//�ǂ��o�Ă��邩

	int playerHP = 100;//�v���C���[�̗̑�
	XMFLOAT3 pPos = { 0, 0, 95 };//�v���C���[�̍��W
	XMFLOAT3 pRot = { 0, 0, 0 };//�v���C���[�̌X��
	XMFLOAT3 pScale = { 10, 10, 10 };//�v���C���[�̑傫��
	XMFLOAT3 pOldPos[255];//�v���C���[�̍��W
	XMFLOAT3 pBullPos[255];//�v���C���[�̒e�̍��W
	XMFLOAT3 pBullScale[255];//�v���C���[�̒e�̑傫��
	int pBullInterval = 30;
	bool pBull[255] = { false };//�v���C���[�̒e����ʏ�ɏo�Ă��邩�ǂ���
	float pBullSpeedX[255], pBullSpeedY[255];//�v���C���[�̒e�̋����p
	float pBullX[255], pBullY[255], pBullXY[255];//�v���C���[�̒e�̋����p
	float speed = 2.0f;
	float accel = 1.5f;//�����̔{��

	int enemyHP = 10;//�G�̗̑�
	int eDamageInterval = 50;//�G�̔�e���̖��G����
	XMFLOAT3 ePos = { 0, 0, 0 };//�G�̍��W
	XMFLOAT3 eScale = { 5, 5, 5 };//�G�̑傫��

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
};