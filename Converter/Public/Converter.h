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
	HRESULT Binarize_Model(string fileName, string savePath, const MODEL_TYPE& modelType);

private:
	/* Read */
	HRESULT Read_AssetFile(string srcPath, const MODEL_TYPE& modelType);

private: 
	/* Export */
	HRESULT	Export_BoneData(string savePath);
	HRESULT Export_MeshData(string savePath, const MODEL_TYPE& modelType);
	HRESULT Export_MaterialData(string srcPath, string savePath);
	HRESULT	Export_AnimData(string savePath);

private:
	/* Bones */
	void Read_BoneData(aiNode* node, int32 index, int32 parent, int32 depth);
	HRESULT Write_BoneData(string savePath);

	/* Meshes */
	HRESULT Read_MeshData(MODEL_TYPE modelType);
	HRESULT Write_MeshData(string savePath);

	/* Materials */
	HRESULT Read_MaterialData();
	HRESULT Write_MaterialData(string srcPath, string savePath);

	/* Anims */
	HRESULT Read_AnimData();
	HRESULT Write_AnimData(string savePath);

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
};

