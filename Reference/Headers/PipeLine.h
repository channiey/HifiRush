#pragma once

#include "Base.h"

/* Tick 함수에서 뷰와 투영의 정행렬, 역행렬을 구하고 멤버변수에 보관한다. */

BEGIN(Engine)

class CPipeLine final : public CBase
{
	DECLARE_SINGLETON(CPipeLine)

public:
	enum TRANSFORM_STATE { D3DTS_VIEW, D3DTS_PROJ, D3DTS_END };

private:
	CPipeLine();
	virtual ~CPipeLine() = default;

public:
	HRESULT Initialize();
	void	Tick();

public:
	Matrix	Get_Transform(const CPipeLine::TRANSFORM_STATE& eState) const;
	Matrix	Get_Transform_Inverse(const CPipeLine::TRANSFORM_STATE& eState) const;

	/* 카메라의 월드 포지션을 받아온다. */
	Vec4	Get_CamPosition() const { return m_vCamPosition; }

public:
	void	Set_Transform(const CPipeLine::TRANSFORM_STATE& eState, Matrix TransformMatrix);
	
public:
	/* 셰이더에 뷰행렬, 투영행렬을 세팅한다. */
	HRESULT Bind_TransformToShader(class CShader* pShader, const char* pConstantName, CPipeLine::TRANSFORM_STATE eState);	


private:
	/* 뷰, 투영 행렬을 보관한다. */
	Matrix	m_TransformMatrices[D3DTS_END];

	/* 뷰, 투영 역행렬을 보관한다. */
	Matrix	m_TransformMatrices_Inverse[D3DTS_END];

	/* 카메라 월드 포지션 */
	Vec4	m_vCamPosition = { 0.0f, 0.f, 0.f, 1.f };

public:
	virtual void Free() override;
};

END