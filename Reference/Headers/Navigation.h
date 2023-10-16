#pragma once

#include "Component.h"

/*
	1. �ϰ� bake : 
		
		- ��� ����ƽ ������Ʈ�� ���� ��ġ�� ���¿��� bake 

		- ���� ���� ��� ����ƽ �޽ø� �ϳ��� �׺�޽÷� ó��

			-> ���� �ٸ� �޽��� ó�� : ���� �������� �� ���� �߰� ó�� 

	2. ���� ó�� : 

		- �ϰ� ����ũ : 

			- �ɼ� 1 : ���� (�����Ϳ� ������ ���� �븻�� ����Ͽ� Ư�� �� �̻��̶�� ���� �������� �ʴ´�.)

			- �ɼ� 2 : ���� (������ ���� �� ���϶�� ���� �������� �ʴ´�.)

		- ���������� ó�� : ������ �ε����� �����̰� �Ǿ����� 

	3. �̿�, �ε��� ó��

		- �ڵ�, ���� �� ���� ������ ���� �̾��ֱ� (�ﰢ���� 2������ ���ٸ� �̿����� ����)
		- �ε����� ���� �ʿ� ���� �� 

	4. ������ ó��
		- isJump�� ���� �׺��� y�� ���� ���ϸ� �ɵ�

	5. �޸𸮴� ? 

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
	vector<CCell*>					m_Cells; /* ���� ���� ó�� */

public:
	static CNavigation* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strNavigationDataFiles);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END