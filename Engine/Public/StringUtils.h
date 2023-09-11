#pragma once

#include "Base.h"

class StringUtils
{
public:
	static bool StartsWith(string str, string comp);
	static bool StartsWith(wstring str, wstring comp);

	static void Replace(OUT string& str, string comp, string rep);
	static void Replace(OUT wstring& str, wstring comp, wstring rep);

	static wstring ToWString(string value);
	static string ToString(wstring value);


	
	
	//cf. wstring -> string -> const char*

	//	wstring wstr = L"banana";

	//	string str(wstr.begin(), wstr.end()); wstring -> string

	//	wstring wstr(str.begin(), str.end()); string -> wstring

	//	const char* ch = str.c_str();
	

	//static std::string WS2S(std::wstring& wstr)
	//{
	//	/* wstring -> string */
	//	std::string str(wstr.begin(), wstr.end());
	//	return str;
	//}

	//static std::wstring S2WS(const std::string str) /* &ºÙÀÌ¸é ¾ÈµÊ */
	//{
	//	/* string -> wstring */
	//	return std::wstring(str.begin(), str.end());
	//}

	//static std::string WC2S(const wchar_t* _in)
	//{
	//	/* const wchar_t* -> string */
	//	std::wstring ws(_in);
	//	return std::string(ws.begin(), ws.end());
	//}

	//static char* WC2C(const wchar_t* _in)
	//{
	//	/* const wchar_t* -> char */
	//	int strSize = WideCharToMultiByte(CP_ACP, 0, _in, -1, NULL, 0, NULL, NULL);
	//	char* pStr = new char[WideCharToMultiByte(CP_ACP, 0, _in, -1, NULL, 0, NULL, NULL)];
	//	WideCharToMultiByte(CP_ACP, 0, _in, -1, pStr, strSize, 0, 0);

	//	return pStr;
	//}

};

