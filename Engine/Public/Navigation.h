#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CNavigation final : public CComponent
{

public:
	/* �� ������Ʈ �ʱ�ȭ�� �����̴� ��ü�� �ʱ� ��ġ�� �Ű������� ������ ���� ����ü  */
	/* ����ƽ�� ������� �ʰ�, ���̳��� �ֵ鸸 �ʱ� ��ġ �ְ� ��� */
	typedef struct tagNavigationDesc 
	{
		_int			iCurIndex = { -1 };

		tagNavigationDesc(const _int& _iCurIndex) : iCurIndex(_iCurIndex) {};

	}NAVIGATION_DESC;

private:
	CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNavigation(const CNavigation& rhs);
	virtual ~CNavigation() = default;

public:
	virtual HRESULT		Initialize_Prototype(const wstring & strNavigationDataFiles);
	virtual HRESULT		Initialize(void* pArg) override;	
	void				Update(_fmatrix WorldMatrix);
#ifdef _DEBUG
	HRESULT				Render();
#endif

public:
	_bool				Can_Move(_fvector vPoint);

private:
	HRESULT				Set_Neighbors();

	HRESULT				Clear_CellData();

private:
	static _float4x4				m_WorldMatrix; // ����ƽ ��ü�� ���� ��Ʈ����
	_int							m_iCurrentIndex = { -1 }; // �� ������̼��� �̿��Ͽ� �����̴� ��ü�� ���� � ���� �ִ��� 
	vector<class CCell*>			m_Cells;

#ifdef _DEBUG
	class CShader*					m_pShader = { nullptr };
#endif

public:
	static CNavigation* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strNavigationDataFiles);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END