
uniform matrix matWorld;
uniform matrix matWorldInv;
uniform matrix matViewProj;

uniform float4 lightPos = { 542, 740, 1000, 1 };
uniform float4 eyePos;
uniform float2 uv = { 1, 1 };

uniform sampler2D sampler0 : register(s0);

void vs_blinnphong(
    in out float4 pos   : POSITION,
        in float3 norm  : NORMAL,
    in out float2 tex   : TEXCOORD0,
       out float3 wnorm : TEXCOORD1,
       out float3 ldir  : TEXCOORD2,
       out float3 vdir  : TEXCOORD3)
{
    pos = mul(pos, matWorld);
    
    vdir = eyePos.xyz - pos.xyz;
    ldir = lightPos.xyz - pos.xyz;
    
    pos = mul(pos, matViewProj);
    wnorm = mul(matWorldInv, float4(norm, 0)).xyz;
}

void ps_blinnphong(
    in float2 tex     : TEXCOORD0,
    in float3 wnorm   : TEXCOORD1,
    in float3 ldir    : TEXCOORD2,
    in float3 vdir    : TEXCOORD3,
   out float4 color0  : COLOR0)
{
    float3 n = normalize(wnorm);
    float3 l = normalize(ldir);
    float3 v = normalize(vdir);
    float3 h = normalize(v + l);
    float4 b = tex2D(sampler0, tex);
    
    float diffuse = dot(n, l);
    float specular = saturate(dot(n, h));
    
    specular = pow(specular, 26) * 0.55f;
    
    color0 = b * saturate(diffuse * 0.7f + 0.3f) + specular;
    color0.a = b.a;
}

void ps_blinnphong_mrt(
    in float2 tex     : TEXCOORD0,
    in float3 wnorm   : TEXCOORD1,
    in float3 ldir    : TEXCOORD2,
    in float3 vdir    : TEXCOORD3,
   out float4 color0  : COLOR0,
   out float4 color1  : COLOR1)
{
    ps_blinnphong(tex, wnorm, ldir, vdir, color0);
    color1 = float4(0, 0, 0, 1);
}

void ps_alpha(
    out float4 color0 : COLOR0)
{
    color0 = float4(0, 0, 0, 1);
}

technique blinnphong
{
    pass p0
    {
        vertexshader = compile vs_2_0 vs_blinnphong();
        pixelshader = compile ps_2_0 ps_blinnphong();
    }
}

technique blinnphong_mrt
{
    pass p0
    {
        vertexshader = compile vs_2_0 vs_blinnphong();
        pixelshader = compile ps_2_0 ps_blinnphong_mrt();
    }
}

technique alpha
{
    pass p0
    {
        vertexshader = compile vs_2_0 vs_blinnphong();
        pixelshader = compile ps_2_0 ps_alpha();
    }
}
