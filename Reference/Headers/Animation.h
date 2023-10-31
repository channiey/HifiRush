#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CAnimation final : public CBase
{
private:
	CAnimation();
	CAnimation(const CAnimation& rhs);
	virtual ~CAnimation() = default;

public:
	HRESULT Initialize_Prototype(const _float& fDuration, const _float& fTickPerSecond, vector<class CChannel*>& Channels, const string& strName);
	HRESULT Initialize(class CModel* pModel);

public:
	const string& Get_Name() const { return m_strName; }
	_uint	Get_MaxFrameCount() const { return m_iMaxFrameCount; }
	_float	Get_TickPerSecond() const { return m_fTickPerSecond; }
	_float	Get_Duration() const { return m_fDuration; }

public:
	HRESULT Calculate_Animation(_uint iFrame);

	HRESULT	Clear_Channels();
	HRESULT Clear_Bones();

private:
	string						m_strName;

	_float						m_fDuration = 0.f;		/* ��ü ��� �ð� */
	_float						m_fPlayTime = 0.f;		/* ���� ��� �ð� */
	_float						m_fTickPerSecond = 0.f; /* �ʴ� ��� �ӵ� (�ʴ� �����Ӽ�) */

	_uint						m_iMaxFrameCount = 0;

	/* Cache */
	vector<_uint>				m_ChannelKeyFrames;		/* �� ä�ε��� ���� Ű������ */
	vector<class CChannel*>		m_Channels;				/* ���Ǵ� ä�� ���� */
	vector<class CBone*>		m_Bones;				/* �� �ִϸ��̼��� ����ϴµ� �ʿ��� ���� */

public:
	static CAnimation* Create(const _float& fDuration, const _float& fTickPerSecond, vector<class CChannel*>& Channels, const string& strName);
	CAnimation* Clone(class CModel* pModel);
	virtual void Free() override;
};

END