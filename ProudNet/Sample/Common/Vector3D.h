/*
ProudNet HERE_SHALL_BE_EDITED_BY_BUILD_HELPER


이 프로그램의 저작권은 넷텐션에게 있습니다.
이 프로그램의 수정, 사용, 배포에 관련된 사항은 본 프로그램의 소유권자와의 계약을 따르며,
계약을 준수하지 않는 경우 원칙적으로 무단 사용을 금지합니다.
무단 사용에 의한 책임은 본 프로그램의 소유권자와의 계약서에 명시되어 있습니다.

** 주의 : 저작물에 관한 위의 명시를 제거하지 마십시오.


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

#include <assert.h>
#include <math.h>

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

/** \addtogroup util_group
*  @{
*/

/** 
\~korean
두 값 v1,v2의 v1->v2로의 내분값 ratio을 구한다.
ratio가 0에 가까울수록 v1에 가깝다. 

\~english
Get a ratio of interior division of v1 and v2 by v1 -> v2.
As ratio is closer to 0, it's closer to v1.

\~chinese
求两个值v1，v2往v1->v2的内分值ratio。
Ratio 越接近0，就会越接近v1。

\~japanese
両値v1、v2のv1->v2への内分値ratioを求めます。
Ratioが0に近づくほどv1に近いです。
\~
*/
template<typename FloatType>
inline FloatType ScalarLerp(const FloatType v1, const FloatType v2, const FloatType ratio)
{
	return v1 + (v2 - v1) * ratio;
}

/** 
\~korean
Vector 3D 클래스
- Vector3T 등이 있음에도 불구하고 별도 구현된 이유는, 플랫폼 비 종속을 위함이다. 
- 보다 높은 정확도를 위해(예: 우주공간 배경의 온라인 게임) FloatType을 쓰고 있다. 보다 낮은 용량을 필요로 하는 경우 Vector3T 대신
다른 객체 타입을 쓰는 것을 권장한다.
- 사칙 연산 연산자를 지원한다. 따라서 아래와 같은 코딩이 가능하다.

\~english
Vector 3D Class
- The purpose of this class is to avoid platform depedence even though Vector 3 already exists.
- To increase the accuracy(i.e. Online game with the outer space backgroundw), FloatType is used. If you require the lower specification then we suggest
to use a different object type rather than Vector 3.
- This module supports arithmetic operators thus it's possible to code as shown in below.

\~chinese
Vector 3D类
- 即使有Vector3T等还要另外体现的理由是为了避开平台的附属。
- 为了更高的正确率（例：宇宙空间背景的网络游戏），在使用FloatType。需要更低容量的话，建议不要使用Vector3T而是其他对象类型。
- 支持四则运算运算符。因此可以如下编程。

\~japanese
Vector3Dクラス
- Vector3Tなどがあるにも関わらず、別に実現された理由は、プラットフォーム非従属のためです。
- より高い正確度のために(例：宇宙空間背景のオンラインゲーム)FloatTypeを使っています。より低い容量を必要とする場合、Vector3Tの代わりに他のオブジェクトタイプを使うことをお勧めします。
- 四則演算演算子を支援します。よって、下のようなコーディングが可能です。

\~


\code
Vector3T a = Vector3T(1,2,3);
Vector3T b = Vector3T(4,5,6);
Vector3T c = (a * b) + c - d;
\endcode

*/
template <typename FloatType>
struct Vector3T
{
	/// x
	FloatType x;
	/// y
	FloatType y;
	/// z
	FloatType z;
public:
	inline Vector3T() {};

	inline Vector3T( const FloatType * from)
	{
		x = from[0];
		y = from[1];
		z = from[2];
	}

	inline Vector3T( FloatType x, FloatType y, FloatType z )
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	// casting
	inline operator FloatType* () {
		return &x;
	}
	inline operator const FloatType* () const {
		return &x;
	}

#ifdef _WIN32
	template<typename D3DXVECTOR3>
	__forceinline Vector3T(const D3DXVECTOR3 &from)
	{
		x = from.x;
		y = from.y;
		z = from.z;
	}

	template<typename D3DXVECTOR3>
	__forceinline operator D3DXVECTOR3 const ()
	{
		D3DXVECTOR3 ret;
		ret.x = x;
		ret.y = y;
		ret.z = z;
		return ret;
	}
#endif // _WIN32

	// assignment operators
	inline Vector3T& operator += ( const Vector3T& src )
	{
		x += src.x;
		y += src.y;
		z += src.z;
		return *this;
	}

	inline Vector3T& operator -= ( const Vector3T& src)
	{
		x -= src.x;
		y -= src.y;
		z -= src.z;
		return *this;
	}

	inline Vector3T& operator *= ( FloatType scale)
	{
		x *= scale;
		y *= scale;
		z *= scale;
		return *this;
	}

	inline Vector3T& operator /= ( FloatType scale)
	{
		x /= scale;
		y /= scale;
		z /= scale;
		return *this;
	}

	// unary operators
	inline Vector3T operator + () const
	{			
		return *this;
	}

	inline Vector3T operator - () const
	{
		return Vector3T(-x,-y,-z);			
	}

	// binary operators
	inline Vector3T operator + ( const Vector3T& src) const
	{
		Vector3T ret;
		ret.x = x + src.x;
		ret.y = y + src.y;
		ret.z = z + src.z;

		return ret;
	}

	inline Vector3T operator - ( const Vector3T& src) const
	{
		Vector3T ret;
		ret.x = x - src.x;
		ret.y = y - src.y;
		ret.z = z - src.z;

		return ret;
	}

	inline Vector3T operator * ( FloatType scale) const
	{
		Vector3T ret;
		ret.x = x * scale;
		ret.y = y * scale;
		ret.z = z * scale;

		return ret;
	}

	inline Vector3T operator / ( FloatType scale) const
	{
		Vector3T ret;
		ret.x = x / scale;
		ret.y = y / scale;
		ret.z = z / scale;

		return ret;
	}

	inline bool operator == ( const Vector3T& rhs) const
	{
		return x == rhs.x && y == rhs.y && z == rhs.z;
	}
	inline bool operator != ( const Vector3T& rhs) const
	{
		return x != rhs.x || y != rhs.y || z != rhs.z;
	}

#if defined(_WIN32)        
	__declspec(property(get = GetLengthSq)) FloatType LengthSq;
	__declspec(property(get = GetLength)) FloatType Length;
#endif

	/** 
	\~korean 
	Normal Vector를 구합니다.		
		
	\~english 
	Calculates the normal vector. 

	\~chinese
	求Normal Vector。

	\~japanese
	Normal Vectorを求めます。
	\~
	*/
	inline Vector3T GetNormal() const
	{
		FloatType length=GetLength();
		return (length==0) ? Vector3T(0,0,0) : (*this) / length;
	}

	/** 
	\~korean 
	길이의 제곱을 구합니다.		
		
	\~english 
	Calculate the square value of the length.	

	\~chinese
	求长度的平方。

	\~japanese
	長さの二乗を求めます。
	\~
	*/
	inline FloatType GetLengthSq() const
	{
		return x*x + y*y + z*z;
	}

	/** 
	\~korean 
	vector의 길이		
		
	\~english 
	Length of the vector

	\~chinese
	Vector 的长度。

	\~japanese
	Vectorの長さ
	\~
	*/
	inline FloatType GetLength() const
	{
		return sqrt(x*x + y*y + z*z);
	}

	/** 
	\~korean 
	두 vector를 보간합니다.  
	- a + (b - a) * ratio

	\~english 
	Interpolate two vectors.  
	- a + (b - a) * ratio


	\~chinese
	插入两个vector。
	- a + (b - a) * ratio

	\~japanese
	両Vectorを補間します。
	- a + (b - a) * ratio

	\~
	*/
	static inline Vector3T Lerp(const Vector3T &a,const Vector3T &b,FloatType ratio)
	{
		Vector3T ret;
		ret.x=ScalarLerp(a.x,b.x,ratio);
		ret.y=ScalarLerp(a.y,b.y,ratio);
		ret.z=ScalarLerp(a.z,b.z,ratio);
			
		return ret;
	}

	/** 
	\~korean 
	두 vector의 내적을 구합니다 
		
	\~english 
	Calculate the interval of two vectors

	\~chinese
	求两个vector的内积。

	\~japanese
	両Vectorの内積を求めます。
	\~
	*/
	static inline FloatType Dot(const Vector3T &a,const Vector3T &b)
	{
		return a.x*b.x+a.y*b.y+a.z*b.z;
	}

	inline FloatType Distance(const Vector3T &a) const
	{
		Vector3T d(a.x - x, a.y - y, a.z - z);
		return d.GetLength();
	}

	inline FloatType Distance2(const Vector3T &a) const
	{
		Vector3T d(a.x - x, a.y - y, a.z - z);
		return d.GetLengthSq();
	}

	/** 
	\~korean 
	벡터의 방향을 유지한 채 길이를 조정합니다.

	\~english 
	Sets the length of the vector to a specified length without changing the orientation.

	\~chinese 
	不改变方向的情况下，设置为指定的长度的向量长度。

	\~japanese
	ベクトルの方向を維持したままの長さを調整します。
	\~
	*/
	inline void SetLength( FloatType length ) 
	{
		Vector3T n = GetNormal();
		n*=length;
		*this = n;
	}
};

template <typename FloatType>
inline Vector3T<FloatType> operator * (FloatType scale, const struct Vector3T<FloatType>& src)
{
	Vector3T<FloatType> ret;
	ret.x = scale * src.x;
	ret.y = scale * src.y;
	ret.z = scale * src.z;
	return ret;
}

typedef Vector3T<double> Vector3;
typedef Vector3T<float> FloatVector3;

/**  @} */

#ifdef _MSC_VER
#pragma pack(pop)
#endif