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

namespace Nettention.Proud
{
	/** \addtogroup util_group
	*  @{
	*/

	/** 
	\~korean
	3D에서 사용될 Vector3입니다.
	- Nettention.Proud.PositionFollower 에서 사용됩니다.
	
	\~english TODO:translate needed.

	\~chinese
	是将在3D中使用的 Vector。
	- 用在 Nettention.Proud.PositionFollower%中。
	
	\~japanese
	
	\~
	*/
	public struct Vector3
	{
		/** 
		\~
		x
		*/
		public double x;

		/** 
		\~
		y
		*/
		public double y;

		/** 
		\~
		Z
		*/
		public double z;
        public Vector3(float x, float y)
        {
            this.x = x;
            this.y = y;
            this.z = 0;
        }

        public Vector3(float x, float y, float z)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }

        public override bool Equals(object obj)
		{
			return base.Equals(obj);
		}
		public override int GetHashCode()
		{
			return base.GetHashCode();
		}

		public static Vector3 From(double x, double y, double z)
		{
			Vector3 ret = new Vector3();
			ret.x = x;
			ret.y = y;
			ret.z = z;

			return ret;
		}

		private static Vector3 From(Vector3 src)
		{
			Vector3 ret = From(src.x, src.y, src.z);

			return ret;
		}

		public static Vector3 operator +(Vector3 c1, Vector3 c2)
		{
			Vector3 ret = new Vector3();
			ret.x = c1.x + c2.x;
			ret.y = c1.y + c2.y;
			ret.z = c1.z + c2.z;

			return ret;
		}
		// assignment operators

		public static Vector3 operator -(Vector3 c1, Vector3 c2)
		{
			Vector3 ret = new Vector3();
			ret.x = c1.x - c2.x;
			ret.y = c1.y - c2.y;
			ret.z = c1.z - c2.z;
			return ret;
		}

		public static Vector3 operator *(Vector3 c1, double scale)
		{
			Vector3 ret = From(c1);
			ret.x *= scale;
			ret.y *= scale;
			ret.z *= scale;

			return ret;
		}

		public static Vector3 operator /(Vector3 c1, double scale)
		{
			Vector3 ret = From(c1);
			ret.x /= scale;
			ret.y /= scale;
			ret.z /= scale;
			return ret;
		}

		// unary operators
		public static Vector3 operator -(Vector3 c1)
		{
			Vector3 ret = new Vector3();
			ret.x = -c1.x;
			ret.y = -c1.y;
			ret.z = -c1.z;

			return ret;
		}

		public static bool operator ==(Vector3 lhs, Vector3 rhs)
		{
			return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
		}

		public static bool operator !=(Vector3 lhs, Vector3 rhs)
		{
			return lhs.x != rhs.x || lhs.y != rhs.y || lhs.z != rhs.z;
		}

		public Vector3 Normal
		{
			get
			{
				double length = Length;
				return (length == 0) ? Vector3.From(0, 0, 0) : this / Length;
			}
		}
		public double LengthSq
		{
			get { return x * x + y * y + z * z; }
		}

		/** 
		\~korean
		벡터의 길이를 얻습니다.
		벡터의 방향을 유지한 채 길이를 조정합니다.
		
		\~english 
		Gets the length of the vector.
		Sets the length of the vector to a specified length without changing the orientation.
		
		\~chinese
		获取Vector的长度。
		在维持Vector方向的情况下调节其长度。

		\~japanese
		
		\~
		*/
		public double Length
		{
			get { return Math.Sqrt(x * x + y * y + z * z); }

			set
			{
				Vector3 n = Normal;
				n *= value;
				this = n;
			}
		}

		public static Vector3 Lerp(Vector3 a, Vector3 b, double ratio)
		{
			Vector3 ret = new Vector3();
			ret.x = Sysutil.Lerp(a.x, b.x, ratio);
			ret.y = Sysutil.Lerp(a.y, b.y, ratio);
			ret.z = Sysutil.Lerp(a.z, b.z, ratio);

			return ret;
		}

		public static double Dot(Vector3 a, Vector3 b)
		{
			return a.x * b.x + a.y * b.y + a.z * b.z;
		}

	}

	/** @} */
}
