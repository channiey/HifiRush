#pragma once

/* ���� �ִϸ��̼� �ð��뺰 Ʈ������ �� ���� ���� Ű�������� �� ���� �ִ�. */

#include "Base.h"

BEGIN(Engine)

class CChannel final : public CBase
{
public:
	CChannel();
	virtual ~CChannel() = default;

public:
	//HRESULT Initialize(aiNodeAnim*	pAIChannel);
	HRESULT Initialize(const string& strName, vector<KEYFRAME>& Keyframes);
	_uint Update_Transformation(_float fPlayTime, _uint iCurrentKeyFrame, class CHierarchyNode* pNode); 

public:
	const char* Get_Name() const { return m_szName; }
	const vector<KEYFRAME>& Get_KeyFrames() const { return m_KeyFrames; }



private:
	char							m_szName[MAX_PATH] = "";
	vector<KEYFRAME>				m_KeyFrames;
	_uint							m_iNumKeyFrames = 0;

public:
	//static CChannel* Create(aiNodeAnim*	pAIChannel);
	static CChannel* Create(const string& strName, vector<KEYFRAME>& Keyframes);
	virtual void Free() override;
};

END