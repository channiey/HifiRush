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
/* 컴포넌트들의 원형을 보관한다. */
/* 사본은 실제 컴포넌트를 사용하고자 하는 객체들이 각각 보관한다. */

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
	/* 컴포넌트 원형 객체를 추가한다. -> 컴포넌트 매니저가 들고 있다.*/
	HRESULT Add_Prototype(_uint iLevelIndex, const wstring& strPrototypeTag, class CComponent* pPrototype);
	/* 컴포넌트 원형을 클론한다. -> 실제 컴포넌트를 사용하는 객체가 들고있다.*/
	class CComponent* Clone_Component(_uint iLevelIndex, const wstring& strPrototypeTag, void* pArg);

private:
	_uint											m_iNumLevels = { 0 };
	/* 원형객체들을 레벨별로 보관할까?! */
	map<const wstring, class CComponent*>*			m_pPrototypes = { nullptr };
	typedef map<const wstring, class CComponent*>	PROTOTYPES;

private:
	class CComponent* Find_Prototype(_uint iLevelIndex, const wstring& strPrototypeTag);

public:
	virtual void Free() override;
};

END