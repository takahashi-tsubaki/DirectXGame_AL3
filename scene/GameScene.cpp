#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <random>

using namespace DirectX;

GameScene::GameScene() {}

GameScene::~GameScene() { delete model_; }

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario(2).jpg");
	//３Dモデルの生成
	model_ = Model::Create();

	for (size_t i = 0; i< _countof(worldTransform_); i++) 
	{
		for (size_t j = 0; j < _countof(worldTransform_); j++) 
		{
			for (size_t k = 0; k < _countof(worldTransform_); k++) 
			{
				////X,Y,Z方向のスケーリングを設定
				// worldTransform_[i].scale_ = {5.0f, 5.0f, 5.0f};

				//////X,Y,Z軸回りの回転角を設定
				////worldTransform_[i].rotation_ = {};

				// X,Y,Z軸回りの平行移動を設定
				worldTransform_[k][j][i].translation_ = {-12.0f + (3.0f * i),12.0f -(3.0f * j), (4.0f * k)};

				//ワールドトランスフォームの初期化
				worldTransform_[k][j][i].Initialize();
			}
			
		}
		
	}
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

}

void GameScene::Update() {}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dモデル描画

	for (size_t i = 0; i < _countof(worldTransform_); i++) 
	{
		for (size_t j = 0; j < _countof(worldTransform_); j++) 
		{
			for (size_t k = 0; k < _countof(worldTransform_); k++) 
			{
				model_->Draw(worldTransform_[k][j][i], viewProjection_, textureHandle_);
			}
		}
	}
	


	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}