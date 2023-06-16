/* 106 228 203 199 203 251 69 164 171 249 223 147 102 233 72 249 25 46 47 89 4 75 196 233 166 154 197 5 225 93 96 137 109 76  */
#pragma once

#include "FastArray.h"
#include "PNString.h"

namespace Proud
{
	/** 
	\~korean 현재 스레드의 현재 실행 지점의 호출 스택을 얻습니다. 
	\~english Gets the call stack at the current execution point of the current thread. 
	获得当前线程的当前执行点的调用堆栈。
	\~chinese
	 Gets the call stack at the current execution point of the current thread.
	\~japanese
	現在スレッドの現在実行地点の呼び出しスタックを得ます。
	\~
	*/
	PROUD_API void GetStackTrace(CFastArray<String>& output);
}

