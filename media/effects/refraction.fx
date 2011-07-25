
#include "raytrace.fxh"

uniform matrix matWorld;
uniform matrix matWorldInv;
uniform matrix matViewProj;

uniform float4 eyePos;

uniform samplerCUBE sampler0 : register(s0); // generator kivül
uniform samplerCUBE sampler1 : register(s1); // minden más

/**
 * \brief Reflection/refraction vertex shader
 */
void vs_refract(
    in out float4 pos   : POSITION,
        in float3 norm  : NORMAL,
       out float3 vdir  : TEXCOORD0,
       out float3 wnorm : TEXCOORD1,
       out float3 x     : TEXCOORD2)
{
    pos = mul(pos, matWorld);
    
    x = pos.xyz;
    vdir = x - eyePos.xyz;
    
    wnorm = mul(matWorldInv, float4(norm, 0)).xyz;
    pos = mul(pos, matViewProj);
}

/**
 * \brief Reflection/refraction pixel shader
 */
void ps_refract(
    in float3 vdir    : TEXCOORD0,
    in float3 wnorm   : TEXCOORD1,
    in float3 x       : TEXCOORD2,
   out float4 color0  : COLOR0)
{
    float3 n = normalize(wnorm);
    float3 v = normalize(vdir);
    float3 i, p;
    float4 f;
   
    rayTrace(i, p, n, v, x);
    f = texCUBE(sampler1, p);
    
    //color0.rgb = p.xyz;
    color0.rgb = i * f.rgb;
    color0.a = 1;
}

/**
 * \brief Reflection/refraction
 */
technique refraction
{
    pass p0
    {
        vertexshader = compile vs_3_0 vs_refract();
        pixelshader = compile ps_3_0 ps_refract();
    }
}

