using System;
using System.Collections;
using UnityEngine;
using System.Runtime.InteropServices;
using AOT;

#if UNITY_WEBGL
namespace Nettention.Proud
{
    public class JSClient
    {
        // returns client instance ID
        public int Create(NetConnectionParam connectionParam)
        {
            System.Uri uri = new Uri(connectionParam.serverIP);
            
            string protocol = uri.Scheme;
            if (!protocol.Equals("ws") && !protocol.Equals("wss"))
                throw new ArgumentException("Unsupported protocol: " + protocol);

            return CreateJSClient(uri.ToString(), Misc.Base64Encode(connectionParam.protocolVersion.ToByteArray()),
                connectionParam.userData == null || connectionParam.userData.data == null ? "" : Misc.Base64Encode(connectionParam.userData.data), Misc.GetInternalVersion());
        }

        public bool SendUserMessage(int instanceID, HostID[] remotes, RmiContext rmiContext, ByteArray payload)
        {
            Message frag = new Message();
            frag.Write((byte)2);
            frag.Data.AddRange(payload.data, payload.Count);

            return JSSendUserMessage(instanceID, Array.ConvertAll(remotes, value => (int)value), remotes.Length,
                (int)rmiContext.reliability, (int)rmiContext.priority, (int)rmiContext.encryptMode, Misc.Base64Encode(frag.Data.data));
        }

        public void Disconnect(int instanceID)
        {
            JSDisconnect(instanceID);
        }

        public void GarbageJSClient(int instanceID)
        {
            JSGarbageClient(instanceID);
        }

        public ConnectionState GetServerConnection(int instanceID)
        {
            return (ConnectionState)JSGetServerConnection(instanceID);
        }
        
        [DllImport("__Internal")]
        private static extern int CreateJSClient(string url, string protocolVersion, string userData, int internalVersion);

        [DllImport("__Internal")]
        private static extern bool JSSendUserMessage(int instanceID, int[] remotes, int remoteLength, int reliability, int priority, int encryptMode, string payload);

        [DllImport("__Internal")]
        private static extern void JSDisconnect(int instanceID);

        [DllImport("__Internal")]
        private static extern void JSGarbageClient(int instanceID);

        [DllImport("__Internal")]
        private static extern int JSGetServerConnection(int instanceID);
    }
}
#endif