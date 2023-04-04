#pragma once

#ifdef _WIN64 

#ifdef _DEBUG 
#pragma comment(lib, "Effects11d.lib")
#else
#pragma comment(lib, "Effects11.lib")
#endif

#else

#ifdef _DEBUG 
#pragma comment(lib, "Effects11d_x86.lib")
#else
#pragma comment(lib, "Effects11_x86.lib")
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

#include <wrl.h>

#include "../CoreEngine/IGraphics.h"
#include "../KH_Math/Vector2D.h"
#include "../KH_Math/Vector3D.h"
#include "../KH_Math/Vector4D.h"
#include "../KH_Math/Matrix4x4.h"

#include "../DXTK/Inc/d3dx11effect.h"

#include <DirectXMath.h>

#include "Color.h"

#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
