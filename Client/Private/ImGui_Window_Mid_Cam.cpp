#ifdef _DEBUG
#include "../Default/stdafx.h"

#include "ImGui_Window_Mid_Cam.h"
#include "GameObject.h"

#include "Util_String.h"
#include "Util_File.h"

#include "Camera_Debug.h"
#include "Camera_Follow.h"


CImGui_Window_Mid_Cam::CImGui_Window_Mid_Cam()
{
}

HRESULT CImGui_Window_Mid_Cam::Initialize()
{
	
	return S_OK;
}

void CImGui_Window_Mid_Cam::Show_Window()
{
	ImGuiWindowFlags window_flags = 0;
	//window_flags |= ImGuiWindowFlags_NoMove;
	//window_flags |= ImGuiWindowFlags_NoResize;

	if (ImGui::Begin(m_pImGui_Manager->str_MainWindowType[m_pImGui_Manager->WINDOW_MAIN_CAM], NULL, window_flags))
	{
		const map<_uint, CGameObject*>* Cameras = ENGINE_INSTANCE->Get_Cameras();

		/* Camera Buttons */
		ImGui::SeparatorText("Cameras");
		{
			for (auto Pair = Cameras->begin(); Pair != Cameras->end(); ++Pair)
			{
				const char* strCamName = Util_String::WC2C(CameraNames[Pair->first]);
				if (ImGui::Button(strCamName))
				{
					string DebugCamName = Util_String::ToString(wstring(CameraNames[CAM_DEBUG]));
					{
						if (DebugCamName == string(strCamName))
							m_pImGui_Manager->m_bDebugCam = TRUE;
						else 
							m_pImGui_Manager->m_bDebugCam = FALSE;
					}
					
					if (FAILED(ENGINE_INSTANCE->Change_Camera(Pair->first, 1.f, LERP_MODE::SMOOTHER_STEP)))
						assert(FALSE);
				}
				delete strCamName;
			}
		}


		/* Current Camera Property */
		ImGui::SeparatorText("Property");
		{
			if (nullptr != Cameras)
			{
				CAMERA_ID iCurCamKey = (CAMERA_ID)ENGINE_INSTANCE->Get_CurCamera()->Get_Camera()->Get_Key();

				switch (iCurCamKey)
				{
				case Client::CAM_DEBUG:
					Show_Property_DebugCam();
					break;
				case Client::CAM_FOLLOW:
					Show_Property_FollowCam();
					break;
				default:
					break;
				}
			}



		}
	}
	ImGui::End();

	/* Childe Window */
	Show_ChildWindow();
}

void CImGui_Window_Mid_Cam::Clear_Reference_Data()
{
	
}

HRESULT CImGui_Window_Mid_Cam::Show_Property_DebugCam()
{
	CCamera_Debug* pCam = dynamic_cast<CCamera_Debug*>(ENGINE_INSTANCE->Get_CurCamera());

	if (nullptr == pCam) return E_FAIL;

	
	/* Speed */
	{
		ImGui::PushItemWidth(100.f);
		{
			_float fSpeed = pCam->Get_SpeedPerSec();

			ImGui::DragFloat("SpeedPerSec", &fSpeed, 0.05f);
			{
				pCam->Set_SpeedPerSec(fSpeed);
			}
		}
		ImGui::PopItemWidth();
	}

	return S_OK;
}

HRESULT CImGui_Window_Mid_Cam::Show_Property_FollowCam()
{
	CCamera_Follow* pCam = dynamic_cast<CCamera_Follow*>(ENGINE_INSTANCE->Get_CurCamera());

	if (nullptr == pCam) return E_FAIL;

	ImGui::PushItemWidth(100.f);
	{
		CCamera* pCameraCom = pCam->Get_Camera();


		/* Fov */
		{
			_float fFov = pCameraCom->Get_Fov();

			ImGui::DragFloat("FOV", &fFov, 0.05f);
			{
				pCameraCom->Set_Fovy(fFov);
			}
		}

		/* Distance */
		{
			_float fDistance = pCameraCom->Get_Distance();

			ImGui::DragFloat("Dist", &fDistance, 0.05f);
			{
				pCameraCom->Set_Distance(fDistance);
			}
		}

		/* LookAt Offfset */
		{
			Vec4	vOffset = pCameraCom->Get_LookAtOffSet();
			_float	fOffsetY = vOffset.y;

			ImGui::DragFloat("LookAtOffsetY", &fOffsetY, 0.05f);
			{
				pCameraCom->Set_LookAtOffSet(Vec4{ vOffset.x, fOffsetY, vOffset.z, vOffset.w });
			}
		}
	}
	ImGui::PopItemWidth();

	return S_OK;

}



CImGui_Window_Mid_Cam* CImGui_Window_Mid_Cam::Create()
{
	CImGui_Window_Mid_Cam* pInstance = new CImGui_Window_Mid_Cam();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CImGui_Window_Mid_Cam");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImGui_Window_Mid_Cam::Free()
{
	__super::Free();
}

#endif // _DEBUG