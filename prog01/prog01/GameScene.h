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

private: // �ÓI�����o�ϐ�
	static const int debugTextTexNumber = 0;

public:
	//SceneNum�p
	enum Scene
	{
		Title, Game, End
	};

public: // �����o�֐�
	// �R���X�g�N���^
	GameScene();
	// �f�X�g���N�^
	~GameScene();
	// ������
	void Initialize(DirectXCommon* dxCommon, Input* input, Audio* audio);
	// ���t���[������
	void Update();
	// �`��
	void Draw();

private: // �����o�ϐ�
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	Audio* audio = nullptr;
	DebugText debugText;

	// �Q�[���V�[���p
	Sprite* sprite = nullptr;
	Model* modelFighter = nullptr;
	
	Object3d* baseObj = nullptr;//�t�B�[���h
	Object3d* playerObj = nullptr;//�v���C���[
	Object3d* enemyObj = nullptr;//�G�l�~�[
	Object3d* bulletObj[255] = { nullptr };//�e

	ParticleManager* particleMan = nullptr;

	Model* modelFighter1 = nullptr;

	int sceneNum = Game;//Title, Game, End�ŊǗ�
	int circle = 2;//�v���C���[�̂���~���̈ʒu�B1���Œ�l�Ő����傫�������O��
	int maxCircle = 3;//���݂̉~���̍ő吔

	int playerHP = 100;//�v���C���[�̗̑�
	XMFLOAT3 pPos = { 100, 1, 0 };//�v���C���[�̍��W
	XMFLOAT3 pScale = { 10, 10, 10 };//�v���C���[�̑傫��
	XMFLOAT3 pBullPos[255];//�v���C���[�̒e�̍��W
	XMFLOAT3 pBullScale[255];//�v���C���[�̒e�̑傫��
	bool pBull[255] = { false };//�v���C���[�̒e����ʏ�ɏo�Ă��邩�ǂ���

	int enemyHP = 100;//�G�̗̑�
	XMFLOAT3 ePos = { 0, 0, 0 };//�G�̍��W
	XMFLOAT3 eScale = { 5, 5, 5 };//�G�̑傫��
};