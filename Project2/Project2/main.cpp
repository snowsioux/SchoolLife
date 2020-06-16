#pragma once
#include "DxLib.h"
#include "Header.h"
//テスト

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//ポート用変数
	int SendPort, RecvPort;//send送るrecv受ける

	//送受信データ処理用
	char StrBuf[256] = { "NULL" };

	//Windowの設定
	ChangeWindowMode(TRUE);//windowモード
	SetGraphMode(640, 480, 32);//windowサイズ640*480 32bit
	SetAlwaysRunFlag(TRUE);//バックグラウンドでも実行できるようにする
	SetDoubleStartValidFlag(TRUE);//多重起動の許可

	//初期化
	if (DxLib_Init() == -1)return -1;//エラーが出たら終了

	//モードのセレクト
	int mode = SelectMode();

	//windowの名前変更
	if (mode == 0)
	{
		SetWindowText("スタート側");
		SendPort = 41;
		RecvPort = 40;
	}
	else
	{
		SetWindowText("待ち受け側");
		SendPort = 40;
		RecvPort = 41;
	}

	//描画の設定
	SetDrawScreen(DX_SCREEN_BACK);

	//画像の読み込み
	int image = LoadGraph("image\\sample.png");

	//画像の位置情報
	float x, y;

	//位置を初期化
	x = 0.0f;
	y = 0.0f;

	//送受信用データ
	char name[15] = "name";
	Data* data = new Data(x, y, name);

	//IPの設定
	IPDATA IP = IP_set();

	//通信用ソケットハンドル
	int NetUDPHandle = MakeUDPSocket(SendPort);//ポートの設定

	while (CheckHitKey(KEY_INPUT_ESCAPE) == 0)//ESCキーで終了
	{

		if (ProcessMessage() == -1)break;

		if (mode == 0)
		{
			if (CheckHitKey(KEY_INPUT_UP))y -= 4.0f;
			if (CheckHitKey(KEY_INPUT_DOWN))y += 4.0f;
			if (CheckHitKey(KEY_INPUT_LEFT))x -= 4.0f;
			if (CheckHitKey(KEY_INPUT_RIGHT))x += 4.0f;

			//データを変換
			//sprintf_s(StrBuf, "%f", x);//文字列を変換

			//送受信データの更新
			data->pos.x = x;
			data->pos.y = y;

			//データの送信
			NetWorkSendUDP(NetUDPHandle, IP, RecvPort, data, 256);
		}

		if (mode == 1)
		{
			//データの受信
			if (CheckNetWorkRecvUDP(NetUDPHandle) == TRUE)
			{
				NetWorkRecvUDP(NetUDPHandle, NULL, NULL, StrBuf, 256, FALSE);
				//受信データを変換
				memcpy_s(data, sizeof(Data), StrBuf, sizeof(Data));
				x = data->pos.x;
				y = data->pos.y;
				//x = atof(StrBuf);//受け取ったデータをfloatに変換
				//DrawFormatString(0, 16, GetColor(255, 0, 0), "%s", StrBuf);
			}
		}
		//描画のクリア
		ClearDrawScreen();

		//描画処理
		DrawGraphF(x, y, image, TRUE);

		//バックバッファと切り替え
		ScreenFlip();

	}

	//何かキー入力があるまで待機
	WaitKey();


	delete data;//送受信データの削除
	//ハンドルの削除
	DeleteUDPSocket(NetUDPHandle);
	//dxライブラリ終了処理
	DxLib_End();

	return 0;

}