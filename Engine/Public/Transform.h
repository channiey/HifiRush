#pragma once

#include "Component.h"

/* 객체의 월드 변환 행렬 (월드 스페이스로 가기 위한 변환 행렬)을 보관한다. */

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };

public:
	typedef struct tagTransformDesc
	{
		_float		fSpeedPerSec = { 0.f };
		_float		fRotRadPerSec = { 0.f };
	}TRANSFORM_DESC;

private:
	CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTransform(const CTransform& rhs);
	virtual ~CTransform() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public: /* Only Transform */
	const Vec4 Get_State(STATE eState) { return XMLoadFloat4x4(&m_WorldMatrix).r[eState]; }
	const Vec3 Get_Scale();
	const Vec3 Get_Rotation();
	const _float& Get_Speed() const { return m_tTrans.fSpeedPerSec; }
	const _float& Get_RotRad() const { return m_tTrans.fRotRadPerSec; }
	const Matrix& Get_WorldMat() const { return m_WorldMatrix; }
	const Matrix Get_WorldMatrix_Inverse() { return m_WorldMatrix.Invert(); }

	void Set_State(STATE eState, Vec4 vState);
	void Set_Scale(const Vec3& vScale);
	void Set_Rotation(Vec4 vAxis, _float fRadian);
	void Set_Speed(const _float& fSpeed) { m_tTrans.fSpeedPerSec = fSpeed; }
	void Set_RotRad(const _float& fRotRad) { m_tTrans.fRotRadPerSec = fRotRad; }
	void Set_WorldMat(const Matrix& matrix) { m_WorldMatrix = matrix; }

	const Vec3 ToEulerAngles(Quaternion quat);


public: /* User Convenient */
	void Move_Forward(_float fTimeDelta);
	void Move_Backward(_float fTimeDelta);
	void Move_Left(_float fTimeDelta);
	void Move_Right(_float fTimeDelta);

	void Roatate(Vec4 vAxis, _float fTimeDelta);

	void Rotate_World(Vec4 vAxis, _float fRadian);
	void Rotate_Local(Vec4 vAxis, _float fRadian);

	void LookAt(Vec4 vPoint);
	void LookAt_LandObj(Vec3 vPoint);
	void Chase(Vec3 vPoint, _float fTimeDelta, _float fMargin = 0.1f);

public:
	HRESULT Bind_ShaderResources(class CShader* pShader, const char* pConstantName);

private:
	Matrix				m_WorldMatrix = {};
	TRANSFORM_DESC		m_tTrans = {};

public:
	static CTransform* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END