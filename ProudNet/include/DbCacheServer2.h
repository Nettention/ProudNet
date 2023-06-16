/* 106 228 203 199 203 251 69 164 171 249 223 147 102 233 72 249 25 46 47 89 4 75 196 233 166 154 197 5 225 93 96 137 109 76  */
/*
ProudNet 1.8.00002-master


이 프로그램의 저작권은 넷텐션에게 있습니다.
이 프로그램의 수정, 사용, 배포에 관련된 사항은 본 프로그램의 소유권자와의 계약을 따르며,
계약을 준수하지 않는 경우 원칙적으로 무단 사용을 금지합니다.
무단 사용에 의한 책임은 본 프로그램의 소유권자와의 계약서에 명시되어 있습니다.

** 주의: 저작물에 관한 위의 명시를 제거하지 마십시오.


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

#if defined(_WIN32)
#include "BasicTypes.h"
#include "dbenums.h"
#include "DbLoadedData2.h"

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

namespace Proud
{
#if (defined(_MSC_VER) && _MSC_VER>=1400)
// 아래 주석처리된 pragma managed 전처리 구문은 C++/CLI 버전이 있었을 때에나 필요했던 것입니다.
// 현재는 필요없는 구문이고, 일부 환경에서 C3295 "#pragma managed는 전역 또는 네임스페이스 범위에서만 사용할 수 있습니다."라는 빌드에러를 일으킵니다.
//#pragma managed(push,off)
#endif
	/** \addtogroup db_group
	*  @{
	*/

	/**
	\~korean
	DB cache에 의해 로딩되는 테이블의 이름들입니다.

	root node를 위한 테이블 이름과 child node를 위한 테이블 이름들을 넣으십시오.
	child node를 위한 테이블 이름은 0개 이상이어야 합니다.
	child node를 위한 테이블 이름들 중에는 root node를 위한 테이블 이름과 같은 것이 없어야 합니다.

	\~english

	\~japanese
	DB casheによってロードされるテーブル名です。
	root nodeのためのテーブル名とchild nodeのためのテーブル名などを入れてください。 
	child nodeのためのテーブル名は0個以上ではなければなりません。
	child nodeのためのテーブル名の中には、root nodeのためのテーブル名と同じものがあってはいけません。

	\~
	*/
	class CCachedTableName
	{
	public:
		/** root node를 위한 테이블 이름 */
		String m_rootTableName;
		/** child node를 위한 테이블 이름 */
		CFastArray<Proud::String> m_childTableNames;
	};

	/**
	\~korean
	CDbCacheServer2에서 콜백하는 이벤트나 개발자가 구현해야 하는 메서드를 콜백을 처리하는 delegate 객체

	\~english
	This is delegate object that handle callback event from CDbCacheServer2 or callback method which developer has to realize.

	\~chinese
	在 CDbCacheServer2%回调处理回调event或者开发者要体现的方法的delegate对象。

	\~japanese
	CDbCacheServer2でコールバックするイベントとか開発者が実現しなければならないメソッドをコールバックするdelegateオブジェクト
	\~
	*/
	class IDbCacheServerDelegate2
	{
	public:
		virtual ~IDbCacheServerDelegate2() {}

		/**
		\~korean
		내부 에러 발생시 호출됩니다
		\param errorInfo 에러에 대한 정보입니다. ErrorInfo::ToString()을 이용하시면 간편하게 자세한 문제의 정보를 보실 수 있습니다.

		\~english TODO:translate needed
		It is called when an internal error occurs
		\This is the information on an error. If you use ErrorInfo::ToString(), you can see the detailed information of the problem. 

		\~chinese
		发生内部错误时呼叫。
		\param errorInfo 对错误的信息。利用 ErrorInfo::ToString()%的话可以方便看出详细的信息。

		\~japanese
		内部エラー発生時に呼び出されます。
		\param errorInfo エラーに関する情報です。ErrorInfo::ToString()を利用すれば簡単に詳しく問題情報が見られます。

		\~
		*/
		virtual void OnError(ErrorInfo *errorInfo) = 0;

		/**
		\~korean
		내부 경고 발생시 호출됩니다. 에러보다는 낮은 수위의 문제를 보고함.
		\param errorInfo 에러에 대한 정보입니다. ErrorInfo::ToString()을 이용하시면 간편하게 자세한 문제의 정보를 보실 수 있습니다.

		\~english TODO:translate needed
		It is called when an internal warning occurs. This reports a problem with the level lower than an error. 
		\param errorInfo This is the information on an error. If you use ErrorInfo::ToString(), you can see the detailed information of the problem

		\~chinese
		发生内部警告时呼叫。报告比错误水平低的问题。
		\param errorInfo 对错误的信息。利用 ErrorInfo::ToString()%的话可以方便看出详细的信息。

		\~japanese
		内部警告が発生時に呼び出されます。エラーよりは低いレベルの問題を報告します。
		\param errorInfo エラーに関する情報です。ErrorInfo::ToString()を利用すれば簡単に詳しく問題情報が見られます。

		\~
		*/
		virtual void OnWarning(ErrorInfo *errorInfo) = 0;

		/**
		\~korean
		Proud.Exception으로 통합합니다.
		- 일반적으로 유저 코드에서 나는 에러를 처리합니다.
		- 유저는 필요한 것들을 구현하면 됩니다.
		- ExceptionType으로 해당 Exception 객체를 얻어서 사용하면 됩니다.
		- _com_error, void*, std::exception등을 모두 처리합니다.
		또한 catch(...)를 처리하는 OnUnhandledException()도 있습니다.
		- void*는 예외 객체의 타입을 캐스팅해서 받을 수 있습니다. 가령 MFC CException을 받는데 사용됩니다.

		\~english
		Merge to Proud.Exception
		- It handle error that occur from user code.
		- User realize things that user needed.
		- You can use Exceptiontype that obtain Exception object.
		- It process _com_error, void*, std::exception, etc. Also there is OnUnhandledException() that handle catch(...)
		- void* can obtain exceptional object type by casting. For example, it uses obtaining MFC CException.

		\~chinese
		用 Proud.Exception%统一起来。
		- 一般处理从用户代码发生的错误。
		- 用户体现需要的东西即可。
		- 用ExceptionType获得并使用相关Exception对象即可。
		- 全部处理_com_error, void*, std::exception 等。也有处理catch(...)的OnUnhandledException()。
		- 选择例外对象的类型以后可以接收void*。比如接收MFC CException%的时候使用。

		\~japanese
		Proud.Exceptionに統合します。
		- 一般的にユーザーコードで現れるエラーを処理します。
		- ユーザーは必要なことを実現すれば良いです。
		- ExceptionTypeで該当Exceptionオブジェクトを得て使えば良いです。
		- _com_error, void*, std::exceptionなどを全て処理します。
		また、catch(...)を処理するOnUnhandledException()もあります。
		- void*は例外オブジェクトのタイプをキャスティングして貰えます。仮に、MFC CExceptionを受けることに使われます。

		\~
		*/
		virtual void OnException(const Exception &e) = 0;


		/**
		\~korean
		DB cache server2에서 DBMS에 직접 기록하는 처리는 비동기로 실행된다. 이 메서드는 비동기로 기록하는 처리가
		완료될 때마다 DB cache server의 내부 스레드로부터 콜백됩니다.
		- Visual Studio 2005 이상에서는 override keyword 사용을 권장합니다.
		\param type 완료된 DBMS 기록(PropNode)의 종류 (참고 \ref DbmsWritePropNodePendType)

		\~english
		Direct writing process to DBMS is running asynchronously in DB cache server2. When asynchronous writting process has finished, this method will callback from internal thread in DB cache server.
		- We recommend to use override keyword on Visual Studio 2005 or above.
		\param type Sort of finished DBMS history (PropNode)

		\~chinese
		从DB cache server往DBMS直接记录的处理要以异步执行。每当异步记录的处理完成的时候此方法会从内部线程回调DB cache server。
		- Visual Studio 2005以上建议使用override keyword。
		\param type 完成的DBMS记录（PropNode）的种类（参考 \ref DbmsWritePropNodePendType）。

		\~japanese
		DB cache serve2でDBMSに直接記録する処理は非同期で実行されます。このメソッドは非同期で記録する処理が完了する時ごとにDB cache serverの内部スレッドからコールバックされます。
		- Visual Studio 2005以上ではoverride keyword使用をお勧めします。
		\param type 完了されたDBMS記録(PropNode)の種類(参考 \ref DbmsWritePropNodePendType)

		\~
		 */
		virtual void OnDbmsWriteDone(DbmsWritePropNodePendType /*type*/) {}
	};

	/**
	\~korean
	CDbCacheServer2가 서버 작동을 시작하기 위해 제공해야 하는 파라메터

	\~english
	Parameter that must be provided in order to initiate CDbCacheServer2

	\~chinese
	CDbCacheServer 为了服务器开始启动提供的参数。

	\~japanese
	CDbCacheServer2がサーバー作動を開始するために提供しなければならないパラメーター
	\~
	 */
	class CDbCacheServer2StartParameter
	{
	public:
		/**
		\~korean
		서버의 주소. 자세한 것은 CStartServerParameter의 같은 이름의 멤버 참고

		\~english
		Server address. Please refer the member of same name at CStartServerParameter for further details.

		\~chinese
		服务器地址。详细的请参考 CStartServerParameter%的同名成员。

		\~japanese
		サーバーのアドレス。詳しくは、CStartServerParameterの同じ名前のメンバーをご参照ください。
		\~
		 */
		Proud::String m_serverIP;

		/**
		\~korean
		클라이언트의 리스닝 소켓이 바인딩될 주소입니다.
		- 초기값은 빈 문자열입니다.
		- 통상적으로 빈 문자열을 지정하지만 클라이언트가 네트워크 인터페이스(NIC)를 2개 이상 가지는 경우,
		그리고 그것들 중 하나만이 다른 CLanClient에서 오는 접속 요청을 받을 수 있는 경우 여기에 해당됩니다.
		NIC에 지정된 IP 또는 호스트 이름을 지정해야 합니다. 호스트에 있는 NIC 주소 리스트를 얻으려면 Proud.GetLocalIPAddresses를 쓸 수 있습니다.
		- 호스트 이름을 지정할 경우 클라이언트는 서버로 접속하기 위한 주소로 localhost를 입력할 경우 연결 실패를 할 수 있습니다. 이러한 경우
		클라이언트는 서버로 접속하기 위해 서버 호스트의 주소를 명시적으로 입력해야 합니다.

		\~english
		The address that the listening socket of the client to be bound.
		- The initial value is an empty string.
		- Usually set as an empty string but in case when the client has 2 or more of network interfaces (NICs)
		   and when only one of them is able to recieve the connection request from other CLanClient, it is the case to use this.
		  The IP address or the host name designated by the NIC must be used. Proud.GetLocalIPAddresses can be used to get the NIC address list located in the host.
		- In the case that a host name is used, a connection failure can occur when localhost is set as the address to connect to the server for a client.
		  In this case, the client must be set clearly as the server host address in order to connect to the server.

		\~chinese
		玩家的收听插口要被boud的地址。
		- 默认值是空字符窜。
		- 通常会指定空的字符串，当玩家拥有两个以上联网界面（NIC）的时候，还有只有其中一个在接收从别的 CLanClient%发来的连接邀请的时候会与此关联。
		要指定从NIC指定的IP或者主机名字。要想获得主机的NIC地址列表的话可以使用 Proud.GetLocalIPAddresses%。
		- 指定主机名字的时候，玩家为了连接服务器输入localhot的时候可能会连接失败。这样的情况下，用户为了连接服务器应该明示性的输入服务器主机的地址。

		\~japanese
		クライアントのリスニングソケットがバインディングされるアドレスです。
		- 初期値は空の文字列です。
		- 通常的に空の文字列を指定しますが、クライアントがネットワークインターフェース(NIC)を2個以上持っている場合、そしてそれらの一つのみが他のCLanClientから来る接続要請を受けられる場合、これに該当します。
		NICに指定されたIPまたはホスト名を指定してください。ホストにあるNICアドレスリストを得るためには、Proud.GetLocalIPAddressesを使うことができます。
		- ホスト名を指定する場合、クライアントはサーバーへ接続するためにアドレスで、localhostを入力する場合、接続失敗の可能性があります。このような場合、クライアントはサーバーへ接続するためにサーバーホストのアドレスを明示的に入力しなければなりません。

		\~
		*/
		String m_localNicAddr;

		/**
		\~korean
		TCP 리스닝 포트.
		자세한 것은 CStartServerParameter의 같은 이름의 멤버 참고

		\~english
		TCP listening port.
		Please refer the member of same name at CStartServerParameter for further details.

		\~chinese
		TCP 收听端口。
		详细的请参考 CStartServerParameter%的同名成员。

		\~japanese
		TCPリスニングポート
		詳しくは、CStartServerParameterの同じ名前のメンバーをご参照くだｓだい。

		\~
		 */
		int m_tcpPort;

		/**
		\~korean
		DBMS에 접속하기 위한 connection string.
		- 문자열에 대해서는 CAdoConnection.Open 도움말 참고

		\~english
		Connection string to connect to DBMS.
		- Please refer CAdoConnection.Open help section for text strings

		\~chinese
		为了连接DBMS的connection string。
		- 对字符串请参考 CAdoConnection.Open%帮助。

		\~japanese
		DBMSに接続するためのconnection string.		
		- 文字列に対しては、CAdoConnection.Openのヘルプをご参照ください。

		\~
		*/
		Proud::String m_DbmsConnectionString;

		/**
		\~korean
		이 DB 서버에 접속할 수 있는 인증키
		- 절대 이 값은 게임 클라이언트에 노출되어서는 안된다.
		이 값은 DB cache client(게임 서버 등)이 본 서버로 접속하기 위한 목적으로 사용된다.

		\~english
		The authentication key that can connect to this DB server
		- This value MUST NOT be exposed to game clients.
		  This value is used by DB cache client(e.g. game servers and etc.) to connect to main server.

		\~chinese
		可以连接此DB服务器的认证码。
		- 这个值绝对不能泄漏给玩家。
		此值是DB cache client（游戏服务器等）为了连接本身服务器为目的使用的。

		\~japanese
		このDBサーバーに接続できる認証キー
		- 絶対この値はゲームクライアントに露出されてはいけません。 
		この値はDB cache client(ゲームサーバーなど)が本サーバーへ接続するための目的で使われます。

		\~
		*/
		Proud::String m_authenticationKey;

		/**
		\~korean
		CDbCacheServer2가 필요로 하는 delegate 객체의 포인터
		- 이 객체는 CDbCacheServer2 객체가 존재하는 한 계속 존재해야 합니다.

		\~english
		Delegate object's pointer that CDbCacheServer requires
		- This object must exist as long as CDbCacheServer object exists.

		\~chinese
		需要 CDbCacheServer2%的delegate对象的pointer。
		- 只要 CDbCacheServer2%对象存在，此对象要一直存在。

		\~japanese
		CDbCacheServer2が必要とするdelegateオブジェクトのポインター
		- このオブジェクトはCDbCacheServer2オブジェクトが存在する限り、続けて存在する必要があります。

		\~
		*/
		IDbCacheServerDelegate2* m_delegate;

		/**
		\~korean
		DB cache에 의해 로딩,저장이 일어날 테이블들의 이름입니다.
		Root table과 child table들의 이름을 여기에 넣으십시오.
		자세한 것은 CCachedTableName 을 참고하십시오.

		\~english
		Names of tables to be loaded and saved by DB cache.
		Input names of root and child tables here.
		Refer to CCachedTable for details.

		\~chinese
		在DB缓存加载保存的表的名称。
		在此输入Root table和child table的名称。
		详细的请参考CCachedTableName。

		\~japanese
		DB cacheによってローディング、保存されるテーブル名です。
		Root tableとchild ｔａｂｌｅの名前をここに入れてください。
		詳しくは、CCachedTableNameをご参照ください。

		\~
		 */
		CFastArray<CCachedTableName> m_tableNames;

		/**
		\~korean
		DBCacheServer2에 비독점적 접근을 허용하는지에 대한 여부입니다. <a target="_blank" href="http://guide.nettention.com/cpp_ko#dbc2_nonexclusive_overview" >비독점적 데이터 접근하기</a> 기능입니다.
		- 기본적으로 false입니다.

		\~english
		Whether to allow or not to allow a non-exclusive access to DBCacheServer. This is a <a target="_blank" href="http://guide.nettention.com/cpp_en#dbc2_nonexclusive_overview" >Accessing non-exclusive data</a> function.
		- The default value is false.

		\~chinese
		是否允许与DBCacheServer2的非独占接近。是<a target="_blank" href="http://guide.nettention.com/cpp_zh#dbc2_nonexclusive_overview" >访问共享性数据</a>%功能。
		- 默认为false。

		\~japanese
		DBCacheServer2に非独占的接近を許容するかどうかに対するセッティングの値です。<a target="_blank" href="http://guide.nettention.com/cpp_jp#dbc2_nonexclusive_overview" >Accessing non-exclusive data</a> 機能です。
		- 基本的に falseです。

		\~
		*/
		bool	m_allowNonExclusiveAccess;

		/**
		\~korean
		이 값이 null이 아니면, 외부 networker thread를 사용합니다.
		Proud.CStartLanServerParameter.m_externalNetWorkerThreadPool와 같은 역할을 합니다.

		\~english 
		If this value is not null, use an external networker thread. 
		It performs the same role as Proud.CStartLanServerParameter.m_externalNetWorkerThreadPool. 

		\~chinese
		此值不是null的话，使用外部networker thread。
		起着与 Proud.CStartLanServerParameter.m_externalNetWorkerThreadPool%一样的作用。

		\~japanese
		この値がnullではなければ、外部networker threadを使用します。
		Proud.CStartLanServerParameter.m_externalNetWorkerThreadPoolと同じ役割をします。

		\~
		*/
		CThreadPool* m_externalNetWorkerThreadPool;

		/**
		\~korean
		이 값이 null이 아니면, 외부 user worker thread를 사용합니다.
		Proud.CStartLanServerParameter.m_externalUserWorkerThreadPool와 같은 역할을 합니다.

		\~english 
		If this value is not null, use an external networker thread.
		It performs the same role as Proud.CStartLanServerParameter.m_externalNetWorkerThreadPool.

		\~chinese
		此值不是null的话，使用外部user worker thread。
		起着与 Proud.CStartLanServerParameter.m_externalUserWorkerThreadPool%一样的作用。

		\~japanese
		この値がnullではなければ、外部user worker threadを使用します。
		Proud.CStartLanServerParameter.m_externalUserWorkerThreadPoolと同じ役割をします。

		\~
		*/
		CThreadPool* m_externalUserWorkerThreadPool;

		/**
		\~korean DB 처리를 위한 스레드 풀의 스래드 개수입니다.
		- 최소한 1은 지정 되어야 합니다.
		- DB 처리는 내부적으로 device time을 가지기 때문에 충분히 많은 수를 두는 것이 좋습니다. (가령 100개)
		- 초기값은 10입니다. 0을 지정하면 CPU 갯수로 지정됩니다.

		\~english It is the number of thread pools for DB processing.
		- At least “1” should be set.
		- DB processing has the internal device time, so it is recommended to have many threads. (ex. 100)
		- Initial value is “10”. If setting “0”, it will be set as the number of CPUs.

		\~chinese 是为处理DB的线程池线程个数。
		- 至少要指定为1.
		- DB处理内部拥有 device time，因此建议存有足够的数量。（如100个）
		- 初始值为10. 如果指定为0则将指定成CPU个数。

		\~japanese DB処理のためのスレッドプールのスレッドの個数です。
		- 最小限１は指定されなければなりません。
		- DB処理は内部的にdevice timeがかかるために十分な数を設定する方が良いです。（仮に100個）
		- デフォルトは10です。0を指定するとCPUの数だけ指定されます。
		\~
		*/
		int m_dbWorkThreadCount;

		/**
		\~korean DB Cache Server에서 DBMS로 Query를 보낼 때의 CommandTimeout 값입니다.
		- 단위는 초단위이고 초기값은 30초 입니다.

		\~english It is CommandTimeout value when sending Query from DB Cache Server to DBMS.
		- Standard unit is second and default value is 30 seconds.

		\~chinese DB Cache Server向 DBMS发送 Query时的 CommandTimeout 值。
		- 以秒为单位，初始值为30秒。

		\~japanese DB Cache ServerからDBMSへQueryを送る際の CommandTimeout値です。
		-単位は秒単位で、初期値は30秒です。

		\~
		*/
		int m_commandTimeoutSec;

		/**
		\~korean
		- OnException 콜백 기능을 사용할 지 여부를 선택합니다.
		- 기본값은 true입니다.
		- 만약 false를 지정하면 유저 콜백에서 예상치 못한 Exception 발생 시 OnException이 호출되지 않고 크래시가 발생합니다.

		\~english
		- Decide whether it uses OnException callback function or not.
		- Default value is true.
		- If setting it as false, when exception occurs during  user callback, crash will occur as OnException is not called.

		\~chinese
		- 选择是否使用 OnException Callback功能。
		- 基本值为 true。
		- 如果指定false，用户在Callback过程中发生意想不到的 Exception 时，OnException将不被呼出，且发生Crash。

		\~japanese
		- OnException Callback機能を使用するかどうかを選択します。
		- デフォルト値はtrueです。
		- もしfalseを指定するとユーザーCallbackから予想できないExceptionが発生した時、OnExceptionが呼び出されずにクラッシュが発生します。
		\~
		*/
		bool m_allowExceptionEvent;

		/**
		\~korean
		- true로 설정되면 RecursiveUpdate계열 요청을 처리 할 때 각 요청을 순서대로 하나씩 수행합니다.
		- 성능에 악영향을 미치지만 Deadlock을 방지하기 위한 옵션입니다. 원인을 알 수 없는 Deadlock이 발생 할 때만 사용하는 것을 권장합니다.
		- 기본값은 false입니다.

		\~english
		- If set to 'true', each of the request for 'RecursiveUpdate' will be conducted in order.
		- Although it can cause harm to its performance, this is an alternative for deadlock prevention. We recommend you to use only as an option when deadlock occurs with an unknown cause.
		- The default value is ‘false’.

		\~chinese
		- 设定为true的话,处理RecursiveUpdate系列的请求时,按照各请求的顺序一个个地履行。
		- 虽然在性能上牵涉坏影响,但却是为防止Deadlock的一项配置。建议只在当无法了解原因的Deadlock发生时才使用。
		- 默认值是假的。

		\~japanese
		- tureに設定されるとRecursiveUpdate系列の要請を処理する際に各要請を順番に一つずつ行います。
		- このオプションは性能に悪影響を及ぼす恐れはありますが、Deadlockを防ぐためのものです。原因のわからないDeadlockが発生した時のみ使用することをお勧めします。
		- デフォルト値はfalseです。
		\~
		*/
		bool m_serializeRecursiveUpdateWork;

		/**
		\~korean
		- 이 값이 true이면 클라이언트가 접속 종료 시 해당 클라이언트가 RequestIsolateData를 요청한 데이터들의 격리를 자동으로 해제합니다.
		- 기본값은 true입니다.

		\~english
		- If this value is true, the client will automatically relieve the isolation of data which requested RequestIsolateData when the client is disconnected.
		- The default value is true.

		\~chinese
		- 如果这价格是true的话, 当客户下线时,该客户对RequestlsolateData所要求的数据隔离将会被自动解除。
		- 默认值是true。

		\~japanese
		- この 値が trueなら クライアントが 接続 終了する 時 該当 クライアントが RequestIsolateDataを 要請した データたちの 隔離を 自動的に 解除します。
		- デフォルト値はtrueです。
		\~
		*/
		bool m_autoDeisolateData;

		/** 켜져있으면, DB 액세스를 하기 전에 다음과 같은 세팅을 하는 SQL 구문이 실행됩니다.
		\code
		SET character_set_results = utf8mb4;
		SET character_set_client = utf8mb4;
		SET character_set_connection = utf8mb4;
		\endcode

		여러분의 서버가 utf8mb4 가령 emoji를 제대로 핸들링하지 못하면 켜십시오. */
		bool m_useUtf8mb4Encoding;


		PROUDSRV_API CDbCacheServer2StartParameter();
	};

	/**
	\~korean
	<a target="_blank" href="http://guide.nettention.com/cpp_ko#dbc2_client_server" >DB cache 클라이언트와 서버</a> 입니다.

	\~english
	<a target="_blank" href="http://guide.nettention.com/cpp_en#dbc2_client_server" >DB cache client and server</a>

	\~chinese
	是<a target="_blank" href="http://guide.nettention.com/cpp_zh#dbc2_client_server" >DB cache客户端和服务器</a>%。

	\~japanese
	\ref dbc2_server です。
	\~
	 */
	class CDbCacheServer2
	{
	protected:
		CDbCacheServer2() {}
	public:
		virtual ~CDbCacheServer2() {}

		/**
		\~korean
		접속된 클라이언트 수를 리턴합니다.

		\~english
		Return the client is connected

		\~chnese
		Return 已连接的客户端数。

		\~japanese
		接続されたクライアント数をリターンします。
		\~
		 */
		virtual intptr_t GetCachingDataTreeCount() = 0;

		/**
		\~korean
		DB cache 서버 인스턴스를 생성합니다.

		\~english
		Creates DB cache server instance

		\~chinese
		创建DB cache服务器实例。

		\~japanese
		DB cacheサーバーインスタンスを生成します。
		\~
		 */
		PROUDSRV_API static CDbCacheServer2* New();

		/**
		\~korean
		클라이언트로부터의 접속을 활성화하고, DBMS와의 실제 연결을 시작합니다.
		\param params CDbCacheServer2를 시작하는데 필요한 설정값 (참고 \ref CDbCacheServer2StartParameter)

		\~english TODO:translate needed
		Enable the connection from the client and start the actual connection with DBMS. 
		\param params The configuration value required to start CDbCacheServer2 (refer to \ref CDbCacheServer2StartParameter)

		\~chinese
		激活与玩家的连接，开始与DBMS的实际连接。
		\param param 开始 CDbCacheServe2r%需要的设置值（参考 \ref CDbCacheServer2StartParameter）。

		\~japanese
		クライアントからの接続を活性化して、DBMSとの実際接続を開始します。
		\param params CDbCacheServer2を開始するに必要な設定値(参考 \ref CDbCacheServer2StartParameter)

		\~
		 */
		virtual void Start(CDbCacheServer2StartParameter &params) = 0;

		/**
		\~korean
		클라이언트와의 접속을 모두 끊고, 하던 작업을 모두 처리후에 스레드를 종료 합니다.

		\~english
		Disconnect with all clients, finish all works that in process then close thread.

		\~chinese
		断开与玩家的连接，处理完进行中的工作后终止线程。

		\~japanese
		クライアントとの接続を全て切って、行っていた作業を全て処理後にスレッドを終了します。
		\~
		*/
		virtual void Stop() = 0;

		/**
		\~korean
		DB cache client2로부터 받은 데이터 추가/변경/제거 내용을 받은 후부터 DBMS에 기록할 때까지
		대기하는 시간입니다.
		- 너무 짧으면 DBMS에 걸리는 부하가 증가합니다.
		- 너무 길면 DB cache server가 비정상 종료(예: 정전)할 경우 DBMS에 기록하지 못하고 메모리에
		대기중이던 데이터는 소실될 확률이 커집니다.
		\param val 밀리초 단위 시간

		\~english TODO:translate needed
		This is the time to stand by from after receiving the data add/modify/delete content from DB cache client 2 until recording it in DBMS. 
		- If too short, the load on DBMS increases
		- If too long, when the DB cache server terminates abnormally (e.g.: power failure), it cannot record in DBMS and the data waiting in the memory is likely to be lost.  
		\param val Time in millisecond unit

		\~chinese
		从DB cache client2接收添加/修改/删除数据的内容后等待至往DBMS记录的时间。
		- 太短的话给DBMS的负荷会增加。
		- 太长的话，当非正常终止（例：停电）DB cache server的时候不能往DBMS记录，而在内存等待的数据消失的概率会增加。
		\param val 秒单位时间。

		\~japanese
		DB cache client2より受けたデータの追加/変更/除去内容を受けた後からDBMSに記録する時まで待つ時間です。
		- あまりにも短いとDBMSにかかる負荷が増加します。
		- あまりにも長いとDB cache serverが異常終了(例：停電)する場合、DBMSに記録できずにメモリーで待っていたデータは消失される確率が大きくなります。
		\param val ミリ秒単位時間

		\~
		 */
		virtual void SetDbmsWriteIntervalMs(int64_t val)=0;

		/**
		\~korean
		DB cache client2에서 unload한 데이터가 DB cache server 메모리에서도 완전히 제거할
		때까지 대기하는 시간입니다.
		- 너무 짧으면 DBMS에 걸리는 부하가 증가합니다.
		- 너무 길면 DB cache server2의 메모리 사용량이 커집니다.
		\param val 밀리초 단위 시간

		\~english TODO:translate needed
		Waiting time that the unloaded data at DB cache client waits until it is fully removed from DB cache memory
		- If this is set too short then the work load to DBMS will increase.
		- If this is set too long then it will increase the amount of memory usage in DB cache server.

		\~chinese
		从DB cache client2 unload的数据在DB cache server内存也完全删除的等待时间。
		- 太短的话给DBMS的负荷会增加。
		- 太长的话DB cache server2的内存使用量会加大。
		\param val 毫秒单位时间

		\~japanese
		DB cache client2でunloadしたデータがDB cache serverメモリーでも完全に除去される時まで待つ時間です。
		- あまりにも短いとDBMSにかかる負荷が増加します。
		- あまりにも長いとDB cache serve2のメモリー使用量が大きくなります。
		\param val ミリ秒単位時間

		\~
		 */
		virtual void SetUnloadedDataHibernateDurationMs(int64_t val)=0;

		/**
		\~korean
		Proud.CNetServer.SetDefaultTimeoutTimeMs과 같은 역할을 합니다.

		\~english
		Performs similar role as Proud.CNetServer.SetDefaultTimeoutTimeMs

		\~chinese
		起着与 Proud.CNetServer.SetDefaultTimeoutTimeMs%一样的作用。

		\~japanese
		Proud.CNetServer.SetDefaultTimeoutTimeMsと同じ役割をします。
		\~
		 */
		virtual void SetDefaultTimeoutTimeMs(int newValInMs) = 0;

		/**
		\~korean
		Proud.CNetServer.SetDefaultTimeoutTimeSec과 같은 역할을 합니다.
		\param val 초 단위 시간

		\~english TODO:translate needed.
		Performs similar role as Proud.CNetServer.SetDefaultTimeoutTimeSec
		\param val

		\~chinese
		起着与 Proud.CNetServer.SetDefaultTimeoutTimeSec%一样的作用。
		\param val 秒单位时间

		\~japanese
		Proud.CNetServer.SetDefaultTimeoutTimeSecと同じ役割をします。
		\param val 秒単位時間

		\~
		 */
		virtual void SetDefaultTimeoutTimeSec(double newValInSec) = 0;


		/**
		\~korean
		이 DB cache server2가 열어 놓은 TCP 리스닝 소켓의 주소를 얻습니다.

		\~english
		Gets the address of TCP listening socket opened by this DB cache server

		\~chinese
		获得此DB cache server2打开的TCP收听插口地址。
		
		\~japanese
		このDB cache server2が開けて置いたTCPリスニングソケットのアドレスを得ます。
		 */
		virtual AddrPort GetTcpListenerLocalAddr() = 0;

		/**
		\~korean
		이 DB cache Server2에 접속한 DB Cache Cleint2 들에 대한 AddrPort 값들을 얻는다.
		- 이 메서드로 얻는 목록은 호출할 시점의 스냅샷일 뿐이다.
		이 함수가 리턴된 후에도 클라이언트 목록의 내용이 실제 서버의 상태와 동일함을 보장하지는 않는다.
		\param output AddrPort들의 목록이 채워질 곳

		\~english TODO:translate needed.
		Obtain the AddrPort for the DB Cache Client 2 connected to this DB Cache Server2. 
		- The list obtained by this method is only the snapshot at the time of being called. 
		Even after this function is returned, it does not guarantee that the content of the client list is the same as the status of the actual server. 
		\param output Place where the list of AddrPort will be filled out. 

		\~chinese
		获得连接这个DB cache Server2的DB Cache Cleint2的AddrPort值。
		- 用这个方法获得的目录只是呼叫时候的snapshot而已。
		此函数返回以后也不能保障玩家目录的内容与实际服务器的状态是相同的。
		\param output AddrPort的目录要被填充的地方。

		\~japanese
		このDB cache Server2に接続したDB Cache Cleint2などに対するAddrPort値を得ます。
		- このメソッドで得るリストは呼び出す時点のスナップショットであるだけです。
		この関数がリターンされた後にもクライアントリストの内容が実際サーバーの状態と同一であることを保障することはありません。
		\param output AddrPortのリストが満たされる所
		\~
		*/
		virtual bool GetAllRemoteClientAddrPort(CFastArray<AddrPort> &ret) = 0;


		/**
		\~korean
		- 독점 로드 요청의 처리를 기다릴 제한 시간을 설정합니다.
		- 이 값으로 설정된 시간 이내에 로드 및 독점권 획득이 완료되지 않으면 해당 요청은 타임아웃으로 실패 처리됩니다.
		- 기본값은 10초입니다. (10000밀리초)
		\param timeoutMs 지정 할 시간 제한입니다. (밀리초 단위)

		\~english
		Configure the time limit to wait for the processing of exclusive load request. 
		- If the load and exclusivity acquisition is not completed within the time set with this value, the request is processed as a failure as a timeout. 
		- The default value is 10 seconds (10000 milliseconds)
		\param timeoutMs The time limit to be designated (millisecond unit)

		\~chinese
		- 对垄断加载请求处理事件设定等待时间。
		- 如果在设定的时间内未完成加载并获得垄断权，该请求就会被超时处理。
		- 默认值为10秒。 (10000微秒)
		\paramtimeoutMs 指定的事件限制。 (微妙单位)

		\~japanese
		- 独占ロード要請の処理を待つ制限時間を設定します。
		- この値で設定された時間以内にロード及び独占権獲得が完了されなければ、該当要請はタイムアウトで失敗処理になります。
		- 基本値は10秒です。(10000ミリ秒)
		\param timeoutMs 指定する時間制限です。(ミリ秒単位)

		\~
		*/
		virtual void SetUnloadRequestTimeoutTimeMs(int64_t timeoutMs) = 0;

		/**
		\~korean
		- 독점 로드 요청의 타임아웃 설정값을 밀리초 단위로 반환합니다.

		\~english
		Return the timeout configuration value of the exclusive load request in millisecond unit. 

		\~chinese
		- 把垄断加载超时设定值转换成微妙单位。

		\~japanese
		- 独占ロード要請のタイムアウト設定値をミリ秒単位で返還します。
		\~
		*/
		virtual int64_t GetUnloadRequestTimeoutTimeMs() const = 0;
	};
	/**  @} */
#if (defined(_MSC_VER) && _MSC_VER>=1400)
//#pragma managed(pop)
#endif
}

#ifdef _MSC_VER
#pragma pack(pop)
#endif
#endif
