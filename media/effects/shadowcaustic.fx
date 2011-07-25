
#include "diffuse.fx"

uniform samplerCUBE sampler1 : register(s1);
uniform samplerCUBE sampler2 : register(s2);

uniform float4 refPoint = { 0, 0, 0, 1 };

/**
 * \brief Light-mapped diffuse vertex shader
 */
void vs_shadowcaustic(
    in out float4 pos   : POSITION,
    in out float2 tex   : TEXCOORD0,
       out float3 pdir  : TEXCOORD1)
{
    pos = mul(pos, matWorld);
    pdir = pos.xyz - refPoint.xyz;
    
    pos = mul(pos, matViewProj);
    tex *= uv;
}

/**
 * \brief Light-mapped diffuse pixel shader
 */
void ps_shadowcaustic(
    in float2 tex    : TEXCOORD0,
    in float3 pdir   : TEXCOORD1,
   out float4 color0 : COLOR0)
{
    float4 base = tex2D(sampler0, tex);
    float i = texCUBE(sampler1, pdir).r;
    float d = texCUBE(sampler2, pdir).a;
    
    //float e = abs(d - length(pdir));
    //color0 = float4(e, e, e, 1);
    
    //if( abs(d - length(pdir)) < 0.1f )
        color0 = base * i;
    //else
    //    color0 = base;
    
    color0.a = 1;
}

/**
 * \brief Projective light-mapped geometry
 */
technique shadowcaustic
{
    pass p0
    {
        vertexshader = compile vs_3_0 vs_shadowcaustic();
        pixelshader = compile ps_3_0 ps_shadowcaustic();
    }
}
