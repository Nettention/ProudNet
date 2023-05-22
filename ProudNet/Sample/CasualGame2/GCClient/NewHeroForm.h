#pragma once

#include "MainWindow.h"

/**
\~korean
이벤트 Handler입니다.

\~english
Event handler

\~chinese
事件Handler。

\~japanese
イベントのHandlerです。

\~
*/
void CALLBACK NewHeroForm_OnGuiEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );

/**
\~korean
새 캐릭터를 만드는 폼 UI를 구성합니다.

\~english
Compose form UI that creates new character.

\~chinese
构成生成新人物的表单UI。

\~japanese
新しいキャラクターを作るフォームUIを構成します。

\~
*/
void InitNewHeroForm();
void ResetNewHeroForm(UINT Width, UINT Height);