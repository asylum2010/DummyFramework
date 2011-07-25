
#include "diffuse.fx"

uniform float4 eyePos;
uniform float3 norm = { 0, 1, 0 };

uniform sampler2D sampler1 : register(s1);
uniform sampler2D sampler2 : register(s2);
uniform sampler2D sampler3 : register(s3);

static const matrix projMat =
{
   -0.5,    0,   0,  0.5, 
      0, -0.5,   0,  0.5,
      0,    0, 0.5,  0.5,
      0,    0,   0,    1
};

void vs_tiles(
    in out float4 pos   : POSITION,
    in out float2 tex   : TEXCOORD0,
       out float4 rtex  : TEXCOORD1,
       out float3 view  : TEXCOORD2)
{
    pos = mul(pos, matWorld);
    view = eyePos.xyz - pos.xyz;
    
    pos = mul(pos, matViewProj);
    tex *= uv;
    
    rtex = mul(projMat, pos);
}

void ps_tiles(
    in float2 tex    : TEXCOORD0,
    in float4 rtex   : TEXCOORD1,
    in float3 view   : TEXCOORD2,
   out float4 color0 : COLOR0)
{
    float4 distort = tex2D(sampler2, tex) * 2 - 1;
    
    distort.w = 0;
    distort = normalize(distort);
    
    rtex += distort;
    
    float4 base = tex2D(sampler0, tex);
    float4 ref = tex2Dproj(sampler1, rtex);
    
    view = normalize(view);
    
    float fresnel = 0.4f + 0.6f * pow((1.0f - dot(view, float3(0, 1, 0))), 4);
    fresnel = saturate(fresnel);
    
    color0 = lerp(base, ref, fresnel);
    color0.a = 1;
}

void ps_tiles_mrt(
    in float2 tex    : TEXCOORD0,
    in float4 rtex   : TEXCOORD1,
    in float3 view   : TEXCOORD2,
   out float4 color0 : COLOR0,
   out float4 color1 : COLOR1)
{
    ps_tiles(tex, rtex, view, color0);
    color1 = float4(0.42f, 0.42f, 0.42f, 1);
}

technique tiles
{
    pass p0
    {
        vertexshader = compile vs_2_0 vs_tiles();
        pixelshader = compile ps_2_0 ps_tiles();
    }
}

technique tiles_mrt
{
    pass p0
    {
        vertexshader = compile vs_2_0 vs_tiles();
        pixelshader = compile ps_2_0 ps_tiles_mrt();
    }
}
