using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;

/*
 * c# 네이밍 규칙.
 * private 멤버변수는 m_를 붙인다.
 * private 멤버변수의 property 는 m_를 빼고 첫글자를 대문자로.
 * public 멤버 변수는 대문자로 시작.
 * 지역 변수는 그냥 _안붙이고 소문자로 시작 ..
 * 
 * */

/*
 * 클라이언트에서 내부적으로 동작 하는것은 모두 이 클래스에서 수행된다.
 * 이 클래스의 데이터는 모든 씬에서 공유를 하기 위해서 싱글톤 패턴으로 구성하였다.
 * 
 * */

public class Client : MonoBehaviour {


	public class ChatRoomInfo
	{

		public System.Collections.Generic.List<Nettention.Proud.HostID> HostIDs;
		public System.Collections.Generic.List<string> ChatString;

		public ChatRoomInfo()
		{
			HostIDs = new System.Collections.Generic.List<Nettention.Proud.HostID>();
			ChatString = new System.Collections.Generic.List<string>();
		}
	}
	

	
	// 싱글톤 패턴 사용을 위해 Client 객체 선언.
	private static Client m_instance;
	
	// 프라우드넷 엔진 을 사용하기 위해 선언한 변수들.
	// 프라우드넷 클라이언트 객체.
	private Nettention.Proud.NetClient m_netClient;
	
	// 서버로 RMI 메시지 전송을 위해 C2S proxy 객체 선언.
	private ChatC2S.Proxy m_c2sProxy;
	// 클라이언트간 P2P 통신시 RMI 메시지를 보내기 위해 C2C Proxy 객체 선언.
	private ChatC2C.Proxy m_c2cProxy;
	
	// 서버로 부터 RMI 메시지를 받기 위해 S2C Stub 객체 선언.
	private ChatS2C.Stub m_s2cStub;
	// 클라이언트간 P2P 통신시 RMI 메시지를 받기 위해 C2C Stub 객체 선언.
	private ChatC2C.Stub m_c2cStub;
	
	// 서버 접속 정보를 담기 위해.
	private Nettention.Proud.NetConnectionParam m_param;
	// 서버에 연결된 유저의 수를 저장힐 int형 변수.
	private int m_userCount;
	
	// userName과 serverIP 주소 저장할 변수.
	private string m_userName;
	private string m_serverIP;
	
	// 서버 연결이 잘 되었는지 확인 하는 bool변수.
	private bool m_isConnect;
	// 서버 연결 요청후 기다리는 상태인지를 체크하는 bool 변수.
	private bool m_isWait;
	// 서버에 연결된 클라이언트가 변화 하였는지 체크하는 bool 변수.
	private bool m_isUserUpdate;
	
	private int m_screenWidth; // 화면 가로의 길이
	private int m_screenHeight; // 화면 세로의 길이
	private int m_screenOneW; // 화면 가로의 길이를 일정 값으로 나눈 값.
	private int m_screenOneH;// 화면 세로의 길이를 일정 값으로 나눈 값.
	
	private Nettention.Proud.HostID m_p2pChatGroupID; // P2PChat 에서 채팅창에 뿌려줄 그룹의 아이디

	// 클라이언트 호스트 아이디(클라이언트),닉네임.
	public Dictionary<Nettention.Proud.HostID,string> ChatUserInfo;
	// 클라이언트 호스트 아이디(서버 or 그룹),채팅내용.
	public Dictionary<Nettention.Proud.HostID,ChatRoomInfo> ChatRooms;
	
	// 싱글턴 패턴 사용을 위한 속성.
	public static Client Instance
	{
		get
		{
			if (m_instance == null) // m_instance 가 널 이라면.
			{	// Find any object of one class or script name using Object.FindObjectsOfType or find the first object of one type using Object.FindObjectOfType. 
				m_instance = FindObjectOfType(typeof(Client)) as Client; // class 나 script의 이름으로 object를 찾거나 한 타입으로 첫번째 object를 찾아 CClient 객체 타입으로 캐스팅..
				if (m_instance == null) // 생성된 오브젝트가 없다면.
				{	// 새로운 GameObject 의 이름을 CClient, 포함할 component에 CClient 스크립트를 넣어 생성하고 GetComponent를 이용해 스크립트를 가져온다.
					m_instance = new GameObject("Client",typeof(Client)).GetComponent<Client>(); // 새로 만들어 준다.
				}
			}
			return m_instance; // 
		}
	}
	// m_screenWidth에 대한 속성. 
	public int ScreenWidth
	{
		get
		{
			return m_screenWidth; // m_screenWidth 를 리턴한다.
		}
	}
	//m_screenHeight 에 대한 속성.
	public int ScreenHeight
	{
		get
		{
			return m_screenHeight; // m_screenHeight 를 리턴한다.
		}
	}
	// m_screenOneW 에 대한 속성.
	public int ScreenOneW
	{
		get
		{
			return m_screenOneW; // m_screenOneW 를 리턴한다.
		}
	}
	// m_screenOneH 에 대한 속성.
	public int ScreenOneH
	{
		get
		{
			return m_screenOneH; // m_screenOneH 를 리턴한다.
		}
	}
	// 닉네임에 대한 속성.
	public string userName
	{
		get
		{
			return m_userName; // 닉네임을 리턴한다.
		}
		set
		{
			m_userName = value; // 닉네임에 입력된 값을 넣는다.
		}
	}
	// 서버 아이피에 대한 속성.
	public string ServerIP
	{
		get
		{
			return m_serverIP; // 서버아이피를 리턴한다.
		}
		set
		{
			m_serverIP = value; // 서버아이피에 값을 넣는다.
		}
	}
	// m_isConnect 에 대한 속성.
	public bool IsConnect 
	{
		get // 리턴 
		{
			return m_isConnect; // m_isConnect 를 리턴한다.
		}
		set
		{
			m_isConnect = value; // m_isConnect 에 해당하는 값을 넣는다.
		}
	}
	// m_UserCount 에 대한 속성.
	public int UserCount
	{
		get
		{
			return m_userCount; // m_usercount 를 리턴한다.
		}
	}
	// P2PChat 에서 뿌려줄 그룹의 아이디를 결정하는 m_p2pChatGroupID 에 대한 속성.
	public Nettention.Proud.HostID P2PChatGroupID
	{
		get
		{
			return m_p2pChatGroupID; // m_p2pChatGroupID 를 리턴한다.
		}
		set
		{
			m_p2pChatGroupID = value; // m_p2pChatGroupID 에 값을 넣는다.
		}
	}
	// 서버 연결 요청후 기다리는 상태 인지체크하는 m_isWait 에 대한 속성.
	public bool IsWait
	{
		get
		{
			return m_isWait; // m_isWait을 리턴해준다.
		}
		set
		{
			m_isWait = value; // m_isWait에 값을 넣늗나.
		}
	}
	// 서버에 연결된 클라이언트 에 대한 정보가 변동이 있는지 없는지 체크 한다.
	public bool IsUserUpdate
	{
		get
		{
			return m_isUserUpdate;
		}
		set
		{
			m_isUserUpdate = value;
		}
	}
	
	
	void Awake() {
		DontDestroyOnLoad(this); // 씬이 변경되어도 파괴되지 않게 하기 위해서.
	}
	
	// Use this for initialization
	void Start () {
		// 클래스 생성.
		m_netClient = new Nettention.Proud.NetClient(); // NetClient 클래스 생성.
		m_c2sProxy = new ChatC2S.Proxy(); // Proxy 클래스 생성(클라에서 서버로 메시지 전송).
		m_c2cProxy = new ChatC2C.Proxy(); // Proxy 클래스 생성(클라에서 클라로 메시지 전송).
		m_s2cStub = new ChatS2C.Stub(); // Stub 클래스 생성(서버에서 클라로 메시지 수신).
		m_c2cStub = new ChatC2C.Stub(); // Stub 클래스 생성(클라에서 클라로 메시지 수신).
		
		// NetClient 이벤트 딜리게이트에 함수 셋팅.
		m_netClient.JoinServerCompleteHandler = OnJoinServerComplete; // 서버로 연결요청을 한 결과가 도창하면 콜백 되는 함수를 넣어준다.
		m_netClient.LeaveServerHandler = OnLeaveServer; // 서버와 연결이 해제되면 콜백 되는 함수를 넣어준다.
		m_netClient.ErrorHandler = OnError; // 내부 에러 발생시 콜백 되는 함수를 넣어준다.
		m_netClient.WarningHandler = OnWarning; // 내부 워닝 발생시 콜백 되는 함수를 넣어준다.
		m_netClient.InformationHandler = OnInformation; // 내부 알림 발생시 콜백 되는 함수를 넣어준다.
		m_netClient.ExceptionHandler = OnException; // 내부 익셉션 발생 시 콜백 되는 함수를 넣어 준다.
		m_netClient.P2PMemberJoinHandler = OnP2PMemberJoin; // 해당 클라이언트와 p2p 그룹을 맺게 되면 콜백 되는 함수를 넣어준다.
		m_netClient.P2PMemberLeaveHandler = OnP2PMemberLeave; // 해당 클라이언트와 p2p 그룹이 해지되면 콜백 되는 함수를 넣어 준다.
		
		// s2cStub RMI 함수 딜리게이트에 각 함수 셋팅.
		m_s2cStub.ShowChat = OnShowChat;
		m_s2cStub.UserList_Add = UserList_Add;
		m_s2cStub.UserList_Remove = UserList_Remove;
		m_c2cStub.P2P_Chat = OnC2CChat;

		// 클라이언트에 Proxy 와 Stub 들을 붙인다.
		m_netClient.AttachStub(m_s2cStub);
		m_netClient.AttachProxy(m_c2sProxy);
		m_netClient.AttachProxy(m_c2cProxy);
		m_netClient.AttachStub(m_c2cStub);
		
		// 접속 하기 위한 서버의 정보를 셋팅하기 위해 NetConnectionParam 클래스 생성.
		m_param = new Nettention.Proud.NetConnectionParam();

		// 프로토콜 버전 셋팅.
        m_param.protocolVersion = new Nettention.Proud.Guid();
        m_param.protocolVersion.Set(ChatUnity.Vars.Version);
		m_param.serverPort = (ushort)ChatUnity.Vars.ServerPort; // 포트번호 셋팅.

		m_screenWidth = Screen.width; // 현재 화면의 가로 값 셋팅.
		m_screenHeight = Screen.height; // 현재 화면의 세로 값 셋팅.
		m_screenOneW = m_screenWidth/5; // 현재 화면의 가로를 5로 나눈값으로 셋팅.
		m_screenOneH = m_screenHeight/10; // 현재 화면의 세로를 10으로 나눈 값으로 셋팅.
		
		ChatUserInfo = new Dictionary<Nettention.Proud.HostID, string>(); // HostID 를 키로 UserName 을 값으로 가지는 dictionary 메모리 할당.
		ChatRooms = new Dictionary<Nettention.Proud.HostID,ChatRoomInfo>(); // P2P ChatRoom 에 대한 메모리 할당.

		m_userName = ""; // m_userName 초기화.
		m_serverIP = ""; // m_serverIP 초기화.
		
		m_isConnect = false; // m_isConnect false 로 초기화.
		m_isWait = false; // m_isWait false 로 초기화.
	}
	
	// Update is called once per frame
	void Update () {
		// 서버 연결을 시도 하였거나 연결 되었을때 Framemove()를 호출한다.
		if (m_isWait || m_isConnect)
		{ 
			// 서버로 부터 수신된 RMI 나 delegate의 콜백 이벤트 처리.
			m_netClient.FrameMove();
		}
		
		/* 
		 * 안드로이드 상에서 뒤로가기 버튼이 먹히지 않는다. 
		 * 안드로이드의 뒤로가기 버튼 == KeyCode.Escape
		 * 뒤로가기 버튼 클릭이 어플리케이션이 종료가 되도록.
		 * */
		if (Input.GetKeyDown(KeyCode.Escape)) // 키가 입력이 되었는데 입력된 키의 KeyCode 가 Escape 라면.(안드로이드 상에서 뒤로가기 버튼)
		{
			Application.Quit (); // 어플리케이션 종료.
		}
	}
	
	//public string eType;

    void OnGUI () {
		/*
		 * GUI label, textField, button, toggle 의 폰트 사이즈를 해상도에 맞춰서 설정 해준다.
		 * */
#if UNITY_ANDROID || UNITY_IPHONE
		GUI.skin.label.fontSize = 30;
		GUI.skin.textField.fontSize = 30;
		GUI.skin.button.fontSize = 30;
		GUI.skin.toggle.fontSize = 30;
#endif
	}
	
	// 프로그램 종료될때 서버와 연결 해제.
	void OnDestroy() {
		m_netClient.Disconnect(); // 서버와 연결을 해제 한다.
	}

	public void C2SChat (Nettention.Proud.RmiContext rmiContext, string m_inputString)
	{
		this.m_c2sProxy.Chat(Nettention.Proud.HostID.HostID_Server, rmiContext, m_inputString);
	}
	
	public void RequestP2PGroup (Nettention.Proud.RmiContext rmiContext, Nettention.Proud.HostIDArray m_hostIDs)
	{
        this.m_c2sProxy.RequestP2PGroup(Nettention.Proud.HostID.HostID_Server, rmiContext, m_hostIDs);
	}
	
	public void RequestLeaveP2PGroup (Nettention.Proud.RmiContext rmiContext, Nettention.Proud.HostID p2PChatGroupID)
	{
        this.m_c2sProxy.RequestLeaveP2PGroup(Nettention.Proud.HostID.HostID_Server, rmiContext, p2PChatGroupID);
	}
	
	public void C2CChat (Nettention.Proud.HostID remote,Nettention.Proud.RmiContext rmiContext, Nettention.Proud.HostID p2pGroupID, string text)
	{
		this.m_c2cProxy.P2P_Chat(remote, rmiContext, p2pGroupID, text);
	}
	
	public Nettention.Proud.HostID GetLocalHostID()
	{
		return m_netClient.LocalHostID;
	}
	
	//서버의 IP 주소를 셋팅하고 접속을 시도한다.
	public void Connect()
	{
#if UNITY_WEBPLAYER
		Security.PrefetchSocketPolicy(m_serverIP, (int)m_param.serverPort);
#endif
		m_param.serverIP = m_serverIP; // 접속할 서버의 IP 주소 셋팅.
		m_netClient.Connect(m_param); // 접속할 서버의 정보를 가지고 있는 m_param 변수를 메개변수로 Connect 함수를 호출해 서버로 접속 요청.
	}
	
	// 서버 연결을 시도한 결과를 콜백 받아 수행.
	void OnJoinServerComplete(Nettention.Proud.ErrorInfo info, Nettention.Proud.ByteArray replyFromServer)
	{
		// 성공적으로 연결 되면.
		if (Nettention.Proud.ErrorType.Ok == info.errorType)
		{

            ChatRooms[Nettention.Proud.HostID.HostID_Server] = new ChatRoomInfo();

			// 사용자의 닉네임을 서버로 전송.
            m_c2sProxy.RequestLogon(Nettention.Proud.HostID.HostID_Server, Nettention.Proud.RmiContext.ReliableSend, m_userName);

			m_isConnect = true; // bool 변수 값 true 로 변경.
		}
		else
		{
			// 에러처리.
			Debug.Log("Server connection failed.");
			Debug.Log(info.ToString());
		}
	}
	
	// 서버와 연결이 해제 되면 콜백.
	void OnLeaveServer(Nettention.Proud.ErrorInfo info)
	{
		if (Nettention.Proud.ErrorType.Ok!= info.errorType)
		{
			// 에러 
			Debug.Log("OnLeaveServer. " + info.ToString());
		}

		// 서버와의 연결이 종료되었으니 초기화면으로 나가거나. 다른처리를 해주어야.
		m_isConnect = false;

		Application.LoadLevel("LogOn");
	}
	
	// p2p 그룹을 맺게 되었을때 콜백 되는 함수.
	void OnP2PMemberJoin(Nettention.Proud.HostID memberHostID, Nettention.Proud.HostID groupHostID, int memberCount, Nettention.Proud.ByteArray customField)
	{
		if (ChatRooms.ContainsKey(groupHostID) == false) // P2P 그룹의 HostID 가 dictionary 에 없다면.
		{
			ChatRooms[groupHostID] = new ChatRoomInfo(); // 아이템을 추가해준다. 그룹ID 를 키로. "" 를 값으로.
		}

		ChatRooms[groupHostID].HostIDs.Add(memberHostID);

		// 누가 P2P그룹에 조인 되었는지 알려준다.
		ChatRooms[groupHostID].ChatString.Add("--- Join p2p Group [" + ChatUserInfo[memberHostID] + "] ---");

		P2PChat.ResetScrollPosition();

	}

	// p2p 그룹을 해제 하였을때 콜백 되는 함수.
	void OnP2PMemberLeave(Nettention.Proud.HostID memberHostID, Nettention.Proud.HostID groupHostID, int memberCount)
	{
		// 이 클라이언트가 P2P 그룹을 해제 하였고 P2P그룹의 HostID 가 dictionary 에 키로 남아 있다면.
		if (memberHostID == m_netClient.LocalHostID && ChatRooms.ContainsKey(groupHostID))
		{
			ChatRooms.Remove(groupHostID); // dictionary 에서 제거한다.
		}
		
		if (memberHostID != m_netClient.LocalHostID) // p2p 그룹을 해제한 클라이언트가 이 클라이언트가 아니라면.
		{
			if (ChatRooms.ContainsKey(groupHostID))
			{
				// 타 클라이언트가 P2P 그룹을 떠났음을 채팅창에 추가한다.
				ChatRooms[groupHostID].ChatString.Add("--- leave p2p Group [" + ChatUserInfo[memberHostID] + "] ---");
				P2PChat.ResetScrollPosition();
			}

		}
	}

	// 내부 에러 발생시
	void OnError(Nettention.Proud.ErrorInfo info)
	{
		Debug.Log("Error : " + info.ToString());
	} 

	// 내부 워닝 발생시.
	void OnWarning(Nettention.Proud.ErrorInfo info)
	{
		Debug.Log("Warning : " + info.ToString());
	}

	// 내부 알림 발생시.
	void OnInformation(Nettention.Proud.ErrorInfo info)
	{
		Debug.Log("Information : " + info.ToString());
	}

	// 내부 익셉션 발생시.
	void OnException(Nettention.Proud.HostID remoteID, System.Exception e)
	{
		Debug.Log("Exception : " + e.ToString());
	}

	// 글로벌 채팅 내용을 리시브 받앗을때 처리하는 함수.
	bool OnShowChat(Nettention.Proud.HostID remote, Nettention.Proud.RmiContext rmiContext, string userName, string text)
	{
		string str = "";

		if (userName.CompareTo(m_userName) == 0)
		{
			str = "[self]* ";
		}
		// ChatRooms dictionary 에 Nettention.Proud.HostID.Server를 키로하는 값에 채팅 내용을 추가한다.
        ChatRooms[Nettention.Proud.HostID.HostID_Server].ChatString.Add(str + userName + ":" + text);

		GlobalChat.ResetScrollPosition();

		return true;
	}
	
	// 타 클라이언트들과 p2p 그룹을 맺고 채팅내용을 리시브 받앗을때 처리하는 함수.
	bool OnC2CChat(Nettention.Proud.HostID remote,Nettention.Proud.RmiContext rmiContext, Nettention.Proud.HostID p2pGroupID, string text)
	{
		string relayed = "";

		// 자신이 보낸 메시지인지 P2P 또는 relayed 인지 표시한다.
		if (remote == m_netClient.LocalHostID)
		{
			relayed = "[self]";
		}
		else if (rmiContext.relayed == false)
		{
			relayed = "[p2p]";
		}
		else
		{
			relayed = "[relayed]";
		}

		// 발송자의 p2pGroupID와 비교하여 해당 p2p룸에 채팅을 뿌려준다
		foreach (KeyValuePair<Nettention.Proud.HostID, Client.ChatRoomInfo> pair in ChatRooms)
		{
			if (pair.Key == p2pGroupID)
			{
				pair.Value.ChatString.Add(relayed + ChatUserInfo[remote] + "(" +  remote + ")]:" + text);
				break;
			}
		}

		P2PChat.ResetScrollPosition();


		return true;
	}
	
	// 새로운 클라이언트가 서버에 연결 되었을때 그 클라이언트의 hostID 와 userName 을 받아 저장한다.
	bool UserList_Add(Nettention.Proud.HostID remote, Nettention.Proud.RmiContext rmiContext, string userName, Nettention.Proud.HostID hostID)
	{
		if (hostID == m_netClient.LocalHostID)
		{
			userName = "* " + userName;
		}

		ChatUserInfo[hostID] = userName;

		//ChatUserInfo dictionary 에 hostID를 키로 userName 을 값으로 아이템을 추가한다.
        ChatRooms[Nettention.Proud.HostID.HostID_Server].HostIDs.Add(hostID);

		// 현재 서버에 연결중인 유저의 수를 갱신한다.
		m_userCount = ChatUserInfo.Count;

		// 새로운 클라이언트가 서버에 연결되었으니 userlist를 갱신 할 필요가 있다. true 로 값을 변경해준다.
		m_isUserUpdate = true;

		return true;
	}
	
	// 클라이언트가 서버로 부터 연결 해제 되었을때 그 클라이언트의 hostID를 받아 제거한다.
	bool UserList_Remove(Nettention.Proud.HostID remote, Nettention.Proud.RmiContext reiContext, Nettention.Proud.HostID hostID)
	{
		// ChatUserInfo dictionary 에 hostID를 키로 가지는 아이템을 제거한다.
		ChatUserInfo.Remove(hostID);

		//ChatRooms 에 hostID 를 제거해야 된다.
		foreach (KeyValuePair<Nettention.Proud.HostID, Client.ChatRoomInfo> pair in ChatRooms)
		{
			pair.Value.HostIDs.Remove(hostID);
		}

		// 현재 서버에 연결중인 유저의 수를 갱신한다.
		m_userCount = ChatUserInfo.Count;

		// 타 클라이언트가 서버와의 연결을 해제 하였으니 userlist를 갱신 할 필요가 있다. true 로 값을 변경해 준다.
		m_isUserUpdate = true;
		return true;

	}

	// m_userCount 변수에 ChatUserInfo 의 아이템 수를 넣는다.
	public void SetUserCount()
	{
		m_userCount = ChatUserInfo.Count;
	}
}
