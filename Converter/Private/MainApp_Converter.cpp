#include "MainApp_Converter.h"
#include "Converter.h"

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
		/*if (FAILED(Convert_Character()))
			return E_FAIL;*/

		//if (FAILED(Convert_Env_Static()))
		//	return E_FAIL;

		//if (FAILED(Convert_Env_Anim()))
		//	return E_FAIL;

		//if (FAILED(Convert_SelectiveModel()))
		//	return E_FAIL;

	}
	cout << "\n\n\nComplete Entire Binarization";
	int temp(getchar());//Sleep(10000);
	return S_OK;
}

HRESULT CMainApp_Converter::Convert_Character()
{
	const MODEL_TYPE	type = MODEL_TYPE::ANIM;

	const string		filePath	= "../Assets/Character";
	vector<string>		fileNames	= Utils_File::Get_AllFolderNames(filePath);
	const string		savePath	= "Character/";

	for (string& name : fileNames)
	{
		shared_ptr<CConverter> converter = make_shared<CConverter>();
		if (FAILED(converter->Binarize_Model(name, savePath + name, type)))
			ASSERT_LOG();
	}
	return S_OK;
}

HRESULT CMainApp_Converter::Convert_Env_Static()
{
	const MODEL_TYPE	type = MODEL_TYPE::STATIC;

	const string		filePath = "../Assets/Environment/Static";
	vector<string>		fileNames = Utils_File::Get_AllFolderNames(filePath);
	const string		savePath = "Environment/Static/";

	for (string& name : fileNames)
	{
		shared_ptr<CConverter> converter = make_shared<CConverter>();
		if (FAILED(converter->Binarize_Model(name, savePath + name, type)))
			ASSERT_LOG();
	}
	return S_OK;
}

HRESULT CMainApp_Converter::Convert_Env_Anim()
{
	const MODEL_TYPE	type = MODEL_TYPE::ANIM;

	const string		filePath = "../Assets/Environment/Anim";
	vector<string>		fileNames = Utils_File::Get_AllFolderNames(filePath);
	const string		savePath = "Environment/Anim/";

	for (string& name : fileNames)
	{
		shared_ptr<CConverter> converter = make_shared<CConverter>();
		if (FAILED(converter->Binarize_Model(name, savePath + name, type)))
			ASSERT_LOG();
	}
	return S_OK;
}

HRESULT CMainApp_Converter::Convert_SelectiveModel()
{
	{
		const MODEL_TYPE	type = MODEL_TYPE::STATIC;
		const string		savePath = "Environment/Static/";
		const string		fileName = "StreetLamp_A";
		shared_ptr<CConverter> converter = make_shared<CConverter>();
		if (FAILED(converter->Binarize_Model(fileName, savePath + fileName, type)))
			ASSERT_LOG();
	}
	return S_OK;
}

