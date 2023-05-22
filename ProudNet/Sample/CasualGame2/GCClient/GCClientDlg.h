// GCClientDlg.h : Header file
// GCClientDlg.h : 헤더 파일
//

#pragma once

/**
\~korean
CGCClientDlg 대화 상자입니다.

\~english
CGCClientDlg dialog box

\~chinese
CGCClientDlg对话框。

\~japanese
CGCClientDlg チャットボックスです。

\~
*/
class CGCClientDlg : public CDialog
{
/**
\~korean
생성

\~english
Creation

\~chinese
生成

\~japanese
生成

\~
*/
public:

	/**
	\~korean
	표준 생성자입니다.

	\~english
	Default constructor

	\~chinese
	标准生成器

	\~japanese
	標準コンストラクタです。
	\~
	*/
	CGCClientDlg(CWnd* pParent = NULL);

	/**
	\~korean
	대화 상자 데이터

	\~english
	Dialog box data

	\~chinese
	对话框数据

	\~japanese
	チャットボックスのデータ

	\~
	*/
	enum { IDD = IDD_GCCLIENT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원


/**
\~korean
구현

\~english
Realization

\~chinese
实现

\~japanese
実装

\~
*/
protected:
	HICON m_hIcon;

	/**
	\~korean
	메시지 맵 함수를 생성했습니다.

	\~english
	Created message map function.

	\~chinese
	已生成消息映射函数。

	\~japanese
	メッセージマップ関数を作成しました。

	\~
	*/
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);

	/**
	\~korean
	대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
	아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
	프레임워크에서 이 작업을 자동으로 수행합니다.

	\~english
	When you add minimize button at dialog box, you may need following code for drawing icon.
	Framework does this operation automatically for MFC application program that use documation/view model.

	\~chinese
	对话框中增加最小化按钮时，如要画图标
	则需要如下代码。如果是使用文件/查看模式的MFC应用程序
	则在framework中手动完成该操作。

	\~japanese
	チャットボックスに最小化ボタンを追加する場合は、アイコンを描画するため
	次のコードが必要です。ドキュメント/ビューモデルを使うMFCアプリケーションの場合は
	フレームワークでこの作業を自動的に行います。

	\~
	*/
	afx_msg void OnPaint();

	/**
	\~korean
	 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
	 이 함수를 호출합니다.

	\~english
	Call this function to show cursor during user close minimized window.

	\~chinese
	系统调用该函数以保证用户关闭最小化窗口时显示鼠标。

	\~japanese
	ユーザーが最小化されたウィンドウを閉じる間にカーソルを表示するためシステムで
	この関数を呼び出します。

	\~
	*/
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
