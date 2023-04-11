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

constexpr float PI = 3.14159265f;
constexpr char VK_A = 0x41;
constexpr char VK_B = 0x42;
constexpr char VK_C = 0x43;
constexpr char VK_D = 0x44;
constexpr char VK_E = 0x45;
constexpr char VK_F = 0x46;
constexpr char VK_G = 0x47;
constexpr char VK_H = 0x48;
constexpr char VK_I = 0x49;
constexpr char VK_J = 0x4A;
constexpr char VK_K = 0x4B;
constexpr char VK_L = 0x4C;
constexpr char VK_M = 0x4D;
constexpr char VK_N = 0x4E;
constexpr char VK_O = 0x4F;
constexpr char VK_P = 0x50;
constexpr char VK_Q = 0x51;
constexpr char VK_R = 0x52;
constexpr char VK_S = 0x53;
constexpr char VK_T = 0x54;
constexpr char VK_U = 0x55;
constexpr char VK_V = 0x56;
constexpr char VK_W = 0x57;
constexpr char VK_X = 0x58;
constexpr char VK_Y = 0x59;
constexpr char VK_Z = 0x5A;