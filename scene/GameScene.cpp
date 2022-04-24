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
		for (int j = 0; j < _countof(worldTransform_); j++) 
		{

			worldTransform_[j][i].translation_ = {-12.0f + (i * 4.0f), -12.0f + (j*4.0f),0.0f};

			//ワールドトランスフォームの初期化
			worldTransform_[j][i].Initialize();
		}
		
	}

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

}

void GameScene::Update() 
{ 
	
	XMFLOAT3 move = {0, 0, 0};

	const float kTargetSpeed = 0.2f;

	if (input_->PushKey(DIK_W)) 
	{
		move = {0, kTargetSpeed, 0};
	}
	else if(input_->PushKey(DIK_S)) 
	{
		move = {0, -kTargetSpeed, 0};
	}
	if (input_->PushKey(DIK_D)) 
	{
		move = {kTargetSpeed,0, 0};
	} 
	else if (input_->PushKey(DIK_A)) 
	{
		move = {-kTargetSpeed,0, 0};
	}

	viewProjection_.target.x += move.x;
	viewProjection_.target.y += move.y;
	viewProjection_.target.z += move.z;

	viewProjection_.UpdateMatrix();

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
	for (int i = 0; i < _countof(worldTransform_); i++) 
	{
		for (int j = 0; j < _countof(worldTransform_); j++) 
		{
			model_->Draw(worldTransform_[j][i], viewProjection_, textureHandle_);
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