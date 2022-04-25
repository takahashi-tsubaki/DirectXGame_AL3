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
	reticleHandle_ = TextureManager::Load("reticle.png");
	scopeHandle_ = TextureManager::Load("scope.png");

	//
	sprite_ = Sprite::Create(reticleHandle_, {576, 296});
	sprite2_ = Sprite::Create(scopeHandle_, {0, 0});
	//３Dモデルの生成
	model_ = Model::Create();

	//乱数シード生成器
	std::random_device seed_gen;

	//メルセンヌ・ツイスター
	std::mt19937_64 engine(seed_gen());

	//乱数範囲(回転角用)
	std::uniform_real_distribution<float> rotDist(0.0f,XM_2PI);

	//乱数範囲(座標用)
	std::uniform_real_distribution<float> posDist(-10.0f,10.0f);


	for (int i = 0;i<_countof(worldTransform_);i++) 
	{
		
		//画像の大きさ
		worldTransform_[i].scale_ = {1.0f, 1.0f, 1.0f};

		//X,Y,Zの回転
		worldTransform_[i].rotation_ = {rotDist(engine), rotDist(engine), rotDist(engine)};

		//X,Y,Z軸の平行移動
		worldTransform_[i].translation_ = {posDist(engine), posDist(engine), posDist(engine)};

		//ワールドトランスフォームの初期化
		worldTransform_[i].Initialize();
		
	}

	viewProjection_.eye = {0,0,-50};

	viewProjection_.fovAngleY = XMConvertToRadians(90.0f);

	//ニアクリップ距離
	viewProjection_.nearZ = 0.1f;

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

}

void GameScene::Update() 
{ 
	
	XMFLOAT3 move = {0, 0, 0};

	const float kTargetSpeed = 0.2f;

	if (input_->PushKey(DIK_UP)) 
	{
		move = {0, kTargetSpeed, 0};
	}
	else if(input_->PushKey(DIK_DOWN)) 
	{
		move = {0, -kTargetSpeed, 0};
	}
	if (input_->PushKey(DIK_RIGHT)) 
	{
		move = {kTargetSpeed,0, 0};
	} 
	else if (input_->PushKey(DIK_LEFT)) 
	{
		move = {-kTargetSpeed,0, 0};
	}

	viewProjection_.target.x += move.x;
	viewProjection_.target.y += move.y;
	viewProjection_.target.z += move.z;

	viewProjection_.UpdateMatrix();

	if (input_->TriggerKey(DIK_SPACE))
	{
		if (scope == 0)
		{
			scope = 1;
		} 
		else 
		{
			scope = 0;
		}
		
	} 
		
	

	if (scope == 1)
	{
		viewProjection_.fovAngleY = 33.349995f;
	}
	else
	{
		viewProjection_.fovAngleY = 90.0f;
	}

	viewProjection_.UpdateMatrix();

	debugText_->SetPos(50, 50);
	debugText_->Printf(
	    "eye:(%f,%f,%f)\n", viewProjection_.eye.x, viewProjection_.eye.y,
	    viewProjection_.eye.z);
	debugText_->SetPos(50, 70);
	debugText_->Printf(
	    "target:(%f,%f,%f)\n", viewProjection_.target.x,
	    viewProjection_.target.y, viewProjection_.target.z);
	debugText_->SetPos(50, 90);
	debugText_->Printf(
	    "up:(%f,%f,%f)\n", viewProjection_.up.x, viewProjection_.up.y,
	    viewProjection_.up.z);

	debugText_->SetPos(50, 110);
	debugText_->Printf("forAngleY(Degree):%f\n",viewProjection_.fovAngleY);

	debugText_->SetPos(50, 130);
	debugText_->Printf("nearZ:%f\n", viewProjection_.nearZ);
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

	if (scope == 1) 
	{
		sprite_->Draw();
		sprite2_->Draw();
	}

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}