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
void CALLBACK SelectHeroForm_OnGuiEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );

/**
\~korean
캐릭터 선택 폼을 구성합니다.

\~english
Compose character selection form.

\~chinese
构成人物选择表单。

\~japanese
キャラクター選択のフォームを構成します。

\~
*/
void InitSelectHeroForm();
void ResetSelectHeroForm(UINT Width, UINT Height);