#include "..\Default\stdafx.h"
#include "State_Gunner_Attack.h"

#include "DebugDraw.h"


CState_Gunner_Attack::CState_Gunner_Attack()
{
}

CState_Gunner_Attack::CState_Gunner_Attack(const CState_Gunner_Attack& rhs)
{
}

HRESULT CState_Gunner_Attack::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName, pOwner)))
		return E_FAIL;

	m_pDevice = ENGINE_INSTANCE->Get_Device();
	m_pContext = ENGINE_INSTANCE->Get_Context();

	m_pBatch = new PrimitiveBatch<VertexPositionColor>(m_pContext);
	m_pEffect = new BasicEffect(m_pDevice);

	m_pEffect->SetVertexColorEnabled(true);

	const void* pShaderByteCodes = nullptr;
	size_t			iLength = 0;

	m_pEffect->GetVertexShaderBytecode(&pShaderByteCodes, &iLength);

	if (FAILED(m_pDevice->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount, pShaderByteCodes, iLength, &m_pInputLayout)))
		return E_FAIL;


	return S_OK;
}

HRESULT CState_Gunner_Attack::Enter()
{
	const _int	iRand = rand() % 2;
	CAnimation* pAnimation = nullptr;

	if (0 == iRand)
	{
		m_eAttackType = ATTACK_TYPE::GROUND;
		pAnimation = m_pModel->Get_Animation(AnimNames_GU[ATK_GROUND_INTRO_GU]);
		m_pModel->Set_Animation(pAnimation, pAnimation->Get_TickPerFrame() * 0.75f, DF_TW_TIME);
		ENGINE_INSTANCE->Play_Sound(SOUND_FILE_ID::EFC_GUNNER_READY, CHANNEL_ID::ENEMY_GUNNER, 0.5f);
	}
	else
	{
		m_eAttackType = ATTACK_TYPE::AIR;
		pAnimation = m_pModel->Get_Animation(AnimNames_GU[ATK_JUMP_INTRO_GU]);
		m_pModel->Set_Animation(pAnimation, pAnimation->Get_TickPerFrame() * 0.75f, DF_TW_TIME, TRUE, TRUE, FALSE);

		m_pGunner->m_tPhysicsDesc.bJump = TRUE;
		m_fOriginHeight = m_pGunner->Get_Transform()->Get_FinalPosition().y;

		ENGINE_INSTANCE->Play_Sound(SOUND_FILE_ID::EFC_GUNNER_JUMP, CHANNEL_ID::ENEMY_GUNNER, 0.3f);

	}

	return S_OK;
}

const wstring CState_Gunner_Attack::Tick(const _double& fTimeDelta)
{
	Detect_AttackCollision();

	return m_strName;
}

const wstring CState_Gunner_Attack::LateTick()
{
	return Check_Transition();
}

void CState_Gunner_Attack::Exit()
{
	m_bSetRay = FALSE;

	m_eAttackType = ATTACK_TYPE::TYPEEND;

	m_pGunner->m_bDebugRay = FALSE;
}

const wstring CState_Gunner_Attack::Check_Transition()
{
	if (m_pGunner->m_tFightDesc.bDamaged)
		return StateNames_GU[STATE_DAMAGED_GU];

	if(m_pModel->Is_Tween())
		return m_strName;

	const string	strCurAnimName = m_pModel->Get_CurAnimation()->Get_Name();
	const _int		iCurFrame = m_pModel->Get_TweenDesc().cur.iCurFrame;

	if (AnimNames_GU[ANIM_GU::ATK_GROUND_INTRO_GU] == strCurAnimName) /* Ground */
	{
		if (20 == iCurFrame)
		{
			CAnimation* pAnimation = m_pModel->Get_Animation(AnimNames_GU[ATK_GROUND_WAIT_GU]);
			m_pModel->Set_Animation(pAnimation, pAnimation->Get_TickPerFrame() * 0.75f, DF_TW_TIME);
			ENGINE_INSTANCE->Play_Sound(SOUND_FILE_ID::EFC_GUNNER_CHARGE, CHANNEL_ID::ENEMY_GUNNER, 0.5f);
		}
	}
	else if (AnimNames_GU[ANIM_GU::ATK_GROUND_WAIT_GU] == strCurAnimName)
	{
		if (5 <= iCurFrame && !m_bSetRay)
		{
			Set_Ray();
			m_pGunner->m_bDebugRay = TRUE;

		}

		if (25 == iCurFrame)
		{
			CAnimation* pAnimation = m_pModel->Get_Animation(AnimNames_GU[ATK_GROUND_SHOOT_GU]);
			m_pModel->Set_Animation(pAnimation, pAnimation->Get_TickPerFrame() * 0.75f, DF_TW_TIME);
			ENGINE_INSTANCE->Play_Sound(SOUND_FILE_ID::EFC_GUNNER_SHOOT, CHANNEL_ID::ENEMY_GUNNER, 0.5f);

			m_pGunner->m_bDebugRay = FALSE;
		}

	}
	else if (AnimNames_GU[ANIM_GU::ATK_GROUND_SHOOT_GU] == strCurAnimName)
	{
		if (40 == iCurFrame)
		{
			return StateNames_GU[STATE_GU::STATE_IDLE_GU];
		}
	}
	else if (AnimNames_GU[ANIM_GU::ATK_JUMP_INTRO_GU] == strCurAnimName) /* Jump */
	{
		if (20 == iCurFrame)
		{
			CAnimation* pAnimation = m_pModel->Get_Animation(AnimNames_GU[ATK_JUMP_WAIT_GU]);
			m_pModel->Set_Animation(pAnimation, pAnimation->Get_TickPerFrame() * 0.75f, DF_TW_TIME, TRUE, TRUE, FALSE);
			ENGINE_INSTANCE->Play_Sound(SOUND_FILE_ID::EFC_GUNNER_CHARGE, CHANNEL_ID::ENEMY_GUNNER, 0.5f);
		}
	}
	else if (AnimNames_GU[ANIM_GU::ATK_JUMP_WAIT_GU] == strCurAnimName)
	{
		if (5 <= iCurFrame && !m_bSetRay)
		{
			Set_Ray();
			m_pGunner->m_bDebugRay = TRUE;

		}

		if (25 == iCurFrame)
		{
			CAnimation* pAnimation = m_pModel->Get_Animation(AnimNames_GU[ATK_JUMP_SHOOT_GU]);
			m_pModel->Set_Animation(pAnimation, pAnimation->Get_TickPerFrame() * 0.75f, DF_TW_TIME, TRUE, TRUE, TRUE);
			ENGINE_INSTANCE->Play_Sound(SOUND_FILE_ID::EFC_GUNNER_SHOOT, CHANNEL_ID::ENEMY_GUNNER, 0.5f);

			m_pGunner->m_bDebugRay = FALSE;
		}
	}
	else if (AnimNames_GU[ANIM_GU::ATK_JUMP_SHOOT_GU] == strCurAnimName)
	{
		if (22 == iCurFrame)
		{
			m_pGunner->m_tPhysicsDesc.bJump = FALSE;
			return StateNames_GU[STATE_GU::STATE_IDLE_GU];
		}
	}

	return m_strName;
}

void CState_Gunner_Attack::Detect_AttackCollision()
{
	if (m_pModel->Is_Tween())
		return;

	const string	strCurAnimName = m_pModel->Get_CurAnimation()->Get_Name();
	const _int		iCurFrame = m_pModel->Get_TweenDesc().cur.iCurFrame;

	/* 그라운드 */
	if (m_bSetRay && AnimNames_GU[ANIM_GU::ATK_GROUND_SHOOT_GU] == strCurAnimName)
	{
		if (0 <= iCurFrame && 10 >= iCurFrame)
		{
			_float fDist = 0.f;
			if (m_Ray.Intersects(m_pGunner->m_tFightDesc.pTarget->Get_Collider_Sphere()->Get_Collider(), fDist))
			{
				if (!m_pGunner->m_tFightDesc.pTarget->Get_FightDesc().bDamaged)
				{
					m_pGunner->m_tFightDesc.pTarget->Damaged(m_pGunner, CCharacter::ATK_TYPE::HEAVY);
				}
			}
		}
	
	}

	/* 점프 */
	if (m_bSetRay && AnimNames_GU[ANIM_GU::ATK_JUMP_SHOOT_GU] == strCurAnimName)
	{
		if (0 <= iCurFrame && 10 >= iCurFrame)
		{
			_float fDist = 0.f;
			if (m_Ray.Intersects(m_pGunner->m_tFightDesc.pTarget->Get_Collider_Sphere()->Get_Collider(), fDist))
			{
				if (!m_pGunner->m_tFightDesc.pTarget->Get_FightDesc().bDamaged)
				{
					m_pGunner->m_tFightDesc.pTarget->Damaged(m_pGunner, CCharacter::ATK_TYPE::HEAVY);
				}
			}
		}
	}
}

void CState_Gunner_Attack::Set_Ray()
{
	Matrix matOrigin = m_pModel->Get_SocketBoneMat(CModel::BONE_SOCKET_RIGHT) * m_pGunner->Get_Transform()->Get_FinalMat();

	m_Ray.position = Vec3{ matOrigin.m[3][0],matOrigin.m[3][1], matOrigin.m[3][2] };
	
	m_Ray.direction = (m_pGunner->m_tFightDesc.pTarget->Get_Transform()->Get_FinalPosition().xyz() - m_Ray.position).Normalized();
	
	m_bSetRay = TRUE;
}

void CState_Gunner_Attack::Render_Ray()
{
	m_vColor;

	m_pEffect->SetWorld(XMMatrixIdentity());

	m_pEffect->SetView(ENGINE_INSTANCE->Get_Transform(CPipeLine::TRANSFORM_STATE::STATE_VIEW));
	m_pEffect->SetProjection(ENGINE_INSTANCE->Get_Transform(CPipeLine::TRANSFORM_STATE::STATE_PROJ));


	m_pEffect->Apply(m_pContext);

	m_pContext->IASetInputLayout(m_pInputLayout);

	m_pBatch->Begin();

	DX::DrawRay(m_pBatch, m_Ray.position, m_Ray.direction, FALSE, XMLoadFloat4(&m_vColor));

	m_pBatch->End();

	return;
}

CState_Gunner_Attack* CState_Gunner_Attack::Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	CState_Gunner_Attack* pInstance = new CState_Gunner_Attack();

	if (FAILED(pInstance->Initialize(pStateMachine, strStateName, pOwner)))
	{
		MSG_BOX("Failed to Created : CState_Gunner_Attack");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Gunner_Attack::Clone(void* pArg)
{
	return nullptr;
}

void CState_Gunner_Attack::Free()
{
}
