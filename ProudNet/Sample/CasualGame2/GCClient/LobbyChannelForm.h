#pragma once

#include "MainWindow.h"

/**
\~korean
서버 연결 폼에서의 이벤트 Handler입니다.

\~english
Event handler at server connection form

\~chinese
服务器连接表单的事件Handler。

\~japanese
サーバ接続フォームでのイベントHandlerです。

\~
*/
void CALLBACK LobbyChannelForm_OnGuiEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );

/**
\~korean
로비 채널 폼을 구성합니다.

\~english
Compose lobby channel form.

\~chinese
构成lobby channel表单。

\~japanese
ロビーチャンネルフォームを構成します。

\~
*/
void InitLobbyChannelForm();
void ResetLobbyChannelForm(UINT Width, UINT Height);