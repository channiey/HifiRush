#ifndef PCH_H
#define PCH_H

#include "framework.h"

/* Warning */
#pragma warning (disable : 4244)

/* Assimp */
#include <Assimp/scene.h>
#include <Assimp/Importer.hpp>
#include <Assimp/postprocess.h>

/* Math */
#include <DirectXMath.h>
using namespace DirectX;
#include "SimpleMath.h"

/* STL */
#include <format>
#include <iostream>
#include <vector> 
#include <string>
#include <filesystem>

using namespace std;

/* Typedef */
using		int8 = __int8;
using		int16 = __int16;
using		int32 = __int32;
using		int64 = __int64;
using		uint8 = unsigned __int8;
using		uint16 = unsigned __int16;
using		uint32 = unsigned __int32;
using		uint64 = unsigned __int64;

typedef		unsigned int				uint;

using Vec2 = DirectX::SimpleMath::Vector2;
using Vec3 = DirectX::SimpleMath::Vector3;
using Vec4 = DirectX::SimpleMath::Vector4;
using Matrix = DirectX::SimpleMath::Matrix;
using Quaternion = DirectX::SimpleMath::Quaternion;
using Ray = DirectX::SimpleMath::Ray;


/* Macro */
#ifndef			MSG
#define			MSG(_message)			MessageBox(nullptr, _message, L"System Message", MB_OK)
#endif

#ifndef			MSG_BOX
#define			MSG_BOX(_message)		MessageBox(nullptr, TEXT(_message), L"System Message", MB_OK)
#endif

#define			ASSERT_LOG()			{ const std::string message{ std::format("Error! \n\nFunction Name : {} \n\nLien Number : {}", __FUNCTION__, __LINE__ - 1) };\
											MSG(Utils_String::ToWString(message).c_str()); } // 사용시  #include "Utils_String.h" 필수

#define			LOG(_message) { cout << _message; }

/* Memory Leak */
#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif

#endif // _DEBUG

#endif //PCH_H
