/*
ProudNet

이 프로그램의 저작권은 넷텐션에게 있습니다.
이 프로그램의 수정, 사용, 배포에 관련된 사항은 본 프로그램의 소유권자와의 계약을 따르며,
계약을 준수하지 않는 경우 원칙적으로 무단 사용을 금지합니다.
무단 사용에 의한 책임은 본 프로그램의 소유권자와의 계약서에 명시되어 있습니다.

** 주의: 저작물에 관한 위의 명시를 제거하지 마십시오.

ProudNet

This program is soley copyrighted by Nettention.
Any use, correction, and distribution of this program are subject to the terms and conditions of the License Agreement.
Any violated use of this program is prohibited and will be cause of immediate termination according to the License Agreement.

** WARNING : PLEASE DO NOT REMOVE THE LEGAL NOTICE ABOVE.

ProudNet

此程序的版权归Nettention公司所有。
与此程序的修改、使用、发布相关的事项要遵守此程序的所有权者的协议。
不遵守协议时要原则性的禁止擅自使用。
擅自使用的责任明示在与此程序所有权者的合同书里。

** 注意：不要移除关于制作物的上述明示。

*/
using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

// 이미 타 C# file에서 도움말 생성이 되고 있다. 따라서 여기서 도움말 생성용 주석은 다 삭제함.

#if UNITY_WEBGL
namespace Nettention.Proud
{
    public class BeforeRmiSummary
    {
        public RmiID rmiID = RmiID.RmiID_None;

        public String rmiName = "";

        public HostID hostID = HostID.HostID_None;

        public Object hostTag = null;
    }

    public class AfterRmiSummary
    {
        public RmiID rmiID = RmiID.RmiID_None;

        public String rmiName = "";

        public HostID hostID = HostID.HostID_None;

        public Object hostTag = null;

        public long elapsedTime = 0;
    }

    public class ReceivedMessage
    {
        public Message unsafeMessage = null;
        public HostID remoteHostID = HostID.HostID_None;
//        public IPEndPoint remoteAddr_onlyUdp = null;
        public bool relayed = true;
        public EncryptMode encryptMode = EncryptMode.EM_None;
        public CompressMode compressMode = CompressMode.CM_None;

        public Message WriteOnlyMessage
        {
            get { return unsafeMessage; }
        }
        public Message ReadOnlyMessage
        {
            get { return unsafeMessage; }
        }
//         public IPEndPoint RemoteAddr
//         {
//             get { return remoteAddr_onlyUdp; }
//         }
        public HostID RemoteHostID
        {
            get { return remoteHostID; }
        }
        public bool IsRelayed
        {
            get { return relayed; }
        }
        public EncryptMode EncryptMode
        {
            get { return encryptMode; }
        }
        public CompressMode CompressMode
        {
            get { return compressMode; }
        }
    };

    public abstract class RmiStub
	{
        public IRmiHost core = null;

        public bool enableNotifyCallFromStub = false;

        public bool enableStubProfiling = false;

        public RmiStub()
        {
        }

        ~RmiStub()
        {
        }

        public virtual RmiID[] GetRmiIDList
        {
            get;
            set;
        }

        public virtual int GetRmiIDListCount
        {
            get;
            set;
        }
		public abstract bool ProcessReceivedMessage(ReceivedMessage pa, Object hostTag);

		public delegate void AfterRmiInvocationDelegate(AfterRmiSummary summary);

        public delegate void BeforeRmiInvocationDelegate(BeforeRmiSummary summary);

        public virtual void NotifyCallFromStub(RmiID rmiID, string methodName, string parameters)
        {

        }

        public void ShowUnknownHostIDWarning(HostID remote)
        {
            UnityEngine.Debug.Log(String.Format("Warning: unknown HostID {0} in ProcessReceivedMessage!", (int)remote));
        }
	}

}
#endif