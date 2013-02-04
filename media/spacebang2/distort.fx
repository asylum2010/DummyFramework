
uniform float time = 0;
uniform float2 uv = { 0.45f, 0.6f };
uniform float2 screensize = { 1024.0f, 768.0f };

uniform sampler2D wavetex	: register(s1);
uniform sampler2D scenemap	: register(s2);

static const float2 wavedir1  = { -0.02, 0 };
static const float2 wavedir2  = { 0, -0.013 };
static const float2 wavedir3  = { 0.007, 0.007 };

void ps_distort(
	out	float4 color0	: COLOR0,
	in	float2 tex		: TEXCOORD0)
{
	float4 distort = 0;
	float2 dtex = tex * uv;

	dtex.x = dtex.x * (screensize.x / screensize.y);

	distort += (tex2D(wavetex, dtex + time * wavedir1) * 2 - 1) * 0.3f;
	distort += (tex2D(wavetex, dtex + time * wavedir2) * 2 - 1) * 0.3f;
	distort += (tex2D(wavetex, dtex + time * wavedir3) * 2 - 1) * 0.4f;

	color0 = tex2D(scenemap, tex + distort.xy * 0.015f);

	float grey = dot(color0.rgb, float3(0.3, 0.59, 0.11));
	float3 h = normalize(float3(0, 0, 1) + normalize(float3(-1, 0, 1)));

	float2 tmp = (tex - 0.5f);
	float dist = 1.0f - saturate(dot(tmp, tmp) * 2);
	dist = dist * dist * 0.65f;

	float specular = saturate(dot(normalize(distort.xyz), h));
	specular = saturate(pow(specular, 150)) * dist;

	color0.rgb = lerp(color0.rgb, grey, 0.4f) + specular;
	color0.a = 1;
}

technique distort
{
	pass p0
	{
		vertexshader = null;
		pixelshader = compile ps_2_0 ps_distort();
	}
}
