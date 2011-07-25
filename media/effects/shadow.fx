
#include "diffuse.fx"

uniform matrix matLightVP : register(c8);

uniform float shadowmapSize = 512.0f;
uniform float farplane = 50.0f;

uniform sampler2D sampler1 : register(s1);

static float4x4 screenToTex =
{
	0.5,    0, 0, 0,
	  0, -0.5, 0, 0,
	  0,    0, 1, 0,
	0.5,  0.5, 0, 1
};
		
void vs_shadow(
     in out float4 pos   : POSITION,
     in out float2 tex   : TEXCOORD0,
        out float4 ltov  : TEXCOORD1)
{
    pos = mul(pos, matWorld);
    tex *= uv;
   
    ltov = mul(pos, matLightVP);
    ltov = mul(ltov, screenToTex);
        
    pos = mul(pos, matViewProj);
}

void ps_shadow(
      in float2 tex    : TEXCOORD0,
      in float4 ltov   : TEXCOORD1,
     out float4 color0 : COLOR0)
{
    float2 texelsize = float2(1.0f / shadowmapSize, 1.0f / shadowmapSize);
	float2 ptex = ltov.xy / ltov.w;
	
	float z = ltov.w / farplane;
	float d;
	float s = 0.0;

	for( float x = -1.0; x <= 1.0; x += 1.0 )
	{
		for( float y = -1.0; y <= 1.0; y += 1.0 )
		{
			d = tex2D(sampler1, ptex + float2(x, y) * texelsize).x;
			s += ((0.0001 > (z - d)) ? 1.0 : 0.0);
		}
	}
	
	s /= 9.0;
	s = clamp(s + 0.6, 0.0, 1.0);
		
	color0 = tex2D(sampler0, tex) * s;
	color0.a = 1;
}

technique shadow
{
	pass p0
    {
        vertexshader = compile vs_2_0 vs_shadow();
        pixelshader = compile ps_2_0 ps_shadow();
    }
}
