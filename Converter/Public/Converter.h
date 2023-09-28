#pragma once

#include "..\Default\pch.h"

#include "asTypes.h"
#include "Utils_String.h"
#include "Utils_File.h"

class CConverter
{
public:
	CConverter();
	virtual ~CConverter();

public:
	void Set_SrcPath(wstring strPath) { _assetPath = strPath; }
	void Set_ExtractPath(wstring extractPath) { _extractPath = extractPath; }

public:
	HRESULT Binarize_Model_Static(wstring srcPath, wstring savePath);
	HRESULT Binarize_Model_Anim(wstring srcPath, wstring savePath);

private:
	HRESULT Read_AssetFile(wstring srcPath, const MODEL_TYPE modelType);

private: 
	HRESULT	Export_BoneData(wstring savePath);
	HRESULT Export_MeshData(wstring savePath);
	HRESULT Export_MaterialData(wstring savePath);
	HRESULT	Export_AnimData(wstring savePath);

private:
	/* Bones */
	HRESULT Read_BoneData(aiNode* node, int32 index, int32 parent, int32 depth);
	HRESULT Write_BoneData(wstring savePath);

	/* Meshes */
	HRESULT Read_MeshData(MODEL_TYPE modelType);
	HRESULT Write_MeshData(wstring savePath);

	/* Materials */
	HRESULT Read_MaterialData();
	HRESULT Write_MaterialData(wstring savePath);

	/* Anims */
	HRESULT Read_AnimData();
	HRESULT Write_AnimData(wstring savePath);

private:
	/* Helper */
	uint32 Get_BoneIndex(const string& name);

private:
	shared_ptr<Assimp::Importer>	_importer;
	const aiScene*					_scene = { nullptr };

private:
	vector<shared_ptr<asBone>>		_bones;
	vector<shared_ptr<asMesh>>		_meshes;
	vector<shared_ptr<asMaterial>>	_materials;
	vector<shared_ptr<asAnimation>>	_animations;

private:
	wstring _assetPath;
	wstring _extractPath;
};

