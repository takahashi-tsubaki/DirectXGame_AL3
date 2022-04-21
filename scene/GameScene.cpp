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

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	for (int i = 0; i < _countof(viewProjection_);i++)
	{
		//
		viewProjection_[i].eye = {0, 0, -25};

		//
		viewProjection_[0].target = {0, 5, 0};
		viewProjection_[1].target = {-4.3, -2.5, 0};
		viewProjection_[2].target = {4.3, -2.5, 0};

		//
		viewProjection_[i].up = {0.0f, 1.0f, 0.0f};

		//ビュープロジェクションの初期化
		viewProjection_[i].Initialize();

	}
	
	
}

void GameScene::Update() 
{ 
	if (input_->TriggerKey(DIK_SPACE)) 
	{
		
		if (targetNum<2) 
		{
			targetNum++;
		} else
		{
			targetNum = 0;
		}
	}
	for (int i = 0; i < _countof(viewProjection_); i++)
	{
		//行列の再計算
		/*viewProjection_.UpdateMatrix();*/

		debugText_->SetPos(50, 50);
		debugText_->Printf(
		  "eye:(%f,%f,%f)\n", viewProjection_[i].eye.x, viewProjection_[i].eye.y,
		  viewProjection_[i].eye.z);
		debugText_->SetPos(50, 70);
		debugText_->Printf(
		  "target:(%f,%f,%f)\n", viewProjection_[i].target.x, viewProjection_[i].target.y,
		  viewProjection_[i].target.z);
		debugText_->SetPos(50, 90);
		debugText_->Printf(
		  "up:(%f,%f,%f)\n", viewProjection_[i].up.x, viewProjection_[i].up.y,
		  viewProjection_[i].up.z);

	}
	
}

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
	model_->Draw(worldTransform_, viewProjection_[targetNum], textureHandle_);

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