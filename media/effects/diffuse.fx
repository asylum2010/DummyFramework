
uniform matrix matWorld;
uniform matrix matViewProj;

uniform float2 uv = { 1, 1 };
uniform float4 diffColor = { 1, 1, 1, 1 };

uniform sampler2D sampler0 : register(s0);

void vs_diffuse(
	in out	float4 pos	: POSITION,
	in out	float2 tex	: TEXCOORD0)
{
	pos = mul(mul(pos, matWorld), matViewProj);
	tex *= uv;
}

void ps_diffuse(
	in	float2 tex		: TEXCOORD0,
	out	float4 color0	: COLOR0)
{
	color0 = tex2D(sampler0, tex);
}

void ps_color(
	out float4 color0 : COLOR0)
{
	color0 = diffColor;
}

technique diffuse
{
	pass p0
	{
		vertexshader = compile vs_2_0 vs_diffuse();
		pixelshader = compile ps_2_0 ps_diffuse();
	}
}

technique color
{
	pass p0
	{
		vertexshader = compile vs_2_0 vs_diffuse();
		pixelshader = compile ps_2_0 ps_color();
	}
}
