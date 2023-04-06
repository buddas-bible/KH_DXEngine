#pragma once

#ifdef _WIN64 

#ifdef _DEBUG 
#pragma comment(lib, "Effects11d.lib")
#pragma comment(lib, "DirectXTKd_x64.lib")
#else
#pragma comment(lib, "Effects11.lib")
#pragma comment(lib, "DirectXTK_x64.lib")
#endif

#else

#ifdef _DEBUG 
#pragma comment(lib, "Effects11d_x86.lib")
#pragma comment(lib, "DirectXTKd_x86.lib")
#else
#pragma comment(lib, "Effects11_x86.lib")
#pragma comment(lib, "DirectXTK_x86.lib")
#endif

#endif

#include <d3d11.h>
#include <d3d11_1.h>
#include <d3d11_2.h>
#pragma comment(lib, "d3d11.lib")

#include <dxgi.h>
#include <dxgi1_2.h>
#include <dxgi1_3.h>
#pragma comment(lib, "dxgi.lib")

/// Comptr을 사용하기 위한 헤더
#include <wrl.h>

#include "../CoreEngine/IGraphics.h"
#include "../KH_Math/Vector2D.h"
#include "../KH_Math/Vector3D.h"
#include "../KH_Math/Vector4D.h"
#include "../KH_Math/Matrix4x4.h"

/// FX를 사용하기 위한 헤더
#include "../DXTK/Inc/d3dx11effect.h"

/// 수학 헤더
#include <DirectXMath.h>

/// 색깔
#include "Color.h"

/// 쉐이더 파일을 직접 컴파일 하기 위한 헤더
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

#include "../DXTK/Inc/DDSTextureLoader.h"