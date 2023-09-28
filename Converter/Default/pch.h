#ifndef PCH_H
#define PCH_H

#include "framework.h"

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
#include <iostream>
#include <vector> 
#include <string>
#include <filesystem>
using namespace std;

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

using		int8 = __int8;
using		int16 = __int16;
using		int32 = __int32;
using		int64 = __int64;
using		uint8 = unsigned __int8;
using		uint16 = unsigned __int16;
using		uint32 = unsigned __int32;
using		uint64 = unsigned __int64;

typedef		unsigned int				uint;


extern string g_srcPath;
extern string g_destPath;


#endif //PCH_H
