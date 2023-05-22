using System;
using Nettention.Proud;

namespace SimpleCSharp
{
    class Program
    {
        // RMI stub instance
        // For details, check client source code first.
        static Simple.Stub g_Stub = new Simple.Stub();

        // RMI proxy
        static Simple.Proxy g_Proxy = new Simple.Proxy();

        // a P2P group where all clients are in.
        static HostID g_groupHostID = HostID.HostID_None;

        static void InitStub()
        {
            g_Stub.Chat = (Nettention.Proud.HostID remote, Nettention.Proud.RmiContext rmiContext, String a, int b, float c) =>
                {
                    Console.Write("[Server] chat message received :");
                    Console.Write(" a={0} b={1} c={2}\n", a, b, c);

                    // Echo chatting message which received from server to client.
                    g_Proxy.ShowChat(remote, RmiContext.ReliableSend, a, b + 1, c + 1);

                    return true;
                };

        }


        internal static void StartServer(NetServer server, Nettention.Proud.StartServerParameter param)
        {
            if ((server == null) || (param == null))
            {
                throw new NullReferenceException();
            }

            try
            {
                server.Start(param);
            }
            catch (System.Exception ex)
            {
                Console.WriteLine("Failed to start server~!!" + ex.ToString());
            }

            Console.WriteLine("Succeed to start server~!!\n");
        }

        static void Main(string[] args)
        {
            // Network server instance.
            NetServer srv = new NetServer();

            // set a routine which is executed when a client is joining.
            // clientInfo has the client info including its HostID.
            srv.ClientJoinHandler = (clientInfo) =>
            {
                Console.Write("Client {0} connected.\n", clientInfo.hostID);
            };

            // set a routine for client leave event.
            srv.ClientLeaveHandler = (clientInfo, errorInfo, comment) =>
            {
                Console.Write("Client {0} disconnected.\n", clientInfo.hostID);
            };

            InitStub();

            // Associate RMI proxy and stub instances to network object.
            srv.AttachStub(g_Stub);
            srv.AttachProxy(g_Proxy);

            var p1 = new StartServerParameter();
            p1.protocolVersion = new Nettention.Proud.Guid(Vars.m_Version); // This must be the same to the client.
            p1.tcpPorts.Add(Vars.m_serverPort); // TCP listening endpoint

            try
            {
                /* Starts the server.
                This function throws an exception on failure.
                Note: As we specify nothing for threading model,
                RMI function by message receive and event callbacks are
                called in a separate thread pool.
                You can change the thread model. Check out the help pages for details. */
                srv.Start(p1);
            }
            catch (Exception e)
            {
                Console.Write("Server start failed: {0}\n", e.ToString());
                return;
            }

            Console.Write("Server started. Enterable commands:\n");
            Console.Write("1: Creates a P2P group where all clients join.\n");
            Console.Write("2: Sends a message to P2P group members.\n");
            Console.Write("q: Quit.\n");

            string userInput;

            while (true)
            {
                // get user input
                userInput = Console.ReadLine();

                if (userInput == "1")
                {
                    // get all client HostID array.
                    HostID[] clients = srv.GetClientHostIDs();

                    // create a P2P group where all clients are in.
                    g_groupHostID = srv.CreateP2PGroup(clients, new ByteArray());
                }
                else if (userInput == "2")
                {
                    // send an RMI message to every client.
                    g_Proxy.SystemChat(g_groupHostID, RmiContext.ReliableSend, "Hello~~~!");
                }
                else if (userInput == "3")
                {
                    // destroy the P2P group.
                    srv.DestroyP2PGroup(g_groupHostID);
                    g_groupHostID = HostID.HostID_None;
                }
                else if (userInput == "q")
                {
                    // exit program.
                    break;
                }
            }

            srv.Stop();
        }
    }
}
