//***************************************************************************************
// color.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Transforms and colors geometry.
//***************************************************************************************


cbuffer cbPerObject
{
    float4x4 worldViewProj;
};

Texture2D g_Texture;
SamplerState g_Sampler;

float3 fLight;

struct VertexIn
{
    float3 PosL     : POSITION;
    float2 uv       : TEXCOORD;
    float3 Normal   : NORMAL;
};

struct VertexOut
{
    float4 PosH     : SV_POSITION;
    float2 uv       : TEXCOORD;
    float3 Normal   : NORMAL;
    float4 Color    : COLOR;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout;

    // Transform to homogeneous clip space.
    vout.PosH = mul(float4(vin.PosL, 1.0f), worldViewProj);
    vout.uv = vin.uv;
    vout.Normal = vin.Normal;
    // Just pass vertex color into the pixel shader.

    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
    // ºû°ú ÇÈ¼¿ ³ë¸»ÀÌ¶û ³»ÀûÇÔ.
    // float s = Dot(fLight, pin.Normal);
    float4 c = g_Texture.Sample(g_Sampler, pin.uv);
    // return c * s;
    return c;
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
