#pragma once
#ifndef  GLOBAL_H_
#define  GLOBAL_H_
#pragma warning(push, 0)
#include <FBXLoader/fbxsdk/scene/geometry/fbxnurbs.h>
#include <FBXLoader/fbxsdk.h>
#include <xstring>
#include <DirectXTex/DirectXTex.h>

#include <algorithm>
#include <limits>
#pragma warning(pop)

#pragma warning(push)
#pragma warning( disable : X4717 C26813 C26495)
#pragma warning(pop)



#include <vector>
#include <list>
#include <map>
#include <string>

using std::vector;
using std::list;
using std::map;
using std::make_pair;

using std::string;
using std::wstring;


#include <typeinfo>

// FileSystem
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
using namespace std::experimental;
using namespace std::experimental::filesystem;

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <wrl.h>

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")

using namespace DirectX;
using namespace DirectX::PackedVector;
using namespace Microsoft::WRL;


#include "SimpleMath.h"
using namespace DirectX::SimpleMath;



// Fbx Loader
#include <FBXLoader/fbxsdk.h>

#ifdef _DEBUG
#pragma comment(lib, "FBXLoader/x64/debug/libfbxsdk-md.lib")
#else
#pragma comment(lib, "FBXLoader/x64/release/libfbxsdk-md.lib")
#endif



#include "singleton.h"
#include "define.h"
#include "struct.h"
#include "func.h"

#include <fstream>
#include "json.h"
#include "json-forwards.h"
#include <cmath>


#endif 