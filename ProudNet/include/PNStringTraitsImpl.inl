// Users do not include this.

namespace Proud
{
	template<typename PNSTRING>
	inline void AnsiStrTraits::ToStdString(const PNSTRING& src, std::string& dest)
	{
		dest = src.GetString();
	}
	template<typename PNSTRING>
	inline void AnsiStrTraits::ToStdWString(const PNSTRING& src, std::wstring& dest)
	{
		dest = StringA2W(src.GetString());
	}

	template<typename PNSTRING>
	inline void AnsiStrTraits::FromStdString(const std::string& src, PNSTRING& dest)
	{
		dest = src.c_str();
	}

	template<typename PNSTRING>
	inline void AnsiStrTraits::FromStdWString(const std::wstring& src, PNSTRING& dest)
	{
		dest = StringW2A(src.c_str());
	}

	template<typename PNSTRING>
	inline void UnicodeStrTraits::ToStdString(const PNSTRING& src, std::string& dest)
	{
		dest = StringW2A(src.GetString());
	}
	template<typename PNSTRING>
	inline void UnicodeStrTraits::ToStdWString(const PNSTRING& src, std::wstring& dest)
	{
		dest = src.GetString();
	}

	template<typename PNSTRING>
	inline void UnicodeStrTraits::FromStdString(const std::string& src, PNSTRING& dest)
	{
		dest = StringA2W(src.c_str());;
	}
	template<typename PNSTRING>
	inline void UnicodeStrTraits::FromStdWString(const std::wstring& src, PNSTRING& dest)
	{
		dest = src.c_str();
	}
}