
#include "raytrace.fxh"

uniform matrix matWorld;
uniform matrix matWorldInv;
uniform matrix matView;
uniform matrix matProj;
uniform matrix lightVP;

uniform float4 lightPos;
uniform float4 refPoint = { 0, 0, 0, 1 };

uniform sampler2D sampler0 : register(s0);
uniform sampler2D sampler1 : register(s1);

static const float4x4 matScale =
{
    0.5,   0,   0, 0,
      0, 0.5,   0, 0,
      0,   0, 0.5, 0,
    0.5, 0.5, 0.5, 1
};

/**
 * \brief Distance map rendering vertex shader
 */
void vs_distancemap(
    in out float4 pos    : POSITION,
        in float3 norm   : NORMAL,
       out float3 wnorm  : TEXCOORD0,
       out float3 vpos   : TEXCOORD1)
{
    pos = mul(pos, matWorld);
    
    vpos = pos.xyz - refPoint.xyz;
    pos = mul(mul(pos, matView), matProj);
        
    wnorm = mul(matWorldInv, float4(norm, 0)).xyz;
}

/**
 * \brief Distance map rendering pixel shader
 */
void ps_distancemap(
     in float3 wnorm   : TEXCOORD0,
     in float3 vpos    : TEXCOORD1,
    out float4 color0  : COLOR0)
{
    color0.rgb = normalize(wnorm);
    color0.a = length(vpos);
}

/**
 * \brief Shadow map rendering vertex shader
 */
void vs_shadowmap(
    in out float4 pos   : POSITION,
       out float3 vdir  : TEXCOORD0)
{
    pos = mul(pos, matWorld);
    
    vdir = pos.xyz - lightPos.xyz;
    pos = mul(pos, lightVP);
}

/**
 * \brief Shadow map rendering pixel shader
 */
void ps_shadowmap(
    in float3 vdir    : TEXCOORD0,
   out float4 color0  : COLOR0)
{
    float c = length(vdir);
    color0 = float4(c, c, c, 1);
}

/**
 * \brief Photon tracing vertex shader
 */
void vs_photontrace(
    in out float4 pos   : POSITION,
        in float3 norm  : NORMAL,
       out float3 vdir  : TEXCOORD0,
       out float3 wnorm : TEXCOORD1,
       out float3 x     : TEXCOORD2)
{
    pos = mul(pos, matWorld);
    
    x = pos.xyz;
    vdir = x.xyz - lightPos.xyz;
    
    wnorm = mul(matWorldInv, float4(norm, 0)).rgb;
    pos = mul(pos, lightVP);
}

/**
 * \brief Photon tracing pixel shader
 */
void ps_photontrace(
    in float3 vdir    : TEXCOORD0,
    in float3 wnorm   : TEXCOORD1,
    in float3 x       : TEXCOORD2,
   out float4 color0  : COLOR0)
{
    float3 n = normalize(wnorm);
    float3 v = normalize(vdir);
    float3 i, p;
    
    rayTrace(i, p, n, v, x);
    
    color0.rgb = p;
    color0.a = i.r + 0.01f;
}

/**
 * \brief Shadow rendering vertex shader
 */
void vs_shadow(
    in out float4 pos   : POSITION,
       out float4 ltov  : TEXCOORD1,
       out float3 vdir  : TEXCOORD2)
{
    pos = mul(pos, matWorld);
    vdir = pos.xyz - lightPos.xyz;
    
    ltov = mul(mul(pos, lightVP), matScale);
    pos = mul(mul(pos, matView), matProj);
}

/**
 * \brief Shadow rendering pixel shader
 */
void ps_shadow(
    in float2 tex    : TEXCOORD0,
    in float4 ltov   : TEXCOORD1,
    in float3 vdir   : TEXCOORD2,
   out float4 color0 : COLOR0)
{
    float sd = tex2D(sampler1, ltov.xy / ltov.w).r;
    float d = length(vdir);
    float s = 1.0f;
    
    if( sd > 0 && sd < d )
        s = 0.6f;
    
    color0 = float4(s, 0, 0, 1);
}

/**
 * \brief Caustics reconstruction vertex shader
 */
void vs_reconstruct(
    in out float4 pos : POSITION,
       out float2 rad : TEXCOORD1)
{
    float4 p = tex2Dlod(sampler0, float4(pos.xy, 0, 0));
    
    rad.x = p.a;
    rad.y = length(p.xyz); // - refPoint.xyz
    
    p.w = 1;
    pos = mul(mul(p, matView), matProj);
    
    if( rad.x == 0 )
        pos = float4(0, 0, 2, 1);
}

/**
 * \brief Caustics reconstruction pixel shader
 */
void ps_reconstruct(
    in float2 rad    : TEXCOORD1,
   out float4 color0 : COLOR0)
{
    color0 = float4(rad.x, rad.y, 0, 1);
}

/**
 * \brief Renders distance maps
 */
technique distancemap
{
    pass p0
    {
        vertexshader = compile vs_3_0 vs_distancemap();
        pixelshader = compile ps_3_0 ps_distancemap();
    }
}

/**
 * \brief Renders distance from light pos
 */
technique shadowmap
{
    pass p0
    {
        vertexshader = compile vs_3_0 vs_shadowmap();
        pixelshader = compile ps_3_0 ps_shadowmap();
    }
}

/**
 * \brief Looks for photon hits on diffuse surfaces
 */
technique photontrace
{
    pass p0
    {
        vertexshader = compile vs_3_0 vs_photontrace();
        pixelshader = compile ps_3_0 ps_photontrace();
    }
}

/**
 * \brief Renders shadows
 */
technique shadow
{
    pass p0
    {
        vertexshader = compile vs_3_0 vs_shadow();
        pixelshader = compile ps_3_0 ps_shadow();
    }
}

/**
 * \brief Reconstructs caustic patterns
 */
technique reconstruct
{
    pass p0
    {
        vertexshader = compile vs_3_0 vs_reconstruct();
        pixelshader = compile ps_3_0 ps_reconstruct();
    }
}
