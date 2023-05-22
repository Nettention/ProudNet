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
void CALLBACK NewRoomForm_OnGuiEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );

/**
\~korean
 게임방 만들기 폼 UI를 구성합니다.

\~english
 Compose game room creation form UI.

\~chinese
构成生成游戏房间的表单UI。

\~japanese
ゲームルームを作成するフォームUIを構成します。

\~
*/
void InitNewRoomForm();
void ResetNewRoomForm(UINT Width, UINT Height);