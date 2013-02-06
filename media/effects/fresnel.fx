
#include "diffuse.fx"

uniform matrix matWorldInv;
uniform float4 eyePos;

uniform samplerCUBE sampler1 : register(s1);

void vs_fresnel(
	in out	float4 pos		: POSITION,
	in		float3 norm		: NORMAL,
	in out	float2 tex		: TEXCOORD0,
	out		float3 view		: TEXCOORD1,
	out		float3 wnorm	: TEXCOORD2)
{
	pos = mul(pos, matWorld);
	wnorm = mul(matWorldInv, float4(norm, 0)).xyz;

	tex *= uv;

	view = pos.xyz - eyePos.xyz;
	pos = mul(pos, matViewProj);
}

void ps_fresnel(
	in	float2 tex		: TEXCOORD0,
	in	float3 view		: TEXCOORD1,
	in	float3 wnorm	: TEXCOORD2,
	out	float4 color0	: COLOR0)
{
	wnorm = normalize(wnorm);
	view = normalize(view);

	float4 base = tex2D(sampler0, tex);
	float4 ref = texCUBE(sampler1, reflect(view, wnorm).xyz);

	// this not the correct approximation
	float fresnel = 1.0f - dot(wnorm, -view);

	color0 = lerp(base, ref, fresnel);
	color0.a = 1;
}

technique fresnel
{
	pass p0
	{
		vertexshader = compile vs_2_0 vs_fresnel();
		pixelshader = compile ps_2_0 ps_fresnel();
	}
}
