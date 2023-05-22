using UnityEngine;
using System.Collections;
/*
 * globalChat 씬에서 구동되는 스크립트.
 * 서버에 연결된 모든 클라이언트들이 이 화면을 통해 채팅 할 수 있다.
 * */
public class GlobalChat : MonoBehaviour {
	// Client 객체 선언.
	private Client m_client;
	
	//GUI 에서 사용할 변수들.
	private string m_inputString; // 채팅 메시지를 입력받을 텍스트필에 쓰일 스트링 변수.
	private ChatUnity.ToolBarState m_toolbarState; // 어떤 툴바를 클릭하였는지 판별할 변수.
	private string[] m_toolbarStrings; // 툴바 의 각 버튼에 들어갈 문자열을 저장할 string 배열.
	
	// 채팅창 크기를 조절하기 위해서.
	private float m_height; // 채팅창 세로의 길이를 저장할 float 변수.

	private static float m_scrollHeight; // 채팅창의 실제 높이.
	private static Vector2 m_scrollPosition; // 스크롤의 위치를 나타낼 벡터2 변수.
	
	// Use this for initialization
	void Start () {
		m_client = Client.Instance; // Client 객체를 가져온다.
		
		m_scrollPosition = Vector2.zero; // 벡터 변수를 0,0 으로 초기화.
		m_inputString = ""; // string 변수를 "" 로 초기화.
		m_toolbarState = ChatUnity.ToolBarState.GlobalChat;
		m_toolbarStrings = new string[] {"G_Chat", "UserList", "Room" }; // string 배열 공간을 만들어 주면서 각 문자열 셋팅.
		
		m_height = m_client.ScreenOneH * 6; // 채팅창 높이 초기화.

		m_scrollHeight = 0;
	}
	
	// Update is called once per frame
	void Awake(){
		//Debug.LogWarning("Awake");
		//DontDestroyOnLoad(this);
	}
	
	void Update () {
		// Userlist 토글바 클릭시 씬 전환.
		/*
		 * 0 == G_Chat
		 * 1 == UserList
		 * 2 == P2PChatRoom
		 */
		if (m_toolbarState == ChatUnity.ToolBarState.UserList) // 툴바에 UserList 클릭시.
		{
			Application.LoadLevel("userList"); // userList 씬 으로 전환.
		}
		else if (m_toolbarState == ChatUnity.ToolBarState.P2PChatRoom) // 툴바에 Room 클릭시.
		{
			Application.LoadLevel("P2PChatRoom"); // P2PChatRoom 씬 으로 전환.
		}
	}
	
	void OnGUI ()
	{
		/*
		 * GUI.skin.textArea.Calcheight() 함수는 입력한 문자와 너비를 이용해 얼마의 높이가 필요한지 리턴해 준다.
		 * 이 함수를 이용해 채팅창의 내용이 채워지면 채팅창의 크기를 계속 증가 시켜준다.
		 * */

		//h = m_client.ChatRooms[Nettention.Proud.HostID.Server].Count * 20;

		// 화면 상단 툴바 그리기.
		m_toolbarState = (ChatUnity.ToolBarState)GUI.Toolbar(new Rect(m_client.ScreenWidth /2 - m_client.ScreenOneW * 2, m_client.ScreenOneH, m_client.ScreenOneW * 4, m_client.ScreenOneH ), (int)m_toolbarState, m_toolbarStrings);

		// Box 를 그린다. 채팅창.
		GUI.Box(new Rect(m_client.ScreenWidth / 2 - m_client.ScreenOneW * 2, m_client.ScreenOneH  * 2, m_client.ScreenOneW  * 4, m_height), "");

		m_scrollHeight = 0;

		// 채팅 내용이 얼마만큼의 길이로 표시가 되어야 하는지 계산.
        foreach (string str in m_client.ChatRooms[Nettention.Proud.HostID.HostID_Server].ChatString)
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
        foreach (string str in m_client.ChatRooms[Nettention.Proud.HostID.HostID_Server].ChatString)
		{
			int labelHeight = (int)GUI.skin.label.CalcHeight(new GUIContent(str), (float)(m_client.ScreenOneW  * 4));
			GUI.Label(new Rect(m_client.ScreenWidth / 2 - m_client.ScreenOneW * 2, m_client.ScreenOneH  * 2 + h, m_client.ScreenOneW  * 4, labelHeight), str);
			h = h + labelHeight;
		}

		// 스크롤바가 그려질 영역이 끝낫음을 의미.
		GUI.EndScrollView();
		
		// 다음 GUI 컨트롤의 이름은 Input.
		GUI.SetNextControlName("Input");
		// TextField 를 그린다. 채팅 내용을 입력 하는 부분.
		m_inputString = GUI.TextField(new Rect(m_client.ScreenWidth / 2 - m_client.ScreenOneW * 2, m_client.ScreenOneH * 8, m_client.ScreenOneW * 3, m_client.ScreenOneH), m_inputString);
		
		// send 버튼을 그린다. 버튼 클릭시 메시지 전송.
		if (GUI.Button(new Rect(m_client.ScreenWidth / 2 + m_client.ScreenOneW, m_client.ScreenOneH * 8, m_client.ScreenOneW, m_client.ScreenOneH), "Send"))
		{
			SendMsg(); // SendMsg() 호출.
		}
		
		// 포커스가 Input 이고 엔터 키가 눌려졌을 떄 메시지 전송.
		if (Event.current.type == EventType.KeyDown && Event.current.character=='\n' && m_inputString.Length > 0 && GUI.GetNameOfFocusedControl() == "Input")
		{
			SendMsg(); // SendMsg() 호출
		}
	}
	
	// m_inputString 에 있는 문자열을 서버로 전송한다.
	void SendMsg()
	{
		// 서버로 채팅 메시지를 전송한다.
		m_client.C2SChat(Nettention.Proud.RmiContext.ReliableSend, m_inputString);
		// 채팅내용을 dictionary 에 추가한다.
		//m_client.ChatRooms[Nettention.Proud.HostID.Server].ChatString.Add(m_inputString);
		// 입력 받는 창의 내용을 지운다.
		m_inputString = "";
	}

	// 스크롤 위치 셋팅.
	public static void ResetScrollPosition()
	{
		m_scrollPosition.y = m_scrollHeight;
	}
}
