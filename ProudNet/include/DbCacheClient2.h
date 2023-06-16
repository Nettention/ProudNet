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
#include "Ptr.h"
#include "NetServerInterface.h"
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
	class IDbCacheClientDelegate2;


	/**
	\~korean
	DB cache server에 접속하기 위한 연결 정보

	\~english
	Connect information in order to connect to DB cache server

	\~chinese
	为了连接DB cache server 的连接信息。

	\~japanese
	DB cache serverに接続するための接続情報
	\~
	*/
	class CDbCacheClient2ConnectParameter
	{
	public:
		/**
		\~korean
		서버 주소

		\~english
		Server address

		\~chinese
		服务器地址

		\~japanese
		サーバーアドレス
		\~
		*/
		String m_serverAddr;

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
		- 通常、空の文字列を指定しますが、クライアントがネットワークインターフェース(NIC)を2個以上持っている場合、そしてそれらのうち一つのみが違うCLanClientから来る接続要請を受けられる場合がこれに該当します。
		NICに指定されたIPまたはホスト名を指定しなければなりません。ホストにあるNICアドレスリストを得るためにはProud.GetLocalIPAddressesを使うことができます。
		- ホスト名を指定する場合、クライアントはサーバーに接続するためのアドレスにlocalhostを入力する場合、接続失敗の可能性があります。このような場合、クライアントはサーバーに接続するためにサーバーホストのアドレスを明示的に入力しなければなりません。

		\~
		*/
		String m_localNicAddr;

		/**
		\~korean
		서버 포트

		\~english
		Server port

		\~chinese
		服务器端口

		\~japanese
		サーバーポート
		\~
		*/
		uint16_t m_serverPort;

		/**
		\~korean
		서버 인증키. CDbCacheServer2StartParameter.m_authenticationKey 와 같은 값이어야 합니다.

		\~english
		Server authentication key. It has to same value with bCacheServer2StartParameter.m_authenticationKey

		\~chinese
		服务器认证码。要与 CDbCacheServer2StartParameter.m_authenticationKey%一样的值。

		\~japanese
		サーバー認証キー。CDbCacheServer2StartParameter.m_authenticationKeyと同じ値ではなければなりません。
		\~
		*/
		String m_authenticationKey;

		/**
		\~korean
		이벤트 콜백을 받기 위한 delegate 객체

		\~english
		Delegate object to receive event callback

		\~chinese
		为了得到event回调的delegate对象。

		\~japanese
		イベントコールバックを受けるためのdelegateオブジェクト
		\~
		*/
		IDbCacheClientDelegate2* m_delegate;

		/**
		\~korean
		스레드 풀의 스레드 갯수. 최소한 1은 지정되어야 합니다.
		- 초기값은 0이다. 0을 지정하면 CPU 갯수로 지정됩니다.

		\~english
		The number of threads in thread pool. Must be set as at least 1.
		- The initial value is 0. If it is set as 0 then it is set as the number of CPUs.

		\~chinese
		线程槽的线程个数。最少要指定为1。
		- 初始值是0。指定0的话便会指定为CUP个数。

		\~japanese
		スレッドプールのスレッド個数。最小限1は指定されなければなりません。
		- 初期値は0です。0を指定すればCPU数で指定されます。
		\~
		*/
		int m_userWorkerThreadCount;

		/**
		\~korean
		이 값이 null이 아니면, 외부 networker thread 를 사용합니다.
		Proud.CStartLanServerParameter.m_externalNetWorkerThreadPool 와 같은 역할을 합니다.

		\~english
		If this value is not null, an external networker thread will be used.
		It acts as Proud.CStartLanServerParameter.m_externalNetWorkerThreadPool.

		\~chinese
		此值不是null的话使用外部networker thread。
		起着与 Proud.CStartLanServerParameter.m_externalNetWorkerThreadPool%一样的作用。

		\~japanese
		この値がnullではない場合、外部 networker threa を使用します。
		Proud.CStartLanServerParameter.m_externalNetWorkerThreadPool と同じ役割をします。

		*/
		CThreadPool* m_externalNetWorkerThreadPool;

		/**
		\~korean
		이 값이 null이 아니면, 외부 user worker thread를 사용합니다.
		Proud.CStartLanServerParameter.m_externalUserWorkerThreadPool 와 같은 역할을 합니다.

		\~english
		If this value is not null, an external user worker thread will be used.
		It acts as Proud.CStartLanServerParameter.m_externalUserWorkerThreadPool.

		\~chinese
		此值不是null的话，使用外部user worker thread。
		起着与 Proud.CStartLanServerParameter.m_externalUserWorkerThreadPool%一样的作用。


		\~japanese
		この値がnullではない場合、外部 user worker threa を使用します。
		Proud.CStartLanServerParameter.m_externalUserWorkerThreadPool と同じ役割をします。

		*/
		CThreadPool* m_externalUserWorkerThreadPool;

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

		PROUDSRV_API CDbCacheClient2ConnectParameter();
	};

	/**
	\~korean
	DB cache client2로부터의 이벤트 콜백을 받기 위한 delegate 객체

	\~english
	Delegate object to receive event callback from DB cache client

	\~chinese
	为了从DB cache client2得到event回调的的delegate对象。

	\~japanese
	DB cache client2からのイベントコールバックを受けるためのdelegateオブジェクト
	\~
	*/
	class IDbCacheClientDelegate2
	{
	public:
		/**
		\~korean
		이벤트가 콜백했을때의 Arguments

		\~english
		Arguments when event called back

		\~chinese
		event 回调的时候的Arguments。

		\~japanese
		イベントがコールバックした時のArguments
		\~
		*/
		class CCallbackArgs
		{
		public:
			/**
			\~korean
			데이터 트리 1개에 대한 정보를 담은 클래스입니다.

			\~english
			This is a class containing the information on one data tree.

			\~chinese
			包含一个数据树信息的类。

			\~japanese
			データツリー1個に対する情報を入れたクラスです。

			\~
			*/
			class CItem
			{
			public:
				/**
				\~korean
				DBCacheClient의 메소드들 중 message를 인자로 받는 요청들은
				해당 요청으로 인해 영향을 받는 다른 Loader 혹은 요청자에게 message를 전달 할 수 있습니다.
				그 message를 전달받는 파라메터가 바로 이것입니다.

				\~english
				The requests receiving the message as a factor amongst the methods of DBCacheClient can deliver the message to another loader or the requester. 
				The parameter receiving the message is this.

				\~chinese
				在DBCacheClient的方法中接收消息参数的请求可以向受到该请求影响的其他Loader或请求者发送消息。
				接收该消息的参数就是这个。

				\~japanese
				DBCacheClientのメソッドのうちmessageを因子で受ける要請は、該当要請によって影響を受ける他のローターまたは要請者にmessageを伝達することができます。
				そのmessageを伝達してもらうパラメーターがすぐこれです。

				\~
				*/
				ByteArray m_message;

				/**
				\~korean
				해당 Callback과 관련된 데이터의 Session입니다.

				\~english
				The data session related to the Callback. 

				\~chinese
				与该Callback相关的数据会话。

				\~japanese
				該当Callbackと関連されたデータのSessionです。
				\~
				*/
				Guid m_sessionGuid;

				/**
				\~korean
				해당 Callback과 관련된 데이터의 RootUUID

				\~english
				The data RootUUID related to the Callback

				\~chinese
				与该Callback相关的数据的RootUUID

				\~japanese
				該当Callbackと関連されたデータのRootUUID
				\~
				*/
				Guid m_rootUUID;

				/**
				\~korean
				- 추가,업데이트, 삭제시에 작업이 완료된 전체 data의 스냅샷 (이값을 변경하는 것은 실제 게임 정보 저장과 아무런 연관이 없습니다.)
				- 해당 Callback과 관련된 데이터 트리입니다.

				\~english
				Snapshot of all completed data when you do add, update, remove (Changing this value does not affect saving actual game information)

				\~chinese
				添加，升级，删除的时候工作完成的全部data的snapshot（改变此值对实际游戏信息的储存没有任何关系。）。

				\~japanese
				- 追加、アップデート、削除時に作業が完了された全体データのスナップショット(この値を変更することは実際ゲーム情報の保存と何らの関連がありません)
				- 該当Callbackと関連されたデータツリーです。

				\~
				*/
				CLoadedData2Ptr m_loadedData;

				/**
				\~korean
				해당 Callback과 관련하여 변경이 일어난 노드의 UUID

				\~english
				The UUID of the node where a change occurred related to the Callback

				\~chinese
				与该Callback相关引起变更的节点的UUID

				\~japanese
				該当Callbackと関連して変更が起きたノードのUUDI
				\~
				*/
				Guid m_UUID;

				/**
				\~korean
				- 추가,업데이트, 삭제시에 작업이 완료된 실작업 data의 스냅샷 (이값을 변경하는 것은 실제 게임 정보 저장과 아무런 연관이 없습니다.)
				- 해당 Callback과 관련된 노드입니다.

				\~english
				Snapshot of actual data when you do add, update, remove (Changing this value does not affect saving actual game information)

				\~chinese
				添加，升级，删除的时候工作完成的全部data的snapshot（改变此值对实际游戏信息的储存没有任何关系。）。

				\~japanese
				- 追加、アップデート、削除時に作業が完了されたリアル作業データのスナップショット(この値を変更することは実際ゲーム情報の保存と何らの関連がありません)
				-　該当Callbackと関連されたノードです。

				\~
				*/
				CPropNodePtr m_data;

				/**
				\~korean
				- 실패 시 실패한 사유.
				- 요청의 성공/실패 여부를 구분하려면 이 값이 ErrorType_Ok가 아닌지 확인해야 합니다.

				\~english
				- Reason for the failure when failed
				- whether the request has succeeded or failed should be determined, check to make sure whether this value is ErrorType_Ok or not. 

				\~chinese
				- 失败理由。
				- 区分请求是否成功之前，需要先确认此值是否是ErrorType_Ok。

				\~japanese
				- 失敗時に失敗した事由
				- 要請の成功/失敗可否を区分するためには、この値がErrorType_OKではないか確認する必要があります。

				\~
				*/
				ErrorType m_reason;

				/**
				\~korean
				실패 시 관련 코멘트

				\~english
				When it failed, comment about reason

				\~chinese
				失败时失败的详细注释。

				\~japanese
				失敗時の関連コメント
				\~
				*/
				String m_comment;

				/**
				\~korean
				실패시에 나온HRESULT 이것이 S_OK라면 com_error로 실패한것이 아닙니다.

				\~english
				HRESULT from when it failed. If it is S_OK, it is not failed with com_error

				\~chinese
				失败时出现的HRESULT是S_OK的话不是com_error失败。

				\~japanese
				失敗時に出たHRESULT　これがS_OKならcom_errorで失敗したのではありません。
				\~
				*/
				HRESULT m_hResult;

				/**
				\~korean
				실패시에 나온 source 이것이 없다면 쿼리중 실패한것이 아닙니다.

				\~english
				Source from when it failed. If it have not got this, it is not failed during quary

				\~chinese
				失败时没有出现source，就不是在quary时失败的。

				\~japanese
				失敗時に出たsource　これがなければクエリー中に失敗したのではありません。
				\~
				*/
				String m_source;

				CItem();
			};
			typedef CFastArray<CItem> Items;

			/**
			\~korean
			요청에 관련된 데이터 트리들을 담은 목록입니다.
			예를들어 byQuery 계열의 요청들은 쿼리 조건에 맞는 여러 개의 데이터 트리들을 반환하게 되는데
			이들을 받아 처리하려면 이 목록을 순회해야 합니다.

			\code
			// example
			void IDbCacheClientDelegate2::OnExclusiveLoadDataComplete(CCallbackArgs& args)
			{
			    for ( CLoadResults::iterator it=m_loadResults.begin(); it!=m_loadResults.end(); ++it )
			    {
			        CLoadResult& result = *it;
			        if ( result.m_reason == ErrorType_Ok )
			        {
			            // success
			        }
			        else
			        {
			            // fail
			        }
			    }
			    // ...
			}

			\endcode

			\~english
			This is the list containing the data trees related to the request. 
			. For example, the byQuery series requests return several data trees matching the query condition and to receive to process them, this list must be circulated. 


			\~chinese
			包含请求相关数据树的列表。
			例如，byQuery 系列的请求将返回符合条件的多个数据树，处理这些数据需要对列表进行循环。

			\endcode

			\~japanese
			要請に関連したデータツリーを込めたリストです。
			例えば、byQuery系列の要請はクエリー条件に合う複数のデータツリーを返還するようになりますが、これを受けて処理するためにはこのリストを巡回しなければなりません。

			\endcode

			\~

			\code
			// example
			void IDbCacheClientDelegate2::OnExclusiveLoadDataComplete(CCallbackArgs&args)
			{
				for ( CLoadResults::iterator it=m_loadResults.begin(); it!=m_loadResults.end(); ++it )
				{
					CLoadResult& result = *it;
					if ( result.m_reason == ErrorType_Ok )
					{
						// success
					}
					else
					{
						// fail
					}
				}
				// ...
			}
			\endcode
			*/
			Items m_items;

			/**
			\~korean
			요청 시 호출자가 넣었던 사용자 정의 값. 보통은 포인터로 사용됩니다.
			tag를 받는 요청들은 호출 시 입력한 tag값을 응답에서 이 변수를 통해 그대로 돌려줍니다.

			\~english
			User defined value that entered by invoker when it requested.
			Normally it uses for pointer. If you request with it then will receive same thing

			\~chinese
			邀请时呼叫者填入的用户定义的值。通常适用于pointer。使用这个发出邀请时 会接收同样的回应。

			\~japanese
			要請時に呼出者が入れたユーザー定義値。普通はポインターで使用されます。
			Tagを受けた要請は呼び出し時に入力したtag値を、応答でこの変数を通じそのまま返します。

			\~
			*/
			intptr_t m_tag;

			CCallbackArgs();
		};

		virtual ~IDbCacheClientDelegate2() {}

		/**
		\~korean
		서버 연결 시도 결과를 리턴합니다.
		\param info 실패했을때의 실패정보입니다. 성공시에는 info.m_errorType 에 ErrorType_Ok 입니다.

		\~english
		Returns the results of server connection attempts.
		\param info This is failed information. When it succeed, ErrorType_Ok will be in info.m_errorType.

		\~chinese
		返回服务器试图连接的结果。
		\param info 失败时侯的失败信息。成功时在 info.m_errorType%会ErrorType_Ok。

		\~japanese
		サーバー接続試しの結果をリターンします。
		\param info 失敗した時の失敗情報です。成功時にはinfo.m_errorTypeにErrorType_Okです。

		\~
		*/
		virtual void OnJoinDbCacheServerComplete(ErrorInfo *info) = 0;

		/**
		\~korean
		DB cache server와의 연결이 해제됐음을 노티합니다.
		- 이것이 호출된 상황에서는, 이미 DB cache client의 모든 데이터가 unload된 상태입니다.
		\param reason 연결해제 사유

		\~english
		Notifies that the connection to DB cache server is finished
		- Under the circumstance that this is called, all data in DB cache client is already unloaded.
		\param reason reason to disconnecting

		\~chinese
		通知与DB cache server 的连接被解除了。
		- 这个被呼叫的情况下，DB cache client 的所有数据已经是unload的状态。
		\param reason 解除连接的理由。

		\~japanese
		DB cache serverとの接続が解除されたことを通知します。
		- これが呼び出された状況ではすでにDB cache clientの全てのデータがunloadされた状態です。
		\param reason 接続解除事由

		\~
		*/
		virtual void OnLeaveDbCacheServer(ErrorType reason) = 0;

		/**
		\~korean
		DB cache server에서 DBMS에 기록하는 과정이 끝날 때마다 도착하는 이벤트입니다.
		\param type 어떤작업을 했는지에대한 type입니다.
		\param loadedDataGuid 완료한 작업 data의 Guid입니다.

		\~english
		Receiving event when writting process to DBMS on DB cache server.
		\param type Type for what process has done.
		\param loadedDataGuid Guid of finished work data

		\~chinese
		每当从DB cache server 结束对DBM记录过程的时候到达的event。
		\param type 是对做了什么工作的type。
		\param loadedDataGuid 结束的工作data的Guid。

		\~japanese
		DB cache serverでDBMSに記録する過程が終わる時ごとに到着するイベントです。
		\param type どんな作業をしたのかに対するtypeです。
		\param loadedDataGuid 完了した作業データのGuidです。

		\~
		*/
		virtual void OnDbmsWriteDone(DbmsWritePropNodePendType type, Guid loadedDataGuid) = 0;

		/**
		\~korean
		- 독점로드 요청에 대한 응답입니다.
		- 요청 시 입력한 검색 조건에 따라 args.m_items를 통해 복수의 데이터 트리를 받을 수도 있습니다.
		- args.m_items에는 독점로드에 성공한 결과와 실패한 결과가 섞여있을 수 있는데 CItem.m_reason값이 ErrorType_Ok이면 성공한 결과입니다.
		- 성공한 경우 CItem객체의 멤버변수 중 m_loadedData, m_data, m_sessionGuid, m_rootUUID, m_UUID, m_message에 관련 정보가 기록됩니다.
		- 실패한 경우 CItem객체의 멤버변수 중 m_reason, m_comment, m_UUID, m_hResult, m_message에 관련 정보가 기록됩니다.
		- m_message는 해당 데이터가 이미 독점되어있는 경우 기존 독점자가 독점권 이양 또는 거부를 하면서 넘긴 메시지가 기록됩니다.
		- args.m_tag에 요청 시 사용자가 임의로 입력했던 tag값이 그대로 돌아오므로 이를 통해 어떤 요청에 대한 응답인지 알 수 있습니다.

		\~english
		- This is the response for the exclusive load request. 
		- When requesting, you may receive multiple data trees through args.m_items depending on the input search conditions. 
		- args.m_items may have successful results and failed results mixed for exclusive load and if the value of CItem.m_reason is ErrorType_Ok, then this is a successful result. 
		- If successful, the related information is recorded in m_loadedData, m_data, m_sessionGuid, m_rootUUID, m_UUID and m_message among the member variables of the CItem object.
		- If failed, the related information is recorded in m_reason, m_comment, m_UUID, m_hResult and m_message among the member variable of the CItem object. 
		- When the data is already excluded, the message transmitted by the current excluder when it transfers or rejects the exclusivity is recorded in m_message.
		- When requesting to args.m_tag, the tag value input randomly by the user is returned as the original value so you can see this is a response to which request. 

		\~chinese
		- 针对垄断加载请求的响应。
		- 根据请求时输入的搜索条件，再通过args.m_items获得多个数据树。
		- args.m_items中可能包含成功的结果和失败的结果，如果CItem.m_reason的值为 ErrorType_Ok，就表示成功。
		- 如果成功，CItem对象的成员变量中的m_loadedData, m_data, m_sessionGuid, m_rootUUID, m_UUID, m_message记录相关信息。
		- 如果失败，CItem对象的成员变量中的m_reason, m_comment, m_UUID, m_hResult, m_message记录相关信息。
		- 当前的垄断者转让或拒绝垄断权时发生的消息将记录到m_message。
		- 向args.m_tag请求时，用户任意输入的tag将会原值返回，通过这个可以知道是对哪一个请求的响应。

		\~japanese
		- 独占ロード要請に対する応答です。
		- 要請時に入力した検索条件によってargs.m_itemsを通じ複数のデータツリーを受けることもできます。
		- args.m_itemsには独占ロードに成功した結果と失敗した結果が混じっていることがありますが、CItem.m_reason値がErrorType_Okであれば成功した結果です。
		- 成功した場合、CItemオブジェクトのメンバー変数のうち、m_loadedData, m_data, m_sessionGuid, m_rootUUID, m_UUID, m_messageに関連情報が記録されます。
		- 失敗した場合、CItemオブジェクトのメンバー変数のうち、m_reason, m_comment, m_UUID, m_hResult, m_messageに関連情報が記録されます。
		- m_messageは該当データが既に独占になっている場合、既存の独占者が独占権の移譲または拒否をしながら渡したメッセージが記録されます。
		- args.m_tagに要請時にユーザーが任意に入力したtag値がそのまま帰ってきますので、これを通じ、どんな要請に対する応答なのか知れません。


		\~
		*/
		virtual void OnExclusiveLoadDataComplete(CCallbackArgs& args) = 0;

		/**
		\~korean
		다른 DB cache client2가 <a target="_blank" href="http://guide.nettention.com/cpp_ko#exclusive_load" >독점적 불러오기</a> 을 시도하는 바람에 이미 로딩했던 데이터의 독점 접근 권한을 소실할 것인지 결정할수 있는 이벤트 입니다.
		- args.m_loadResults[index].m_loadedData 에 값이 채워집니다.
		- DbCacheClient 의 DenyUnloadData 를 콜 하거나, UnloadDataBySessionGuid 를 콜하여 독점권을 넘겨 줄것인지 결정할수 있습니다.
		\param args IDbCacheClientDelegate2의 이벤트 argument 구조체입니다. 참고 \ref CCallbackArgs

		\~english
		This is event that decide losing exclusive loading or not because other DB cahce client2 tried <a target="_blank" href="http://guide.nettention.com/cpp_en#exclusive_load" >Exclusive Load</a>
		- Value will write to args.m_loadResults[index].m_loadedData
		- It can choose passing exclusive rights with calling DenyUnloadData or UnloadDataBySessionGuid in DbCacheClient
		\param args Event argument object of IDbCacheClientDelegate2

		\~chinese
		因为其他DB cache client2试图 <a target="_blank" href="http://guide.nettention.com/cpp_zh#exclusive_load" >独占性引入</a>%，决定是否丢失已经加载的数据独占接近权限的event。
		-填充值到 args.m_loadResults[index].m_loadedData。
		- 可以拨 DbCacheClient的DenyUnloadData%或者拨 UnloadDataBySessionGuid%，决定是否送出独占权。
		\param args IDbCacheClientDelegate2是event的argument构造体。参考\ref CcallbackArgs%。

		\~japanese
		他のDB cache client2が\ref exclusive_loadを試したため、既にローディングしたデータの独占アクセス権限を消失するかどうかを決定できるイベントです。
		- args.m_loadResults[index].m_loadedDataに値が満たされます。
		- DbCacheClientのDenyUnloadDataを呼び出すか、UnloadDataBySessionGuidを呼び出して独占権を渡すかどうかを決定することができます。
		\param args IDbCacheClientDelegate2のイベントargument構造体です。参考 \ref CCallbackArgs

		*/
		virtual void OnDataUnloadRequested(CCallbackArgs& args) = 0;

		/**
		\~korean
		특정 데이터가 서버로부터 강제로 Unload 되었을 때 발생하는 이벤트입니다.
		현재 독점로드 중인 데이터에 또다른 독점요청이 들어오는 경우 OnDataUnloadRequested()가 콜백되지만,
		<a target="_blank" href="http://guide.nettention.com/cpp_ko#dbc2_isolate_overview" >DB cache가 다루는 데이터를 DB가 직접 억세스하기</a> 를 하려고 하는 경우에는 불가피하게 이 함수가 콜백됩니다.

		\~english
		This is the event when particular data is forcibly unloaded from the server.
		If another exclusivity request is received by the data exclusively loaded currently, OnDataUnloadRequested() is called back, but
		If \ref dbc2_isolate_overview is attempted, inevitably this function is called back. 

		\~chinese
		当特定数据从服务器强制上传时发生的事件。
		当针对当前已被垄断中的数据发出其他的垄断请求，就会回调OnDataUnloadRequested()，如果要进行\ref dbc2_isolate_overview ， 将不可避免地回调此函数。

		\~japanese
		特定データがサーバーから強制にUnloadされた時に発生するイベントです。
		現在独占ロード中であるデータにまた違う独占要請が入る場合、 OnDataUnloadRequested()がコールバックされますが、
		\ref dbc2_isolate_overview をしようとする場合にはやむを得ずにこの関数がコールバックされます。

		*/
		virtual void OnDataForceUnloaded(CCallbackArgs& args) = 0;

		/**
		\~korean
		<a target="_blank" href="http://guide.nettention.com/cpp_ko#dbc2_isolate_overview" >DB cache가 다루는 데이터를 DB가 직접 억세스하기</a> 기능입니다.
		Proud.CDbCacheClient2.RequestIsolateData() 실행 성공을 알립니다.
		args에는 m_rootUUID, m_data, m_sessionGuid가 채워집니다.

		\~english
		<a target="_blank" href="http://guide.nettention.com/cpp_en#dbc2_isolate_overview" >DB directly has access to data that DB cache deals with </a> function.
		Proud.CDbCacheClient2.RequestIsolateData() inform you of execution success.
		m_rootUUID, m_data and m_sessionGuid are included in args.

		\~chinese
		<a target="_blank" href="http://guide.nettention.com/cpp_zh#dbc2_isolate_overview" >DB直接访问DB cache操作的数据</a> 功能。
		告知Proud.CDbCacheClient2.RequestIsolateData() 运行成功。
		在args中将填充 m_rootUUID, m_data, m_sessionGuid。

		\~japanese
		<a target="_blank" href="http://guide.nettention.com/cpp_jp#dbc2_isolate_overview" >DB cacheが操るデータをDBが直接アクセス</a>機能です。
		Proud.CDbCacheClient2.RequestIsolateData()実行の成功を知らせます。
		Argsにはm_rootUUID, m_data, m_sessionGuidが埋められます。
		*/
		virtual void OnIsolateDataSuccess(CCallbackArgs& args) = 0;

		/**
		\~korean
		<a target="_blank" href="http://guide.nettention.com/cpp_ko#dbc2_isolate_overview" >DB cache가 다루는 데이터를 DB가 직접 억세스하기</a> 기능입니다.
		Proud.CDbCacheClient2.RequestIsolateData() 실행 실패를 알립니다.
		args에는 m_rootUUID, m_data, m_sessionGuid가 채워집니다.

		\~english
		<a target="_blank" href="http://guide.nettention.com/cpp_en#dbc2_isolate_overview" >DB directly has access to data that DB cache deals with </a> function.
		Proud.CDbCacheClient2.RequestIsolateData() inform you of execution failure.
		m_rootUUID, m_data and m_sessionGuid are included in args.

		\~chinese
		<a target="_blank" href="http://guide.nettention.com/cpp_zh#dbc2_isolate_overview" >DB直接访问DB cache操作的数据</a> 功能。
		告知Proud.CDbCacheClient2.RequestIsolateData() 运行失败。
		在args将填充 m_rootUUID, m_data, m_sessionGuid。

		\~japanese
		<a target="_blank" href="http://guide.nettention.com/cpp_jp#dbc2_isolate_overview" >DB cacheが操るデータをDBが直接アクセス</a>機能です。
		Proud.CDbCacheClient2.RequestIsolateData() 実行の失敗を知らせます。
		Argsにはm_rootUUID, m_data, m_sessionGuidが埋められます。
		*/
		virtual void OnIsolateDataFailed(CCallbackArgs& args) = 0;

		/**
		\~korean
		<a target="_blank" href="http://guide.nettention.com/cpp_ko#dbc2_isolate_overview" >DB cache가 다루는 데이터를 DB가 직접 억세스하기</a> 기능입니다.
		Proud.CDbCacheClient2.RequestDeisolateData() 실행 성공을 알립니다.
		args에는 m_rootUUID, m_data, m_sessionGuid가 채워집니다.

		\~english
		<a target="_blank" href="http://guide.nettention.com/cpp_en#dbc2_isolate_overview" >DB directly has access to data that DB cache deals with </a> function.
		Proud.CDbCacheClient2.RequestDeisolateData() inform you of execution success.
		m_rootUUID, m_data and m_sessionGuid are included in args.

		\~chinese
		<a target="_blank" href="http://guide.nettention.com/cpp_zh#dbc2_isolate_overview" >DB直接访问DB cache操作的数据</a> 功能。
		告知Proud.CDbCacheClient2.RequestDeisolateData() 运行成功。
		在args中将填充 m_rootUUID, m_data, m_sessionGuid。

		\~japanese
		<a target="_blank" href="http://guide.nettention.com/cpp_jp#dbc2_isolate_overview" >DB cacheが操るデータをDBが直接アクセス</a>機能です。
		Proud.CDbCacheClient2.RequestDeisolateData() 実行の成功を知らせます。
		Argsには m_rootUUID, m_data, m_sessionGuidが埋められます。
		*/
		virtual void OnDeisolateDataSuccess(CCallbackArgs& args) = 0;

		/**
		\~korean
		<a target="_blank" href="http://guide.nettention.com/cpp_ko#dbc2_isolate_overview" >DB cache가 다루는 데이터를 DB가 직접 억세스하기</a> 기능입니다.
		Proud.CDbCacheClient2.RequestDeisolateData() 실행 실패를 알립니다.
		args에는 m_rootUUID, m_data, m_sessionGuid가 채워집니다.

		\~english
		<a target="_blank" href="http://guide.nettention.com/cpp_en#dbc2_isolate_overview" >DB directly has access to data that DB cache deals with </a> function.
		Proud.CDbCacheClient2.RequestDeisolateData() inform you of execution failure.
		m_rootUUID, m_data and m_sessionGuid are included in args.

		\~chinese
		<a target="_blank" href="http://guide.nettention.com/cpp_zh#dbc2_isolate_overview" >DB直接访问DB cache操作的数据</a> 功能。
		告知Proud.CDbCacheClient2.RequestDeisolateData() 运行失败。
		在args中将填充 m_rootUUID, m_data, m_sessionGuid。

		\~japanese
		<a target="_blank" href="http://guide.nettention.com/cpp_jp#dbc2_isolate_overview" >DB cacheが操るデータをDBが直接アクセス</a>機能です。
		Proud.CDbCacheClient2.RequestDeisolateData()実行の失敗を知らせます。.
		Argsには m_rootUUID, m_data, m_sessionGuidが埋められます。
		*/
		virtual void OnDeisolateDataFailed(CCallbackArgs& args) = 0;

		/**
		\~korean
		CDbCacheClient2.RequestAddData, CDbCacheClient2.BlockedAddData 에서 실패했을때 도착하는 이벤트입니다.
		- args.m_loadResults[index].m_reason,args.m_loadResults[index].m_comment에 값이 채워집니다.
		\param args IDbCacheClientDelegate2의 이벤트 argument 구조체입니다. 참고 \ref CCallbackArgs

		\~english
		This event will arrive when it failed in CDbCacheClient2.RequestAddData or CDbCacheClient2.BlockedAddData
		- Value will write to args.m_loadResults[index].m_reason, args.m_loadResults[index].m_comment
		\param args Event argument object of IDbCacheClientDelegate2

		\~chinese
		在 CDbCacheClient2.RequestAddData, CDbCacheClient2.BlockedAddData%失败时到达的Event。
		- 填充值到 args.m_loadResults[index].m_reason,args.m_loadResults[index].m_comment。
		\param args IDbCacheClientDelegate2%的事件argument构造体。参考\ref CCallbackArgs%。

		\~japanese
		CDbCacheClient2.RequestAddData, CDbCacheClient2.BlockedAddDataで失敗した時に到着するイベントです。
		- args.m_loadResults[index].m_reason,args.m_loadResults[index].m_commentに値が満たされます。
		\param args IDbCacheClientDelegate2のイベントargumentの構造体です。参考 \ref CCallbackArgs

		\~
		*/
		virtual void OnAddDataFailed(CCallbackArgs& args) = 0;

		/**
		\~korean
		CDbCacheClient2.RequestAddData, CDbCacheClient2.BlockedAddData 에서 성공했을때 도착하는 이벤트입니다.
		- args.m_loadResults[index].m_loadedData,args.m_loadResults[index].m_data에 값이 채워집니다.
		\param args IDbCacheClientDelegate2의 이벤트 argument 구조체입니다. 참고 \ref CCallbackArgs

		\~english
		This event will arrive when it succeed in CDbCacheClient2.RequestAddData or CDbCacheClient2.BlockedAddData
		- Value will write to args.m_loadResults[index].m_reason,args.m_loadResults[index].m_comment
		\param args Event argument object of IDbCacheClientDelegate2

		\~chinese
		在 CDbCacheClient2.RequestAddData, CDbCacheClient2.BlockedAddData%成功时到达的event。
		- 往 args.m_loadResults[index].m_loadedData,args.m_loadResults[index].m_data 填充值。
		\param args IDbCacheClientDelegate2是event的argument构造体。参考\ref CCallbackArgs%。

		\~japanese
		CDbCacheClient2.RequestAddData, CDbCacheClient2.BlockedAddData で成功した時に到着するイベントです。
		- args.m_loadResults[index].m_loadedData,args.m_loadResults[index].m_dataに値が満たされます。
		\param args IDbCacheClientDelegate2のイベントargument構造体です。参考 \ref CCallbackArgs

		*/
		virtual void OnAddDataSuccess(CCallbackArgs& args) = 0;

		/**
		\~korean
		\ref OnAddDataFailed 참고.
		- args.m_loadResults[index].m_reason,args.m_loadResults[index].m_comment에 값이 채워집니다.
		\param args IDbCacheClientDelegate2의 이벤트 argument 구조체입니다. 참고 \ref CCallbackArgs

		\~english
		Refer to OnAddDataFailed
		- Value will write to args.m_loadResults[index].m_reason,args.m_loadResults[index].m_comment
		\param args Event argument object of IDbCacheClientDelegate2

		\~chinese
		参考\ref OnAddDataFailed%。
		- 填充值到 args.m_loadResults[index].m_reason,args.m_loadResults[index].m_comment。
		\param args IDbCacheClientDelegate2是event的argument构造体。参考\ref CCallbackArgs%。

		\~japanese
		\ref OnAddDataFailed 参考
		- args.m_loadResults[index].m_reason,args.m_loadResults[index].m_commentに値が満たされます。
		\param args IDbCacheClientDelegate2のイベントargument構造体です。参考 \ref CCallbackArgs

		\~
		*/
		virtual void OnUpdateDataFailed(CCallbackArgs& args) = 0;

		/**
		\~korean
		\ref OnAddDataSuccess 참고.
		-  args.m_loadResults[index].m_loadedData,args.m_loadResults[index].m_data에 값이 채워집니다.
		\param args IDbCacheClientDelegate2의 이벤트 argument 구조체입니다. 참고 \ref CCallbackArgs

		\~english
		Refer to OnAddDataSuccess
		- Value will write to args.m_loadResults[index].m_loadedData,args.m_loadResults[index].m_data
		\param args Event argument object of IDbCacheClientDelegate2

		\~chinese
		参考 \ref OnAddDataSuccess%。
		-填充值到 args.m_loadResults[index].m_loadedData,args.m_loadResults[index].m_data。
		\param args IDbCacheClientDelegate2%是event的argument构造体。参考\ref CCallbackArgs%。

		\~japanese
		\ref OnAddDataSuccess 参考
		-  args.m_loadResults[index].m_loadedData,args.m_loadResults[index].m_dataに値が満たされます。
		\param args IDbCacheClientDelegate2のイベントargument構造体です。参考 \ref CCallbackArgs

		\~
		*/
		virtual void OnUpdateDataSuccess(CCallbackArgs& args) = 0;

		/**
		\~korean
		\ref OnAddDataFailed 참고.
		-  args.m_loadResults[index].m_reason,args.m_loadResults[index].m_comment에 값이 채워집니다.
		\param args IDbCacheClientDelegate2의 이벤트 argument 구조체입니다. 참고 \ref CCallbackArgs

		\~english
		Refer to OnAddDataFailed
		- Value will write to args.m_loadResults[index].m_reason,args.m_loadResults[index].m_comment
		\param args Event argument object of IDbCacheClientDelegate2

		\~chinese
		参考 \ref OnAddDataFailed%。
		- 填充值到 args.m_loadResults[index].m_reason,args.m_loadResults[index].m_comment。
		\param args IDbCacheClientDelegate2%是event的argument构造体。参考\ref CCallbackArgs%。

		\~japanese
		\ref OnAddDataFailed 参考
		-  args.m_loadResults[index].m_reason,args.m_loadResults[index].m_commentに値が満たされます。
		\param args IDbCacheClientDelegate2のイベントargument構造体です。参考 \ref CCallbackArgs

		\~
		*/
		virtual void OnRemoveDataFailed(CCallbackArgs& args) = 0;

		/**
		\~korean
		\ref OnAddDataFailed 참고.
		-  args.m_loadResults[index].m_reason,args.m_loadResults[index].m_comment에 값이 채워집니다.
		\param args IDbCacheClientDelegate2의 이벤트 argument 구조체입니다. 참고 \ref CCallbackArgs

		\~english
		Refer to OnAddDataFailed
		- Value will write to args.m_loadResults[index].m_reason,args.m_loadResults[index].m_comment
		\param args Event argument object of IDbCacheClientDelegate2

		\~chinese
		参考 \ref OnAddDataFailed%。
		-填充值到 args.m_loadResults[index].m_reason,args.m_loadResults[index].m_comment。
		\param args IDbCacheClientDelegate2%是event的argument构造体。参考\ref CCallbackArgs%。

		\~japanese
		\ref OnAddDataFailed 参考
		-  args.m_loadResults[index].m_reason,args.m_loadResults[index].m_commentに値が満たされます。
		\param args IDbCacheClientDelegate2のイベントargument構造体です。参考 \ref CCallbackArgs

		\~
		*/
		virtual void OnRemoveDataSuccess(CCallbackArgs& args) = 0;

		/**
		\~korean
		- 비독점 조회 요청에 대한 응답입니다.
		- 요청 시 입력한 검색 조건에 따라 args.m_items를 통해 복수의 데이터 트리를 받을 수도 있습니다.
		- args.m_items에는 조회에 성공한 결과와 실패한 결과가 섞여있을 수 있는데 CItem.m_reason값이 ErrorType_Ok이면 성공한 결과입니다.
		- 성공한 경우 CItem객체의 멤버변수 중 m_loadedData, m_data, m_sessionGuid, m_rootUUID, m_UUID에 관련 정보가 기록됩니다.
		- 실패한 경우 CItem객체의 멤버변수 중 m_reason, m_comment, m_UUID, m_hResult에 관련 정보가 기록됩니다.
		- args.m_tag에 요청 시 사용자가 임의로 입력했던 tag값이 그대로 돌아오므로 이를 통해 어떤 요청에 대한 응답인지 알 수 있습니다.

		\~english
		- This is the request for the nonexclusive search request. 
		- When requesting, you may receive multiple data trees through args.m_items depending on the input search conditions
		- args.m_items may have successful results and failed results mixed for search success and if the value of CItem.m_reason is ErrorType_Ok, then this is a successful result.
		- If successful, the related information is recorded in m_loadedData, m_data, m_sessionGuid, m_rootUUID and m_UUID among the member variables of the CItem object.
		- If failed, the related information is recorded in m_reason, m_comment, m_UUID and m_hResult among the member variable of the CItem object.
		- When requesting to args.m_tag, the tag value input randomly by the user is returned as the original value so you can see this is a response to which request.


		\~chinese
		- 针对非垄断查询请求的响应。
		- 根据请求时输入的搜索条件，再通过args.m_items获得多个数据树。
		- args.m_items中可能包含成功的结果和失败的结果，如果CItem.m_reason的值为 ErrorType_Ok，就表示成功。
		- 如果成功，CItem对象的成员变量中的m_loadedData, m_data, m_sessionGuid, m_rootUUID, m_UUID, m_message记录相关信息。
		- 如果失败，CItem对象的成员变量中的m_reason, m_comment, m_UUID, m_hResult, m_message记录相关信息。
		- 向args.m_tag请求时，用户任意输入的tag将会原值返回，通过这个可以知道是对哪一个请求的响应。

		\~japanese
		- 非独占照会要請に対する応答です。
		- 要請時に入力した検索条件によってargs.m_itemsを通じ複数のデータツリーを受けることもできます。
		- args.m_itemsには照会に成功した結果と失敗した結果が混じられますが、CItem.m_reason値がErrorType_Okであれば成功した結果です。
		- 成功した場合、CItemオブジェクトのメンバー変数のうち、m_loadedData, m_data, m_sessionGuid, m_rootUUID, m_UUIDに関連情報が記録されます。
		- 失敗した場合、CItemオブジェクトのメンバー変数のうち、m_reason, m_comment, m_UUID, m_hResultに関連情報が記録されます。	
		- args.m_tagに要請時にユーザーが任意に入力したtag値がそのまま帰ってくるので、これを通じどんな要請に対する応答なのか知られます。

		\~
		*/
		virtual void OnNonExclusiveSnapshotDataComplete(CCallbackArgs& args) { args; }

		/**
		\~korean
		비독점적 접근에 대한 콜백. <a target="_blank" href="http://guide.nettention.com/cpp_ko#dbc2_nonexclusive_overview" >비독점적 데이터 접근하기</a> 참고.
		- 실패시엔  args.m_loadResults[index].m_reason, args.m_loadResults[index].m_comment에 값이 채워집니다.
		- 성공시엔  args.m_loadResults[index].m_rootUUID, args.m_loadResults[index].m_data에 값이 채워집니다.
		\param args IDbCacheClientDelegate2 의 이벤트 argument 구조체입니다. 참고 \ref CCallbackArgs

		\~english
		A callback to non monopolistic approach. Please refer to <a target="_blank" href="http://guide.nettention.com/cpp_en#dbc2_nonexclusive_overview" >Accessing non-exclusive data</a>.
		- Value will write in to args.m_loadResults[index].m_reason, args.m_loadResults[index].m_comment when it succeed.
		- Value will write in to args.m_loadResults[index].m_rootUUID, args.m_loadResults[index].m_data when it failed.
		\param args Event argument object of IDbCacheClientDelegate2.

		\~chinese
		对非独占性接近的回调。请参考<a target="_blank" href="http://guide.nettention.com/cpp_zh#dbc2_nonexclusive_overview" >访问共享性数据</a>%。
		- 失败时填充值到  args.m_loadResults[index].m_reason,args.m_loadResults[index].m_comment。
		- 成功时填充值到  args.m_loadResults[index].m_rootUUID,args.m_loadResults[index].m_data。
		\param args IDbCacheClientDelegate2%是event的argument构造体。参考\ref CCallbackArgs%。

		\~japanese
		非独占的接近に対するコールバック <a target="_blank" href="http://guide.nettention.com/cpp_jp#dbc2_nonexclusive_overview" >Accessing non-exclusive data</a> 参考
		- 失敗時には  args.m_loadResults[index].m_reason, args.m_loadResults[index].m_commentに値が埋められます。
		- 成功時には  args.m_loadResults[index].m_rootUUID, args.m_loadResults[index].m_removeUUIDに値が埋められます。
		\param args IDbCacheClientDelegate2 のイベント argument 構造体です。 参考 \ref CCallbackArgs

		\~
		*/
		virtual void OnNonExclusiveAddDataAck(CCallbackArgs& args) { args; }

		/**
		\~korean
		비독점적 접근에 대한 콜백. <a target="_blank" href="http://guide.nettention.com/cpp_ko#dbc2_nonexclusive_overview" >비독점적 데이터 접근하기</a> 참고.
		- 실패시엔  args.m_loadResults[index].m_reason, args.m_loadResults[index].m_comment에 값이 채워집니다.
		- 성공시엔  args.m_loadResults[index].m_rootUUID, args.m_loadResults[index].m_removeUUID에 값이 채워집니다.
		\param args IDbCacheClientDelegate2 의 이벤트 argument 구조체입니다. 참고 \ref CCallbackArgs

		\~english
		A callback to non monopolistic approach. Please refer to <a target="_blank" href="http://guide.nettention.com/cpp_en#dbc2_nonexclusive_overview" >Accessing non-exclusive data</a>.
		- Value will write in to args.m_loadResults[index].m_reason, args.m_loadResults[index].m_comment when it succeed.
		- Value will write in to args.m_loadResults[index].m_rootUUID, args.m_loadResults[index].m_data when it failed.
		\param args Event argument object of IDbCacheClientDelegate2.

		\~chinese
		对非独占性接近的回调。请参考<a target="_blank" href="http://guide.nettention.com/cpp_zh#dbc2_nonexclusive_overview" >访问共享性数据</a>%。
		- 失败时填充值到  args.m_loadResults[index].m_reason, args.m_loadResults[index].m_comment。
		- 成功时填充值到  args.m_loadResults[index].m_rootUUID, args.m_loadResults[index].m_removeUUID。
		\param args IDbCacheClientDelegate2是event的argument构造体。参考\ref CCallbackArgs%。

		\~japanese
		非独占的接近に対するコールバック<a target="_blank" href="http://guide.nettention.com/cpp_jp#dbc2_nonexclusive_overview" >Accessing non-exclusive data</a>参考
		- 失敗時には  args.m_loadResults[index].m_reason, args.m_loadResults[index].m_commentに値が埋められます。
		- 成功時には  args.m_loadResults[index].m_rootUUID, args.m_loadResults[index].m_removeUUIDに値が埋められます。
		\param args IDbCacheClientDelegate2 のイベント argument 構造体です。 参考 \ref CCallbackArgs


		\~
		*/
		virtual void OnNonExclusiveRemoveDataAck(CCallbackArgs& args) { args; }


		/**
		\~korean
		비독점적 접근에 대한 콜백. <a target="_blank" href="http://guide.nettention.com/cpp_ko#dbc2_nonexclusive_overview" >비독점적 데이터 접근하기</a> 참고.
		-  args.m_loadResults[index].m_rootUUID, args.m_loadResults[index].m_data에 값이 채워집니다.
		\param args IDbCacheClientDelegate2 의 이벤트 argument 구조체입니다. 참고 \ref CCallbackArgs

		\~english
		A callback to non monopolistic approach. Please refer to <a target="_blank" href="http://guide.nettention.com/cpp_en#dbc2_nonexclusive_overview" >Accessing non-exclusive data</a>.
		- Value will write in to args.m_loadResults[index].m_rootUUID, args.m_loadResults[index].m_data.
		\param args Event argument object of IDbCacheClientDelegate2.

		\~chinese
		对非独占性接近的回调。请参考<a target="_blank" href="http://guide.nettention.com/cpp_zh#dbc2_nonexclusive_overview" >访问共享性数据</a>%。
		- 填充值到  args.m_loadResults[index].m_rootUUID, args.m_loadResults[index].m_data。
		\param args IDbCacheClientDelegate2%是event的argument构造体。参考\ref CCallbackArgs%。

		\~japanese
		非独占的接近に対するコールバック <a target="_blank" href="http://guide.nettention.com/cpp_jp#dbc2_nonexclusive_overview" >Accessing non-exclusive data</a> 参考
		-  args.m_loadResults[index].m_rootUUID, args.m_loadResults[index].m_dataに値が埋められます。
		\param args IDbCacheClientDelegate2 のイベントargument 構造体です。 参考 \ref CCallbackArgs

		\~
		*/
		virtual void OnNonExclusiveModifyValueSuccess(CCallbackArgs& args) { args; }

		/**
		\~korean
		비독점적 접근에 대한 콜백. <a target="_blank" href="http://guide.nettention.com/cpp_ko#dbc2_nonexclusive_overview" >비독점적 데이터 접근하기</a> 참고.
		-  args.m_loadResults[index].m_reason, args.m_loadResults[index].m_comment에 값이 채워집니다.
		\param args IDbCacheClientDelegate2 의 이벤트 argument 구조체입니다. 참고 \ref CCallbackArgs

		\~english
		A callback to non monopolistic approach. Please refer to <a target="_blank" href="http://guide.nettention.com/cpp_en#dbc2_nonexclusive_overview" >Accessing non-exclusive data</a>.
		- Value will write in to args.m_loadResults[index].m_reason, args.m_loadResults[index].m_comment.
		\param args Event argument object of IDbCacheClientDelegate2.

		\~chinese
		对非独占性接近的回调。请参考<a target="_blank" href="http://guide.nettention.com/cpp_zh#dbc2_nonexclusive_overview" >访问共享性数据</a>%。
		- 填充值到  args.m_loadResults[index].m_reason, args.m_loadResults[index].m_comment。
		\param args IDbCacheClientDelegate2%的事件argument构造体。参考\ref CCallbackArgs%。

		\~japanese
		非独占的接近に対するコールバック <a target="_blank" href="http://guide.nettention.com/cpp_jp#dbc2_nonexclusive_overview" >Accessing non-exclusive data</a> 参考
		-  args.m_loadResults[index].m_reason, args.m_loadResults[index].m_commentに値が埋められます。
		\param args IDbCacheClientDelegate2 のイベントargument 構造体です。 参考 \ref CCallbackArgs

		\~
		*/
		virtual void OnNonExclusiveModifyValueFailed(CCallbackArgs& args) { args; }

		/**
		\~korean
		비독점적 접근에 대한 콜백. <a target="_blank" href="http://guide.nettention.com/cpp_ko#dbc2_nonexclusive_overview" >비독점적 데이터 접근하기</a> 참고.
		-  args.m_loadResults[index].m_rootUUID, args.m_loadResults[index].m_data에 값이 채워집니다.
		\param args IDbCacheClientDelegate2 의 이벤트 argument 구조체입니다. 참고 \ref CCallbackArgs

		\~english
		A callback to non monopolistic approach. Please refer to <a target="_blank" href="http://guide.nettention.com/cpp_en#dbc2_nonexclusive_overview" >Accessing non-exclusive data</a>.
		- Value will write in to args.m_loadResults[index].m_rootUUID, args.m_loadResults[index].m_data.
		\param args Event argument object of IDbCacheClientDelegate2.

		\~chinese
		对非独占性接近的回调。请参考<a target="_blank" href="http://guide.nettention.com/cpp_zh#dbc2_nonexclusive_overview" >访问共享性数据</a>%。
		- 填充值到  args.m_loadResults[index].m_rootUUID, args.m_loadResults[index].m_data。
		\param args IDbCacheClientDelegate2%的事件argument构造体。参考\ref CCallbackArgs%。

		\~japanese
		非独占的接近に対するコールバック <a target="_blank" href="http://guide.nettention.com/cpp_jp#dbc2_nonexclusive_overview" >Accessing non-exclusive data</a> 参考
		-  args.m_loadResults[index].m_rootUUID, args.m_loadResults[index].m_dataに値が埋められます。
		\param args IDbCacheClientDelegate2 のイベントargument 構造体です。 参考 \ref CCallbackArgs

		\~
		*/
		virtual void OnNonExclusiveSetValueIfSuccess(CCallbackArgs& args) { args; }

		/**
		\~korean
		비독점적 접근에 대한 콜백. <a target="_blank" href="http://guide.nettention.com/cpp_ko#dbc2_nonexclusive_overview" >비독점적 데이터 접근하기</a> 참고.
		-  args.m_loadResults[index].m_reason, args.m_loadResults[index].m_comment에 값이 채워집니다.
		\param args IDbCacheClientDelegate2 의 이벤트 argument 구조체입니다. 참고 \ref CCallbackArgs

		\~english
		A callback to non monopolistic approach. Please refer to <a target="_blank" href="http://guide.nettention.com/cpp_en#dbc2_nonexclusive_overview" >Accessing non-exclusive data</a>.
		- Value will write in to args.m_loadResults[index].m_reason, args.m_loadResults[index].m_comment.
		\param args Event argument object of IDbCacheClientDelegate2.

		\~chinese
		对非独占性接近的回调。请参考<a target="_blank" href="http://guide.nettention.com/cpp_zh#dbc2_nonexclusive_overview" >访问共享性数据</a>%。
		- 填充值到  args.m_loadResults[index].m_reason, args.m_loadResults[index].m_comment。
		\param args IDbCacheClientDelegate2%的事件argument构造体。参考\ref CCallbackArgs%。

		\~japanese
		非独占的接近に対するコールバック <a target="_blank" href="http://guide.nettention.com/cpp_jp#dbc2_nonexclusive_overview" >Accessing non-exclusive data</a> 参考
		-  args.m_loadResults[index].m_reason, args.m_loadResults[index].m_commentに値が埋められます。
		\param args IDbCacheClientDelegate2 のイベントargument 構造体です。 参考 \ref CCallbackArgs

		\~
		*/
		virtual void OnNonExclusiveSetValueIfFailed(CCallbackArgs& args) { args; }

		/**
		\~korean
		비독점적 접근에 대한 콜백. <a target="_blank" href="http://guide.nettention.com/cpp_ko#dbc2_nonexclusive_overview" >비독점적 데이터 접근하기</a> 참고.
		- args.m_loadResults[index].m_loadedData, args.m_loadResults[index].m_data, args.m_loadResults[index].m_message에 값이 채워집니다.
		\param args IDbCacheClientDelegate2 의 이벤트 argument 구조체입니다. 참고 \ref CCallbackArgs

		\~english
		A callback to non monopolistic approach. Please refer to <a target="_blank" href="http://guide.nettention.com/cpp_en#dbc2_nonexclusive_overview" >Accessing non-exclusive data</a>.
		- Value will write in to args.m_loadResults[index].m_loadedData and args.m_loadResults[index].m_data,  args.m_loadResults[index].m_message.
		\param args Event argument object of IDbCacheClientDelegate2.

		\~chinese
		对非独占性接近的回调。请参考<a target="_blank" href="http://guide.nettention.com/cpp_zh#dbc2_nonexclusive_overview" >访问共享性数据</a>%。
		- 填充值到 args.m_loadResults[index].m_loadedData, args.m_loadResults[index].m_data, args.m_loadResults[index].m_message。
		\param args IDbCacheClientDelegate2的事件argument构造体。参考\ref CCallbackArgs%。

		\~japanese
		非独占的接近に対するコールバック <a target="_blank" href="http://guide.nettention.com/cpp_jp#dbc2_nonexclusive_overview" >Accessing non-exclusive data</a> 参考
		- args.m_loadResults[index].m_loadedData, args.m_loadResults[index].m_data, args.m_loadResults[index].m_messageに値が埋められます。
		\param args IDbCacheClientDelegate2 のイベントargument 構造体です。 参考 \ref CCallbackArgs

		\~
		*/
		virtual void OnSomeoneAddData(CCallbackArgs& args) { args; }

		/**
		\~korean
		비독점적 접근에 대한 콜백. <a target="_blank" href="http://guide.nettention.com/cpp_ko#dbc2_nonexclusive_overview" >비독점적 데이터 접근하기</a> 참고.
		- args.m_loadResults[index].m_loadedData, args.m_loadResults[index].m_data, args.m_loadResults[index].m_message에 값이 채워집니다.
		\param args IDbCacheClientDelegate2 의 이벤트 argument 구조체입니다. 참고 \ref CCallbackArgs

		\~english
		A callback to non monopolistic approach. Please refer to <a target="_blank" href="http://guide.nettention.com/cpp_en#dbc2_nonexclusive_overview" >Accessing non-exclusive data</a>.
		- Value will write in to args.m_loadResults[index].m_loadedData and args.m_loadResults[index].m_data,  args.m_loadResults[index].m_message.
		\param args Event argument object of IDbCacheClientDelegate2.

		\~chinese
		对非独占性接近的回调。请参考<a target="_blank" href="http://guide.nettention.com/cpp_zh#dbc2_nonexclusive_overview" >访问共享性数据</a>%。
		- 填充值到 args.m_loadResults[index].m_loadedData, args.m_loadResults[index].m_data,  args.m_loadResults[index].m_message。
		\param args IDbCacheClientDelegate2%是event的argument构造体。参考\ref CCallbackArgs%。

		\~japanese
		非独占的接近に対するコールバック <a target="_blank" href="http://guide.nettention.com/cpp_jp#dbc2_nonexclusive_overview" >Accessing non-exclusive data</a> 参考
		- args.m_loadResults[index].m_loadedData, args.m_loadResults[index].m_data,  args.m_loadResults[index].m_messageに値が埋められます。
		\param args IDbCacheClientDelegate2 のイベントargument 構造体です。 参考 \ref CCallbackArgs

		\~
		*/
		virtual void OnSomeoneRemoveData(CCallbackArgs& args) { args; }

		/**
		\~korean
		비독점적 접근에 대한 콜백. <a target="_blank" href="http://guide.nettention.com/cpp_ko#dbc2_nonexclusive_overview" >비독점적 데이터 접근하기</a> 참고.
		- args.m_loadResults[index].m_loadedData, args.m_loadResults[index].m_data,  args.m_loadResults[index].m_message에 값이 채워집니다.
		\param args IDbCacheClientDelegate2 의 이벤트 argument 구조체입니다. 참고 \ref CCallbackArgs

		\~english
		A callback to non monopolistic approach. Please refer to <a target="_blank" href="http://guide.nettention.com/cpp_en#dbc2_nonexclusive_overview" >Accessing non-exclusive data</a>.
		- Value will write in to args.m_loadResults[index].m_loadedData and args.m_loadResults[index].m_data,  args.m_loadResults[index].m_message.
		\param args Event argument object of IDbCacheClientDelegate2.

		\~chinese
		对非独占性接近的回调。请参考<a target="_blank" href="http://guide.nettention.com/cpp_zh#dbc2_nonexclusive_overview" >访问共享性数据</a>%。
		- 填充值到 args.m_loadResults[index].m_loadedData, args.m_loadResults[index].m_data,  args.m_loadResults[index].m_message。
		\param args IDbCacheClientDelegate2%是event的argument构造体。参考\ref CCallbackArgs%。

		\~japanese
		非独占的接近に対するコールバック <a target="_blank" href="http://guide.nettention.com/cpp_jp#dbc2_nonexclusive_overview" >Accessing non-exclusive data</a> 参考
		- args.m_loadResults[index].m_loadedData, args.m_loadResults[index].m_data,  args.m_loadResults[index].m_messageに値が埋められます。
		\param args IDbCacheClientDelegate2 のイベントargument 構造体です。 参考 \ref CCallbackArgs

		\~
		*/
		virtual void OnSomeoneModifyValue(CCallbackArgs& args) { args; }

		/**
		\~korean
		비독점적 접근에 대한 콜백. <a target="_blank" href="http://guide.nettention.com/cpp_ko#dbc2_nonexclusive_overview" >비독점적 데이터 접근하기</a> 참고.
		- args.m_loadResults[index].m_loadedData, args.m_loadResults[index].m_data,  args.m_loadResults[index].m_message에 값이 채워집니다.
		\param args IDbCacheClientDelegate2 의 이벤트 argument 구조체입니다. 참고 \ref CCallbackArgs

		\~english
		A callback to non monopolistic approach. Please refer to <a target="_blank" href="http://guide.nettention.com/cpp_en#dbc2_nonexclusive_overview" >Accessing non-exclusive data</a>.
		- Value will write in to args.m_loadResults[index].m_loadedData and args.m_loadResults[index].m_data,  args.m_loadResults[index].m_message.
		\param args Event argument object of IDbCacheClientDelegate2.

		\~chinese
		对非独占性接近的回调。请参考<a target="_blank" href="http://guide.nettention.com/cpp_zh#dbc2_nonexclusive_overview" >访问共享性数据</a>%。
		- 填充值到 args.m_loadResults[index].m_loadedData, args.m_loadResults[index].m_data, args.m_loadResults[index].m_message。
		\param args IDbCacheClientDelegate2%是event的argument构造体。参考\ref CCallbackArgs%。

		\~japanese
		非独占的接近に対するコールバック <a target="_blank" href="http://guide.nettention.com/cpp_jp#dbc2_nonexclusive_overview" >Accessing non-exclusive data</a> 参考
		- args.m_loadResults[index].m_loadedData, args.m_loadResults[index].m_data, args.m_loadResults[index].m_messageに値が埋められます。
		\param args IDbCacheClientDelegate2 のイベントargument 構造体です。 参考 \ref CCallbackArgs

		\~
		*/
		virtual void OnSomeoneSetValue(CCallbackArgs& args) { args; }

		/**
		\~korean
		Data Access 에러
		- 콜백되는 예 : 요청형 메서드가 실행되는 사이에 일방적 메서드가 호출된경우
		업데이트시에 업데이트할 데이터를 못찾는 경우, 혹은 지워야할 데이터를 못찾는 경우 등등에 이 이벤트가 호출됩니다.
		\param args IDbCacheClientDelegate2의 이벤트 argument 구조체입니다. 참고 \ref CCallbackArgs

		\~english
		Data Access error
		- Example of callback : This event will call some situation such as one-sided method is called while requested method is running, could not find update data or could not find data that need to delete.
		\param args Event argument object of IDbCacheClientDelegate2

		\~chinese
		Data Access 错误
		- 回调的例：邀请型方法实行期间，单方面的方法呼叫的情况，升级时找不到升级数据的情况，或者没有找到要删除的数据的情况等时候此event会被呼叫。
		\param args IDbCacheClientDelegate2是event的argument构造体。参考\ref CCallbackArgs%。

		\~japanese
		Data Access エラー
		- コールバックされる例：要請型メソッドが実行される間に一方的にメソッドが呼び出される場合
		アップデート時にアップデートするデータが見つからない場合、または削除すべきデータが見つからない場合などにこのイベントが呼び出されます。
		\param args IDbCacheClientDelegate2のイベントargument構造体です。参考 \ref CCallbackArgs

		\~
		*/
		virtual void OnAccessError(CCallbackArgs& args) = 0;

		/**
		\~korean
		내부 에러 발생시 호출됩니다
		\param errorInfo 에러에 대한 정보입니다. ErrorInfo::ToString 으로 편하게 자세한 Error 정보를 확인하실 수 있습니다.

		\~english
		It calls when it occurs internal error.
		Information of \param errorinfo error.

		\~chinese
		内部发生错误时会被呼叫。
		\param errorInfo 对错误的信息。利用 ErrorInfo::ToString()%的话能方便的看出详细的Error信息。

		\~japanese
		内部エラーが発生時に呼び出されます。
		\param errorInfo エラーに対する情報です。ErrorInfo::ToStringで楽に詳しいError情報を確認することができます。

		\~
		*/
		virtual void OnError(ErrorInfo *errorInfo) = 0;

		/**
		\~korean
		내부 경고 발생시 호출됩니다. 에러보다는 낮은 수위의 문제를 보고함.
		\param errorInfo 에러에 대한 정보입니다. ErrorInfo::ToString 으로 편하게 자세한 Error 정보를 확인하실 수 있습니다.

		\~english
		It calls when it occurs internal warning. It reports problem that not serious than error.
		Information of \param errorinfo error.

		\~chinese
		内部发生警告时会被呼叫。报告比错误水平低的问题。
		\param errorInfo 对错误的信息。利用 ErrorInfo::ToString()%的话能方便的看出详细的Error信息。

		\~japanese
		内部警告が発生時に呼び出されます。エラーよりは低いレベルの問題を報告します。
		\param errorInfo エラーに関する情報です。ErrorInfo::ToString で楽に詳しいError情報を確認することができます。

		\~
		*/
		virtual void OnWarning(ErrorInfo *errorInfo) = 0;

		/**
		\~korean
		Proud.Exception 으로 통합되었습니다.
		- 일반적으로 유저 코드에서 나는 에러를 처리합니다.
		- 유저는 필요한 것들을 구현하면 됩니다.
		- ExceptionType 으로 해당 Exception 객체를 얻어서 사용하면 됩니다.
		- _com_error, void*, std::exception 등을 모두 처리합니다.
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
		统一成 Proud.Exception%。
		- 一般处理从用户代码发生的错误。
		- 用户只要体现需要的东西。
		- 用ExceptionType获得有关Exception个体后使用即可。
		- 处理_com_error, void*, std::exception 等全部。
		还有处理catch(...)的OnUnhandledException()。
		- 选择例外对象的类型以后可以接收void*。比如接收MFC CException 的时候使用。

		\~japanese
		Proud.Exception で統合されました。
		- 一般的にユーザーコードから出るエラーを処理します。
		- ユーザーは必要なことを実現すれば良いです。
		- ExceptionType で該当Exceptionのオブジェクトを得て使えば良いです。
		- _com_error, void*, std::exception などを全て処理します。
		また、catch(...)を処理するOnUnhandledException()もあります。
		- void*は例外オブジェクトのタイプをキャスティングしてもらうことができます。仮にMFC CExceptionを受けることに使われます。

		\~
		*/
		virtual void OnException(const Exception &e) = 0;

	};

	/**
	\~korean
	<a target="_blank" href="http://guide.nettention.com/cpp_ko#dbc2_client_server" >DB cache 클라이언트와 서버</a> 입니다.

	\~english
	<a target="_blank" href="http://guide.nettention.com/cpp_en#dbc2_client_server" >DB cache client and server</a>

	\~chinese
	是 <a target="_blank" href="http://guide.nettention.com/cpp_zh#dbc2_client_server" >DB cache客户端和服务器</a> %。

	\~japanese
	\ref dbc2_clientです。
	\~
	*/
	class CDbCacheClient2
	{
	protected:
		CDbCacheClient2() {}
	public:
		virtual ~CDbCacheClient2() {}

		/**
		\~korean
		DB cache client2 인스턴스를 생성합니다.

		\~english
		Creates DB cache client2 instance

		\~chinese
		生成DB cache client2实例。

		\~japanese
		DB cache client2 インスタンスを生成します。
		\~
		*/
		 PROUDSRV_API static CDbCacheClient2* New();

		/**
		\~korean
		DB cache server2에 접속을 시도합니다.
		\param param CDbCacheClient2를 구동하고 DBCacheServer2에 접속하기 위한 정보 객체 입니다.

		\~english
		Attempts to connect to DB cache server2

		\~chinese
		尝试与DB cache server2连接。
		\param param 启动 CDbCacheClient2%，为了连接DBCacheServer2的信息对象。

		\~japanese
		DB cache server2に接続を試します。
		\param param CDbCacheClient2を駆動してDBCacheServer2に接続するための情報オブジェクトです。

		\~
		*/
		virtual bool Connect(CDbCacheClient2ConnectParameter& param, ErrorInfoPtr& outError) = 0;
		virtual bool Connect(CDbCacheClient2ConnectParameter& param) = 0;

		/**
		\~korean
		DB cache server2와 접속을 종료하고 모든 데이터를 일소합니다.

		\~english
		Disconnect from DB cache server2 and sweep all data

		\~chinese
		终止与DB cache server2的连接，扫除所有数据。

		\~japanese
		DB cache server2と接続を終了して全てのデータを一掃します。
		\~
		*/
		virtual void Disconnect() = 0;

		/**
		\~korean
		<a target="_blank" href="http://guide.nettention.com/cpp_ko#exclusive_load" >독점적 불러오기</a> 요청 메서드입니다.
		\param rootTableName 추가할 데이터의 root 테이블 이름
		\param fieldName 찾을 fieldName입니다.
		\param cmpValue 찾아서 비교할 값입니다.
		\param tag IDbCacheClientDelegate2.OnExclusiveLoadDataComplete 응답을 받을 때 그대로 돌려받을 사용자 임의 데이터입니다. 이 값은 인자 args.m_tag를 통해 돌려받습니다.
		\param message 이미 독점로딩되어있는 데이터를 요청할 경우 기존 독점자에게 전달되는 사용자 임의 데이터입니다. 기존 독점자는 IDbCacheClientDelegate2.OnDataUnloadRequested의 인자 args.m_loadResults[index].m_message를 통해 이 메시지를 전달받습니다.


		\~english
		<a target="_blank" href="http://guide.nettention.com/cpp_en#exclusive_load" >Exclusive Load</a> request method.
		\param rootTbaleName root table name of additional data
		\param fieldName fieldName that will search
		\param cmpValue coparable value
		\param tag It is a user random data that returns when IDbCacheClientDelegate2.OnExclusiveLoadDataComplete is responded. This value returns through parameter args.m_tag.
		\param message It is a user random data that is sent to the existing sole owner in case of requesting exclusive data (loaded exclusively). This message is sent to the existing sole owner through parameter args.m_loadResults[index].m_message of IDbCacheClientDelegate2.OnDataUnloadRequested.

		\~chinese
		<a target="_blank" href="http://guide.nettention.com/cpp_zh#exclusive_load" >独占性引入</a> 邀请方法。
		\param rootTableName 要添加的root table 名称。
		\param fieldName 要找的fieldName。
		\param cmpValue 找到并且比较的值。
		\param tag 在接收到 IDbCacheClientDelegate2.OnExclusiveLoadDataComplete的应答时，原样收回的任意用户数据。此值通过Parameter args.m_tag收回。
		\param message在请求独占加载中的数据时， 传达给现有独占者的任意用户数据。


		\~japanese
		<a target="_blank" href="http://guide.nettention.com/cpp_jp#exclusive_load" >独占的ロード</a> 要請メッソドです。
		\param rootTableName 追加するデータのrootテーブル名
		\param fieldName 探す fieldNameです。
		\param cmpValue 探して比べる値です。
		\param tag IDbCacheClientDelegate2.OnExclusiveLoadDataComplete応答を受ける時にそのまま返して貰うユーザー任意データです。この値は因子args.m_tagを通して返して貰います。
		\param message 既に独占ローディングされているデータを要請する場合、既存独占者に伝達されるユーザー任意データです。
		既存独占者は IDbCacheClientDelegate2.OnDataUnloadRequestedの因子 args.m_loadResults[index].m_messageを通してこのメッセージを受けます。

		*/
		virtual void RequestExclusiveLoadData(String rootTableName, String fieldName, CVariant cmpValue, intptr_t tag, const ByteArray& message = ByteArray()) = 0;

		/**
		\~korean
		<a target="_blank" href="http://guide.nettention.com/cpp_ko#exclusive_load" >독점적 불러오기</a> 요청 메서드입니다.
		\param rootTableName 추가할 데이터의 root 테이블 이름
		\param rootUUID 찾을 rootUUID입니다.
		\param tag IDbCacheClientDelegate2.OnExclusiveLoadDataComplete 응답을 받을 때 그대로 돌려받을 사용자 임의 데이터입니다. 이 값은 인자 args.m_tag를 통해 돌려받습니다.
		\param message 이미 독점로딩되어있는 데이터를 요청할 경우 기존 독점자에게 전달되는 사용자 임의 데이터입니다. 기존 독점자는 IDbCacheClientDelegate2.OnDataUnloadRequested의 인자 args.m_loadResults[index].m_message를 통해 이 메시지를 전달받습니다.

		\~english
		<a target="_blank" href="http://guide.nettention.com/cpp_en#exclusive_load" >Exclusive Load</a> it is a request method.
		\param rootTableName it is a root table name of data that needs to be added.
		\param rootUUID it is rootUUID that needs to be found.
		\param tag It is a user random data that returns when IDbCacheClientDelegate2.OnExclusiveLoadDataComplete is responded. This value returns through parameter args.m_tag.
		\param message It is a user random data that is sent to the existing sole owner in case of requesting exclusive data (loaded exclusively). This message is sent to the existing sole owner through parameter args.m_loadResults[index].m_message of IDbCacheClientDelegate2.OnDataUnloadRequested.

		\~chinese
		<a target="_blank" href="http://guide.nettention.com/cpp_zh#exclusive_load" >独占性引入</a> 是请求Method。
		\param rootTableName 要添加的数据root Table名
		\param rootUUID 要查找的 rootUUID
		\param tag 在接收到 IDbCacheClientDelegate2.OnExclusiveLoadDataSuccess 或 IDbCacheClientDelegate2.OnExclusiveLoadDataFailed的应答时，原样收回的任意用户数据。此值通过Parameter args.m_tag收回。
		\param message在请求独占加载中的数据时， 传达给现有独占者的任意用户数据。

		\~japanese
		<a target="_blank" href="http://guide.nettention.com/cpp_jp#exclusive_load" >独占的ロード</a> 要請メソッドです。
		\param rootTableName 追加するデータの root テーブルの名前
		\param rootUUID 探す rootUUIDです。
		\param tag IDbCacheClientDelegate2.OnExclusiveLoadDataSuccess または IDbCacheClientDelegate2.OnExclusiveLoadDataFailed応答を受ける時にそのまま返して貰うユーザー任意データです。この値は因子args.m_tagを通して返して貰います。
		\param message 既に独占ローディングされているデータを要請する場合、既存独占者に伝達されるユーザー任意データです。
		既存独占者は IDbCacheClientDelegate2.OnDataUnloadRequestedの因子 args.m_loadResults[index].m_messageを通してこのメッセージを受けます。

		\~
		*/

		virtual void RequestExclusiveLoadDataByGuid(String rootTableName, Guid rootUUID, intptr_t tag, const ByteArray &message = ByteArray()) = 0;

		/**
		\~korean
		<a target="_blank" href="http://guide.nettention.com/cpp_ko#exclusive_load" >독점적 불러오기</a> 요청 메서드입니다.
		\param rootTableName 추가할 데이터의 root 테이블 이름
		\param queryString RootUUID를 찾기위해 "select RootUUID from %s where %s" 다음과 같은 쿼리를 날리게 되는데 첫번째 인자는 rootTableName이고 where이후 필드에 들어갈 문자열이다.
		SQL문법에 맞게 작성해야 한다.
		\param tag IDbCacheClientDelegate2.OnExclusiveLoadDataComplete 응답을 받을 때 그대로 돌려받을 사용자 임의 데이터입니다. 이 값은 인자 args.m_tag를 통해 돌려받습니다.
		\param message 이미 독점로딩되어있는 데이터를 요청할 경우 기존 독점자에게 전달되는 사용자 임의 데이터입니다. 기존 독점자는 IDbCacheClientDelegate2.OnDataUnloadRequested의 인자 args.m_loadResults[index].m_message를 통해 이 메시지를 전달받습니다.

		\~english
		<a target="_blank" href="http://guide.nettention.com/cpp_en#exclusive_load" >Exclusive Load</a> it is a request method.
		\param rootTableName it is a root table name of data that needs to be added.
		\param queryString send the query like "select RootUUID from %s where %s" to find RootUUID. The first parameter is rootTableName and it is a string that is going to be inserted into a field after where.
		It must be written in accordance with SQL syntax.
		\param tag It is a user random data that returns when IDbCacheClientDelegate2.OnExclusiveLoadDataComplete is responded. This value returns through parameter args.m_tag.
		\param message It is a user random data that is sent to the existing sole owner in case of requesting exclusive data (loaded exclusively). This message is sent to the existing sole owner through parameter args.m_loadResults[index].m_message of IDbCacheClientDelegate2.OnDataUnloadRequested.

		\~chinese
		<a target="_blank" href="http://guide.nettention.com/cpp_zh#exclusive_load" >独占性引入</a> 是请求Method。
		\param rootTableName 要添加的数据 root Table名
		\param queryString 为查找RootUUID， "select RootUUID from %s where %s" 将发出如下Query,第一个参数为 rootTableName，是在where后将插入Field中的字符串。
		需要按照SQL语法制作。
		\param tag 在接收到 IDbCacheClientDelegate2.OnExclusiveLoadDataSuccess 或 IDbCacheClientDelegate2.OnExclusiveLoadDataFailed的应答时，原样收回的任意用户数据。此值通过Parameter args.m_tag收回。
		\param message在请求独占加载中的数据时， 传达给现有独占者的任意用户数据。

		\~japanese
		<a target="_blank" href="http://guide.nettention.com/cpp_jp#exclusive_load" >独占的ロード</a> 要請メソッドです。
		\param rootTableName 追加するデータのrootテーブルの名前
		\param queryString RootUUIDを探すために"select RootUUID from %s where %s"次のようなクエリーを送るが、1番目のパラメーターは rootTableNameであり、 where以降のフィールドに入る文字列です。
		SQL文法に合うように作成しなければなりません。
		\param tag IDbCacheClientDelegate2.OnExclusiveLoadDataSuccess または IDbCacheClientDelegate2.OnExclusiveLoadDataFailed応答を受ける時にそのまま返して貰うユーザー任意データです。この値は因子args.m_tagを通して返して貰います。
		\param message 既に独占ローディングされているデータを要請する場合、既存独占者に伝達されるユーザー任意データです。
		既存独占者は IDbCacheClientDelegate2.OnDataUnloadRequestedの因子 args.m_loadResults[index].m_messageを通してこのメッセージを受けます。

		\~
		*/
		virtual void RequestExclusiveLoadDataByQuery(String rootTableName, String queryString, intptr_t tag, const ByteArray &message = ByteArray()) = 0;

		/**
		\~korean
		RootTable에 데이터를 추가하고 바로 사용할때 사용되는 함수입니다.
		- 아래는 사용예입니다.

		\code
		CPropNodePtr newNode = CPropNodePtr(new CPropNode(L"Gamer"));
		newNode->Fields[L"GamerID"] = "Ulelio";

		intptr_t tag; // User Defined Identifier
		m_dbCacheClient->RequestExclusiveLoadNewData(L"Gamer", newNode, tag, transaction);
		\endcode

		\param rootTableName 추가할 데이터의 root 테이블 이름
		\param addData 추가할 데이터
		\param tag IDbCacheClientDelegate2.OnExclusiveLoadDataComplete 응답을 받을 때 그대로 돌려받을 사용자 임의 데이터입니다. 이 값은 인자 args.m_tag를 통해 돌려받습니다.
		\param transaction 본요청에 transaction을 할지에 대한 여부입니다.

		\~english
		Function use for add data to RootTable and use it immediately.
		- Sample is as followed

		\code
		CPropNodePtr newNode = CPropNodePtr(new CPropNode(L"Gamer"));
		newNode->Fields[L"GamerID"] = "Ulelio";

		intptr_t tag; // User Defined Identifier
		m_dbCacheClient->RequestExclusiveLoadNewData(L"Gamer", newNode, tag, transaction);
		\endcode

		\param rootTbaleName root table name of additional data
		\param addData Additional data
		\param tag It is a user random data that returns when IDbCacheClientDelegate2.OnExclusiveLoadDataComplete is responded. This value returns through parameter args.m_tag.
		\param transaction Do transaction as request whether or not

		\~chinese
		往RootTable添加数据后立即使用的函数。
		- 下面是使用例。

		\code
		CPropNodePtr newNode = CPropNodePtr(new CPropNode(L"Gamer"));
		newNode->Fields[L"GamerID"] = "Ulelio";

		intptr_t tag; // User Defined Identifier
		m_dbCacheClient->RequestExclusiveLoadNewData(L"Gamer", newNode, tag, transaction);
		\endcode

		\param rootTableName 要添加的数据的root table 名称
		\param addData 要添加的数据。
		\param tag 在接收到 IDbCacheClientDelegate2.OnExclusiveLoadDataSuccess 或 IDbCacheClientDelegate2.OnExclusiveLoadDataFailed的应答时，原样收回的任意用户数据。此值通过Parameter args.m_tag收回。
		\param transaction 对此邀请是否要transaction的与否。

		\~japanese
		RootTableにデータを追加してすぐ使用する時に使われる関数です。
		- 下は使用例です。
		\code
		CPropNodePtr newNode = CPropNodePtr(new CPropNode(L"Gamer"));
		newNode->Fields[L"GamerID"] = "Ulelio";
		intptr_t tag; // User Defined Identifier
		m_dbCacheClient->RequestExclusiveLoadNewData(L"Gamer", newNode, tag, transaction);
		\endcode
		\param rootTableName 追加するデータのrootテーブル名
		\param addData 追加するデータ
		\param tag IDbCacheClientDelegate2.OnExclusiveLoadDataComplete 応答を受けた時、そのまま返してもらうユーザー任意のデータです。この値は因子 args.m_tagを通じ返してもらいます。
		\param transaction 本要請にtransactionをするかどうかです。

		\~
		*/
		virtual void RequestExclusiveLoadNewData(String rootTableName, CPropNodePtr addData, intptr_t tag, bool transaction = false) = 0;

		/**
		\~korean
		요청 응답형 메서드입니다.
		- field에 constraints가 있는 경우 이 메서드를 쓰면 됩니다.
		- 각 함수에대해 Sucess 및 failed 이벤트가 도착합니다.
		- 데이터를 메모리에 캐쉬한뒤 나중에 DBMS에 기록하는게 아니라 요청시 DBMS에 기록후 메모리에 업데이트됩니다.
		- 아래는 사용 예입니다.
		\code
		intptr_t tag = new object;
		RequestAddData(rootUUID,ownerUUID,addData,tag);

		->이후
		OnAddDataSuccess(CCallbackArgs &args)
		{
		args.m_tag;//여기로 아까 설정한 값이 들어옵니다.
		}
		\endcode
		\param rootUUID add할 데이터의 rootUUID
		\param ownerUUID add할 데이터의 ownerUUID
		\param addData add할 데이터
		\param tag 이 요청에대한 사용자가 넣어줄 식별 값입니다. 콜백시 \ref CCallbackArg 안의 m_tag 로 들어갑니다.

		\~english
		Request response type method
		- If field contain constraints, use this method
		- Succeed or failed event will arrive to each functions
		- It writes data to DBMS then update to memory rather than cached to memory then write to DBMS.
		- Example is as followed
		\code
		intptr_t tag = new object;
		RequestAddData(rootUUID,ownerUUID,addData,tag);

		->이후
		OnAddDataSuccess(CCallbackArgs &args)
		{
		args.m_tag;// Setting value will be here
		}
		\endcode
		\param rootUUID rootUUID of additional data
		\param ownerUUID ownerUUID of additional data
		\param addData additional data
		\param tag Verifying value about request that user need to put in. When you callback, it go throuth m_tag in CCallbackArg.

		\~chinese
		邀请应答型方法。
		- field里有constraints的情况用这个方法即可。
		- 对各个函数到达Success及failedevent。
		- 不是把数据缓存起来以后以后往DBMS记录，而是邀请时记录在DBMS以后在内存升级。
		- 以下是使用例。
		\code
		intptr_t tag = new object;
		RequestAddData(rootUUID,ownerUUID,addData,tag);

		->之后
		OnAddDataSuccess(CCallbackArgs &args)
		{
		args.m_tag;//之前设置的值到这里。
		}
		\endcode
		\param rootUUID 要add的数据rootUUID
		\param ownerUUID 要add的数据ownerUUID
		\param addData 要add的数据
		\param tag 对此邀请，用户要填充的识别值。回调时进入\ref CcallbackArg%的m_tag。

		\~japanese
		要請応答型メソッドです。
		- field에 constraintsがある場合、このメソッドを使えば良いです。
		- 各関数に対してSuccess及びfailedイベントが到着します。
		- データをメモリーにキャッシュして、後でDBMSに記録するのではなく、要請時にDBMSに記録しメモリーにアップデートされます。
		- 下は使用例です。
		\code
		intptr_t tag = new object;
		RequestAddData(rootUUID,ownerUUID,addData,tag);

		->以降
		OnAddDataSuccess(CCallbackArgs &args)
		{args.m_tag;//ここに先に設定した値が入ってきます。}
		\endcode
		\param rootUUID addするデータのrootUUID
		\param ownerUUID addするデータのownerUUID
		\param addData addするデータ
		\param tag この要請に対するユーザーが入れてくれる識別値です。コールバック時、\ref CCallbackArg内のm_tagに入ります。

		\~
		*/
		virtual bool RequestAddData(Guid rootUUID, Guid ownerUUID, CPropNodePtr addData, intptr_t tag) = 0;

		/**
		\~korean
		자세한 설명은 \ref RequestAddData 참고
		\param updateData 변경된 노드의 data
		\param tag 이 요청에대한 사용자가 넣어줄 식별 값입니다. 콜백시 \ref CCallbackArg 안의 m_tag 로 들어갑니다.

		\~english
		Refer to RequestAddData
		\param updateData
		\param tag

		\~chinese
		详细说明请参考\ref RequestAddData%。
		\param updateData 修改的node的data
		\param tag 对此邀请用户要添加的识别值。回调时进入\ref CCallbackArg%里的m_tag。

		\~japanese
		詳しくは、\ref RequestAddDataを参考
		\param updateData 変更されたノードのデータ
		\param tag この要請に対するユーザーが入れてくれる識別値です。コールバック時、\ref CCallbackArg内のm_tagに入ります。

		\~
		*/
		virtual bool RequestUpdateData(CPropNodePtr updateData, intptr_t tag) = 0;

		/**
		\~korean
		자세한 설명은 \ref RequestAddData 참고
		\param rootUUID root의 UUID
		\param removeUUID 제거할 UUID
		\param tag 이 요청에대한 사용자가 넣어줄 식별 값입니다. 콜백시 \ref CCallbackArg 안의 m_tag로 들어갑니다.

		\~english TODO:translate needed.
		Refer to RequestAddData

		\~chinese
		详细说明请参考\ref RequestAddData%。
		\param rootUUID root 的UUID。
		\param removeUUID 要删除的UUID。
		\param tag 对此邀请用户要添加的识别值。回调时进入\ref CCallbackArg%里的m_tag。

		\~japanese
		詳しくは、\ref RequestAddDataを参考
		\param rootUUID rootのUUID
		\param removeUUID 除去する UUID
		\param tag この要請に対するユーザーが入れてくれる識別値です。コールバック時、\ref CCallbackArg内のm_tagに入ります。

		\~
		*/
		virtual bool RequestRemoveData(Guid rootUUID, Guid removeUUID, intptr_t tag) = 0;

		/**
		\~korean
		RequestUpdateData(CPropNodePtr,Guid&)과 달리 LoadedData가 가지고 있는
		하위의 모든 node 를 검색하여 add,update,remove등을 처리해 줍니다.
		- transactional을 지정하지 않으면 실패하였을때 일부 데이터가 dbms 에 commit 되었을수 있습니다.
		- Notify를 받기전에는 데이터를 신뢰 하시면 안됩니다.
		- 실패했을 경우 롤백하시려면 GetClonedDataBySessionGuid 를 사용하십시요.
		\param loadedData 변화를 저장할 LoadedData입니다.
		\param tag 이요청에대한 사용자가 넣어줄 식별 값입니다. 콜백시 \ref CCallbackArg 안의 m_tag로 들어갑니다.
		\param transactional 본요청에 transaction을 할지에 대한 여부입니다.

		\~english
		Instead RequestUpdateData(CPropNodePtr,Guid&), it process add, update, remove, etc to search all under node that LoadedData owned.
		- If you do not set transactional, some data will commit to dbms when it failed.
		- Do not believe data before you receive Notify.
		- Use GetClonedDataBySessionGuid when you need to roll-back.
		\param loadedData LoadedData to save changes
		\param tag Verifying value about request that user need to put in. When you callback, it go throuth m_tag in CCallbackArg.
		\param transactional Do transaction as request whether or not

		\~chinese
		与RequestUpdateData(CPropNodePtr,Guid&)不同，搜索LoadedData拥有的下属的所有node并处理add,update,remove 等。
		- 不指定transactional的话，当失败的时候一部分数据可能在dbms被commit。
		- 接到Notify之前不能信赖数据。
		- 失败的时候想roll-back 的话请使用GetClonedDataBySessionGuid。
		\param loadedData 储存变化的LoadedData。
		\param tag 对此邀请用户要添加的识别值。回调时进入\ref CCallbackArg%里的m_tag。
		\param transactional 对此邀请是否要transaction的与否。

		\~japanese
		RequestUpdateData(CPropNodePtr,Guid&)とは違って、LoadedDataが持っている下位の全てのノードを検索してadd,update,removeなどを処理します。
		- transactionalを指定しなければ、失敗した時に一部のデータがdbmsにcommitされた可能性があります。
		- Notifyを受ける前にはデータを信頼してはいけません。
		- 失敗した場合、ロールバックするためには、GetClonedDataBySessionGuidを使ってください。
		\param loadedData 変化を保存するLoadedDataです。
		\param tag この要請に対するユーザーが入れてくれる識別値です。コールバック時、\ref CCallbackArg内のm_tagに入ります。
		\param transactional この要請にtransactionをするかどうかに対する可否です。

		\~
		*/
		virtual bool RequestRecursiveUpdateData(CLoadedData2Ptr loadedData, intptr_t tag, bool transactional = false) = 0;

		/**
		\~korean
		블록형 메서드 입니다.
		- 요청응답형과 마찬가지로 먼저 DBMS에 기록후 메모리에 업데이트 됩니다.
		- 요청응답형과의 차이는 요청이 처리되기 전까지 리턴하지 않습니다. 요청이 처리되면 리턴합니다.
		\param rootUUID add할 데이터의 rootUUID
		\param ownerUUID add할 데이터의 ownerUUID
		\param addData add할 데이터
		\param timeOutTime 무한루프에 빠지지 않기위해 리턴을 기다리는시간. default 30sec
		\param outError 에러타입

		\~english
		Block type method.
		- It write to DBMS first then update to memory like request response type.
		- Only difference is not return before request has done. Once request is finished then return.
		\param rootUUID rootUUID of additional data
		\param ownerUUID ownerUUID of additional data
		\param addData additional data
		\param timeOutTime Waiting time to prevent unlimited roop. default 30sec
		\param outError Error type

		\~chinese
		Block 型方法。
		- 与邀请应答型一样，先往DBMS记录后升级到内存。
		- 与邀请应答型的区别是，邀请被处理之前不会返回。邀请被处理的话会返回。
		\param rootUUID 要add的数据rootUUID
		\param ownerUUID 要add的数据ownerUUID
		\param addData 要add的数据。
		\param timeOutTime 为了不陷入无限roop，等返回的时间。default 30sec。
		\param outError 错误类型。

		\~japanese
		ブロック型メソッドです。
		- 要請応答型と同様に先にDBMSに記録後メモリーにアップデートされます。
		- 要請応答型との差は要請が処理される前までリターンしません。要請が処理されたらリターンします。
		\param rootUUID addするデータのrootUUID
		\param ownerUUID addするデータのownerUUID
		\param addData addするデータ
		\param timeOutTime 無限ループに落ちないようにするため、リターンを待つ時間。default 30sec
		\param outError エラータイプ

		\~
		*/
		virtual bool BlockedAddData(Guid rootUUID, Guid ownerUUID, CPropNodePtr addData, uint32_t timeOutTime = 30000, ErrorInfoPtr outError = ErrorInfoPtr()) = 0;

		/**
		\~korean
		자세한 설명은 \ref BlockedNonCacheAddData 참고
		\param updateData update할 node
		\param timeOutTime 무한루프에 빠지지 않기위해 리턴을 기다리는시간. default 30sec
		\param outError 에러타입

		\~english
		For a detailed explanation, refer to \ref BlockedNonCacheAddData
		\param updateData Node to be updated.
		\param timeOutTime  The time waiting for the return to avoid falling into an infinite loop. Default is 30 seconds. 
		\param outError Error type

		\~chinese
		详细说明请参考\ref BlockedNonCacheAddData%。
		\param updateData 要update的数据。
		\param timeOutTime 为了不陷入无限roop，等返回的时间。default 30sec。
		\param outError 错误类型。

		\~japanese
		詳しくは、\ref BlockedNonCacheAddData 参考
		\param updateData updateするnode
		\param timeOutTime 無限ループに落ちないようにするため、リターンを待つ時間。default 30sec
		\param outError エラータイプ

		\~
		*/
		virtual bool BlockedUpdateData(CPropNodePtr updateData, uint32_t timeOutTime = 30000, ErrorInfoPtr outError = ErrorInfoPtr()) = 0;

		/**
		\~korean
		BlockedNonCacheAddData 참고
		\param rootUUID add할 데이터의 rootUUID
		\param removeUUID 제거하고자 하는 UUID
		\param timeOutTime 무한루프에 빠지지 않기위해 리턴을 기다리는시간. default 30sec
		\param outError 에러타입

		\~english
		Refer to BlockedNonCacheAddData
		\param rootUUID rootUUID of the data to be added 
		\param removeUUID to be deleted
		\param The time waiting for the return to avoid falling into an infinite loop. Default is 30 seconds.
		\param outError type

		\~chinese
		参考BlockedNonCacheAddData。
		\param rootUUID 要add的数据rootUUID。
		\param removeUUID 要删除的UUID。
		\param timeOutTime 为了不陷入无限roop，等返回的时间。default 30sec。
		\param outError 错误类型。

		\~japanese
		BlockedNonCacheAddData 参考
		\param rootUUID addするデータのrootUUID
		\param removeUUID 除去しようとするUUID
		\param timeOutTime 無限ループに落ちないようにするため、リターンを待つ時間。default 30sec
		\param outError エラータイプ

		\~
		*/
		virtual bool BlockedRemoveData(Guid rootUUID, Guid removeUUID, uint32_t timeOutTime = 30000, ErrorInfoPtr outError = ErrorInfoPtr()) = 0;

		/**
		\~korean
		BlockedUpdateData(CPropNodePtr,...)과 달리 LoadedData가 가지고 있는 하위의 모든 node를 검색하여 add,update,remove등을 처리해 줍니다.
		- transactional을 지정하지 않으면 실패하였을때 일부 데이터가 dbms에 commit되었을수 있습니다.
		- Notify를 받기전에는 데이터를 신뢰 하시면 안됩니다.
		- 실패했을 경우 롤백하시려면 GetClonedDataBySessionGuid를 사용하십시요.
		\param loadedData 변화를 저장할 LoadedData입니다.
		\param transactional 본요청에 transaction을 할지에 대한 여부입니다.
		\param timeOutTime 무한대기에 빠지지 않기위해 리턴을 기다리는 시간.default 30sec
		\param outError 에러타입

		\~english
		Instead BlockedUpdateData(CPropNodePtr,...), it process add, update, remove, etc to search all under node that LoadedData owned.
		- If you do not set transactional, some data will commit to dbms when it failed.
		- Do not believe data before you receive Notify.
		- Use GetClonedDataBySessionGuid when you need to roll-back.
		\param loadedData LoadedData to save changes
		\param transactional Do transaction as request whether or not
		\param timeOutTime Waiting time to prevent unlimited roop. default 30sec
		\param outError Error type

		\~chinese
		与RequestUpdateData(CPropNodePtr,Guid&)不同，搜索LoadedData拥有的下属的所有node并处理add,update,remove 等。
		- 不指定transactional的话，当失败的时候一部分数据可能在dbms被commit。
		- 接到Notify之前不能信赖数据。
		- 失败的时候想roll-back 的话请使用GetClonedDataBySessionGuid。
		\param loadedData 储存变化的LoadedData。
		\param transactional 对此邀请是否要transaction的与否。
		\param timeOutTime 为了不陷入无限roop，等返回的时间。default 30sec。
		\param outError 错误类型。

		\~japanese
		BlockedUpdateData(CPropNodePtr,...)とは違って、LoadedDataが持っている下位の全てのnodeを検索してadd,update,removeなどを処理します。
		- transactionalを指定しなければ失敗した時、一部のデータがdbmsにcommitされた可能性があります。
		- Notifyを受ける前にはデータを信頼してはいけません。
		- 失敗した場合、ロールバックするためには、GetClonedDataBySessionGuidを使ってください。
		\param loadedData 変化を保存するLoadedDataです。
		\param transactional この要請にtransactionをするかどうかです。
		\param timeOutTime 無限ループに落ちないようにするため、リターンを待つ時間。default 30sec
		\param outError エラータイプ

		\~
		*/
		virtual bool BlockedRecursiveUpdateData(CLoadedData2Ptr loadedData, bool transactional = false, uint32_t timeOutTime = 30000, ErrorInfoPtr outError = ErrorInfoPtr()) = 0;

		/**
		\~korean
		일방적(Unilateral) 메서드입니다.
		- 데이터가 메모리에 Cache된뒤 나중에 DBMS에 기록됩니다.
		- 기존의 DbCacheClient 과 같은방식입니다.
		- field에 constraints가 없다면 이방식을 쓰는것을 권장합니다.
		- 일방메서드이기 때문에 따로 이벤트 콜백이 오지 않습니다.
		\param rootUUID add할 데이터의 rootUUID
		\param ownerUUID add할 데이터의 ownerUUID
		\param addData add할 데이터
		\param writeDbmsImmediately true이면 DBMS에 아이템 추가 사실을 최대한 빨리 기록을 수행한다.

		\~english
		Unilateral method
		- Data get cached to memory first then write to DBMS
		- Same type as DbCacheClient
		- We recommend to use it if field does not contain constraints.
		- It does not send event callback because it is one-sided method
		\param rootUUID rootUUID of additional data
		\param ownerUUID ownerUUID of additional data
		\param addData additional data
		\param writeDbmsImmediately If it is true, it write information of added item to DBMS as soon as possible.

		\~chinese
		是单方面（Unilateral）方法。
		- 数据在内存Cache后，再往DBMS记录。
		- 与现有的DbCacheClient相同的方式。
		- field里没有constraints的话建议使用此方式。
		- 因为是单方面方法，不会另外再来回调event。
		\param rootUUID 要add的数据的rootUUID。
		\param ownerUUID 要add的数据的ownerUUID。
		\param addData 要add的数据。
		\param writeDbmsImmediately true 的话最快速的执行记录在DBMS添加道具的事情。

		\~japanese
		一方的(Unilateral)なメソッドです。
		- データがメモリーにCacheされて、後でDBMSに記録されます。
		- 既存のDbCacheClientと同じ方式です。
		- fieldにconstraintsがなければこの方式を使うことをお勧めします。
		- 一方メソッドであるため、別にイベントコールバックが来ません。
		\param rootUUID addするデータのrootUUID
		\param ownerUUID addするデータのownerUUID
		\param addData addするデータ
		\param writeDbmsImmediately trueであればDBMSにアイテム追加事実を最大限に速く記録を遂行します。

		\~
		*/
		virtual bool UnilateralAddData(Guid rootUUID, Guid ownerUUID, CPropNodePtr addData, bool writeDbmsImmediately = true) = 0;

		/**
		\~korean
		자세한 설명은 \ref UnilateralAddData 참고
		\param update 하고자 하는 node의 data
		\param writeDbmsImmediately true이면 DBMS에 최대한 빨리 기록을 수행

		\~english TODO:translate needed.
		For a detailed explanation, refer to \ref UnilateralAddData
		\param update Node data to be updated
		\param writeDbmsImmediately     If true, execute record as fast as possible in DBMS. 

		\~chinese
		详细的请参考\ref UnilateralAddData%。
		\param update 想做的node的data。
		\param writeDbmsImmediately true 的话最快速的执行记录。

		\~japanese
		詳しくは、\ref UnilateralAddData 参考
		\param update したいnodeのdata
		\param writeDbmsImmediately trueであればDBMSに最大限に速く記録を遂行

		\~
		*/
		virtual bool UnilateralUpdateData(CPropNodePtr updateData, bool writeDbmsImmediately = true) = 0;


		/**
		\~korean
		일방적(Unilateral) 메서드입니다.
		- 데이터가 메모리에 Cache된뒤 나중에 DBMS에 기록됩니다.
		- 데이터 노드(rootUUID,nodeUUID)를 정해진 트리 노드(destRootUUID,destNodeUUID) 하위로 이동 시 킵니다.
		- field에 constraints가 없다면 이방식을 쓰는것을 권장합니다.
		- 데이터가 이동된 DBCacheClient 에게 이벤트가 콜백됩니다.
		\param rootUUID move할 rootUUID 노드
		\param ownerUUID move할 ownerUUID 노드
		\param destRootUUID attach할 destRootUUID 노드
		\param destNodeUUID attach할 destNodeUUID 노드

		\~english
		Unilateral method
		- Data get cached to memory first then write to DBMS
		- Data node(rootUUID,nodeUUID) attach to specified tree node(destRootUUID,destNodeUUID)
		- We recommend to use it if field does not contain constraints.
		- An event is callbacked to DBCacheclient in which data has been moved.
		\param rootUUID rootUUID of move node
		\param ownerUUID ownerUUID of move node
		\param destRootUUID destNodeUUID of attach node
		\param writeDbmsImmediately If it is true, it write information of added item to DBMS as soon as possible.

		\~chinese
		是单方面（Unliateral）的方法。
		- Data会在内存Cashe，之后会被记录到DBMS。
		- 将Datanode(rootUUID,nodeUUID)移至已确定的Treenode(destRootUUID,destNodeUUID)的下位。
		- 如果field内没有constraints则不建议使用此方法。
		- 会向移动数据的DBCacheClient回调Event。
		\param rootUUID 要move的rootUUID node。
		\param ownerUUID 要move的ownerUUID node。
		\param destRootUUID 要attach的destRootUUID node。
		\param destNodeUUID  要attach的destNodeUUID node。

		\~japanese
		一方的(Unilateral)メソッドです。
		- データがメモリーにCacheされて、後でDBMSに記録されます。
		- データノード(rootUUID,nodeUUID)を決まったツリーノード(destRootUUID,destNodeUUID)の下位に移動させます。
		- fieldにconstraintsがなければこの方式を使うことをお勧めします。
		- データが移動されたDBCacheClientにイベントがコールバックされます。
		\param rootUUID moveする rootUUID ノード
		\param ownerUUID moveする ownerUUID ノード
		\param destRootUUID attachする destRootUUID ノード
		\param destNodeUUID attachする destNodeUUID ノード

		\~
		*/
		virtual bool UnilateralMoveData(String rootTableName, Guid rootUUID, Guid nodeUUID, Guid destRootUUID, Guid destNodeUUID, bool writeDbmsImmediately = true) = 0;

		/**
		\~korean
		자세한 설명은 \ref UnilateralAddData 참고
		\param rootUUID root의 UUID
		\param removeUUID 제거할 UUID
		\param writeDbmsImmediately true이면 DBMS에 최대한 빨리 기록을 수행

		\~english TODO:translate needed.
		For a detailed explanation, refer to \ref UnilateralAddData
		\param rootUUID     UUID of the root
		\param removeUUID      UUID to be deleted
		\param writeDbmsImmediately          If true, execute record as fast as possible in DBMS.

		\~chinese
		详细请参考\ref UnilateralAddData%。
		\param rootUUID 是root的UUID。
		\param removeUUID 要删除的UUID。
		\param writeDbmsImmediately true 的话最快速的在DBMS执行记录。

		\~japanese
		詳しくは、\ref UnilateralAddData 参考
		\param rootUUID rootの UUID
		\param removeUUID 除去する UUID
		\param writeDbmsImmediately trueであればDBMSに最大限速く記録を遂行します。

		\~
		*/
		virtual bool UnilateralRemoveData(Guid rootUUID, Guid removeUUID, bool writeDbmsImmediately = true) = 0;

		/**
		\~korean
		자세한 설명은 \ref UnilateralAddData 참고
		\param loadedData 로드된 Data
		\param transactional 트렌젝션 기능 사용 여부
		\param writeDbmsImmediately true이면 DBMS에 최대한 빨리 기록을 수행

		\~english TODO:translate needed.
		For a detailed explanation, refer to \ref UnilateralAddData
		\param loadedData         Data Loaded data
		\param transactional      Whether to use the transaction feature 
		\param writeDbmsImmediately      If true, execute record as fast as possible in DBMS.

		\~chinese
		详细说明请参考\ref UnilateralAddData%。
		\param loadedData 加载的Data。
		\param transactional 使用transaction技能与否。
		\param writeDbmsImmediately true 的话最快速的在DBMS执行记录。

		\~japanese
		詳しくは、\ref UnilateralAddData 参考
		\param loadedData ロードされたData
		\param transactional トランザクション機能の使用可否
		\param writeDbmsImmediately trueであればDBMSに最大限に速く記録を遂行します。

		
		\~
		*/
		virtual bool UnilateralRecursiveUpdateData(CLoadedData2Ptr loadedData, bool transactional = false, bool writeDbmsImmediately = true) = 0;


		/**
		\~korean
		- 특정 Session의 Unload를 요청하고 해당 데이터의 독점권을 포기합니다.
		- \ref OnUnloadDataRequested 가 호출되었을 때 요청자에게 독점권을 넘겨주려면 이 함수를 호출해야 합니다.
		\param sessionGuid Unload 할 데이터의 session Guid입니다.
		\param messageToNextLoader 독점권 요청자에게 전달하는 사용자 임의 메시지입니다. 요청자는 IDbCacheClientDelegate2.OnExclusiveLoadDataSuccess의 args.m_loadResults[index].m_message를 통해 해당 메시지를 확인 할 수 있습니다.

		\~english
		- It requests Unload of the certain session and abandons the exclusive right of the relevant data.
		- In order to devolve the exclusive right upon a requester, you need to call this function when \ref OnUnloadDataRequested is called.
		\param sessionGuid It is a session Guid of data that needs to be unloaded.
		\param messageToNextLoader It is a user random message that is sent to the exclusive right requester. The requester may check the relevant message through args.m_loadResults[index].m_message of IDbCacheClientDelegate2.OnExclusiveLoadDataSuccess.

		\~chinese
		- 请求特征Session的Unload后，放弃与它相对应的独占权。
		- \ref OnUnloadDataRequested被呼出时，想要将独占权传递给请求者的话，需呼出此函数。
		\param sessionGuid Unload数据的session Guid。
		\param messageToNextLoader 传达给独占权请求者的任意用户信息。请求者通过 IDbCacheClientDelegate2.OnExclusiveLoadDataSuccess的 args.m_loadResults[index].m_message，可以确认该消息。

		\~japanese
		- 特定 Sessionの Unloadを要請し、該当データの独占件を諦めます。
		- \ref OnUnloadDataRequested が呼び出された時、要請者に独占権を渡すためにはこの関数を呼び出す必要があります。
		\param sessionGuid Unload するデータのsession Guidです。
		\param messageToNextLoader 独占権要請者に伝達するユーザー任意メッセージです。要請者はIDbCacheClientDelegate2.OnExclusiveLoadDataSuccessの args.m_loadResults[index].m_messageを通して該当メッセージを確認できます。
		*/
		virtual bool UnloadDataBySessionGuid(Guid sessionGuid, const ByteArray& messageToNextLoader = ByteArray()) = 0;


		/**
		\~korean
		Unload를 인정하지 않습니다.
		- \ref OnUnloadDataRequested 에 콜백이 온 이후 호출해야 하는 인터페이스입니다.
		- 이 함수를 호출하면 다른 클라이언트의 독점권을 넘겨주지 않습니다.
		\param sessionGuid 독점중인 데이터의 session Guid입니다.
		\messageToRequester 독점권 요청자에게 전달하는 사용자 임의 메시지입니다. 요청자는 IDbCacheClientDelegate2.OnExclusiveLoadDataFailed의 args.m_loadResults[index].m_message를 통해 해당 메시지를 확인 할 수 있습니다.

		\~english
		It does not admit Unload
		- This interface has to call after callback arrived to \ref OnUnloadDataRequested.
		- If you call this function, it does not pass exclusive rights to other client.
		\param sessionGuid It is a session Guid of the exclusive data.
		\param messageToRequester It is a user random message that is sent to the exclusive right requester. The requester may check the relevant message through args.m_loadResults[index].m_message of IDbCacheClientDelegate2.OnExclusiveLoadDataFailed.

		\~chinese
		不承认unload。
		- 回调到\ref OnUnloadDataRequested%以后要呼叫的实例。
		- 呼叫此函数的话，不会给其他玩家的独占权。
		\param sessionGuid 独占中数据的session Guid。
		\param messageToRequester 传达给独占权请求者的任何用户信息。请求者通过IDbCacheClientDelegate2.OnExclusiveLoadDataFailed的 args.m_loadResults[index].m_message，可以确认该消息。

		\~japanese
		Unloadを認めません。
		- \ref OnUnloadDataRequested にコールバックが来た以後、呼出必要があるインターフェースです。
		- この関数を呼出したら違うクライアントの独占権を渡しません。
		\param sessionGuid 独占中のデータの session Guidです。
		\param messageToRequester 独占権要請者に伝達するユーザー任意メッセージです。要請者は IDbCacheClientDelegate2.OnExclusiveLoadDataFailedの args.m_loadResults[index].m_messageを通じて該当メッセージを確認できます。

		*/
		virtual void DenyUnloadData(Guid sessionGuid, const ByteArray& messageToRequester = ByteArray()) = 0;

		/*
		\~korean
		같은 이름의 메서드 참고

		\~english
		Refer to same name method

		\~chinese
		参考同名的方法。

		\~japanese
		同名のメソッド参考
		\~
		*/
		/*inline bool UnloadDataBySessionGuid(CLoadedDataPtr data)
		{
		return UnloadDataBySessionGuid(data->m_sessionGuid);
		} */

		/**
		\~korean
		Data tree를 DB cache로부터 완전히 unload해 버립니다. DB cache client뿐만 아니라
		DB cache server에서도 unload를 합니다.

		DB cache가 사용중인 data tree를 직접 DB 억세스를 통해 다루고자 한다면,
		이 함수 대신 <a target="_blank" href="http://guide.nettention.com/cpp_ko#dbc2_isolate_overview" >DB cache가 다루는 데이터를 DB가 직접 억세스하기</a> 기능을 사용하십시오.

		\param rootUUID unload하고자 하는 data tree의 root UUID입니다.

		\~english
		Data tree is completely uploaded from DB cache. It must be uploaded in DB
		cache client and DB cache server.

		If you want to deal with data tree that DB cache is using by direct DB access,
		you need to use <a target="_blank" href="http://guide.nettention.com/cpp_en#dbc2_isolate_overview" >DB directly has access to data that DB cache deals with </a> function instead of this function.

		\param rootUUID this is root UUID of data tree that is going to be unloaded.

		\~chinese
		从DB cache完全unload Data tree。除了DB cache client，
		在DB cache server中也进行 unload。

		如果DB cache想要通过DB访问操作正在使用中的data tree，
		请用 <a target="_blank" href="http://guide.nettention.com/cpp_zh#dbc2_isolate_overview" >DB直接访问DB cache操作的数据</a> 功能代替此函数。

		\param rootUUID 要进行unload的data tree的 root UUID.

		\~japanese
		Data treeをDB cacheから完全にunloadします。 DB cache clientだけではなくて、
		DB cache serverでもunloadをします。

		DB cacheが使用中のdata treeを直接 DBアクセスを通して扱おうとする際には、
		この関数の代わりに <a target="_blank" href="http://guide.nettention.com/cpp_jp#dbc2_isolate_overview" >DB cacheが操るデータをDBが直接アクセス</a> の機能を使用してください。

		\param rootUUID unloadしようとする data treeのroot UUIDです。
		*/
		virtual void ForceCompleteUnload(Guid rootUUID) = 0;

		/**
		\~korean
		비독점적으로 data의 최신 상태를 얻어옵니다. <a target="_blank" href="http://guide.nettention.com/cpp_ko#dbc2_nonexclusive_overview" >비독점적 데이터 접근하기</a> 기능입니다. \n
		Callback:
		- IDbCacheClientDelegate2::OnNonExclusiveSnapshotDataSuccess (CCallbackArgs args)
		- IDbCacheClientDelegate2::OnNonExclusiveSnapshotDataFailed (CCallbackArgs args)

		\param rootTableName root 테이블의 이름
		\param fieldName 필드명
		\param cmpValue 비교할 값. 이를 SQL문에 비유하면 WHERE fieldName = cmpValue 와 동일합니다.
		\param tag 이요청에대한 사용자가 넣어줄 식별 값입니다.콜백시 \ref CCallbackArg 안의 m_tag로 들어갑니다.

		\~english
		Gets the latest status value as non-exclusively. <a target="_blank" href="http://guide.nettention.com/cpp_en#dbc2_nonexclusive_overview" >Accessing non-exclusive data</a> function.
		\param rootTableName Name of root table
		\param fieldName
		\param cmpValue
		\param tag Verifying value about request that user need to put in. When you callback, it go throuth m_tag in CCallbackArg.

		\~chinese
		非独占性的获取最新的状态。是<a target="_blank" href="http://guide.nettention.com/cpp_zh#dbc2_nonexclusive_overview" >访问共享性数据</a>%技能。\n
		Callback:
		- IDbCacheClientDelegate2::OnNonExclusiveSnapshotDataSuccess (CCallbackArgs args)
		- IDbCacheClientDelegate2::OnNonExclusiveSnapshotDataFailed (CCallbackArgs args)
		\param rootTableName root table 的名称。
		\param fieldName
		\param cmpValue
		\param tag 对此邀请用户要填入的识别值。回调时进入\ref CCallbackArg%的m_tag。

		\~japanese
		非独占的にデータの最新状態を得てきます。\ref pndb_nonexclusive_access 機能です。\n
		Callback:
		- IDbCacheClientDelegate2::OnNonExclusiveSnapshotDataSuccess (CCallbackArgs args)
		- IDbCacheClientDelegate2::OnNonExclusiveSnapshotDataFailed (CCallbackArgs args)
		\param rootTableName root テーブル名
		\param rootUUID データ検索をRootUUID
		\param tag この要請に対するユーザーが入れてくれる識別値です。コールバック時、\ref CCallbackArg内のm_tagに入ります。

		\~
		*/
		virtual void RequestNonExclusiveSnapshotData(String rootTableName, String fieldName, CVariant cmpValue, intptr_t tag) = 0;

		/**
		\~korean
		비독점적으로 data의 최신 상태를 얻어옵니다. <a target="_blank" href="http://guide.nettention.com/cpp_ko#dbc2_nonexclusive_overview" >비독점적 데이터 접근하기</a> 기능입니다. \n
		Callback:
		- IDbCacheClientDelegate2::OnNonExclusiveSnapshotDataSuccess (CCallbackArgs args)
		- IDbCacheClientDelegate2::OnNonExclusiveSnapshotDataFailed (CCallbackArgs args)

		\param rootTableName root 테이블의 이름
		\param rootUUID 데이터 검색을 RootUUID
		\param tag 이요청에대한 사용자가 넣어줄 식별 값입니다.콜백시 \ref CCallbackArg 안의 m_tag로 들어갑니다.

		\~english
		Gets the latest status value as non-exclusively. <a target="_blank" href="http://guide.nettention.com/cpp_en#dbc2_nonexclusive_overview" >Accessing non-exclusive data</a> function.
		\param rootTableName Name of root table
		\param rootUUID
		\param tag Verifying value about request that user need to put in. When you callback, it go throuth m_tag in CCallbackArg.

		\~chinese
		非独占性的获取最新的状态。是<a target="_blank" href="http://guide.nettention.com/cpp_zh#dbc2_nonexclusive_overview" >访问共享性数据</a>%技能。\n
		Callback:
		- IDbCacheClientDelegate2::OnNonExclusiveSnapshotDataSuccess (CCallbackArgs args)
		- IDbCacheClientDelegate2::OnNonExclusiveSnapshotDataFailed (CCallbackArgs args)
		\param rootTableName root table 的名称。
		\param rootUUID
		\param tag 对此邀请用户要填入的识别值。回调时进入\ref CCallbackArg%的m_tag。

		\~japanese
		
		\~
		*/
		virtual void RequestNonExclusiveSnapshotDataByGuid(String rootTableName, Guid rootUUID, intptr_t tag) = 0;

		/**
		\~korean
		비독점적으로 data의 최신 상태를 얻어옵니다. <a target="_blank" href="http://guide.nettention.com/cpp_ko#dbc2_nonexclusive_overview" >비독점적 데이터 접근하기</a> 기능입니다. \n
		Callback:
		- IDbCacheClientDelegate2::OnNonExclusiveSnapshotDataSuccess (CCallbackArgs args)
		- IDbCacheClientDelegate2::OnNonExclusiveSnapshotDataFailed (CCallbackArgs args)

		\param rootTableName root 테이블의 이름
		\param searchString 데이터 검색을 위한 쿼리문 ( where절 이후에 들어갈 구문 )
		\param tag 이요청에대한 사용자가 넣어줄 식별 값입니다.콜백시 \ref CCallbackArg 안의 m_tag로 들어갑니다.

		\~english
		Gets the latest status value as non-exclusively. <a target="_blank" href="http://guide.nettention.com/cpp_en#dbc2_nonexclusive_overview" >Accessing non-exclusive data</a> function.
		Callback:
		- IDbCacheClientDelegate2::OnNonExclusiveSnapshotDataSuccess (CCallbackArgs args)
		- IDbCacheClientDelegate2::OnNonExclusiveSnapshotDataFailed (CCallbackArgs args)

		\param rootTableName Name of root table
		\param searchString Query for data searching( For after "where")
		\param tag Verifying value about request that user need to put in. When you callback, it go throuth m_tag in CCallbackArg.

		\~chinese
		非独占性的获取最新的状态。是<a target="_blank" href="http://guide.nettention.com/cpp_zh#dbc2_nonexclusive_overview" >访问共享性数据</a>%技能。\n
		Callback:
		- IDbCacheClientDelegate2::OnNonExclusiveSnapshotDataSuccess (CCallbackArgs args)
		- IDbCacheClientDelegate2::OnNonExclusiveSnapshotDataFailed (CCallbackArgs args)

		\param rootTableName root table 的名称。
		\param searchString 为了数据搜索的query语句。（where 以后进入的语句）
		\param tag 对此邀请用户要填入的识别值。回调时进入\ref CCallbackArg%的m_tag。

		\~japanese
		非独占的に dataの最新状態を読み込みます。 <a target="_blank" href="http://guide.nettention.com/cpp_jp#dbc2_nonexclusive_overview" >Accessing non-exclusive data</a> 機能です。\n
		Callback:
		- IDbCacheClientDelegate2::OnNonExclusiveSnapshotDataSuccess (CCallbackArgs args)
		- IDbCacheClientDelegate2::OnNonExclusiveSnapshotDataFailed (CCallbackArgs args)

		\param rootTableName root テーブルの名前
		\param searchString データ検索のためのクエリー文 ( where節の以降に入る構文)
		\param tag この要請に対してユーザが入力する識別の値です。コールバック時 \ref CCallbackArg 中の m_tagに入ります。

		\~
		*/
		virtual void RequestNonExclusiveSnapshotDataByQuery(String rootTableName, String searchString, intptr_t tag) = 0;

		/**
		\~korean
		<a target="_blank" href="http://guide.nettention.com/cpp_ko#dbc2_nonexclusive_overview" >비독점적 데이터 접근하기</a> 기능입니다.
		Callback:
		- IDbCacheClientDelegate2::OnNonExclusiveAddDataAck (CCallbackArgs args)

		독점중인 DB Cache Client Callback:
		- IDbCacheClientDelegate2::OnSomeoneAddData(CCallbackArgs& args)

		\param rootTableName root테이블의 이름입니다.
		\param rootUUID root의 UUID입니다.
		\param ownerUUID owner의 UUID입니다.
		\param addData 추가할 node의 data입니다.
		\param tag 요청 식별을 위한 사용자 임의 데이터입니다. 콜백 시 \ref CCallbackArg 의 m_tag를 통해 돌려받습니다.
		\param messageToLoader 해당 데이터의 독점자에게 전달하는 사용자 임의 메시지입니다. 독점자는 IDbCacheClientDelegate2::OnSomeoneAddData의 args.m_loadResults[index].m_message를 통해 이를 전달받습니다.


		\~english
		<a target="_blank" href="http://guide.nettention.com/cpp_en#dbc2_nonexclusive_overview" >Accessing non-exclusive data</a> function.
		Callback:
		- IDbCacheClientDelegate2::OnNonExclusiveAddDataAck (CCallbackArgs args)

		Exclusive DB Cache Client Callback:
		- IDbCacheClientDelegate2::OnSomeoneAddData(CCallbackArgs& args)

		\param rootTableName it is name of root table.
		\param rootUUID it is UUID of root.
		\param ownerUUID it is UUID of owner.
		\param addData it is data of node that is going to be added.
		\param tag Verifying value about request that user need to put in. When you callback, it go throuth m_tag in CCallbackArg.
		\param messageToLoader It is a user random message that is sent to a sole owner of the relevant data. The sole owner has this through args.m_loadResults[index].m_message of IDbCacheClientDelegate2::OnSomeoneAddData.


		\~chinese
		是<a target="_blank" href="http://guide.nettention.com/cpp_zh#dbc2_nonexclusive_overview" >访问共享性数据</a>%功能。
		Callback:
		- IDbCacheClientDelegate2::OnNonExclusiveAddDataAck (CCallbackArgs args)

		独占中的DB Cache Client Callback:
		- IDbCacheClientDelegate2::OnSomeoneAddData(CCallbackArgs& args)
		\param rootTableName root table 的名称。
		\param rootUUID 是root的UUID。
		\param ownerUUID 是owner的UUID。
		\param addData 要添加的node的data。
		\param tag 对此邀请，用户要填充的识别值。回调时进入\ref CcallbackArg%的m_tag。
		\param messageToLoader 传达给与 相对应的数据独占者的任何用户信息。独占者通过IDbCacheClientDelegate2::OnSomeoneAddData的 args.m_loadResults[index].m_message接收此消息。

		\~japanese
		<a target="_blank" href="http://guide.nettention.com/cpp_jp#dbc2_nonexclusive_overview" >Accessing non-exclusive data</a> 機能です。
		Callback:
		- IDbCacheClientDelegate2::OnNonExclusiveAddDataAck (CCallbackArgs args)

		独占中の DB Cache Client Callback:
		- IDbCacheClientDelegate2::OnSomeoneAddData(CCallbackArgs& args)

		\param rootTableName rootテーブルの名前です。
		\param rootUUID rootの UUIDです。
		\param ownerUUID ownerの UUIDです。
		\param addData 追加するnodeの dataです。
		\param tag この要請に対してユーザが入力する識別の値です。コールバック時 \ref CCallbackArg 中の m_tagに入ります。
		\param messageToLoader 該当データの独占者に渡すユーザー任意メッセージです。独占者は IDbCacheClientDelegate2::OnSomeoneAddDataの args.m_loadResults[index].m_messageを通じてこれを伝達されます。

		\~
		*/
		virtual void RequestNonExclusiveAddData(String rootTableName, Guid rootUUID, Guid ownerUUID, CPropNodePtr addData, intptr_t tag, const ByteArray &messageToLoader=ByteArray()) = 0;

		/**
		\~korean
		<a target="_blank" href="http://guide.nettention.com/cpp_ko#dbc2_nonexclusive_overview" >비독점적 데이터 접근하기</a> 기능입니다.

		Callback:
		- IDbCacheClientDelegate2::OnNonExclusiveRemoveDataAck (CCallbackArgs args)

		독점 중인 DB Cache Client Callback:
		- IDbCacheClientDelegate2::OnSomeoneRemoveData (CCallbackArgs& args)

		\param rootTableName root테이블의 이름입니다.
		\param rootUUID root의 UUID입니다.
		\param removeUUID 제거하고자 하는 UUID입니다.
		\param tag 요청 식별을 위한 사용자 임의 데이터입니다. 콜백 시 \ref CCallbackArg 의 m_tag를 통해 돌려받습니다.
		\param messageToLoader 해당 데이터의 독점자에게 전달하는 메시지입니다. 독점자는 IDbCacheClientDelegate2::OnSomeoneRemoveData의 args.m_loadResults[index].m_message를 통해 이를 전달받습니다.


		\~english
		<a target="_blank" href="http://guide.nettention.com/cpp_en#dbc2_nonexclusive_overview" >Accessing non-exclusive data</a> function.

		Callback:
		- IDbCacheClientDelegate2::OnNonExclusiveRemoveDataAck (CCallbackArgs args)

		Exclusive DB Cache Client Callback:
		- IDbCacheClientDelegate2::OnSomeoneRemoveData (CCallbackArgs& args)

		\param rootTableName it is name of root table.
		\param rootUUID it is UUID of root.
		\param removeUUID it is UUID that is going to be eliminated.
		\param tag Verifying value about request that user need to put in. When you callback, it go throuth m_tag in CCallbackArg.
		\param messageToLoader It is a user random message that is sent to a sole owner of the relevant data. The sole owner has this through args.m_loadResults[index].m_message of IDbCacheClientDelegate2::OnSomeoneAddData.


		\~chinese
		是<a target="_blank" href="http://guide.nettention.com/cpp_zh#dbc2_nonexclusive_overview" >访问共享性数据</a>%功能。
		Callback:
		- IDbCacheClientDelegate2::OnNonExclusiveRemoveDataAck (CCallbackArgs args)

		独占中的DB Cache Client Callback:
		- IDbCacheClientDelegate2::OnSomeoneRemoveData (CCallbackArgs& args)
		\param rootTableName root table 的名称。
		\param rootUUID 是root的UUID。
		\param removeUUID 要删除的UUID。
		\param tag 对此邀请，用户要填充的识别值。回调时进入\ref CcallbackArg%的m_tag。
		\param messageToLoader 传达给与 相对应的数据独占者的任何用户信息。独占者通过IDbCacheClientDelegate2::OnSomeoneAddData的 args.m_loadResults[index].m_message接收此消息。


		\~japanese
		<a target="_blank" href="http://guide.nettention.com/cpp_jp#dbc2_nonexclusive_overview" >Accessing non-exclusive data</a> 機能です。

		Callback:
		- IDbCacheClientDelegate2::OnNonExclusiveRemoveDataAck (CCallbackArgs args)

		独占中のDB Cache Client Callback:
		- IDbCacheClientDelegate2::OnSomeoneRemoveData (CCallbackArgs& args)

		\param rootTableName rootテーブルの名前です。
		\param rootUUID rootの UUIDです。
		\param removeUUID 除去しようとするUUIDです。
		\param tag この要請に対してユーザが入力する識別の値です。コールバック時 \ref CCallbackArg 中の m_tagに入ります。
		\param messageToLoader 該当データの独占者に渡すユーザー任意メッセージです。独占者は IDbCacheClientDelegate2::OnSomeoneAddDataの args.m_loadResults[index].m_messageを通じてこれを伝達されます。

		\~
		*/
		virtual void RequestNonExclusiveRemoveData(String rootTableName, Guid rootUUID, Guid removeUUID, intptr_t tag, const ByteArray &messageToLoader=ByteArray()) = 0;

		/**
		\~korean
		<a target="_blank" href="http://guide.nettention.com/cpp_ko#dbc2_nonexclusive_overview" >비독점적 데이터 접근하기</a> 기능입니다.
		- DBCacheServer에 로드되어있는 기존값과 compareValue를 비교하여 조건을 충족하면 newValue로 변경합니다.

		Callback:
		- IDbCacheClientDelegate2::OnNonExclusiveSetValueIfSuccess (CCallbackArgs args)
		- IDbCacheClientDelegate2::OnNonExclusiveSetValueIfFailed (CCallbackArgs args)

		독점 중인 DB Cache Client Callback:
		- IDbCacheClientDelegate2::OnSomeoneSetValue (CCallbackArgs& args)

		\param rootTableName root테이블의 이름입니다.
		\param rootUUID root의 UUID입니다.
		\param nodeUUID node의 UUID입니다.
		\param propertyName 값을 setting할 속성의 이름입니다.
		\param newValue 비교 조건을 충족하는 경우 새로 대입될 값입니다.
		\param compareType 비교 조건 type입니다.
		\param compareValue 기존 값과 비교할 값입니다.
		\param tag 요청 식별을 위한 사용자 임의 데이터입니다. 콜백 시 \ref CCallbackArg 의 m_tag를 통해 돌려받습니다.
		\param messageToLoader 해당 데이터의 독점자에게 전달하는 메시지입니다. 독점자는 IDbCacheClientDelegate2::OnSomeoneSetValue의 args.m_loadResults[index].m_message를 통해 이를 전달받습니다.


		\~english
		<a target="_blank" href="http://guide.nettention.com/cpp_en#dbc2_nonexclusive_overview" >Accessing non-exclusive data</a>	 function.
		It is changed to newValue if the conditions are satisfied, after comparing the existing value that has been loaded at DBCacheServer and compareValue.

		Callback:
		- IDbCacheClientDelegate2::OnNonExclusiveSetValueIfSuccess (CCallbackArgs args)
		- IDbCacheClientDelegate2::OnNonExclusiveSetValueIfFailed (CCallbackArgs args)

		Exclusive DB Cache Client Callback:
		- IDbCacheClientDelegate2::OnSomeoneSetValue (CCallbackArgs& args)

		\param rootTableName it is name of root.
		\param rootUUID it is UUID of root.
		\param nodeUUID it is UUID of node.
		\param propertyName it is name of attribute that sets values.
		\param newValue It is a value that will be substituted in case relation condition is satisfied.
		\param compareType it is the type of relation condition.
		\param compareValue It is a value that will be compared with the existing value.
		\param tag Verifying value about request that user need to put in. When you callback, it go throuth m_tag in CCallbackArg.
		\param messageToLoader It is a user random message that is sent to a sole owner of the relevant data. The sole owner has this through args.m_loadResults[index].m_message of IDbCacheClientDelegate2::OnSomeoneAddData.


		\~chinese
		是<a target="_blank" href="http://guide.nettention.com/cpp_zh#dbc2_nonexclusive_overview" >访问共享性数据</a>%功能。
		对比载入DBCacheServer中的基本值与compareValue后，如果条件充足的话变更为 newValue。

		Callback:
		- IDbCacheClientDelegate2::OnNonExclusiveSetValueIfSuccess (CCallbackArgs args)
		- IDbCacheClientDelegate2::OnNonExclusiveSetValueIfFailed (CCallbackArgs args)

		独占中的DB Cache Client Callback:
		- IDbCacheClientDelegate2::OnSomeoneSetValue (CCallbackArgs& args)
		\param rootTableName root table 的名称。
		\param rootUUID 是root的UUID。
		\param nodeUUID 是node的UUID。
		\param propertyName 要setting的值的属性名字。
		\param newValue 在对比条件充足的情况，将新代入的值。
		\param compareType 比较条件type。
		\param compareValue 与基本值对比的值。
		\param tag 对此邀请，用户要填充的识别值。回调时进入\ref CcallbackArg%的m_tag。
		\param messageToLoader 传达给与 相对应的数据独占者的任何用户信息。独占者通过IDbCacheClientDelegate2::OnSomeoneAddData的 args.m_loadResults[index].m_message接收此消息。


		\~japanese
		<a target="_blank" href="http://guide.nettention.com/cpp_jp#dbc2_nonexclusive_overview" >Accessing non-exclusive data</a> 機能です。
		DBCacheServerにロードされていえる既存値と compareValueを比較し、条件を満たしたら newValueに変更します。

		Callback:
		- IDbCacheClientDelegate2::OnNonExclusiveSetValueIfSuccess (CCallbackArgs args)
		- IDbCacheClientDelegate2::OnNonExclusiveSetValueIfFailed (CCallbackArgs args)

		独占中のDB Cache Client Callback:
		- IDbCacheClientDelegate2::OnSomeoneSetValue (CCallbackArgs& args)

		\param rootTableName rootテーブルの名前です。
		\param rootUUID rootの UUIDです。
		\param nodeUUID nodeの UUIDです。
		\param propertyName 値を settingする属性の名前です。
		\param newValue 比較条件を満たす場合新しく代入される値です。
		\param compareType 比較条件 typeです。
		\param compareValue 既存値と比較する値です。
		\param tag この要請に対してユーザが入力する識別の値です。コールバック時 \ref CCallbackArg 中の m_tagに入ります。
		\param messageToLoader 該当データの独占者に渡すユーザー任意メッセージです。独占者は IDbCacheClientDelegate2::OnSomeoneAddDataの args.m_loadResults[index].m_messageを通じてこれを伝達されます。

		\~
		*/
		virtual void RequestNonExclusiveSetValueIf(String rootTableName, Guid rootUUID, Guid nodeUUID, String propertyName, CVariant newValue, ValueCompareType compareType, CVariant compareValue, intptr_t tag, const ByteArray &messageToLoader=ByteArray()) = 0;

		/**
		\~korean
		<a target="_blank" href="http://guide.nettention.com/cpp_ko#dbc2_nonexclusive_overview" >비독점적 데이터 접근하기</a> 기능입니다.

		Callback:
		- IDbCacheClientDelegate2::OnNonExclusiveModifyValueSuccess (CCallbackArgs args)
		- IDbCacheClientDelegate2::OnNonExclusiveModifyValueFailed (CCallbackArgs args)

		독점 중인 DB Cache Client Callback:
		- IDbCacheClientDelegate2::OnSomeoneModifyValue (CCallbackArgs& args)

		\param rootTableName root테이블의 이름입니다.
		\param rootUUID root의 UUID입니다.
		\param nodeUUID node의 UUID입니다.
		\param propertyName 값을 setting할 속성의 이름입니다.
		\param operType 연산 타입
		\param value 적용시킬 값입니다.
		\param tag 요청 식별을 위한 사용자 임의 데이터입니다. 콜백 시 \ref CCallbackArg 의 m_tag를 통해 돌려받습니다.
		\param messageToLoader 해당 데이터의 독점자에게 전달하는 메시지입니다. 독점자는 IDbCacheClientDelegate2::OnSomeoneModifyValue의 args.m_loadResults[index].m_message를 통해 이를 전달받습니다.


		\~english
		<a target="_blank" href="http://guide.nettention.com/cpp_en#dbc2_nonexclusive_overview" >Accessing non-exclusive data</a> function.

		Callback:
		- IDbCacheClientDelegate2::OnNonExclusiveModifyValueSuccess (CCallbackArgs args)
		- IDbCacheClientDelegate2::OnNonExclusiveModifyValueFailed (CCallbackArgs args)

		Exclusive DB Cache Client Callback:
		- IDbCacheClientDelegate2::OnSomeoneModifyValue (CCallbackArgs& args)

		\param rootTableName it is name of root table.
		\param rootUUID it is UUID of root.
		\param nodeUUID it is UUID of node.
		\param propertyName it is name of attribute that sets values.
		\param operType it is the type of calculation.
		\param value it is value that is going to be applied.
		\param tag Verifying value about request that user need to put in. When you callback, it go throuth m_tag in CCallbackArg.
		\param messageToLoader It is a user random message that is sent to a sole owner of the relevant data. The sole owner has this through args.m_loadResults[index].m_message of IDbCacheClientDelegate2::OnSomeoneAddData.


		\~chinese
		是<a target="_blank" href="http://guide.nettention.com/cpp_zh#dbc2_nonexclusive_overview" >访问共享性数据</a>%功能。

		Callback:
		- IDbCacheClientDelegate2::OnNonExclusiveModifyValueSuccess (CCallbackArgs args)
		- IDbCacheClientDelegate2::OnNonExclusiveModifyValueFailed (CCallbackArgs args)

		独占中的DB Cache Client Callback:
		- IDbCacheClientDelegate2::OnSomeoneModifyValue (CCallbackArgs& args)
		\param rootTableName root table 的名称。
		\param rootUUID 是root的UUID。
		\param nodeUUID 是node的UUID。
		\param propertyName 要setting的值的属性名字。
		\param operType 运算型。
		\param value 要适用的值。
		\param tag 对此邀请，用户要填充的识别值。回调时进入\ref CcallbackArg%的m_tag。
		\param messageToLoader 传达给与 相对应的数据独占者的任何用户信息。独占者通过IDbCacheClientDelegate2::OnSomeoneAddData的 args.m_loadResults[index].m_message接收此消息。


		\~japanese
		<a target="_blank" href="http://guide.nettention.com/cpp_jp#dbc2_nonexclusive_overview" >Accessing non-exclusive data</a> 機能です。

		Callback:
		- IDbCacheClientDelegate2::OnNonExclusiveModifyValueSuccess (CCallbackArgs args)
		- IDbCacheClientDelegate2::OnNonExclusiveModifyValueFailed (CCallbackArgs args)

		独占中のDB Cache Client Callback:
		- IDbCacheClientDelegate2::OnSomeoneModifyValue (CCallbackArgs& args)

		\param rootTableName rootテーブルの名前です。
		\param rootUUID rootの UUIDです。
		\param nodeUUID nodeの UUIDです。
		\param propertyName 値を settingする属性の名前です。
		\param operType 演算タイプ
		\param value 適用させる値です。
		\param tag この要請に対してユーザが入力する識別の値です。コールバック時 \ref CCallbackArg 中の m_tagに入ります。
		\param messageToLoader 該当データの独占者に渡すユーザー任意メッセージです。独占者は IDbCacheClientDelegate2::OnSomeoneAddDataの args.m_loadResults[index].m_messageを通じてこれを伝達されます。

		\~
		*/
		virtual void RequestNonExclusiveModifyValue(String rootTableName, Guid rootUUID, Guid nodeUUID, String propertyName, ValueOperType operType, CVariant value, intptr_t tag, const ByteArray &messageToLoader=ByteArray()) = 0;

		/**
		\~korean
		<a target="_blank" href="http://guide.nettention.com/cpp_ko#dbc2_isolate_overview" >DB cache가 다루는 데이터를 DB가 직접 억세스하기</a> 기능입니다.
		DB cache가 사용중인 data tree를 직접 DB 억세스를 통해 접근할 수 있도록 요청합니다.
		DB cache는 사용자가 안전하게 접근할 수 있도록 캐싱하던 Data tree 를 unload 합니다.

		isolate 요청이 성공하면 DB cache 는 data tree 에 절대 접근하지 않습니다.
		사용자는 이 함수를 호출한 뒤 data 를 직접 억세스합니다.
		사용자는 작업을 완료하면 RequestDeisolateData 를 호출하여 DB cache 에 작업이 끝났음을 알려야 합니다.

		- DB cache 에서 data tree 가 성공적으로 isolate 되면 OnIsolateDataSuccess 이벤트가 발생합니다.
		- data tree 가 이미 isolate 상태이거나 unload 에 실패하면 OnIsolateDataFailed 이벤트가 발생합니다.

		\param rootUUID isolate하고자 하는 data tree의 root UUID입니다.
		\param rootTableName isolate하고자 하는 data tree의 root테이블의 이름입니다.
		\param [out] outSessionGuid session 식별자입니다.

		\~english
		<a target="_blank" href="http://guide.nettention.com/cpp_en#dbc2_isolate_overview" >DB directly has access to data that DB cache deals with </a> function.
		It requests access to data tree that DB cache is using by direct DB access.
		DB cache unloads data tree that is the process of caching for user’s safe access.

		If isolate request is successful, DB cache will never have access to data tree.
		After calling this function, a user has access to data directly.
		Once works are done, a user must inform DB cache by calling RequestDeisolateData that works have been done.

		- If data tree is successfully isolated from DB cache, OnIsolateDataSuccess event will occur.
		- If data tree has been already isolated or unloading fails, OnIsolateDataFailed event will occur.

		\param rootUUID It is root UUID of data tree that is going to be isolated.
		\param rootTableName It is root table name of data tree that is going to be isolated.
		\param [out] outSessionGuid session identifier.

		\~chinese
		<a target="_blank" href="http://guide.nettention.com/cpp_zh#dbc2_isolate_overview" >DB直接访问DB cache操作的数据</a> 功能。
		请求DB cache能够通过DB访问直接访问正在使用中的data tree。
		为用户能够安全访问，DB cache unload正在Caching的Data tree。

		如果isolate 请求成功则DB cache绝不访问 data tree。
		用户呼出此函数后可以直接访问data。
		用户结束操作后需要呼出 RequestDeisolateData ，向DB cache告知已结束操作。

		- 在DB cache 中 data tree 成功进行 isolate 后会发生 OnIsolateDataSuccess 事件。
		- 如果data tree 已经是 isolate 状态或 unload 失败时将发生OnIsolateDataFailed 事件。

		\param rootUUID 要进行 isolate的 data tree的 root UUID。
		\param rootTableName 要进行isolate的 data tree的 root Table名。
		\param [out] outSessionGuid session 识别者。

		\~japanese
		<a target="_blank" href="http://guide.nettention.com/cpp_jp#dbc2_isolate_overview" >DB cacheが操るデータをDBが直接アクセス</a>機能です。
		DB cacheが使用中のdata treeを直接DBアクセスを通して接近できるように要請します。
		DB cacheはユーザが安全に接近できるようにCachingしていたData treeをunload します。

		isolate要請が成功するとDB cacheはdata treeに接近しません。
		ユーザはこの関数を呼び出した後, dataを直接アクセスします。
		ユーザは作業を完了すると RequestDeisolateDataを呼び出して DB cacheに作業の終わりを知らせなければなりません。

		- DB cacheでdata treeが成功的にisolateされるとOnIsolateDataSuccessイベントが発生します。
		- data treeが既にisolate状態であったunloadに失敗するとOnIsolateDataFailed イベントが発生します。

		\param rootUUID isolateしようとする data treeのroot UUIDです。
		\param rootTableName isolateしようとするdata treeのrootテーブルの名前です。
		\param [out] outSessionGuid session識別子です。
		*/
		virtual bool RequestIsolateData(Guid rootUUID, String rootTableName, Guid &outSessionGuid) = 0;

		/**
		\~korean
		<a target="_blank" href="http://guide.nettention.com/cpp_ko#dbc2_isolate_overview" >DB cache가 다루는 데이터를 DB가 직접 억세스하기</a> 기능입니다.
		isolate 한 data tree 를 deisolate 하도록 요청합니다.

		deisolate 요청이 성공하면 다시 DB cache 가 load & save 할 수 있는 상태가 됩니다.

		- DB cache 에서 data tree 가 성공적으로 deisolate 되면 OnDeisolateDataSuccess 이벤트가 발생합니다.
		- data tree 가 이미 isolate 상태이거나 deisolate 에 실패하면 OnDeisolateDataFailed 이벤트가 발생합니다.

		\param rootUUID deisolate하고자 하는 data tree의 root UUID입니다.
		\param rootTableName deisolate하고자 하는 data tree의 root테이블의 이름입니다.
		\param [out] outSessionGuid session 식별자입니다.

		\~english
		<a target="_blank" href="http://guide.nettention.com/cpp_en#dbc2_isolate_overview" >DB directly has access to data that DB cache deals with </a> function.
		Request for deisolating data tree that has been isolated.

		If deisolating request is successful, DB cache will be ready to be loaded & saved.

		- If data tree is successfully deisolated from DB cache, OnDeisolateDataSuccess event will occur.
		- If data tree has been already isolated or deisolating fails, OnDeisolateDataFailed event will occur.

		\param rootUUID It is root UUID of data tree that is going to be deisolated.
		\param rootTableName It is root table name of data tree that is going to be deisolated.
		\param [out] outSessionGuid session identifier.

		\~chinese
		<a target="_blank" href="http://guide.nettention.com/cpp_zh#dbc2_isolate_overview" >DB直接访问DB cache操作的数据</a> 功能。
		请求将已进行isolate 的 data tree 进行 deisolate。

		deisolate 请求成功后 DB cache 可以进行 load & save 。

		- 在DB cache 中 data tree 成功进行了 deisolate 则会发生 OnDeisolateDataSuccess 事件。
		- 如果data tree已处于 isolate 状态或 deisolate 失败则会发生 OnDeisolateDataFailed 事件。

		\param rootUUID 要进行deisolate的 data tree的 root UUID。
		\param rootTableName 要进行deisolate的 data tree的 root Table名。
		\param [out] outSessionGuid session 识别者。

		\~japanese
		<a target="_blank" href="http://guide.nettention.com/cpp_jp#dbc2_isolate_overview" >DB cacheが操るデータをDBが直接アクセス</a>機能です。
		Isolateしたdata treeをdeisolateするように要請します。

		Deisolateの要請が成功すると再びDB cacheがload & saveできる状態になります。

		- DB cacheでdata treeが成功的にdeisolateされるとOnDeisolateDataSuccessイベントが発生します。
		- data treeが既にisolateの状態であったりdeisolateに失敗するとOnDeisolateDataFailed イベントが発生します。

		\param rootUUID deisolateしようとする data treeのroot UUIDです。
		\param rootTableName deisolateしようとする data treeのrootテーブルの名前です。
		\param [out] outSessionGuid session識別氏です。
		*/
		virtual bool RequestDeisolateData(Guid rootUUID, String filterText, Guid &outSessionGuid) = 0;

		/**
		\~korean
		데이터의 최신 상태 값을 얻는다.
		\param sessionGuid RequestExclusiveLoadData 나 RequestExclusiveLoadNewData 에서 받은 session guid
		\return 얻은 데이터 객체. 사본이므로 얻은 객체의 값을 변형해도 실제 DB에 업데이트되지 못한다.
		실제 DB에 업데이트하려면 변경한 후 객체를 UpdateData 함수를 통해 서버에 전송해야 한다.

		\~english
		Gets the latest status value of data
		\param sessionGuid Session guid obtained from RequestExclusiveLoadData or RequestExclusiveLoadNewData
		\return Obtained object It does not update to actual DB because it is a copy
		If you want to update it to actual DB, you need send it to server through UpdateData function.

		\~chinese
		获得数据的最新状态值。
		从\param sessionGuid RequestExclusiveLoadData或RequestExclusiveLoadNewData接收的session guid。
		\return 获得的数据的对象。因为是副本，改变获得的对象值，也不会上传到实际DB。
		想上传到实际DB的话，要把修改后的对象通过UpdateData函数传送的服务器。

		\~japanese
		データの最新状態値を得ます。
		\param sessionGuid RequestExclusiveLoadDataかRequestExclusiveLoadNewDataより受けたsession guid
		\return 得たデータオブジェクト。コピーなので得たオブジェクトの値を変更しても実際DBにアップデートされません。
		実際DBにアップデートするためには変更した後、オブジェクトをUpdateData関数を通じサーバーに転送しなければなりません。

		\~
		*/
		virtual CLoadedData2Ptr GetClonedLoadedDataBySessionGuid(Guid sessionGuid) = 0;

		/**
		\~korean
		위 \ref GetClonedLoadedDataBySessionGuid 와 같은 함수이나, rootUUID로 찾는다.
		자세한 설명은 \ref GetClonedLoadedDataBySessionGuid 를 참고.
		\return 얻은 데이터 객체. 사본이므로 얻은 객체의 값을 변형해도 실제 DB에 업데이트되지 못한다.
		실제 DB에 업데이트하려면 변경한 후 객체를 UpdateData 함수를 통해 서버에 전송해야 한다.

		\~english TODO:translate needed.
		It’s the same function as the above \ref GetClonedLoadedDataBySessionGuid, but it is searched with rootUUID. 
		Check out \ref GetClonedLoadedDataBySessionGuid. For a detailed explanation, refer to \ref GetClonedLoadedDataBySessionGuid
		\Obtained data object. Since this is a copy, it is not updated in the real DB even though you transform the value of the obtained object. 
		If you want to update it in the actual DB, you have to transmit the object through the UpdateData function. 

		\~chinese
		与上面的\ref GetClonedLoadedDataBySessionGuid%一样的函数，用rootUUID查询。
		详细说明请参考\ref GetClonedLoadedDataBySessionGuid%。
		\return 获得的数据对象。因为是副本，改变获得的对象值，也不会上传到实际DB。
		想上传到实际DB的话，要把修改后的对象通过UpdateData函数传送的服务器。

		\~japanese
		上記の\ref GetClonedLoadedDataBySessionGuidと同じ関数ですが、rootUUIDで探します。
		詳しくは、\ref GetClonedLoadedDataBySessionGuid を参考。
		\return 得たデータオブジェクト。コピーなので得たオブジェクトの値を変更しても実際DBにアップデートされません。
		実際DBにアップデートするためには、変更した後オブジェクトをUpdateData関数を通じサーバーに転送しなければなりません。

		\~
		*/
		virtual CLoadedData2Ptr GetClonedLoadedDataByUUID(Guid rootUUID) = 0;

		/**
		\~korean
		이 클라이언트의 HostID를 얻는다.

		\~english
		Obtain HostID of this client

		\~chinese
		获得此玩家的Host ID。

		\~japanese
		このクライアントのHostIDを得ます。
		\~
		*/
		virtual HostID GetLocalHostID() = 0;

		/**
		\~korean
		이 DB cache client2가 DB cache server2에 로그온(인증)이 완료된 상태인지 여부를 구한다.
		\return 로그인 되어 있으면 true.

		\~english
		Find out DB cache client2 has got authentication from DB cache server2 whether or not.

		\~chinese
		求此DB cache client2是否是登录（认证）DB cache server2完毕状态的与否。

		\~japanese
		このDB cache client2がDB cache server2にログオン(認証)が完了された状態なのかどうかを求めます。
		\return ログインされていればtrue

		\~
		*/
		virtual bool IsLoggedOn() = 0;

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
