#include "KamataEngine.h"
#include "GameScene.h"
#include <Windows.h>

using namespace KamataEngine;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	//エンジンの初期化
	KamataEngine::Initialize(L"LE3C_12_スズキ_ソラ");

	GameScene* gameScene = new GameScene();
	gameScene->Init();

	//メインループ
	while (true) {
		//エンジンの更新
		if (KamataEngine::Update()) {
			break;
		}
		gameScene->Update();

		gameScene->Draw();
	}

	//エンジンの終了処理
	KamataEngine::Finalize();

	delete gameScene;

	return 0;
}
