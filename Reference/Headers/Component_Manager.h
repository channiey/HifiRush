#pragma once

#pragma region headers

#include "Transform.h"
#include "Model.h"

#include "Shader.h"
#include "Texture.h"
#include "Renderer.h"

#include "Collider_OBB.h"
#include "Collider_AABB.h"
#include "Collider_Sphere.h"

#include "VIBuffer_Cell.h"
#include "VIBuffer_Rect.h"
#include "VIBuffer_Cube.h"
#include "VIBuffer_Terrain.h"
#include "Mesh.h"

#include "StateMachine.h"
#include "State.h"

#include "BehaviourTree.h"
#include "Blackboard.h"
#include "Node_Root.h"
#include "Node_Sequence.h"
#include "Node_Selector.h"
#include "Node_Parallel.h"
#include "Node_Action.h"
#include "Node_Wait.h"

#include "Camera.h"
#include "Rigidbody.h"

#include "NavMeshAgent.h"

#include "MonoBehaviour.h"

#pragma endregion

BEGIN(Engine)

class CComponent_Manager final : public CBase
{
	DECLARE_SINGLETON(CComponent_Manager)

private:
	CComponent_Manager();
	virtual ~CComponent_Manager() = default;

public:
	HRESULT				Reserve_Manager(_uint iNumLevels);

public:
	HRESULT				Add_Prototype(_uint iLevelIndex, const wstring& strPrototypeTag, class CComponent* pPrototype); // 원형 -> 매니저 보관
	class CComponent*	Clone_Component(_uint iLevelIndex, const wstring& strPrototypeTag, void* pArg); // 사본 -> 객체 보관 

private:
	class CComponent*	Find_Prototype(_uint iLevelIndex, const wstring& strPrototypeTag);

private:
	_uint											m_iNumLevels = { 0 };
	map<const wstring, class CComponent*>*			m_pPrototypes = { nullptr }; // 원형 객체는 레벨별로 보관
	typedef map<const wstring, class CComponent*>	PROTOTYPES;

public:
	virtual void Free() override;
};

END