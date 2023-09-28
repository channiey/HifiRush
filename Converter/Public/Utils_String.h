#pragma once

#include "..\Default\pch.h"

class Utils_String
{
public:
	/* String */
	static bool StartsWith(string str, string comp);
	static bool StartsWith(wstring str, wstring comp);

	static void Replace(OUT string& str, string comp, string rep);
	static void Replace(OUT wstring& str, wstring comp, wstring rep);

	static wstring ToWString(string value);
	static string ToString(wstring value);

};

