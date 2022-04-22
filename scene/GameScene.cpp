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

	for (int i = 0;i<_countof(worldTransform_);i++) 
	{


		//X、Y、Zの平行移動
		worldTransform_[0].translation_ = {};
		worldTransform_[1].translation_ = {5,-3,0};
		worldTransform_[2].translation_ = {-5,-3,0};

		//ワールドトランスフォームの初期化
		worldTransform_[i].Initialize();
	}

	

	for (int i = 0; i < _countof(viewProjection_);i++)
	{
		//
		viewProjection_[i].eye = {0, 0, -25};

		//
		viewProjection_[0].target = {-0, 5, 0};
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
	
	
		

		debugText_->SetPos(50, 50);
		debugText_->Printf(
	      "eye:(%f,%f,%f)\n", viewProjection_[targetNum].eye.x, viewProjection_[targetNum].eye.y,
	      viewProjection_[targetNum].eye.z);
		debugText_->SetPos(50, 70);
		debugText_->Printf(
	      "target:(%f,%f,%f)\n", viewProjection_[targetNum].target.x,
	      viewProjection_[targetNum].target.y, viewProjection_[targetNum].target.z);
		debugText_->SetPos(50, 90);
		debugText_->Printf(
	      "up:(%f,%f,%f)\n", viewProjection_[targetNum].up.x, viewProjection_[targetNum].up.y,
	      viewProjection_[targetNum].up.z);

		////行列の再計算
		//viewProjection_[i].UpdateMatrix();

	
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
	for (int i = 0;i<3;i++)
	{
		model_->Draw(worldTransform_[i], viewProjection_[targetNum], textureHandle_);

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