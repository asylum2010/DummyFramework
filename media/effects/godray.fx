
#define NUM_SAMPLES 23

uniform float3 lightPos;
uniform float exposure;
uniform float2 texelSize;

uniform float density	= 0.8f;
uniform float decay		= 1.0f;
uniform float weight	= 1.0f;

uniform sampler2D sampler0 : register(s0);
uniform sampler2D sampler1 : register(s1);

void ps_godray(
	in	float2 tex		: TEXCOORD0,
	out	float4 color0	: COLOR0)
{
	float2 delta = (tex - lightPos.xy);
	float3 color = tex2D(sampler0, tex);
	float3 sample;

	// elhalványulás
	float illum = 1.0f;

	// milyen sürüséggel veszünk mintát
	delta *= density / NUM_SAMPLES;

	for( int i = 0; i < NUM_SAMPLES; ++i )
	{
		tex -= delta;
		sample = tex2D(sampler0, tex);

		sample *= (illum * weight);
		color += sample;

		illum *= decay;
	}

	color /= NUM_SAMPLES;
	color0 = float4(color * exposure, 1);
}

void ps_blur(
	in	float2 tex		: TEXCOORD0,
	out	float4 color0	: COLOR0)
{
	float2 delta = normalize(tex - lightPos.xy) * texelSize;
	color0 = 0;

	for( int i = 0; i <= NUM_SAMPLES; ++i )
		color0 += tex2D(sampler0, tex - i * delta);

	color0 /= NUM_SAMPLES;
}

void ps_final(
	in	float2 tex		: TEXCOORD0,
	out	float4 color0	: COLOR0)
{
	float4 base = tex2D(sampler0, tex);
	float4 shaft = tex2D(sampler1, tex) * float4(1.0f, 0.97f, 0.88f, 1);
	shaft = saturate(pow(shaft, 2.0f) * 3.5f);

	tex -= 0.5f;
	float v = 1 - dot(tex, tex);

	shaft *= v;
	color0 = base + shaft;
}

void ps_nothing(
	in	float2 tex		: TEXCOORD0,
	out	float4 color0	: COLOR0)
{
	color0 = tex2D(sampler0, tex);
}

technique godray
{
	pass p0
	{
		pixelshader = compile ps_2_0 ps_godray();
	}
}

technique blur
{
	pass p0
	{
		pixelshader = compile ps_2_0 ps_blur();
	}
}

technique final
{
	pass p0
	{
		pixelshader = compile ps_2_0 ps_final();
	}
}

technique nothing
{
	pass p0
	{
		pixelshader = compile ps_2_0 ps_nothing();
	}
}
