#pragma once

#include "..\Default\pch.h"

class CMainApp_Converter
{
public:
	CMainApp_Converter();
	virtual ~CMainApp_Converter();

public:
	HRESULT Start_Convert();

private:
	HRESULT Convert_Player();
	HRESULT	Convert_Monster();
	HRESULT	Convert_Npc();
	HRESULT Convert_Env();

private:

private:


};

