//***************************************************************************************
// color.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Transforms and colors geometry.
//***************************************************************************************

cbuffer cbPerObject
{
    float4x4 worldViewProj;
};

cbuffer cbPerFrame
{
    float3 lightDirection;
    float3 lightPosition;
    float4 lightColor;
};

struct VertexIn
{
    float3 PosL : POSITION;
    float3 normal : NORMAL;
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float4 Color : COLOR0;
    float Diffuse : COLOR1;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout;

    // Transform to homogeneous clip space.
    vout.PosH = mul(float4(vin.PosL, 1.0f), worldViewProj);

    float3 N = vin.normal;
    float3 L = normalize(lightDirection);
    
    // Just pass vertex color into the pixel shader.
    vout.Color = float4(0.5f, 0.5f, 0.5f, 1.f);
    vout.Diffuse = dot(N, L) * 0.5f + 0.5f;

    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
    float4 c = pin.Color * pin.Diffuse;
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
