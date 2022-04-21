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

	//乱数シード生成器
	std::random_device seed_gen;
	//メルセンヌ・ツイスター
	std::mt19937_64 engine(seed_gen());
	//乱数範囲(回転角用)

	//乱数範囲(座標用)
	std::uniform_real_distribution<float> posDist(-20.0f, 10.0f); //最小値と最大値

	
	//X,Y,Z方向のスケーリングを設定
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};

	////X,Y,Z軸回りの回転角を設定
	//worldTransform_.rotation_ = {};

	// X,Y,Z軸回りの平行移動を設定
	worldTransform_.translation_ = {0.0f ,0.0f,0.0f};

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
		
	for (int i = 0;i<_countof(viewProjection_);i++) 
	{
		/*eyePosX[i] = posDist(engine);
		eyePosY[i] = posDist(engine);
		eyePosZ[i] = posDist(engine);*/

		//
		/*viewProjection_[i].eye = {eyePosX[i], eyePosY[i], eyePosZ[i]};*/
		viewProjection_[i].eye = {posDist(engine), posDist(engine), posDist(engine)};
		//
		viewProjection_[i].target = {10,0,0};

		// 
		viewProjection_[i].up = {0.0f,1.0f, 0.0f};

		//ビュープロジェクションの初期化
		viewProjection_[i].Initialize();

	}
}

void GameScene::Update() 
{ 
	
	if (input_->TriggerKey(DIK_SPACE)) {
		if (cameraNum < 2) {
			cameraNum++;
		} else {
			cameraNum = 0;
		}
	}
		
	for (int i = 0; i < _countof(viewProjection_); i++)
	{
		debugText_->SetPos(50, 50 + (i*70));
		/*debugText_->Printf("camera = %d", cameraNum);*/
		debugText_->Printf(
		  "eye:(%f,%f,%f)\n", viewProjection_[i].eye.x, viewProjection_[i].eye.y,
		  viewProjection_[i].eye.z);
		debugText_->SetPos(50, 70 + (i * 70));
		debugText_->Printf(
		  "target:(%f,%f,%f)\n", viewProjection_[i].target.x, viewProjection_[i].target.y,
		  viewProjection_[i].target.z);
		debugText_->SetPos(50, 90 + (i * 70));
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
	model_->Draw(worldTransform_,viewProjection_[cameraNum],textureHandle_);

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