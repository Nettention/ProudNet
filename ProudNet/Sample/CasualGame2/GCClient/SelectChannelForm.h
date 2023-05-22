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
void CALLBACK SelectChannelForm_OnGuiEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );

/**
\~korean
entry client에 접속한 상태에서, 로비 채널 선택 폼을 구성합니다.

\~english
Compose lobby channel selection form with connecting entry client.

\~chinese
连接entryclient状态下构成lobby channel选择表单。

\~japanese
entry clientに接続した状態で、ロビーチャンネルの選択フォームを構成します。

\~
*/
void InitSelectChannelForm();
void ResetSelectChannelForm(UINT Width, UINT Height);