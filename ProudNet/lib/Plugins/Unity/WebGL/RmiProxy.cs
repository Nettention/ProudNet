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
using System.Runtime.InteropServices;

// 이미 타 C# file에서 도움말 생성이 되고 있다. 따라서 여기서 도움말 생성용 주석은 다 삭제함.

#if UNITY_WEBGL
namespace Nettention.Proud
{
    public interface IRmiHost
    {
        bool IsSimplePacketMode();

        void PostCheckReadMessage(Message msg, string RMIName);

        void ShowNotImplementedRmiWarning(String RMIName);

        int InstanceID { get; }
    }

    public class MessageSummary
    {
        
        public int payloadLength = 0;

        public RmiID rmiID = RmiID.RmiID_None;

        public String rmiName = "";

        public EncryptMode encryptMode = EncryptMode.EM_None;

        public CompressMode compressMode = CompressMode.CM_None;
    }

    public abstract class RmiProxy
	{
        public IRmiHost core = null;

        public bool internalUse = false;

        public bool enableNotifySendByProxy = true;

        public RmiProxy()
        {

        }

        ~RmiProxy()
        {
        }

        public RmiID[] RmiIDList
        {
            get { return GetRmiIDList(); }
        }

        public abstract RmiID[] GetRmiIDList();

        public virtual int GetRmiIDListCount
        {
            get;
            set;
        }
        
        public virtual void NotifySendByProxy(HostID[] remotes, MessageSummary summary, RmiContext rmiContext, Message msg) { }

        public virtual bool RmiSend(HostID[] remotes, RmiContext rmiContext, Message msg, String rmiName, RmiID rmiID)
        {
            if(core.InstanceID == -1)
            {
                UnityEngine.Debug.Log("ProudNet RMI Proxy is not attached yet!");
                return false;
            }

            Message frag = new Message();
            frag.Write((byte)1);
			
            frag.Data.AddRange(msg.Data.data, msg.Data.data.Length);

            JSRmiSend(core.InstanceID, Array.ConvertAll(remotes, value => (int)value), remotes.Length,
                (int)rmiContext.reliability, (int)rmiContext.priority, (int)rmiContext.encryptMode, Misc.Base64Encode(frag.Data.data));

            if(internalUse == false)
            {
                MessageSummary msgSumm = new MessageSummary();
                msgSumm.payloadLength = sizeof(byte) + msg.Data.Count;
                msgSumm.rmiID = rmiID;
                msgSumm.rmiName = rmiName;
                msgSumm.encryptMode = rmiContext.encryptMode;
                msgSumm.compressMode = rmiContext.compressMode;

                if(enableNotifySendByProxy)
                {
                    NotifySendByProxy(remotes, msgSumm, rmiContext, msg);
                }
            }

            return true;
        }

        [DllImport("__Internal")]
        private static extern void JSRmiSend(int instanceID, int[] remotes, int remoteLength, int reliability, int priority, int encryptMode, string msg);

    }

}
#endif