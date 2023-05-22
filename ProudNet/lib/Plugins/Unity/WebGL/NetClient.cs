using AOT;
using System;
using System.Collections.Generic;
using UnityEngine;
using System.Runtime.InteropServices;

#if UNITY_WEBGL
namespace Nettention.Proud
{
    public class NetClient : IRmiHost, IDisposable
    {
        public NetClient()
        {
            m_jsClient = new JSClient();
        }
        ~NetClient()
        {
            Dispose();
        }

        public void Dispose()
        {
            Disconnect();

            m_proxyRmiIDList.Clear();
            m_proxyList.Clear();

            m_stubRmiIDList.Clear();
            m_stubList.Clear();

            GarbageJSClient();

            GC.SuppressFinalize(this);
        }

        public delegate void JoinServerCompleteDelegate(Nettention.Proud.ErrorInfo info, ByteArray replyFromServer);
        public delegate void LeaveServerDelegate(Nettention.Proud.ErrorInfo info);
        public delegate void SynchronizeServerTimeDelegate();
        public delegate void NoRmiProcessedDelegate(RmiID rmiID);
        public delegate void ReceiveUserMessageDelegate(HostID sender, RmiContext rmiContext, ByteArray payload);

        public delegate void ErrorInfoDelegate(Nettention.Proud.ErrorInfo errorInfo);
        public delegate void ExceptionDelegate(HostID remoteID, Exception e);
        public delegate void P2PMemberJoinDelegate(HostID memberHostID, HostID groupHostID, int memberCount, ByteArray customField);
        public delegate void P2PMemberLeaveDelegate(HostID memberHostID, HostID groupHostID, int memberCount);

        // callback handlers to define
        internal JoinServerCompleteDelegate joinServerCompleteHandler = null;
        public JoinServerCompleteDelegate JoinServerCompleteHandler { set { joinServerCompleteHandler = value; } }

        internal LeaveServerDelegate leaveServerHandler = null;
        public LeaveServerDelegate LeaveServerHandler { set { leaveServerHandler = value; } }

        internal SynchronizeServerTimeDelegate synchronizeServerTime = null;
        public SynchronizeServerTimeDelegate SynchronizeServerTimeHandler { set { synchronizeServerTime = value; } }

        internal NoRmiProcessedDelegate noRmiProcessedHandler = null;
        public NoRmiProcessedDelegate NoRmiProcessedHandler { set { noRmiProcessedHandler = value; } }

        internal ReceiveUserMessageDelegate receiveUserMessageHandler = null;
        public ReceiveUserMessageDelegate ReceiveUserMessageHandler { set { receiveUserMessageHandler = value; } }


        internal ErrorInfoDelegate errorHandler = null;
        public ErrorInfoDelegate ErrorHandler { set { errorHandler = value; } }

        internal ErrorInfoDelegate warningHandler = null;
        public ErrorInfoDelegate WarningHandler { set { warningHandler = value; } }

        internal ErrorInfoDelegate informationHandler = null;
        public ErrorInfoDelegate InformationHandler { set { informationHandler = value; } }

        internal ExceptionDelegate exceptionHandler = null;
        public ExceptionDelegate ExceptionHandler { set { exceptionHandler = value; } }

        internal P2PMemberJoinDelegate p2pMemberJoinHandler = null;
        public P2PMemberJoinDelegate P2PMemberJoinHandler { set { p2pMemberJoinHandler = value; } }

        internal P2PMemberLeaveDelegate p2pMemberLeaveHandler = null;
        public P2PMemberLeaveDelegate P2PMemberLeaveHandler { set { p2pMemberLeaveHandler = value; } }

        public int InstanceID { get { return m_clientInstanceID; } }


        public HostID LocalHostID = HostID.HostID_None;
        public int m_clientInstanceID = -1;
        private static bool m_callbackRegistered = false;

        private const String DuplicatedRmiIDErrorText = "Duplicated RMI ID is found. Review RMI ID declaration in .PIDL files.";
        private const String AsyncCallbackMayOccurErrorText = "Already async callback may occur! server start or client connection should have not been done before here!";
        private const String BadRmiIDErrorText = "Wrong RMI ID is found. RMI ID should be >=1300 or <63000.";

        private List<RmiProxy> m_proxyList = new List<RmiProxy>();
        private List<RmiStub> m_stubList = new List<RmiStub>();

        private List<RmiID> m_proxyRmiIDList = new List<RmiID>();
        private List<RmiID> m_stubRmiIDList = new List<RmiID>();

        private NetConnectionParam m_connectionParam;
        private JSClient m_jsClient;

        private static System.Object thisLock = new System.Object();
        private static List<CallbackEvent> m_eventQueue = new List<CallbackEvent>();
        private Dictionary<HostID, System.Object> m_hostTagDictionary = new Dictionary<HostID, System.Object>();

        public void AttachProxy(RmiProxy proxy)
        {
            if (proxy == null)
                throw new NullReferenceException();

            RmiID[] rmiIDList = proxy.GetRmiIDList();

            for (int i = 0; i < rmiIDList.Length; i++)
            {
                if ((ushort)rmiIDList[i] < 1300 || (ushort)rmiIDList[i] >= 63000)
                    throw new Exception(BadRmiIDErrorText);

                if (m_proxyRmiIDList.Contains(rmiIDList[i]))
                    throw new Exception(DuplicatedRmiIDErrorText);
                else
                    m_proxyRmiIDList.Add(rmiIDList[i]);
            }

            proxy.core = this;

            m_proxyList.Add(proxy);
        }

        public void AttachStub(RmiStub stub)
        {
            if (stub == null)
                throw new NullReferenceException();

            RmiID[] rmiIDList = stub.GetRmiIDList;

            for (int i = 0; i < rmiIDList.Length; i++)
            {
                if ((ushort)rmiIDList[i] < 1300 || (ushort)rmiIDList[i] >= 63000)
                    throw new Exception(BadRmiIDErrorText);

                if (m_stubRmiIDList.Contains(rmiIDList[i]))
                    throw new Exception(DuplicatedRmiIDErrorText);
                else
                    m_stubRmiIDList.Add(rmiIDList[i]);
            }

            stub.core = this;

            m_stubList.Add(stub);
        }

        public bool SendUserMessage(HostID remote, RmiContext rmiContext, ByteArray payload)
        {
            HostID[] __list = new HostID[1];
            __list[0] = remote;
            return m_jsClient.SendUserMessage(m_clientInstanceID, __list, rmiContext, payload);
        }

        public bool SendUserMessage(HostID[] remotes, RmiContext rmiContext, ByteArray payload)
        {
            return m_jsClient.SendUserMessage(m_clientInstanceID, remotes, rmiContext, payload);
        }

        public bool Connect(NetConnectionParam param)
        {
            // Javascript는 멀티스레드로 동작하지 않으며 Connect시 콜백으로 수행되는 것도 없기 때문에 참조복사도 괜찮다.
            m_connectionParam = param;

            m_clientInstanceID = m_jsClient.Create(m_connectionParam);

            if (!m_callbackRegistered)
            {
                RegisterCallbacks();
                m_callbackRegistered = true;
            }

            // javascript에서 생성한 instanceID가 0이상이면 성공으로 간주한다.
            return m_clientInstanceID >= 0;
        }

        public void Disconnect()
        {
            m_jsClient.Disconnect(m_clientInstanceID);

            // NetClient가 Disconnect되면 Queue에 쌓여있던 내 이벤트를 삭제한다.
            lock(thisLock)
            {
                int removeIdx = -1;
                do
                {
                    removeIdx = m_eventQueue.FindIndex(x => x.clientInstanceID == m_clientInstanceID);
                    if(removeIdx != -1)
                        m_eventQueue.RemoveAt(removeIdx);
                }
                while (removeIdx != -1);
            }
        }

        public void GarbageJSClient()
        {
            m_jsClient.GarbageJSClient(m_clientInstanceID);
        }

        public ConnectionState GetServerConnectionState(ServerConnectionState output)
        {
            output.realUdpEnabled = false;

            return m_jsClient.GetServerConnection(m_clientInstanceID);
        }

        public void RegisterCallbacks()
        {
            // instanceID에 따라 콜백 등록
            SetOnJoinServerComplete(OnJoinServerComplete);
            SetOnLeaveServer(OnLeaveServer);
            SetOnSynchronizeServerTime(OnSynchronizeServerTime);
            SetProcessMessage_UserMessage(ProcessMessage_UserMessage);
            SetProcessMessage_Rmi(ProcessMessage_Rmi);
            SetOnHostIDIssued(OnHostIDIssued);
        }

        public FrameMoveResult FrameMove()
        {
            FrameMoveResult frameMoveResult = new FrameMoveResult();

            for (int i = 0; i < m_eventQueue.Count; i++)
            {
                CallbackEvent evt;
                lock (thisLock)
                {
                    // 먼저 들어온 다른 클라이언트의 이벤트를 더 늦게 처리하지 않기 위해 제일 앞의 연속된 이벤트만 처리한다.
                    if (m_eventQueue[i].clientInstanceID != m_clientInstanceID)
                    {
                        Misc.WriteLog("instanceID : " + m_clientInstanceID + ", instanceID doesn't match. evt : " + m_eventQueue[0], System.Reflection.MethodBase.GetCurrentMethod().Name);
                        break;
                    }

                    evt = m_eventQueue[i];
                    m_eventQueue.RemoveAt(i);
                    i--;
                }
                
                Misc.WriteLog("instanceID : " + m_clientInstanceID + ", CallbackEvent : " + evt, System.Reflection.MethodBase.GetCurrentMethod().Name);

                switch (evt.type)
                {
                    case CallbackType.JoinServerComplete:
                        if (joinServerCompleteHandler != null)
                        {
                            joinServerCompleteHandler(evt.errorInfo, evt.replyFromServer);
                            frameMoveResult.processedEventCount++;
                        }
                        break;
                    case CallbackType.LeaveServer:
                        if (leaveServerHandler != null)
                        {
                            leaveServerHandler(evt.errorInfo);
                            frameMoveResult.processedEventCount++;
                        }
                        break;
                    case CallbackType.SynchronizeServerTime:
                        if (synchronizeServerTime != null)
                        {
                            synchronizeServerTime();
                            frameMoveResult.processedEventCount++;
                        }
                        break;
                    case CallbackType.NoRmiProcessed:
                        if (noRmiProcessedHandler != null)
                        {
                            noRmiProcessedHandler(evt.rmiID);
                            frameMoveResult.processedEventCount++;
                        }
                        break;
                    case CallbackType.ReceiveUserMessage:
                        if (receiveUserMessageHandler != null)
                        {
                            receiveUserMessageHandler(evt.sender, evt.rmiContext, evt.byteArr);
                            frameMoveResult.processedMessageCount++;
                        }
                        break;
                    case CallbackType.Rmi:
                        if (ProcessMessage_Rmi(evt.sender, evt.byteArr))
                            frameMoveResult.processedMessageCount++;
                        break;
                    case CallbackType.HostIDIssued:
                        LocalHostID = evt.sender;
                        break;
                    default:
                        break;
                }
            }

            Misc.WriteLog("instanceID : " + m_clientInstanceID + ", FrameMoveResult : [" + frameMoveResult.processedMessageCount + ", " + frameMoveResult.processedEventCount + "]", System.Reflection.MethodBase.GetCurrentMethod().Name);

            return frameMoveResult;
        }

        private bool ProcessMessage_Rmi(HostID remote, ByteArray arr)
        {
            Message msg = new Message(arr);
            int orgReadOffset = msg.ReadOffset;

            RmiID __rmiID;
            msg.Read(out __rmiID);

            // ProudNetLayer에서도 사용자정의Rmi에서도 해당 RmiID를 찾을 수 없다.
            if (!m_stubRmiIDList.Contains(__rmiID))
            {
                if (noRmiProcessedHandler != null)
                    noRmiProcessedHandler(__rmiID);

                return false;
            }

            foreach (RmiStub stub in m_stubList)
            {
                msg.ReadOffset = orgReadOffset;

                ReceivedMessage recvMsg = new ReceivedMessage();
                recvMsg.remoteHostID = remote;
                recvMsg.unsafeMessage = msg;

                if (stub.ProcessReceivedMessage(recvMsg, GetHostTag(remote)))
                    return true;
            }

            return false;
        }

        public bool SetHostTag(HostID hostID, System.Object hostTag)
        {
            lock (thisLock)
            {
                if (m_hostTagDictionary.ContainsKey(hostID))
                    m_hostTagDictionary[hostID] = hostTag;
                else
                    m_hostTagDictionary.Add(hostID, hostTag);

                return true;
            }
        }

        public System.Object GetHostTag(HostID hostID)
        {
            System.Object hostTag = null;

            lock (thisLock)
            {
                m_hostTagDictionary.TryGetValue(hostID, out hostTag);
            }

            return hostTag;
        }

        public bool IsSimplePacketMode()
        {
            return false;
        }

        public void PostCheckReadMessage(Message msg, string RMIName)
        {

        }

        public void ShowNotImplementedRmiWarning(String RMIName)
        {

        }

        [DllImport("__Internal")]
        public static extern void SetOnJoinServerComplete(Action<int, string, string, string> action);

        [MonoPInvokeCallback(typeof(Action))]
        public static void OnJoinServerComplete(int instanceID, string types, string comment, string replyFromServer)
        {
            CallbackEvent evt = new CallbackEvent();

            evt.clientInstanceID = instanceID;

            evt.type = CallbackType.JoinServerComplete;

            string[] splitted = types.Split('|');

            evt.errorInfo.errorType = (ErrorType)Int32.Parse(splitted[0]);
            evt.errorInfo.detailType = (ErrorType)Int32.Parse(splitted[1]);

            evt.errorInfo.comment = comment;
            evt.replyFromServer = new ByteArray(Misc.Base64Decode(replyFromServer));

            Misc.WriteLog("instanceID : " + instanceID + ", CallbackEvent : " + evt, System.Reflection.MethodBase.GetCurrentMethod().Name);

            lock (thisLock)
            {
                m_eventQueue.Add(evt);
            }
        }

        [DllImport("__Internal")]
        public static extern void SetOnLeaveServer(Action<int, int, int, string> action);

        [MonoPInvokeCallback(typeof(Action))]
        public static void OnLeaveServer(int instanceID, int errorType, int detailType, string comment)
        {
            CallbackEvent evt = new CallbackEvent();

            evt.clientInstanceID = instanceID;

            evt.type = CallbackType.LeaveServer;

            evt.errorInfo.errorType = (ErrorType)errorType;
            evt.errorInfo.detailType = (ErrorType)detailType;
            evt.errorInfo.comment = comment;

            Misc.WriteLog("instanceID : " + instanceID + ", CallbackEvent : " + evt, System.Reflection.MethodBase.GetCurrentMethod().Name);

            lock (thisLock)
            {
                // 기존에 쌓여있던 이벤트를 삭제
                int removeIdx = -1;
                do
                {
                    removeIdx = m_eventQueue.FindIndex(x => x.clientInstanceID == instanceID);
                    if (removeIdx != -1)
                        m_eventQueue.RemoveAt(removeIdx);
                }
                while (removeIdx != -1);

                m_eventQueue.Add(evt);
            }
        }

        [DllImport("__Internal")]
        public static extern void SetOnSynchronizeServerTime(Action<int> action);

        [MonoPInvokeCallback(typeof(Action))]
        public static void OnSynchronizeServerTime(int instanceID)
        {
            CallbackEvent evt = new CallbackEvent();

            evt.clientInstanceID = instanceID;

            evt.type = CallbackType.SynchronizeServerTime;

            Misc.WriteLog("instanceID : " + instanceID + ", CallbackEvent : " + evt, System.Reflection.MethodBase.GetCurrentMethod().Name);

            lock (thisLock)
            {
                m_eventQueue.Add(evt);
            }
        }

        [DllImport("__Internal")]
        public static extern void SetOnHostIDIssued(Action<int, int> action);

        [MonoPInvokeCallback(typeof(Action))]
        public static void OnHostIDIssued(int instanceID, int hostID)
        {
            CallbackEvent evt = new CallbackEvent();

            evt.clientInstanceID = instanceID;
            evt.sender = (HostID)hostID;

            evt.type = CallbackType.HostIDIssued;

            Misc.WriteLog("instanceID : " + instanceID + ", CallbackEvent : " + evt, System.Reflection.MethodBase.GetCurrentMethod().Name);

            lock (thisLock)
            {
                m_eventQueue.Add(evt);
            }
        }

        [DllImport("__Internal")]
        public static extern void SetProcessMessage_UserMessage(Action<int, int, string> action);

        [MonoPInvokeCallback(typeof(Action))]
        public static void ProcessMessage_UserMessage(int instanceID, int remote, string userMsg)
        {
            CallbackEvent evt = new CallbackEvent();

            evt.clientInstanceID = instanceID;

            evt.type = CallbackType.ReceiveUserMessage;

            evt.sender = (HostID)remote;
            evt.byteArr = new ByteArray(Misc.Base64Decode(userMsg));

            Misc.WriteLog("instanceID : " + instanceID + ", CallbackEvent : " + evt, System.Reflection.MethodBase.GetCurrentMethod().Name);

            lock (thisLock)
            {
                m_eventQueue.Add(evt);
            }
        }

        [DllImport("__Internal")]
        public static extern void SetProcessMessage_Rmi(Action<int, int, string> action);

        [MonoPInvokeCallback(typeof(Action))]
        public static void ProcessMessage_Rmi(int instanceID, int remote, string base64encoded)
        {
            CallbackEvent evt = new CallbackEvent();

            evt.clientInstanceID = instanceID;

            evt.type = CallbackType.Rmi;

            evt.sender = (HostID)remote;
            evt.byteArr = new ByteArray(Misc.Base64Decode(base64encoded));

            Misc.WriteLog("instanceID : " + instanceID + ", CallbackEvent : " + evt, System.Reflection.MethodBase.GetCurrentMethod().Name);

            lock (thisLock)
            {
                m_eventQueue.Add(evt);
            }
        }
    }
}
#endif