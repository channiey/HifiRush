#pragma once

#include "Base.h"

BEGIN(Engine)

class CAnimation final : public CBase
{
private:
	CAnimation();
	CAnimation(const CAnimation& rhs);
	virtual ~CAnimation() = default;

public:
	HRESULT Initialize_Prototype(const _float& fDuration, const _float& fTickPerSecond, vector<class CChannel*>& Channels);
	HRESULT Initialize(class CModel* pModel);

public:
	HRESULT Play_Animation(_float fTimeDelta);
	HRESULT Calculate_Animation(_uint iFrame);
	void Reset_Animation();

public:
	vector<class CChannel*>& GetChannels() { return m_Channels; }
	_uint	GetMaxFrameCount() { return m_iMaxFrameCount; }
	_float	GetTickPerSecond() { return m_fTickPerSecond; }

private:
	vector<class CChannel*>		m_Channels;				/* 사용되는 채널 집합 */

	_float						m_fDuration = 0.f;		/* 전체 재생 시간 */
	_float						m_fPlayTime = 0.f;		/* 현재 재생 시간 */
	_float						m_fTickPerSecond = 0.f; /* 초당 재생 속도 */

	vector<class CBone*>		m_Bones;				/* 이 애니메이션을 재생하는데 필요한 뼈들 */
	vector<_uint>				m_ChannelKeyFrames;		/* 각 채널들의 현재 키프레임 */
	vector<_uint>				m_ChannelOldKeyFrames;
	_uint						m_iMaxFrameCount = 0;

	_bool						m_bPause = FALSE;

public:
	static CAnimation* Create(const _float& fDuration, const _float& fTickPerSecond, vector<class CChannel*>& Channels);
	CAnimation* Clone(class CModel* pModel);
	virtual void Free() override;
};

END