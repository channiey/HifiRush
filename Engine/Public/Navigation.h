#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CNavigation final : public CComponent
{

public:
	/* 이 컴포넌트 초기화시 움직이는 객체의 초기 위치를 매개변수로 던지기 위한 구조체  */
	/* 스태틱은 사용하지 않고, 다이나믹 애들만 초기 위치 주고 사용 */
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
	static _float4x4				m_WorldMatrix; // 스태틱 객체의 월드 매트릭스
	_int							m_iCurrentIndex = { -1 }; // 이 내비게이션을 이용하여 움직이는 객체가 현재 어떤 셀에 있는지 
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