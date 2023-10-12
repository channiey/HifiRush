#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CModel;
class CShader;
class CCollider;
class CRenderer;
class CTransform;
END

class CCharacter abstract  : public CGameObject
{
protected:
	CCharacter(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCharacter(const CCharacter& rhs);
	virtual ~CCharacter() = default;
};

