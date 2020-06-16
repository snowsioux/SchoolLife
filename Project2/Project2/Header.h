#pragma once
#include "DxLib.h"
#include <string.h>
#include <string>
#include <sstream>

IPDATA IP_set();//IP入力
int SelectMode();//モードセレクト

//座標
struct  Pos
{
	float x, y;
};

//データクラスの定義
class Data
{
private:
public:
	char name[16];
	Pos pos;

	//コンストラクタ
	Data(float _x, float _y, char* _name);
};