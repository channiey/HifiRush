#pragma once

/* Warning */
#pragma warning (disable : 4251)
#pragma warning (disable : 4038) /* warning STL4038: The contents of <filesystem> are available only with C++17 or later. */
#pragma warning (disable : 4244) /* stringUtils */

/* DX*/
#include <d3d11.h>

#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <Effect11\d3dx11effect.h>

using namespace DirectX;

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
