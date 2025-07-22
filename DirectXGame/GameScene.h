#pragma once
#include "KamataEngine.h"
#include "json.hpp"
#include <vector>
#include <map>

class GameScene {

/// レベルデータを格納するための構造体
// オブジェクト　1個分のデータ
struct ObjectData {
		std::string type; //"type"
		std::string name; //"name"

		//"transform"
		struct Transform {
			KamataEngine::Vector3 translation; //"translation"
			KamataEngine::Vector3 rotation;    //"rotation"
			KamataEngine::Vector3 scaling;     //"scaling"
		};

		Transform transform; // メンバの準備

		//"file_name"
		std::string file_name;
	};
// レベルデータ
struct LevelData {
		//"name"
		std::string name;

		//"objects"
		std::vector<ObjectData> objects;
	};

struct Object {
		//"name"
		std::string name;
		//"chilren"
		std::list<Object> children;
	};

public:
	void Init();//初期化処理
	void Update();//更新処理
	void Draw();//描画処理

private:
	KamataEngine::DirectXCommon* dxCommon_;
	/// レベルデータを構造体に格納していく
	LevelData* levelData = nullptr;
	// モデルデータコンテナ
	std::map<std::string, KamataEngine::Model*> models;
	// ワールド行列
	std::vector<KamataEngine::WorldTransform*> worldTransforms;

	KamataEngine::Camera camera_;
};
