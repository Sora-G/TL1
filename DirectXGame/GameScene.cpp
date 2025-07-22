#include "GameScene.h"

#include <string>
#include <fstream>
#include <cassert>

using namespace KamataEngine;

void GameScene::Init() {
	//初期化処理

	///jsonファイルのシリアライズ化
	//jsonファイルのパス名
	const std::string fullpath = std::string("Resources/levels/") + "scene.json";

	//ファイルストリーム
	std::ifstream file;

	//ファイルを開く
	file.open(fullpath);
	//ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}

	nlohmann::json deserialized; //逆シリアライズ化

	//ファイルから読み込み、メモリへ格納
	file >> deserialized;

	//正しいレベルデータファイルかチェック
	assert(deserialized.is_object());//objectか？
	assert(deserialized.contains("name"));//"name"が含まれているか
	assert(deserialized["name"].is_string());//["name"]は文字列か？

	// レベルデータ生成
	levelData = new LevelData();

	//"name"を文字列として取得
	levelData->name = deserialized["name"].get<std::string>();
	assert(levelData->name == "scene");//それはsceneか？

	//"objects"の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"]) {
		//オブジェクト　１つ分の妥当性のチェック
		assert(object.contains("type"));
		
		if (object["type"].get<std::string>() == "MESH") {
			//1個分の要素の準備
			levelData->objects.emplace_back(ObjectData{});
			ObjectData& objectData = levelData->objects.back();//追加要素の参照を用意し可能性も良くなる

			objectData.type = object["type"].get<std::string>();//"type"
			objectData.name = object["name"].get<std::string>();//"name"

			//トランスフォームのパラメータ読み込み
			nlohmann::json& transform = object["transform"];
			//平行移動 "translation"
			objectData.transform.translation.x = (float)transform["translation"][0];
			objectData.transform.translation.y = (float)transform["translation"][2];
			objectData.transform.translation.z = (float)transform["translation"][1];
			//回転角 "lotation"
			objectData.transform.rotation.x = -(float)transform["rotation"][0];
			objectData.transform.rotation.y = -(float)transform["rotation"][2];
			objectData.transform.rotation.z = -(float)transform["rotation"][1];
			//拡大縮小
			objectData.transform.scaling.x = (float)transform["scaling"][0];
			objectData.transform.scaling.y = (float)transform["scaling"][2];
			objectData.transform.scaling.z = (float)transform["scaling"][1];

			//"file_name"
			if (object.contains("file_name")) {
				objectData.file_name = object["file_name"].get<std::string>();
			}
		}
		// 再帰処理
		//ToDo:オブジェクト走査を再起関数でまとめ、再帰呼び出しで枝を走査する
		if (object.contains("children")) {
		
		}
	}


	// レベルデータに出現するモデルの読み込み
	for (auto& objectData : levelData->objects) {
		// ファイル名から登録済みモデルを検索
		decltype(models)::iterator it = models.find(objectData.file_name);

		//未読み込みの場合読み込む
		if (it == models.end()) {
			model->CreateFromOBJ(objectData.file_name);
			models[objectData.file_name] = model;
		}
	}

	//レベルデータからワールド行列を生成
	for (auto& objectData : levelData->objects) {
		//ワールド行列を生成
		WorldTransform* newObject = new WorldTransform();
		//座標
		newObject->translation_ = objectData.transform.translation;
		//回転角
		newObject->rotation_ = objectData.transform.rotation;
		//拡縮
		newObject->scale_ = objectData.transform.scaling;

		newObject->Initialize();
		
		//配列に登録
		worldTransforms.push_back(newObject);
	}

	camera_.Initialize();
}

void GameScene::Update() {
	//更新処理
	for (WorldTransform* object : worldTransforms) {

		//Todo:ワールド行列の計算
		
		//定数バッファの転送
		object->TransferMatrix();
	}
}

void GameScene::Draw() {
	//描画処理
#pragma region 3Dオブジェクト描画
	//3Dオブジェクト描画前処理
	Model::PreDraw();

	int i = 0;
	// レベルデータからオブジェクトを生成＆配置
	for (auto& objectData : levelData->objects) {
		// ファイル名から登録済みモデルを検索
		decltype(models)::iterator it = models.find(objectData.file_name);
		if (it != models.end()) {
			model = it->second;
		}
	
		model->Draw(*worldTransforms[i], camera_);
		i++;
	}


	//3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// スプライト描画前処理
	Sprite::PreDraw(dxCommon_->GetCommandList());

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}

