//***************************************************************************************
// color.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Transforms and colors geometry.
//***************************************************************************************


cbuffer cbPerObject
{
    float4x4 worldViewProj;
    float4x4 invTworldViewProj;
};

Texture2D g_Texture;
SamplerState g_Sampler
{
    Filter = ANISOTROPIC;
    AddressU = WRAP;
    AddressV = WRAP;
    AddressW = WRAP;
    MaxAnisotropy = 1;
};

bool istexture;

cbuffer cbPerFrame
{
    float3 lightDirection;
    float3 lightPosition;
    float4 lightColor;
};

struct VertexIn
{
    float3 PosL : POSITION;
    float2 uv : TEXCOORD;
    float3 Normal : NORMAL;
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float2 uv : TEXCOORD;
    float Diffuse : COLOR;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout;
    float3 N = vin.Normal;
    N = normalize(N);
    float3 L = lightDirection;

    vout.PosH = mul(float4(vin.PosL, 1.0f), worldViewProj);
    vout.uv = vin.uv;
    vout.Diffuse = dot(N, L) * 0.5f + 0.5f; // Half Lambert

    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
    float4 c;
    if(istexture)
    {
        c = g_Texture.Sample(g_Sampler, pin.uv);
        return c * pin.Diffuse;
    }
    else
    {
        c = float4(1.f, 1.f, 1.f, 1.f);
        return c * pin.Diffuse;
    }
    // ºû°ú ÇÈ¼¿ ³ë¸»ÀÌ¶û ³»ÀûÇÔ.
}

technique11 Tech
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
}
