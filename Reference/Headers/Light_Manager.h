#pragma once

#include "Base.h"

BEGIN(Engine)

class CLight_Manager final : public CBase
{
	DECLARE_SINGLETON(CLight_Manager)
private:
	CLight_Manager();
	virtual ~CLight_Manager() = default;

public:
	const LIGHT_DESC*	Get_LightDesc(_uint iLightIndex);

public:
	HRESULT				Add_Light(const LIGHT_DESC& LightDesc);
	HRESULT				Render(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);

public:
	/* Shadow */
	void				Set_ShadowLight_OriginPos(Vec4 vPos)	{ memcpy(&m_ShadowLight_OriginPos, &vPos, sizeof(Vec4)); }
	Vec4				Get_ShadowLight_OriginPos() const		{ return m_ShadowLight_OriginPos; }

	void				Set_ShadowLight_TargetPos(Vec4 vPos)	{ memcpy(&m_ShadowLight_TargetPos, &vPos, sizeof(Vec4)); }
	Vec4				Get_ShadowLight_TargetPos() const		{ return m_ShadowLight_TargetPos; }

	Matrix				Get_ShadowLight_MatView() const;
	Matrix				Get_ShadowLight_MatProj() const { 
		return m_ShadowLight_MatProj; 
	}
	void				Set_ShadowLight_MatProj();

private:
	list<class CLight*>	m_Lights;

	/* Shadow */
	Vec4				m_ShadowLight_OriginPos; /* 그림자를 만들 빛의 위치 */
	Vec4				m_ShadowLight_TargetPos; /* 그림자를 만들 빛이, 바라보는 지점 */

	Matrix				m_ShadowLight_MatProj;
public:
	virtual void Free() override;
};

END