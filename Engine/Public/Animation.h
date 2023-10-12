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

	_float						m_fDuration = 0.f;		/* ��ü ��� �ð� */
	_float						m_fPlayTime = 0.f;		/* ���� ��� �ð� */
	_float						m_fTickPerSecond = 0.f; /* �ʴ� ��� �ӵ� */

	_uint						m_iMaxFrameCount = 0;

	/* Cache */
	vector<_uint>				m_ChannelKeyFrames;		/* �� ä�ε��� ���� Ű������ */
	vector<class CChannel*>		m_Channels;				/* ���Ǵ� ä�� ���� */
	vector<class CBone*>		m_Bones;				/* �� �ִϸ��̼��� ����ϴµ� �ʿ��� ���� */

public:
	static CAnimation* Create(const _float& fDuration, const _float& fTickPerSecond, vector<class CChannel*>& Channels);
	CAnimation* Clone(class CModel* pModel);
	virtual void Free() override;
};

END