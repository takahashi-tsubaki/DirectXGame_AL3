#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
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

	uint32_t textureHandle_ = 0;
	//スプライト
	Sprite* sprite_ = nullptr;

	//３Dモデル
	Model* model_ = nullptr;

	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

	////値を表示したい変数
	float translationX = 10.0f;
	float translationY = 10.0f;
	float translationZ = 10.0f;

	float rotationX = 0.785398f;
	float rotationY = 0.785398f;
	float rotationZ = 0.0f;

	float scaleX = 5.0f;
	float scaleY = 5.0f;
	float scaleZ = 5.0f;
	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
