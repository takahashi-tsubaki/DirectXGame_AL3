#pragma once

#include "Audio.h"
#include "DebugText.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <DirectXMath.h>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

  public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;

	//モデルの表示
	Model* model_ = nullptr;

	//ワールドトランスフォーム
	WorldTransform worldTransform_;
	
	//ビュープロジェクション
	ViewProjection viewProjection_[3];

	float eyePosX[3] = {0.0f, 0.0f, 0.0f};
	float eyePosY[3] = {0.0f, 0.0f, 0.0f};
	float eyePosZ[3] = {0.0f, 1.0f, 0.0f};

	float targetPosX[3] = {0.0f, 0.0f, 0.0f};
	float targetPosY[3] = {0.0f, 0.0f, 0.0f};
	float targetPosZ[3] = {0.0f, 1.0f, 0.0f};

	float upPosX[3] = {0.0f, 0.0f, 0.0f};
	float upPosY[3] = {0.0f, 0.0f, 0.0f};
	float upPosZ[3] = {0.0f, 1.0f, 0.0f};

	int cameraNum = 0;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};