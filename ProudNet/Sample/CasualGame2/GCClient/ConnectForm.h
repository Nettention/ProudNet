#pragma once

#include "MainWindow.h"

/**
\~korean
서버 연결 폼에서의 이벤트 Handler입니다.

\~english
Event handler at server connecting form

\~chinese
服务器连接表单中的事件Handler。

\~japanese
サーバ接続フォームでのイベントHandlerです。

\~
*/
void CALLBACK ConnectForm_OnGuiEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );

/**
\~korean
서버에 연결하는 폼을 구성합니다.

\~english
Compose server connecting form.

\~chinese
构成连接服务器的表单。

\~japanese
サーバに接続するフォームを構成します。

\~
*/
void InitConnectForm();
void ResetConnectForm(UINT Width, UINT Height);