#pragma once
#include "../GCCommon/Vars.h"

class CItem_C : public CItem
{
public:
	CItem_C(void);
	~CItem_C(void);

	/**
	\~korean
	그리는 역할만 합니다.

	\~english
	Does only drawing.

	\~chinese
	只负责画。

	\~japanese
	描画する役割のみです。

	\~
	*/
	void Render();
};
typedef RefCount<CItem_C> Item_C_Ptr;