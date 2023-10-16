#pragma once

#include "Transform.h"
#include "Model.h"

#include "Shader.h"
#include "Texture.h"
#include "Renderer.h"

#include "Collider_OBB.h"
#include "Collider_AABB.h"
#include "Collider_Sphere.h"

#include "VIBuffer_Rect.h"
#include "VIBuffer_Cube.h"
#include "VIBuffer_Terrain.h"
#include "Mesh.h"

#include "StateMachine.h"
#include "BehaviourTree.h"

#include "Camera.h"

#include "Navigation.h"

#include "MonoBehaviour.h"
/* ������Ʈ���� ������ �����Ѵ�. */
/* �纻�� ���� ������Ʈ�� ����ϰ��� �ϴ� ��ü���� ���� �����Ѵ�. */

BEGIN(Engine)

class CComponent_Manager final : public CBase
{
	DECLARE_SINGLETON(CComponent_Manager)

private:
	CComponent_Manager();
	virtual ~CComponent_Manager() = default;

public:
	HRESULT Reserve_Manager(_uint iNumLevels);

public:
	/* ������Ʈ ���� ��ü�� �߰��Ѵ�. -> ������Ʈ �Ŵ����� ��� �ִ�.*/
	HRESULT Add_Prototype(_uint iLevelIndex, const wstring& strPrototypeTag, class CComponent* pPrototype);
	/* ������Ʈ ������ Ŭ���Ѵ�. -> ���� ������Ʈ�� ����ϴ� ��ü�� ����ִ�.*/
	class CComponent* Clone_Component(_uint iLevelIndex, const wstring& strPrototypeTag, void* pArg);

private:
	_uint											m_iNumLevels = { 0 };
	/* ������ü���� �������� �����ұ�?! */
	map<const wstring, class CComponent*>*			m_pPrototypes = { nullptr };
	typedef map<const wstring, class CComponent*>	PROTOTYPES;

private:
	class CComponent* Find_Prototype(_uint iLevelIndex, const wstring& strPrototypeTag);

public:
	virtual void Free() override;
};

END