#include "Header.h"
//データクラス

//コンストラクタ
Data::Data(float _x, float _y, char* _name)
{
	pos.x = _x;
	pos.y = _y;

	strcpy_s(name, _name);
}
