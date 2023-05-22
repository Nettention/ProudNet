using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class P2PChatRoom : MonoBehaviour {
	
	private Client m_client; // Client 객체 선언.
	private ChatUnity.ToolBarState m_toolbarState; // 어떤 툴바를 클릭하였는지 판별할 변수.
	private string[] m_toolbarStrings; // 툴바의 버튼들에 나타날 문자열을 저장할 string 배열.
	private float m_height; // 채팅창의 높이를 결정할 float 변수.
	private Vector2 m_scrollPosition; // 스크롤의 위치를 나타낼 vector2 변수.
	
	// Use this for initialization
	void Start () {
		m_client = Client.Instance; // 생성된 CClient 를 가져온다.
		
		m_toolbarState = ChatUnity.ToolBarState.P2PChatRoom; // m_toolbarState 를 P2PChatRoom 으로 초기화 한다.
		m_toolbarStrings = new string[] {"G_Chat", "UserList", "Room" }; // 툴바의 버튼에 나타낼 문자열을 각각 넣어서 string 배열을 생성한다.
		m_height = m_client.ScreenOneH * 6; // 채팅창의 높이를 해당 크기로 초기화.
	}
	
	// Update is called once per frame
	void Update () {
		// p2p 그룹의 수를 이용해 화면에 뿌려줄 높이를 계산한다.
		m_height = (m_client.ChatRooms.Count - 1) * m_client.ScreenOneH;
		
		/*
		 * 0 == G_Chat
		 * 1 == UserList
		 */
		// G_Chat 툴바 클릭시 씬전환.
		if (m_toolbarState == ChatUnity.ToolBarState.GlobalChat) // G_Chat 클릭시.
		{
			Application.LoadLevel("globalChat"); // globalChat 씬으로 씬전환.
		}
		else if (m_toolbarState == ChatUnity.ToolBarState.UserList) // UserList 툴바 클릭시.
		{
			Application.LoadLevel("userList"); // userList 씬으로 씬전환.
			
		}
	}
		
	void OnGUI()
	{

		// 화면 상단 툴바를 그린다.
		m_toolbarState = (ChatUnity.ToolBarState)GUI.Toolbar(new Rect(m_client.ScreenWidth / 2 - m_client.ScreenOneW * 2, m_client.ScreenOneH, m_client.ScreenOneW * 4, m_client.ScreenOneH), (int)m_toolbarState, m_toolbarStrings);
		
		// 스크롤바.
		m_scrollPosition = GUI.BeginScrollView(new Rect(m_client.ScreenWidth / 2 - m_client.ScreenOneW * 2, m_client.ScreenOneH * 2, m_client.ScreenOneW * 4, m_client.ScreenOneH * 6), m_scrollPosition, new Rect(m_client.ScreenWidth / 2 - m_client.ScreenOneW * 2, m_client.ScreenOneH * 2, m_client.ScreenOneW * 4, m_height), GUIStyle.none, GUI.skin.verticalScrollbar);
		int i=0;
		// 현재 클라이언트가 연결 중인 p2p그룹의 갯수만큼 버튼을 그린다. 클릭시 씬 전환. 
        foreach (KeyValuePair<Nettention.Proud.HostID, Client.ChatRoomInfo> pair in m_client.ChatRooms) // dictionary 에 있는 아이템을 하나씩 빼온다.
		{
            if (pair.Key != Nettention.Proud.HostID.HostID_Server) // 키가 Server 가 아니면. ChatRooms 에는 server hostID, P2P그룹들의 hostID 가 있다. serverID 는 glovalChat에서 쓰고 있으니 필요가 없다..
			{
				// 버튼을 그린다. 
				if (GUI.Button(new Rect(m_client.ScreenWidth / 2 - m_client.ScreenOneW * 2, m_client.ScreenOneH * (i + 1), m_client.ScreenOneW * 4, m_client.ScreenOneH), "GroupHostID:" + pair.Key.ToString()))
				{// 버튼 클릭시.
					// P2PChat 에서 채팅창에 뿌려줄 그룹 의 아이디를 셋팅.
					m_client.P2PChatGroupID = pair.Key;
					Application.LoadLevel("P2PChat"); // 씬 p2pChat으로 전환.
				}
			}
			i++;
		}
		// 스크롤바 표현 영역 끝.
		GUI.EndScrollView();

	}
}
