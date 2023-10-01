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

private:
	_uint						m_iNumChannels = 0; /* 채널 갯수 */
	vector<class CChannel*>		m_Channels; /* 사용되는 채널 집합 */

	_float						m_fDuration = 0.f; /* 전체 재생 시간 */
	_float						m_fPlayTime = 0.f; /* 현재 재생 시간 */
	_float						m_fTickPerSecond = 0.f; /* 초당 재생 속도 */

private: /* 복제된 애니메이션 마다 따로 가진다. */
	vector<class CBone*>	m_Bones; /* 이 애니메이션을 재생하는데 필요한 뼈 */
	vector<_uint>					m_ChannelKeyFrames;

public:
	static CAnimation* Create(const _float& fDuration, const _float& fTickPerSecond, vector<class CChannel*>& Channels);
	CAnimation* Clone(class CModel* pModel);
	virtual void Free() override;
};

END