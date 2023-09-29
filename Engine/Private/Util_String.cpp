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
	/* atoi�� �����ϰ�� 0, ������ ��� �ش� ���ڸ� ��ȯ�Ѵ�. */
	/* ������ ���� 0�� ��쿡�� 0�� ��ȯ�ϹǷ� �߰� ������ �Ǵ�. */
	/* compare�� ���� ���ٸ� 0, �ٸ��� 1�� �����Ѵ�. */
	
	if (ToString(str).compare("0") == 0
		|| ToString(str).compare("00") == 0
		|| ToString(str).compare("000") == 0
		|| ToString(str).compare("0000") == 0) /* ���� ���� 0, 00, 000�� ��� true*/
		return true;
	else if (atoi(ToString(str).c_str()) != 0) /* ���� atoi����� 0�� �ƴ� ��� true */
		return true;
	else
		return false;
}

const _bool Util_String::Has_ClonePin(const wstring& str)
{
	/* ���� ��ȣ �տ� _�� �پ������Ƿ� �ڸ����� 1�� �����Ѵ�. */
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
