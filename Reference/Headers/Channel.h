#pragma once

/* 본의 애니메이션 시간대별 트랜스폼 즉 여러 개의 키프레임을 갖고 있다. */

#include "Base.h"

BEGIN(Engine)

class CChannel final : public CBase
{
public:
	CChannel();
	virtual ~CChannel() = default;

public:
	HRESULT Initialize(const string strName, vector<KEYFRAME>& Keyframes);
	_uint	Update_Transformation(_float fPlayTime, _uint iCurrentKeyFrame, class CBone* pNode); 
	_uint	Update_Transformation_NoneLerp(_uint iCurrentKeyFrame, class CBone* pNode);

public:
	string					Get_Name() const { return m_szName; }
	const vector<KEYFRAME>& Get_KeyFrames() const { return m_KeyFrames; }

private:
	string					m_szName = {};
	vector<KEYFRAME>		m_KeyFrames;

public:
	static CChannel* Create(const string strName, vector<KEYFRAME>& Keyframes);
	virtual void Free() override;
};

END