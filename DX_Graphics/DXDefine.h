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
#include "../KH_Math/Matrix3x3.h"
#include "../KH_Math/Matrix4x4.h"
#include "Vertex.h"

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

#define HR(x) 	if (FAILED(x)) { return hr; }

constexpr float PI = 3.14159265f;
constexpr int VK_A = 0x41;
constexpr int VK_B = 0x42;
constexpr int VK_C = 0x43;
constexpr int VK_D = 0x44;
constexpr int VK_E = 0x45;
constexpr int VK_F = 0x46;
constexpr int VK_G = 0x47;
constexpr int VK_H = 0x48;
constexpr int VK_I = 0x49;
constexpr int VK_J = 0x4A;
constexpr int VK_K = 0x4B;
constexpr int VK_L = 0x4C;
constexpr int VK_M = 0x4D;
constexpr int VK_N = 0x4E;
constexpr int VK_O = 0x4F;
constexpr int VK_P = 0x50;
constexpr int VK_Q = 0x51;
constexpr int VK_R = 0x52;
constexpr int VK_S = 0x53;
constexpr int VK_T = 0x54;
constexpr int VK_U = 0x55;
constexpr int VK_V = 0x56;
constexpr int VK_W = 0x57;
constexpr int VK_X = 0x58;
constexpr int VK_Y = 0x59;
constexpr int VK_Z = 0x5A;