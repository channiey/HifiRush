#pragma once

/* Warning */
#pragma warning (disable : 4251)
#pragma warning (disable : 4038) /* warning STL4038: The contents of <filesystem> are available only with C++17 or later. */
#pragma warning (disable : 4244) /* string utils */

/* DX (순서 함부로 바꿨다가 Simple Math랑 충돌 일어난다.) */
#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <d3dcompiler.h>
#include <Effect11\d3dx11effect.h>
#include <DirectXTK\DDSTextureLoader.h>
#include <DirectXTK\WICTextureLoader.h>
#include <DirectXTK\ScreenGrab.h>
using namespace DirectX;

#define DIRECTINPUT_VERSION	0x0800
#include <dinput.h>

/* STL */
#include <iostream>
#include <memory>

#include <vector> 
#include <list> 
#include <map> 
#include <array>
#include <queue>
#include <unordered_map>

#include <algorithm>
#include <functional>
#include <string>

/* Thread */
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>

/* Engine */
#include "Engine_Typedef.h"
#include "Engine_Macro.h"
#include "Engine_Struct.h"
#include "Engine_Function.h"

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

/* Etc */
#include <ctime>
#include <filesystem>

/* Using */
using namespace Engine;
using namespace std;
using std::thread;

/* Utility */
#include "SimpleMath.h"