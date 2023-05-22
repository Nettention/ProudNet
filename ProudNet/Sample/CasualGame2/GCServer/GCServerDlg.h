// GCServerDlg.h : Header file
//

#pragma once

#include "resource.h"
#include "FarmCommon.h"
#include "ServerBase.h"
#include "afxcmn.h"
#include "afxwin.h"
#define MESSAGE_LOG WM_USER+10 
const int lineLength = 50;

enum
{
	WM_SET_TITLE = WM_USER + 1,	// Parameter: String object, server object pointer
	WM_SET_STATUS,				// Parameter: String object, server object pointer
	WM_REMOVE_ITEM,				// Parameter: Server object pointer
	WM_LOG_ERROR,				// Parameter: Name of string, server object
	WM_LOG_EVENT,				// Parameter: Name of string, server object
};

/*
enum
{
	WM_SET_TITLE = WM_USER + 1,	// 파라메터: 문자열 객체,서버 객체 포인터
	WM_SET_STATUS,				// 파라메터: 문자열 객체,서버 객체 포인터
	WM_REMOVE_ITEM,				// 파라메터: 서버 객체 포인터
	WM_LOG_ERROR,				// 파라메터: 문자열,서버 객체 이름
	WM_LOG_EVENT,				// 파라메터: 문자열,서버 객체 이름
};
*/

struct UiAccessParameter
{
	String m_text;
	String m_serverName;
	CServerBase* m_serverObj;
};

/**
\~korean
서버 프로그램 메인 대화 상자입니다.
- 본 샘플 프로그램은 학습을 목적으로 UI를 가진 프로그램이지만, 상업용 목적의 게임 서버는
콘솔 프로그램으로 제작되는 것이 권장됩니다.

\~english
Main dialog box of server program
- This sample program have UI for tutorial purpose, we recommend develop with console program for commercial purpose game server.

\~chinese
服务器程序的主对话框。
-本示例程序以学习为目的提供UI，但商业游戏服务器建议使用Console程序制作。

\~japanese
サーバプログラムのメインチャットボックスです。
- 本サンプルプログラムは学習目的としてUIを持つプログラムであるが、商業目的のゲームサーバは
コンソールアプリケーションとして製作するのをお勧めします。

\~
*/
class CGCServerDlg : public CDialog
{
// Creation
// 생성

public:
	CGCServerDlg(CWnd* pParent = NULL);	// Standard creator

// Dialog box data
// 대화 상자 데이터
	enum { IDD = IDD_GCSERVER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// Support DDX/DDV


// Realization
protected:
	HICON m_hIcon;

	// Created message map function.
	// 메시지 맵 함수를 생성했습니다.
	virtual BOOL OnInitDialog();

	/**
	\~korean
	이 메서드 호출 전에 이미 분석된 실행 옵션에 따라 서버 역할을 시작합니다.

	\~english
	Starts server role that depends on excution option already analysed before calling this method.

	\~chinese
	调用该method之前根据已分析的执行选项开始服务器的工作。

	\~japanese
	このメソッドを呼び出す前に既に分析された実行オプションに応じてサーバの役割を開始します。
	\~
	*/
	void AddServerByPredefinedSettings();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:

	/**
	\~korean
	이 서버 프로세스가 어떤 역할을 할 것인지를 결정

	\~english
	Decide about role of this server process

	\~chinese
	决定该服务器进程的职责

	\~japanese
	このサーバのプロセスがどのような役割をするかを決定

	\~
	*/
	ServerMode m_launchMode;

	/**
	\~korean
	 이 서버 프로세스가 로비 서버인 경우 로비 서버가 가질 이름을 지칭
	 - 이 서버 프로세스가 배틀 서버인 경우 어떤 로비 서버에 종속된 것인지 지칭

	\~english
	 Named lobby server if this server process is lobby server
	 - If this server process is battle server then choose which lobby server is subordinated with it

	\~chinese
	该服务器进程为lobby服务器时的lobby服务器的名称
	-该服务器进程为作战服务器时所属的lobby服务器

	\~japanese
	このサーバのプロセスがロビーサーバである場合、ロビーサーバが持つ名前を指す。
	- このサーバのプロセスがバトルサーバである場合、どのロビーサーバに属しているかを指す

	\~
	*/
	String m_launchLobbyName;

	Proud::CFastArray<CServerBasePtr> m_serverList;

	bool m_debugging;

	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR);

	CriticalSection m_cs;

	/**
	\~korean
	실행 파라메터를 입력받아 서버 역할을 시작시킵니다.

	\~english
	Starts role of server by receive excution parameter.

	\~chinese
	根据输入的执行变量开始服务器的工作。

	\~japanese
	実行パラメータの入力を受け取り、サーバの役割を開始させます。

	\~
	*/
	void AddServer(LPCWSTR param);

	/**
	\~korean
	실행 파라메터를 근거로 이 프로그램이 어떤 역할을 하는 서버가 되어야 할지를 결정합니다.

	\~english
	This program decide role of server that based on excution parameter.

	\~chinese
	根据执行变量决定该程序服务器的职责。

	\~japanese
	実行パラメータに基づいてこのプログラムがどのような役割をするサーバであるかを決定します。

	\~
	*/
	void GetLaunchSettingsByCommandLine( String cmdParam);
	CListCtrl m_serverListCtl;
	
	int FindOrAddListViewIndex(CServerBase* srv);
	CEdit m_logEditCtl;


protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void OnOK();
	virtual void OnCancel();

	/**
	\~korean
	서버를 모두 중지시킵니다.

	\~english
	Stop all servers.

	\~chinese
	停止所有服务器。

	\~japanese
	すべてのサーバを停止させます。

	\~
	*/
	void StopServers();
	
};
