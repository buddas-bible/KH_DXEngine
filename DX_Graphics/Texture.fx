//***************************************************************************************
// color.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Transforms and colors geometry.
//***************************************************************************************


cbuffer cbPerObject
{
    float4x4 worldViewProj;
    // sampler2D Texture;
};

struct VertexIn
{
    float3 PosL     : POSITION;
    // float3 Normal   : NORMAL;
    // float2 uv       : TEXCOORD0;
    float4 Color    : COLOR;
};

struct VertexOut
{
    float4 PosH     : SV_POSITION;
    // float2 uv       : TEXCOORD;
    float4 Color    : COLOR;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout;

    // Transform to homogeneous clip space.
    vout.PosH = mul(float4(vin.PosL, 1.0f), worldViewProj);
    // vout.uv = vin.uv;
    vout.Color = vin.Color;
    // Just pass vertex color into the pixel shader.

    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
    // float4 c = tex2D(Texture, pin.uv);
    // return c;
    return pin.Color;
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
