#pragma once

#include "Component.h"

/* ��ü�� ���� ��ȯ ��� (���� �����̽��� ���� ���� ��ȯ ���)�� �����Ѵ�. */

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };

private:
	CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTransform(const CTransform& rhs);
	virtual ~CTransform() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public: 
	const Vec3		Get_Scale();
	const Vec3		Get_Rotation(); /* Euler */
	const Vec4		Get_Position()  const { return  static_cast<Vec4>(m_WorldMatrix.m[STATE_POSITION]); }
	const Vec4		Get_FinalPosition();

	const Vec4		Get_Right()		const { return  static_cast<Vec4>(m_WorldMatrix.m[STATE_RIGHT]); }
	const Vec4		Get_Left()		const { return -(static_cast<Vec4>(m_WorldMatrix.m[STATE_RIGHT])); }
	const Vec4		Get_Up()		const { return  static_cast<Vec4>(m_WorldMatrix.m[STATE_UP]); }
	const Vec4		Get_Down()		const { return -(static_cast<Vec4>(m_WorldMatrix.m[STATE_UP])); }
	const Vec4		Get_Forward()	const { return  static_cast<Vec4>(m_WorldMatrix.m[STATE_LOOK]); }
	const Vec4		Get_Backward()	const { return -(static_cast<Vec4>(m_WorldMatrix.m[STATE_LOOK])); }

	const Matrix&	Get_LocalMat() const { return m_WorldMatrix; }
	const Matrix	Get_WorldMat(); /* ��Ʈ �ִϸ��̼� ������ ������� ���� ��Ʈ���� */
	const Matrix	Get_FinalMat(); /* ��Ʈ �ִϸ��̼� ������ ����� ��Ʈ���� */
	const Vec4		Get_State(STATE eState) { return XMLoadFloat4x4(&m_WorldMatrix).r[eState]; }

public: 
	void	Set_Scale(const Vec3& vScale);

	void	Set_Rotation(const Vec3& vEulers, const _bool& bWorld = FALSE); /* �׵� ����� �������� ȸ������ �����Ѵ�. (����X) */
	void	Set_Rotation(const Vec4& vEulers, const _bool& bWorld = FALSE) { Set_Rotation(vEulers.ToVec3(), bWorld); }
	void	Set_Rotation(const Vec3& vAxis, const _float fRad);
	void	Rotate(const Vec3& vEulers, const _bool& bWorld = FALSE); /* ���� ����� �������� ȸ�� ���� �����Ѵ� (����O) */
	void	Rotate(const Vec4& vEulers, const _bool& bWorld = FALSE) { Rotate(vEulers.ToVec3(), bWorld); }
	void	Rotate(const Vec4& vAxis, const _float& fRad);

	void	Translate(const Vec3& vTranslation);
	void	Translate(const Vec4& vTranslation) { Translate(vTranslation.ToVec3()); }

	void	Set_Position(Vec4& vPos) { memcpy(m_WorldMatrix.m[STATE_POSITION], &vPos, sizeof(Vec4)); }
	void	Set_Position(Vec3& vPos) { memcpy(m_WorldMatrix.m[STATE_POSITION], &vPos, sizeof(Vec3)); }

	void	Set_WorldMat(const Matrix& matrix) { memcpy(&m_WorldMatrix, &matrix, sizeof(Matrix)); }
	void	Set_Look(const Vec4& vLookDir);
	void	Set_State(STATE eState, Vec4 vState);

	void	Set_RootPos(const Vec4& vPos);

public:
	const Vec3	ToEulerAngles(Quaternion quat);
	void		LookAt(Vec4 vPoint);

public:
	HRESULT		Bind_ShaderResources(class CShader* pShader, const char* pConstantName);

private:
	Matrix		m_WorldMatrix	= {};
	Matrix		m_RotMatrix		= {};
	Vec4		m_vRootPos		= {};

public:
	static CTransform* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END