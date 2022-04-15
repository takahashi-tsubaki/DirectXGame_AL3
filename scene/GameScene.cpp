﻿#include "GameScene.h"
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
	std::uniform_real_distribution<float> posDist(-50.0f, 50.0f);

	for (size_t i = 0;i< _countof(worldTransform_);i++) 
	{
		// X、Y、Z方向のスケーリングを設定
		worldTransform_[i].scale_ = {5.0f, 5.0f, 5.0f};
		// X、Y、Z軸周りの回転角を設定(単位はラジアン)
		worldTransform_[i].rotation_ = {
		  rotDist(engine), rotDist(engine), rotDist(engine)}; // XM_PIは180度
		// X、Y、Z軸周りの平行移動を設定
		worldTransform_[i].translation_ = {posDist(engine), posDist(engine), posDist(engine)};

		//☝の３つは同時に機能する

		// X、Y、Z軸周りの回転角を設定(単位はラジアン)
		// worldTransform_.rotation_ = {0.0f,XMConvertToRadians(45.0f), 0.0f};

		//ワールドトランスフォームの初期化
		worldTransform_[i].Initialize();
	}
	//カメラ視点座標を設定
	viewProjection_.eye = {0,0,-250}; //初期値は{0,0,-50}

	//カメラ注視点座標を設定
	viewProjection_.target = {10, 0, 0};
	
	//カメラの上方向ベクトルを設定(右上45度指定)
	viewProjection_.up = {cosf(XM_PI / 4.0f), sinf(XM_PI / 4.0f), 0.0f};

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

	//視点移動処理
	//視点の移動ベクトル
	{
		XMFLOAT3 move = {0, 0, 0};

		//視点の移動速度
		const float kEyeSpeed = 0.2f;

		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_W)) {
			move = {0, 0, kEyeSpeed};
		} else if (input_->PushKey(DIK_S)) {
			move = {0, 0, -kEyeSpeed};
		}
		//視点移動(ベクトルの加算)
		viewProjection_.eye.x += move.x;
		viewProjection_.eye.y += move.y;
		viewProjection_.eye.z += move.z;

		//行列の再計算
		viewProjection_.UpdateMatrix();

		//デバック用表示
		debugText_->SetPos(50, 50);
		debugText_->Printf("eye:(%f,%f,%f,)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);

		XMFLOAT3 move2 = {0, 0, 0};

		//注視点の移動速度
		const float kTargetSpeed = 0.2f;

		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_LEFT)) 
		{
			move2 = {-kTargetSpeed,0,0};
		} 
		else if (input_->PushKey(DIK_RIGHT)) 
		{
			move2 = {kTargetSpeed, 0, 0};
		}

		//注視点移動(ベクトルの加算)
		viewProjection_.target.x += move2.x;
		viewProjection_.target.y += move2.y;
		viewProjection_.target.z += move2.z;

		//行列の再計算
		viewProjection_.UpdateMatrix();
		//デバック用表示
		debugText_->SetPos(50, 70);
		debugText_->Printf(
		  "target:(%f,%f,%f,)", viewProjection_.target.x, viewProjection_.target.y,
		  viewProjection_.target.z);

		//上方向回転処理
		
		//上方向の回転速度[ラジアン/frame]
		const float kUpRotSpeed = 0.05f;

		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_SPACE))
		{
			viewAngle += kUpRotSpeed;
			//2πを超えたら元の戻す
			viewAngle = fmodf(viewAngle,XM_2PI);
		} 
		
		//上方向ベクトルを計算(半径1の円周上の計算)
		viewProjection_.up = {cosf(viewAngle), sinf(viewAngle), 0.0f};

		//行列の再計算
		viewProjection_.UpdateMatrix();

		//デバック用表示
		debugText_->SetPos(50, 90);
		debugText_->Printf(
		  "up:(%f,%f,%f,)", viewProjection_.up.x, viewProjection_.up.y,
		  viewProjection_.up.z);


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