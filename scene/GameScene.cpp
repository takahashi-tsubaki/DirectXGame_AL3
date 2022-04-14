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
	textureHandle_ = TextureManager::Load("mario.jpg");
	//３Dモデルの生成
	model_ = Model::Create();

	//乱数シード生成器
	std::random_device seed_gen;
	//メンセンヌ・ツイスター
	std::mt19937_64 engine(seed_gen());
	//乱数範囲(回転角用)
	std::uniform_real_distribution<float> rotDist(0.0f, XM_2PI);
	//乱数範囲(座標用)
	std::uniform_real_distribution<float> posDist(-10.0f, 10.0f);

	for (size_t i = 0;i< _countof(worldTransform_);i++) 
	{
		// X、Y、Z方向のスケーリングを設定
		worldTransform_[i].scale_ = {5.0f, 5.0f, 5.0f};
		// X、Y、Z軸周りの回転角を設定(単位はラジアン)
		worldTransform_[i].rotation_ = {
		  rotDist(engine), rotDist(engine), rotDist(engine)}; // XM_PIは180度
		// X、Y、Z軸周りの平行移動を設定
		worldTransform_[i].translation_ = {posDist(engine), posDist(engine), posDist(engine)};

		// X、Y、Z軸周りの回転角を設定(単位はラジアン)
		// worldTransform_.rotation_ = {0.0f,XMConvertToRadians(45.0f), 0.0f};

		//ワールドトランスフォームの初期化
		worldTransform_[i].Initialize();
	}

	
	//☝の３つは同時に機能する

	
	
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	

}

void GameScene::Update() {

	/*std::string strDebugTra = std::string("transtion:(") + std::to_string(translationX) +
	                          std::string(",") + std::to_string(translationY) + std::string(",") +
	                          std::to_string(translationZ) + std::string(")");
	debugText_->Print(strDebugTra, 50, 50, 1.0f);

	std::string strDebugRota = std::string("rotation:(") + std::to_string(rotationX) +
	                           std::string(",") + std::to_string(rotationY) + std::string(",") +
	                           std::to_string(rotationZ) + std::string(")");
	debugText_->Print(strDebugRota, 50, 70, 1.0f);

	std::string strDebugScale = std::string("scale:(") + std::to_string(scaleX) + std::string(",") +
	                            std::to_string(scaleY) + std::string(",") + std::to_string(scaleZ) +
	                            std::string(")");
	debugText_->Print(strDebugScale, 50, 90, 1.0f);*/
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
	/*model_->Draw(worldTransform_, viewProjection_, textureHandle_);*/

	for (size_t i = 0; i < _countof(worldTransform_); i++) 
	{
		model_->Draw(worldTransform_[i], viewProjection_, textureHandle_);
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