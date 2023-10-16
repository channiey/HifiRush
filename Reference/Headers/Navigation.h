#pragma once

#include "Component.h"

/*
	1. 일괄 bake : 
		
		- 모든 스태틱 오브젝트를 전부 배치한 상태에서 bake 

		- 레벨 상의 모든 스태틱 메시를 하나의 네비메시로 처리

			-> 서로 다른 메시의 처리 : 이후 수동으로 셀 삭제 추가 처리 

	2. 복층 처리 : 

		- 일괄 베이크 : 

			- 옵션 1 : 각도 (업벡터와 생성될 셀의 노말을 계산하여 특정 각 이상이라면 셀을 생성하지 않는다.)

			- 옵션 2 : 면적 (면적이 지정 값 이하라면 셀을 생성하지 않는다.)

		- 복층에서의 처리 : 어차피 인덱스로 움직이게 되어있음 

	3. 이웃, 인덱스 처리

		- 자동, 수동 다 찍은 다음에 서로 이어주기 (삼각형의 2정점이 같다면 이웃으로 판정)
		- 인덱스는 굳이 필요 없을 듯 

	4. 떨어짐 처리
		- isJump일 때는 네비의 y값 적용 안하면 될듯

	5. 메모리는 ? 

*/

BEGIN(Engine)
class CCell;

class ENGINE_DLL CNavigation final : public CComponent
{
private:
	CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNavigation(const CNavigation& rhs);
	virtual ~CNavigation() = default;

public:
	virtual HRESULT Initialize_Prototype(const wstring & strNavigationDataFiles);
	virtual HRESULT Initialize(void* pArg) override;	

private:
	_uint							m_iCurrentIndex = { 0 };
	vector<CCell*>					m_Cells; /* 얕은 복사 처리 */

public:
	static CNavigation* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strNavigationDataFiles);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END