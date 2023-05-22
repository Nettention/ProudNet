using System;
using System.Threading;
using Nettention.Proud;

namespace SimpleCSharp
{
    class Program
    {
        // Protects all variable here.
        // If you are developing a game program or an app with fast-running loop,
        // this is not needed, as you are making a single threaded app.
        // This is required as this sample program uses two threads.
        // One is for waiting user input, the other is for processing
        // received messages and events.
        static object g_critSec = new object();

        /* RMI proxy.
        RMI proxy is used for sending messages aka.
        calling a function which resides other process. */
        static Simple.Proxy g_Proxy = new Simple.Proxy();

        /* RMI stub for receiving messages.
        Unlike RMI proxy, it is derived and RMI function is
        implemented by function override.
        */
        static Simple.Stub g_Stub = new Simple.Stub();

        static void InitStub()
        {
            g_Stub.P2PChat = (remote, rmiContext, a, b, c) =>
            {
                lock (g_critSec)
                {
                    Console.Write("[Client] P2PChat : a={0}, b={1}, c={2}, relay={3}, rmiID={4}\n", a, b, c, rmiContext.relayed, rmiContext.rmiID);
                }
                return true;
            };

            g_Stub.ShowChat = (remote, rmiContext, a, b, c) =>
            {
                lock (g_critSec)
                {
                    Console.Write("[Client] ShowChat : a={0}, b={1}, c={2}, rmiID={3}\n", a, b, c, rmiContext.rmiID);
                }
                return true;
            };

            g_Stub.SystemChat = (remote, rmiContext, txt) =>
            {
                lock (g_critSec)
                {
                    Console.Write("[Client] SystemChat : txt={0}, rmiID={1}\n", txt, rmiContext.rmiID);
                }
                return true;
            };
        }

        static void Main(string[] args)
        {
            InitStub();

            // Network client object.
            NetClient netClient = new NetClient();

            // set to false to exit the main loop.
            bool keepWorkerThread = true;
            // set to true if server connection is established.
            bool isConnected = false;
            // changed if another P2P peer joined.
            HostID recentP2PGroupHostID = HostID.HostID_None;

            // set a routine which is run when the server connection attempt
            // is success or failed.
            netClient.JoinServerCompleteHandler = (info, replyFromServer) =>
            {
                // as here is running in 2nd thread, lock is needed for console print.
                lock (g_critSec)
                {
                    if (info.errorType == ErrorType.Ok)
                    {
                        Console.Write("Succeed to connect server. Allocated hostID={0}", netClient.GetLocalHostID());
                        isConnected = true;
                        // send a message.
                        g_Proxy.Chat(HostID.HostID_Server, // send destination
                        RmiContext.ReliableSend, // how to send
                        "Hello ProudNet~!!!.", 333, 22.33f); // user defined parameters
                    }
                    else
                    {
                        // connection failure.
                        Console.Write("Failed to connect server.\n");
                        Console.WriteLine("errorType = {0}, detailType = {1}, comment = {2}", info.errorType, info.detailType, info.comment);
                    }
                }
            };

            // set a routine for network disconnection.
            netClient.LeaveServerHandler = (errorInfo) =>
            {
                // lock is needed as above.
                lock (g_critSec)
                {
                    // show why disconnected.
                    Console.Write("OnLeaveServer: {0}\n", errorInfo.comment);

                    // let main loop exit
                    isConnected = false;
                    keepWorkerThread = false;
                }
            };

            // set a routine for P2P member join (P2P available)
            netClient.P2PMemberJoinHandler = (memberHostID, groupHostID, memberCount, customField) =>
            {
                // lock is needed as above.
                lock (g_critSec)
                {
                    // memberHostID = P2P connected client ID
                    // groupHostID = P2P group ID where the P2P peer is in.
                    Console.Write("[Client] P2P member {0} joined group {1}.\n", memberHostID, groupHostID);

                    g_Proxy.P2PChat(memberHostID, RmiContext.ReliableSend,
                    "Welcome!", 5, 7);
                    recentP2PGroupHostID = groupHostID;
                }
            };

            // called when a P2P member left.
            netClient.P2PMemberLeaveHandler = (memberHostID, groupHostID, memberCount) =>
            {
                Console.Write("[Client] P2P member {0} left group {1}.\n", memberHostID, groupHostID);
            };

            // attach RMI proxy and stub to client object.
            netClient.AttachProxy(g_Proxy);	// Client-to-server =>
            netClient.AttachStub(g_Stub);		// server-to-client <=

            NetConnectionParam cp = new NetConnectionParam();
            // Protocol version which have to be same to the value at the server.
            cp.protocolVersion.Set(SimpleCSharp.Vars.m_Version);

            // server address
            cp.serverIP = "localhost";

            // server port
            cp.serverPort = (ushort)SimpleCSharp.Vars.m_serverPort;

            // Starts connection.
            // This function returns immediately.
            // Meanwhile, connection attempt is process in background
            // and the result is notified by OnJoinServerComplete event.
            netClient.Connect(cp);

            // As we have to be notified events and message receivings,
            // We call FrameMove function for every short interval.
            // If you are developing a game, call FrameMove
            // without doing any sleep.
            // If you are developing an app, call FrameMove
            // in another thread or your timer callback routine.
            Thread workerThread = new Thread(() =>
            {
                while (keepWorkerThread)
                {
                    // Prevent CPU full usage.
                    Thread.Sleep(10);

                    // process received RMI and events.
                    netClient.FrameMove();
                }
            });

            workerThread.Start();

            Console.Write("a: Send a P2P message to current P2P group members except for self.\n");
            Console.Write("q: Exit.\n");

            while (keepWorkerThread)
            {
                // get user input
                string userInput = Console.ReadLine();

                // received user command. process it.
                if (userInput == "q")
                {
                    // let worker thread exit.
                    keepWorkerThread = false;
                }
                else if (userInput == "a")
                {
                    if (isConnected)
                    {
                        // As we access recentP2PGroupHostID which is also accessed by
                        // another thread, lock it.
                        lock (g_critSec)
                        {
                            // Sends a P2P message to everyone in a group
                            // specified at recentP2PGroupHostID.
                            RmiContext sendHow = RmiContext.ReliableSend;
                            sendHow.enableLoopback = false; // don't sent to myself.
                            g_Proxy.P2PChat(recentP2PGroupHostID, sendHow,
                            "Welcome ProudNet!!", 1, 1);
                        }
                    }
                    else
                    {
                        // We have no server connection. Show error.
                        Console.Write("Not yet connected.\n");
                    }
                }
            }

            // Waits for 2nd thread exits.
            workerThread.Join();

            // Disconnects.
            // Note: deleting NetClient object automatically does it.
            netClient.Disconnect();

        }
    }
}
