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
void CALLBACK BattleWaitingForm_OnGuiEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );

/**
\~korean
게임방 대기실 폼을 구성합니다.

\~english
Compose game waiting room form.

\~chinese
构成游戏房间等候室表单。

\~japanese
ゲームルームの控室フォームを構成します。

\~
*/
void InitBattleWaitingForm();
void ResetBattleWaitingForm(UINT Width, UINT Height);
void BattleWaitingForm_RefreshButtons();
