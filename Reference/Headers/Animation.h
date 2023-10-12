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
	_uint	Get_MaxFrameCount() const { return m_iMaxFrameCount; }
	_float	Get_TickPerSecond() const { return m_fTickPerSecond; }
	_float	Get_Duration() const { return m_fDuration; }
	HRESULT	Clear_Channels();
	HRESULT Clear_Bones();

public:
	HRESULT Calculate_Animation(_uint iFrame);

private:

	_float						m_fDuration = 0.f;		/* 전체 재생 시간 */
	_float						m_fPlayTime = 0.f;		/* 현재 재생 시간 */
	_float						m_fTickPerSecond = 0.f; /* 초당 재생 속도 */

	_uint						m_iMaxFrameCount = 0;

	/* Cache */
	vector<_uint>				m_ChannelKeyFrames;		/* 각 채널들의 현재 키프레임 */
	vector<class CChannel*>		m_Channels;				/* 사용되는 채널 집합 */
	vector<class CBone*>		m_Bones;				/* 이 애니메이션을 재생하는데 필요한 뼈들 */

public:
	static CAnimation* Create(const _float& fDuration, const _float& fTickPerSecond, vector<class CChannel*>& Channels);
	CAnimation* Clone(class CModel* pModel);
	virtual void Free() override;
};

END