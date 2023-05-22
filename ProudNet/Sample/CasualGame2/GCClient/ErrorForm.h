#pragma once

#include "MainWindow.h"

/**
\~korean
에러 표시 폼에서의 이벤트 Handler입니다.

\~english
Event handler at error indication form

\~chinese
错误显示表单中的事件Handler。

\~japanese
エラー表示フォームでのイベントHandlerです。

\~
*/
void CALLBACK ErrorForm_OnGuiEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );
void ResetErrorForm(UINT Width, UINT Height);

/**
\~korean
에러가 나서 종료해야 하는 상황에 나타나는 폼을 구성합니다.

\~english
Compose form when it appears quit situation with error.

\~chinese
构成出现错误而终止时显示的表单。

\~japanese
エラーが発生して終了せざるを得ない時に表示されるフォームを構成します。

\~
*/
void InitErrorForm();
