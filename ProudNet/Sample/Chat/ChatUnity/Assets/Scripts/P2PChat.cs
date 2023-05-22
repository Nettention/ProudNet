using UnityEngine;
using System.Collections;

public class P2PChat : MonoBehaviour {
	
	private Client m_client; // Client 객체 생성.

	private string m_inputString; // 채팅 입력창의 문자열을 가져올 string 변수.
	private ChatUnity.P2PChatToolBarState m_toolbarState; // 어떤 툴바를 클릭하였는지 판별할 변수.
	private string[] m_toolbarStrings; // 툴바의 각 버튼에 보여질 택스를 셋팅할 stirng 배열.
	private float m_height; // 채팅창의 디폴트 높이.
	
	private static float m_scrollHeight; // 실제 창 높이.
	private static Vector2 m_scrollPosition; // 스크롤바의 위치.

	// Use this for initialization
	void Start () {
		//Screen.SetResolution(Screen.width, Screen.width/2*3, true);
		m_client = Client.Instance; // 
		
		m_scrollPosition = Vector2.zero; // 스크롤의 초기값을 0,0 으로 정한다.
		m_inputString = ""; // "" 로 초기화.

		m_toolbarState = ChatUnity.P2PChatToolBarState.None;
		m_toolbarStrings = new string[] {"Back", "Leave Room"}; // 각각의 문자열로 배열 생성.
		
		if (m_client.ChatRooms.ContainsKey(m_client.P2PChatGroupID) == false) // 해당 P2P 그룹 번호가 dictionary 에 존재 하지 않는다면.
		{
			m_client.ChatRooms[m_client.P2PChatGroupID] = new Client.ChatRoomInfo(); // dictionary에 그룹아이디를 키로 ""를 값으로 아이템을 넣는다.
		}
		
		m_height = m_client.ScreenOneH * 6; // 채팅창 높이 초기화.

		m_scrollHeight = 0;
	}
	
	// Update is called once per frame
	void Update () {
		/*
		 * 0 == Back
		 * 1 == Leave Room
		 */
		// 툴바 클릭시 해당 씬으로 화면 전환.
		if (m_toolbarState == ChatUnity.P2PChatToolBarState.Back) // Back 툴바 버튼 클릭시.
		{
			Application.LoadLevel("P2PChatRoom"); // P2PChatRoom 씬으로 씬전환.
		}
		else if (m_toolbarState == ChatUnity.P2PChatToolBarState.LeaveRoom) // Leave Room 툴바 버튼 클릭시.
		{
			// p2p그룹에서 나가겠다는 메시지를 서버로 보낸다.
			m_client.RequestLeaveP2PGroup(Nettention.Proud.RmiContext.ReliableSend, m_client.P2PChatGroupID);
			Application.LoadLevel("P2PChatRoom"); // P2PChatRoom 으로 씬 전환.
		}
	}

	void OnGUI ()
	{

		// 화면 상단 툴바 그리기.
		m_toolbarState = (ChatUnity.P2PChatToolBarState)GUI.Toolbar(new Rect(m_client.ScreenWidth / 2 - m_client.ScreenOneW * 2, m_client.ScreenOneH, m_client.ScreenOneW * 4, m_client.ScreenOneH), (int)m_toolbarState, m_toolbarStrings);

		// 채팅창 box로 그리기.
		GUI.Box(new Rect(m_client.ScreenWidth / 2 - m_client.ScreenOneW * 2, m_client.ScreenOneH * 2, m_client.ScreenOneW * 4, m_height), "");

		m_scrollHeight = 0;

		// 채팅 내용이 얼마만큼의 길이로 표시가 되어야 하는지 계산.
		foreach (string str in m_client.ChatRooms[m_client.P2PChatGroupID].ChatString)
		{
			m_scrollHeight = m_scrollHeight + (int)GUI.skin.label.CalcHeight(new GUIContent(str), (float)(m_client.ScreenOneW  * 4));
		}

		if (m_height > m_scrollHeight)
		{
			m_scrollHeight = m_height;
		}

		// 채팅창 스크롤바.
		m_scrollPosition = GUI.BeginScrollView(new Rect(m_client.ScreenWidth / 2 - m_client.ScreenOneW * 2, m_client.ScreenOneH * 2, m_client.ScreenOneW * 4, m_client.ScreenOneH * 6), m_scrollPosition, new Rect(m_client.ScreenWidth / 2 - m_client.ScreenOneW * 2, m_client.ScreenOneH * 2, m_client.ScreenOneW * 4, m_scrollHeight), GUIStyle.none, GUI.skin.verticalScrollbar);

		int h = 0;

		// list 에 저장된 내용을 창에 뿌려준다.
		foreach (string str in m_client.ChatRooms[m_client.P2PChatGroupID].ChatString)
		{
			int labelHeight = (int)GUI.skin.label.CalcHeight(new GUIContent(str), (float)(m_client.ScreenOneW  * 4));
			GUI.Label(new Rect(m_client.ScreenWidth / 2 - m_client.ScreenOneW * 2, m_client.ScreenOneH  * 2 + h, m_client.ScreenOneW  * 4, labelHeight), str);
			h = h + labelHeight;
		}

		GUI.EndScrollView();
		
		// 채팅 내용 입력 받는 창 이름을 Input. 
		GUI.SetNextControlName("Input");
		// 채팅 내용 입력 받는 창. TextField로 그리기.
		m_inputString = GUI.TextField(new Rect(m_client.ScreenWidth / 2 - m_client.ScreenOneW * 2, m_client.ScreenOneH * 8, m_client.ScreenOneW * 3, m_client.ScreenOneH), m_inputString);
		
		// Send 버튼 그리기. 버튼 클릭시 메시지 전송.
		if(GUI.Button(new Rect(m_client.ScreenWidth / 2 + m_client.ScreenOneW, m_client.ScreenOneH * 8, m_client.ScreenOneW, m_client.ScreenOneH), "Send"))
		{
			SendMsg();
		}
		
		// 입력 창에 엔터 누를시 메시지 전송.
		if(Event.current.type == EventType.KeyDown && Event.current.character=='\n' && m_inputString.Length > 0 && GUI.GetNameOfFocusedControl() == "Input")
		{
			SendMsg();
		}
	}

	// p2p 그룹으로 메시지 전송.
	void SendMsg()
	{
		//메시지 전송.
		m_client.C2CChat(m_client.P2PChatGroupID, Nettention.Proud.RmiContext.ReliableSend, m_client.P2PChatGroupID, m_inputString);

		// 입력 받는 창 문자열 초기화.
		m_inputString = "";
	}

	// 스크롤 위치 셋팅.
	public static void ResetScrollPosition()
	{
		m_scrollPosition.y = m_scrollHeight;
	}
	
}
