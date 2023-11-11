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
	const _uint& 	Get_Index() const { return m_iIndex; }

	const _double& Get_FrameAcc() const { return m_dFrameAcc; }
	void Set_FrameAcc(const _double& dFrameAcc) { m_dFrameAcc = dFrameAcc; }
	void Set_Index(const _uint& iIndex) { m_iIndex = iIndex; }
	void Add_FrameAcc(const _double& dFrameAcc) { m_dFrameAcc += dFrameAcc; }

	const _double& Get_SecondPerFrame() const { return m_dSecondPerFrame; }
	void Set_SecondPerFrame(const _double& dSecondPerFrame) { m_dSecondPerFrame = dSecondPerFrame; }

public:
	HRESULT Calculate_Animation(_uint iFrame);

	HRESULT	Clear_Channels();
	HRESULT Clear_Bones();

private:
	string						m_strName;

	_float						m_fDuration = 0.f;		/* ��ü ��� �ð� */
	_double						m_dFrameAcc = 0.f;		/* ���� ��� �ð� */
	_float						m_fTickPerSecond = 0.f; /* �ʴ� ��� �ӵ� (�ʴ� �����Ӽ�) */

	_double						m_dSecondPerFrame = 0.f;

	_uint						m_iMaxFrameCount = 0;

	_uint						m_iIndex = 0; /* �ִϸ��̼� �����̳ʿ��� �� �ִϸ��̼��� �ε���*/

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