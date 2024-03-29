/*
ProudNet 1.8.58849-master


이 프로그램의 저작권은 넷텐션에게 있습니다.
이 프로그램의 수정, 사용, 배포에 관련된 사항은 본 프로그램의 소유권자와의 계약을 따르며,
계약을 준수하지 않는 경우 원칙적으로 무단 사용을 금지합니다.
무단 사용에 의한 책임은 본 프로그램의 소유권자와의 계약서에 명시되어 있습니다.

** 주의 : 저작물에 관한 위의 명시를 제거하지 마십시오.


This program is soley copyrighted by Nettention.
Any use, correction, and distribution of this program are subject to the terms and conditions of the License Agreement.
Any violated use of this program is prohibited and will be cause of immediate termination according to the License Agreement.

** WARNING : PLEASE DO NOT REMOVE THE LEGAL NOTICE ABOVE.


此程序的版权归Nettention公司所有。
与此程序的修改、使用、发布相关的事项要遵守此程序的所有权者的协议。
不遵守协议时要原则性的禁止擅自使用。
擅自使用的责任明示在与此程序所有权者的合同书里。

** 注意：不要移除关于制作物的上述明示。


このプログラムの著作権はNettentionにあります。
このプログラムの修正、使用、配布に関する事項は本プログラムの所有権者との契約に従い、
契約を遵守しない場合、原則的に無断使用を禁じます。
無断使用による責任は本プログラムの所有権者との契約書に明示されています。

** 注意：著作物に関する上記の明示を除去しないでください。

*/

#pragma once

// Linux에서 ODBC를 쓰고 싶으면 ODBC for unix나 여타를 설치하십시오.
// 예: sudo apt-get install unixodbc-dev
// 예: yum install unixODBC-devel
#include <sqltypes.h>

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

namespace Proud
{
	/**
	 \~korean
	 ODBC API에 이진 데이터를 입출력할때 사용됩니다.
	 이진 데이터가 덩치가 매우 큰 경우, 데이터 입출력 과정에서 불필요한 복사 과정을 생략하기 위해 이 클래스가 사용됩니다.
	 사용자가 이진 데이터를 직접 소유해야 하고, 이 클래스는 그 데이터를 참조하는 역할을 합니다.

	 \~english
	 It is used for input & output of binary data at ODBC API.
	 If the size of binary data is too big, this class will be used to omit unnecessary copy process in the process of data input & output. 
	 A user must possess binary data and this class refers to the data. 

	 \~chinese
	 在ODBC API输出入二进制数据时使用。
	 二进制数据量非常大时，为了在输出入数据的过程中省略不必要的复制过程，使用该class。
	 用户应直接所有二进制数据，而该class起到参照该数据的作用。

	 \~japanese
	 ODBC APIにパイナリデータを入出力する場合に使われます。
	 パイナリデータのサイズが大変大きい場合、データ入出力過程で不必要なコピー過程を省略するためにこのクラスが使用されます。
	 ユーザーがパイナリデータを直接所有する必要があり、このクラスはそのデータを参照する役割をします。

	 \~
	 */
	class COdbcByteData
	{
	private:
		// 사용자가 마음대로 수정 할 수 없는 변수 입니다.
		// dataPtr로 지정한 사용자 데이터 버퍼 내부는 마음대로 수정할 수 있지만 그 객체 자체(포인터값)가 변경되면 안됩니다.
		// 객체가 변경되지 못하기 때문에 maxLen도 한번 설정하면 바꾸지 못합니다.
		SQLCHAR* m_dataPtr;

		SQLLEN m_dataLength;
		SQLULEN m_maxLength;

	public:
		/**
		 \~korean
		 생성자 함수. 여기서 사용자가 갖고 있는 이진 데이터의 주소와 크기를 입력하십시오.
		 \param pData 이진 데이터의 주소
		 \param dataLength 이진 데이터의 유효한 현재 크기입니다
		 \param maxLength 이진 데이터가 저장되는 최대 크기

		 \~english
		 Constructor function. Please input the size & address of binary data that a user has. 
		 \param pData Address of binary data.
		 \param dataLength Current valid size of binary data. 
		 \param maxLength Maximum size of binary data saving.

		 \~chinese
		 生成者 函数。在此输入用户的二进制数据的地址和大小。
		 \param pData 二进制数据的地址
		 \param dataLength 二进制数据的当前有效大小。
		 \param maxLength 二进制数据可保存的最大限度。

		 \~japanese
		 生成者関数。ここでユーザーが持っているパイナリデータのアドレスとサイズを入力してください。
		 \param pData パイナリデータのアドレス
		 \param dataLength パイナリデータの有効な現在サイズです。
		 \param maxLength パイナリデータがセーブされる最大サイズ

		 \~
		 */
		COdbcByteData(SQLCHAR* pData, SQLLEN dataLength, SQLULEN maxLength);

		/**
		 \~korean
		 설정된 데이터의 주소를 가져 옵니다.

		 \~english
		 Get the data address that has been set. 

		 \~chinese
		 获取已设定的数据地址。

		 \~japanese
		 設定されたデータのアドレスをロードします。

		 \~
		 */
		SQLCHAR* GetDataPtr();

		/**
		 \~korean
		 설정된 데이터의 크기를 가져 옵니다.

		 \~english
		 Get the data size that has been set. 

		 \~chinese
		 获取已设定的数据大小。

		 \~japanese
		 設定されたデータのサイズをロードします。.

		 \~
		 */
		SQLLEN GetDataLength();

		/**
		 \~korean
		 데이터의 크기를 재조정합니다.
		 생성자에서 지정한 사용자 데이터 버퍼를 수정하였을 때 그것의 길이가 바뀌면 이 함수를 호출하여 크기를 변경해야 합니다.

		 \~english
		 Readjust the size of data.
		 If the length of data buffer changes when modifying data buffer that has been set by a constructor, you will be required to change the size by calling this function. 

		 \~chinese
		 重新调整数据大小。
		 修改生成者指定的用户数据buffer时，如果该长度有所改变，需呼出此函数进行大小变更。

		 \~japanese
		 データのサイズを再調整します。
		 生成者で指定したユーザーデータバッファーを修正したとき、それの長さが変わるとこの関数を呼び出し、サイズを変更する必要があります。

		 \~
		 */
		void SetDataLength(SQLLEN dataLength);

		/**
		 \~korean
		 설정된 데이터의 최대 크기를 얻습니다.

		 \~english
		 Obtain the maximum size of data. 

		 \~chinese
		 获取已设定数据的最大值。

		 \~japanese
		 設定されたデータの最大サイズを取得します。

		 \~
		 */
		SQLULEN GetMaxLength();

	private:
		// 복사 금지
		COdbcByteData(const COdbcByteData& other);
		COdbcByteData& operator=(const COdbcByteData& other);
	};
}

#ifdef _MSC_VER
#pragma pack(pop)
#endif