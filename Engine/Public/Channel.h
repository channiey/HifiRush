#pragma once

/* 본의 애니메이션 시간대별 트랜스폼 즉 여러 개의 키프레임을 을 갖고 있다. */

#include "Base.h"

BEGIN(Engine)

class CChannel final : public CBase
{
public:
	CChannel();
	virtual ~CChannel() = default;

public:
	HRESULT Initialize(aiNodeAnim*	pAIChannel);
	_uint Update_Transformation(_float fPlayTime, _uint iCurrentKeyFrame, class CHierarchyNode* pNode); 

public:
	const char* Get_Name() const { return m_szName; }
	const vector<KEYFRAME>& Get_KeyFrames() const { return m_KeyFrames; }

private:
	char							m_szName[MAX_PATH] = "";
	vector<KEYFRAME>				m_KeyFrames;
	_uint							m_iNumKeyFrames = 0;

public:
	static CChannel* Create(aiNodeAnim*	pAIChannel);
	virtual void Free() override;
};

END