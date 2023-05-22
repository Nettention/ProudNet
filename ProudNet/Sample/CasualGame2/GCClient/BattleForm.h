#pragma once

#include "MainWindow.h"

/**
\~korean
서버 연결 폼에서의 이벤트 Handler입니다.

\~english
Event handler at server connecting form

\~chinese
服务器连接表单的事件Handler。

\~japanese
サーバ接続フォームでのイベントHandlerです。

\~
*/
void CALLBACK BattleForm_OnGuiEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );

/**
\~korean
전투중 UI 폼을 준비합니다.

\~english
Ready UI form during battle.

\~chinese
战斗中准备UI表单。

\~japanese
戦闘中のUIフォームを準備します。

\~
*/
void InitBattleForm();
void ResetBattleForm(UINT Width, UINT Height);

