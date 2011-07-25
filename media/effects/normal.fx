//=============================================================================================================
uniform matrix matWorld     : WorldTranspose    : register(c0);
uniform matrix matViewProj  : ViewProjTranspose : register(c4);

uniform float4 ambient  = { 0.3f, 0.3f, 0.3f, 1.0f };
uniform float4 lightDir = { 0, 0, -1, 1 };
uniform float2 uv       = { 1, 1 };
uniform float4 eyePos;

uniform sampler sampler0 : register(s0);
uniform sampler sampler1 : register(s1);
//=============================================================================================================
void vs_main(
        in out float4 pos     : POSITION,
            in float3 norm    : NORMAL,
            in float3 bin     : BINORMAL,
            in float3 tan     : TANGENT,
        in out float2 tex     : TEXCOORD0,
           out float3 lightts : TEXCOORD1,
           out float3 viewts  : TEXCOORD2)
{
	tex *= uv;
	
    pos = mul(matWorld, pos);
	viewts = eyePos.xyz - pos.xyz;
	pos = mul(matViewProj, pos);

	tan = normalize(mul(matWorld, float4(tan, 1))).xyz;
	bin = normalize(mul(matWorld, float4(bin, 1))).xyz;
	norm = normalize(mul(matWorld, float4(norm, 1))).xyz;

	float3x3 tbn = { tan, bin, norm };

	lightts = normalize(mul(lightDir.xyz, tbn));
	viewts = normalize(mul(viewts, tbn));
}
//=============================================================================================================
void ps_main(
         in float2 tex      : TEXCOORD0,
         in float3 lightts  : TEXCOORD1,
         in float3 viewts   : TEXCOORD2,
        out float4 color0   : COLOR)
{
	float4 base = tex2D(sampler0, tex);
	float3 norm = normalize(tex2D(sampler1, tex).rgb * 2 - 1);
	float3 h = normalize(viewts + lightts);

	float diffuse = saturate(dot(norm, lightts));

	float specular = saturate(dot(norm, h));
	specular = pow(specular, 100);
	
	color0 = base * (diffuse + ambient) + specular;
}
//=============================================================================================================
technique normal
{
    pass p0
    {
        vertexshader = compile vs_2_0 vs_main();
        pixelshader = compile ps_2_0 ps_main();
    }
}
//=============================================================================================================
