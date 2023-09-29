#include "MainApp_Converter.h"
#include "Converter.h"

static string g_srcPath = "../Assets/";
static string g_destPath = "../../Client/Bin/Resources/Models/";
CMainApp_Converter::CMainApp_Converter()
{
}

CMainApp_Converter::~CMainApp_Converter()
{
}

HRESULT CMainApp_Converter::Start_Convert()
{
	cout << "Start Binarization\n\n\n\n";
	{
		if (FAILED(Convert_Player()))
			return E_FAIL;

		if (FAILED(Convert_Env()))
			return E_FAIL;




	}
	cout << "\n\n\nComplete Entire Binarization";
	int temp(getchar());//Sleep(10000);
	return S_OK;
}

HRESULT CMainApp_Converter::Convert_Player()
{
	const MODEL_TYPE type = MODEL_TYPE::ANIM;
	{
		shared_ptr<CConverter> converter = make_shared<CConverter>();
		if (FAILED(converter->Binarize_Model("Chai", "Character/Chai", type)))
			ASSERT_LOG();
	}


	return S_OK;
}

HRESULT CMainApp_Converter::Convert_Monster()
{
	const MODEL_TYPE type = MODEL_TYPE::ANIM;
	{

	}

	return S_OK;
}

HRESULT CMainApp_Converter::Convert_Npc()
{
	const MODEL_TYPE type = MODEL_TYPE::ANIM;
	{

	}

	return S_OK;
}

HRESULT CMainApp_Converter::Convert_Env()
{
	const MODEL_TYPE type = MODEL_TYPE::STATIC;
	{
		shared_ptr<CConverter> converter = make_shared<CConverter>();
		if (FAILED(converter->Binarize_Model("Architecture", "Environment/Architecture", type)))
			ASSERT_LOG();
	}

	return S_OK;
}

