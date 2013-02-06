
uniform matrix matWorld;
uniform matrix matViewProj;
uniform float4 eyePos;

uniform samplerCUBE sampler0 : register(s0);

void vs_sky(
	in out	float4 pos	: POSITION,
	out		float3 view	: TEXCOORD0)
{
	pos = mul(pos, matWorld);
	view = pos.xyz - eyePos.xyz;

	pos = mul(pos, matViewProj);
}

void ps_sky(
	in	float3 view		: TEXCOORD0,
	out	float4 color0	: COLOR0)
{
	color0 = float4(texCUBE(sampler0, view).rgb, 1);
}

void ps_sky_mrt(
	in	float3 view		: TEXCOORD0,
	out	float4 color0	: COLOR0,
	out	float4 color1	: COLOR1)
{
	color0 = float4(texCUBE(sampler0, view).rgb, 1);
	color1 = float4(0.42f, 0.42f, 0.42f, 1);
}

technique sky
{
	pass p0
	{
		vertexshader = compile vs_2_0 vs_sky();
		pixelshader = compile ps_2_0 ps_sky();
	}
}

technique sky_mrt
{
	pass p0
	{
		vertexshader = compile vs_2_0 vs_sky();
		pixelshader = compile ps_2_0 ps_sky_mrt();
	}
}
