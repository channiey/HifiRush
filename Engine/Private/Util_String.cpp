#include "Util_String.h"

bool Util_String::StartsWith(string str, string comp)
{
	wstring::size_type index = str.find(comp);
	if (index != wstring::npos && index == 0)
		return true;

	return false;
}

bool Util_String::StartsWith(wstring str, wstring comp)
{
	wstring::size_type index = str.find(comp);
	if (index != wstring::npos && index == 0)
		return true;

	return false;
}

void Util_String::Replace(OUT string& str, string comp, string rep)
{
	string temp = str;

	size_t start_pos = 0;
	while ((start_pos = temp.find(comp, start_pos)) != wstring::npos)
	{
		temp.replace(start_pos, comp.length(), rep);
		start_pos += rep.length();
	}

	str = temp;
}

void Util_String::Replace(OUT wstring& str, wstring comp, wstring rep)
{
	wstring temp = str;

	size_t start_pos = 0;
	while ((start_pos = temp.find(comp, start_pos)) != wstring::npos)
	{
		temp.replace(start_pos, comp.length(), rep);
		start_pos += rep.length();
	}

	str = temp;
}

const wstring Util_String::Combine(const wstring& str1, const wstring& str2)
{
	return str1 + str2;
}

const wstring Util_String::Remove_LastNumChar(const wstring& str, const _uint& iNumCutWord)
{
	size_t iLength = str.length();

	if (iLength <= iNumCutWord) 
		return L"";

	wstring res = str.substr(0, iLength - iNumCutWord);
	
	return res;
}

const wstring Util_String::Get_LastNumChar(const wstring& str, const _uint& iNumCutWord)
{
	size_t iLength = str.length();

	if (iLength <= iNumCutWord) 
		return str;

	wstring res = str.substr(iLength - iNumCutWord);
	
	return res;
}


const _bool Util_String::Is_Digit(const wstring& str)
{
	/* atoi는 문자일경우 0, 숫자일 경우 해당 숫자를 반환한다. */
	/* 하지만 숫자 0일 경우에도 0을 반환하므로 추가 조건을 건다. */
	/* compare는 서로 같다면 0, 다르면 1을 리턴한다. */
	
	if (ToString(str).compare("0") == 0
		|| ToString(str).compare("00") == 0
		|| ToString(str).compare("000") == 0
		|| ToString(str).compare("0000") == 0) /* 만약 숫자 0, 00, 000이 라면 true*/
		return true;
	else if (atoi(ToString(str).c_str()) != 0) /* 만약 atoi결과가 0이 아닐 경우 true */
		return true;
	else
		return false;
}

const _bool Util_String::Has_ClonePin(const wstring& str)
{
	/* 고유 번호 앞에 _가 붙어있으므로 자릿수를 1개 제외한다. */
	_bool b = Is_Digit(Get_LastNumChar(str, CLONE_PIN_MAX_DIGIT - 1));

	return b;
}

std::wstring Util_String::ToWString(string value)
{
	return wstring(value.begin(), value.end());
}

std::string Util_String::ToString(wstring value)
{
	return string(value.begin(), value.end());
}

string Util_String::RemoveFileExtension(string str)
{
	size_t first_not_dot = str.find_first_not_of('.');
	size_t last_dot = str.find_last_of(".");
	if (last_dot == std::string::npos
		|| last_dot <= first_not_dot)
	{
		return str;
	}
	return str.substr(0, last_dot);
}

wstring Util_String::RemoveFileExtension(wstring str)
{
	string path = ToString(str);

	size_t first_not_dot = path.find_first_not_of('.');
	size_t last_dot = path.find_last_of(".");
	if (last_dot == std::string::npos
		|| last_dot <= first_not_dot)
	{
		return ToWString(path);
	}
	return ToWString(path.substr(0, last_dot));
}

string Util_String::GetFileExtension(string str)
{
	return str.substr(str.find_last_of(".") + 1);
}

wstring Util_String::GetFileExtension(wstring str)
{
	string str2 = ToString(str);

	return ToWString(str2.substr(str2.find_last_of(".") + 1));
}
