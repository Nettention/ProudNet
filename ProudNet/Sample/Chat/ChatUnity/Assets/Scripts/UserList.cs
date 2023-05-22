using UnityEngine;
using System.Collections;
using System.Collections.Generic;

/*
 * UserList 씬.
 * 화면 상단의 Userlist 토글바를 클릭시 이 씬으로 전환되어
 * 서버에 접속중인 클라이언트들을 보여 준다.
 * 클라이언트를 선택하고 Create 버튼을 클릭하면
 * 선택한 클라이언트들과 p2p 그룹이 맺어 진다.
 * */

public class UserList : MonoBehaviour {
	// Client 객체 선언.
	private Client m_client;
	// 툴바에 사용될 int 변수.
	private ChatUnity.ToolBarState m_toolbarState; // 어떤 툴바를 클릭하였는지 판별할 변수.
	private string[] m_toolbarStrings; // 툴바에 들어갈 문자열 배열.
	private bool[] m_toggleBool; // 토클바가 선택되었는지를 확인할 bool 변수.
	private Nettention.Proud.HostIDArray m_hostIDs; // 서버로 전송할 hostID들의 문자열.
	private Vector2 m_scrollPosition; // 스크롤의 위치를 저장할 Vector2 변수.
	
	private float m_height; // 창의 높이를 저장할 float 변수.
	
	// Use this for initialization
	void Start ()
	{
		m_client = Client.Instance; // CClient 를 가져온다.
		
		m_toolbarState = ChatUnity.ToolBarState.UserList; // 툴바 값을 1로 초기화.
		m_toolbarStrings = new string[] {"G_Chat", "UserList", "Room" }; // string 배열 각각 문자로 생성.
		SetToggleVal(); // 토글바를 그리기 전에 사전 셋팅을 한다.

		m_hostIDs = new Nettention.Proud.HostIDArray();

		m_height = m_client.ScreenOneH * 6; // 창 높이 초기화.
	}

	// Update is called once per frame
	void Update ()
	{
		m_height = m_client.UserCount * m_client.ScreenOneH; // 서버에 접속중인 유저의 수만큼 창의 크기를 조절한다.

		if (m_client.IsUserUpdate) // 만약 서버에 연결된 클라이언트의 수가 바뀌엇다면.
		{
			SetToggleVal(); // 토글바 서버에 연결된 클라이언트 만큼 다시 생성.
			m_client.IsUserUpdate = false; // userlist를 갱신하였으니 false로 값을 바꿔준다.
		}
		
		// G_Chat 툴바 클릭시.
		if (m_toolbarState == ChatUnity.ToolBarState.GlobalChat)
		{
			Application.LoadLevel("globalChat"); // globalChat 씬으로 전환.
		}
		else if (m_toolbarState == ChatUnity.ToolBarState.P2PChatRoom) // Room 툴바 클릭시.
		{
			Application.LoadLevel("P2PChatRoom"); // P2PChatRoom 씬으로 전환.
		}
	}

	void OnGUI ()
	{
		// 화면 상단 툴바를 그린다.
		m_toolbarState = (ChatUnity.ToolBarState)GUI.Toolbar(new Rect(m_client.ScreenWidth / 2 - m_client.ScreenOneW * 2, m_client.ScreenOneH, m_client.ScreenOneW * 4, m_client.ScreenOneH), (int)m_toolbarState, m_toolbarStrings);
		
		// 채팅창 스크롤 바를 그린다.
		m_scrollPosition = GUI.BeginScrollView(new Rect(m_client.ScreenWidth / 2 - m_client.ScreenOneW * 2, m_client.ScreenOneH * 2, m_client.ScreenOneW * 4, m_client.ScreenOneH * 6), m_scrollPosition, new Rect(m_client.ScreenWidth / 2 - m_client.ScreenOneW * 2, m_client.ScreenOneH * 2, m_client.ScreenOneW * 4, m_height), GUIStyle.none, GUI.skin.verticalScrollbar);
		
		int i = 0;
		// 서버에 접속중인 클라이언트들의 정보를 저장중인 dictionary 에서 아이템을 하나씩 빼서 토글바를 그린다.
		foreach (KeyValuePair<Nettention.Proud.HostID, string> pair in m_client.ChatUserInfo)
		{
			m_toggleBool[i] = GUI.Toggle(new Rect(m_client.ScreenWidth / 2 - m_client.ScreenOneW * 2, m_client.ScreenOneH + m_client.ScreenOneH * (i + 1), m_client.ScreenOneW, m_client.ScreenOneH), m_toggleBool[i], pair.Value);
			i++;
		}

		// 스크롤 바를 그릴 영역이 끝났음을 의미.
		GUI.EndScrollView();

		// Create 버튼을 그리고 버튼클릭시 선택한 클라이언트 들의 HostID를 서버로 전송한다. 서버는 전송된 HostID를 가지고 p2p 그룹을 맺는다.
		if (GUI.Button(new Rect(m_client.ScreenWidth / 2 + m_client.ScreenOneW , m_client.ScreenOneH * 8, m_client.ScreenOneW, m_client.ScreenOneH),"Create"))
		{
			m_hostIDs.Clear();
			m_hostIDs.Add(m_client.GetLocalHostID());
			i = 0; // int 형 변수 i 초기화.

			// 토글바에서 선택된 클라이언트들의 HostID 를 HostIDArray 에 넣는다.
			foreach (KeyValuePair<Nettention.Proud.HostID, string> pair in m_client.ChatUserInfo)
			{
				if (m_toggleBool[i] == true)
				{
					m_hostIDs.Add(pair.Key);
				}
				i++;
			}

			if (m_hostIDs.Count != 0)
			{

				m_client.RequestP2PGroup(Nettention.Proud.RmiContext.ReliableSend, m_hostIDs);
			}

			Application.LoadLevel("P2PChatRoom"); // P2PChatRoom 씬으로 전환.
		}
	}
	
	// m_toggleBool을 동적으로 접속중인 클라이언트의 수만큼 생성.
	public void SetToggleVal()
	{
		m_client.SetUserCount(); // m_client 에서 서버에 연결중인 유저의 수를 구한다.
		m_toggleBool = new bool[m_client.UserCount]; // 서버에 연결중인 유저의 수만큼 배열을 생성한다.
	}

}

