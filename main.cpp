#include <Novice.h>
#include <KamataEngine.h>
#include "MassFunction.h"
using namespace KamataEngine;

const char kWindowTitle[] = "学籍番号";
const int kwindowHight = 1280;
const int kwindowWidth = 720;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};
	Vector3 v1 = { 1.2f, -3.9f, 2.5f };
	Vector3 v2 = { 2.8f, 0.4f, -1.3f };
	Vector3 cross = Cross(v1, v2);

	Vector3 rotate = { 0.0f, 0.0f, 0.0f };
	Vector3 traslate = { 0.0f, 0.0f, 0.0f };

	Vector3 cameraPos = { 0.0f, 0.0f, -5.0f };
	
	Vector3 kLocalVertices[3] = {
		{0.0f, 0.1f, 0.0f},
		{-0.1f, -0.1f, 0.0f},
		{0.1f, -0.1f, 0.0f}
	};
	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		rotate.y += 0.1f;
		if (keys[DIK_W]){
			traslate.z += 0.1f;
		}else if(keys[DIK_S]){
			traslate.z -= 0.1f;
		};
		if(keys[DIK_A]){
			traslate.x -= 0.1f;
		}else if (keys[DIK_D]) {
			traslate.x += 0.1f;
		};


		Matrix4x4 worldMatrix = MakeAfineMatrix({ 1.0f, 1.0f, 1.0f }, rotate, traslate);
		//MatrixScreenPrintf(0, kRowHeight, worldMatrix, "worldMatrix");
		Matrix4x4 cameraMatrix = MakeAfineMatrix({ 1.0f, 1.0f, 1.0f }, {0.0f,0.0f,0.0f}, cameraPos);
		//MatrixScreenPrintf(0, kRowHeight * 20, cameraMatrix, "cameraMatrix");
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		//MatrixScreenPrintf(0, kRowHeight * 30, viewMatrix, "viewMatrix");
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, static_cast<float>(kwindowWidth)/static_cast <float>(kwindowHight) , 0.1f, 100.0f);
		//MatrixScreenPrintf(kColumnWidth*10, kRowHeight  , projectionMatrix, "projectionMatrix");
		Matrix4x4 worldViewProjectionMatrix = Multiply( worldMatrix,Multiply(viewMatrix,projectionMatrix));
		//MatrixScreenPrintf(kColumnWidth*10, kRowHeight * 20, worldViewProjectionMatrix, "worldViewProjectionMatrix");
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0.0f, 0.0f, static_cast<float>(kwindowWidth), static_cast<float>(kwindowHight), 0.0f, 1.0f);
		//MatrixScreenPrintf(kColumnWidth*10, kRowHeight * 30, viewportMatrix, "viewportMatrix");
		Vector3 screenVertices[3];
		for (uint32_t i = 0; i < 3; i++){
			Vector3 nbcVertex = Transform(kLocalVertices[i], worldViewProjectionMatrix);
			screenVertices[i] = Transform(nbcVertex, viewportMatrix);

		}



		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		VectorScreenPrintf(0,0,cross,"Cross");
		//MatrixScreenPrintf(0, kRowHeight, worldMatrix, "v1");
		Novice::DrawTriangle(
			static_cast<int>(screenVertices[0].x),
			static_cast<int>(screenVertices[0].y),
			static_cast<int>(screenVertices[1].x),
			static_cast<int>(screenVertices[1].y),
			static_cast<int>(screenVertices[2].x),
			static_cast<int>(screenVertices[2].y),
			RED,
			kFillModeSolid
		);


		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
