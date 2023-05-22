using UnityEngine;
using System.Collections;

namespace ChatUnity
{
	public enum ToolBarState
	{
		GlobalChat,
		UserList,
		P2PChatRoom,
		None
	}

	public enum P2PChatToolBarState
	{
		Back,
		LeaveRoom,
		None
	}

	public class Vars
	{
		public static System.Guid Version = new System.Guid("{0xafa3c0c,0x77d7,0x4b74,{0x9d,0xdb,0x1c,0xb3,0xd2,0x5e,0x1e,0x64}}");
		public static int ServerPort  = 17788;

		public Vars()
		{

		}
	}
}

