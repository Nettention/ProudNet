#pragma once

#include "MainWindow.h"

/**
\~korean
 서버 연결 폼에서의 이벤트 Handler입니다.

\~english
 Event handler at server connection form

\~chinese
服务器连接表单中的事件Handler。

\~japanese
サーバ接続フォームでのイベントHandlerです。

\~
*/
void CALLBACK JoinForm_OnGuiEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );

/**
\~korean
 서버에 로그인하는 폼을 구성합니다.

\~english
 Compose logon form to server.

\~chinese
构成服务器登录表单。

\~japanese
サーバにログインするフォームを構成します。

\~
*/
void InitJoinForm();
void ResetJoinForm(UINT Width, UINT Height);