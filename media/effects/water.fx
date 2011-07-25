
uniform matrix matWorld;
uniform matrix matViewProj;

uniform float4 eyePos;
uniform float2 uv = { 1.7f, 1.7f };

uniform float time = 0;
uniform float waterheight = 0.15f;
      
uniform sampler2D wavetex       : register(s0);
uniform sampler2D refractionmap : register(s1);
uniform sampler2D reflectionmap : register(s2);

static const matrix matReflect =
{
    -0.5,    0,   0, 0.5,
       0, -0.5,   0, 0.5,
       0,    0, 0.5, 0.5,
       0,    0,   0,   1
};

static const matrix matRefract =
{
     0.5,    0,   0, 0.5,
       0, -0.5,   0, 0.5, 
       0,    0, 0.5, 0.5,
       0,    0,   0,   1
};

static const float2 wavedir1 = { -0.02, 0 };
static const float2 wavedir2 = { 0, -0.013 };
static const float2 wavedir3 = { 0.007, 0.007 };

void vs_water(
    in out float4 pos      : POSITION,
    in out float2 tex      : TEXCOORD0,
       out float4 ratex    : TEXCOORD1,
       out float4 letex    : TEXCOORD2,
       out float3 viewws   : TEXCOORD3)
{
    pos = mul(pos, matWorld);
    viewws = eyePos.xyz - pos.xyz;
    
    tex *= uv;
    pos = mul(pos, matViewProj);
    
    ratex = mul(matRefract, pos);
    letex = mul(matReflect, pos);
}

void ps_water(
    in float2 tex      : TEXCOORD0,
    in float4 ratex    : TEXCOORD1,
    in float4 letex    : TEXCOORD2,
    in float3 viewws   : TEXCOORD3,
   out float4 color0   : COLOR)
{
    viewws = normalize(viewws);
    float4 distort = 0;
    
    distort += (tex2D(wavetex, tex + time * wavedir1) * 2 - 1) * 0.3f;
    distort += (tex2D(wavetex, tex + time * wavedir2) * 2 - 1) * 0.3f;
    distort += (tex2D(wavetex, tex + time * wavedir3) * 2 - 1) * 0.4f;

    tex = ratex.xy / ratex.w;
    
    float4 refr1 = tex2D(refractionmap, tex + distort.xy * 0.02f);
    float4 refr2 = tex2D(refractionmap, tex);
    float4 refr = refr2 * refr1.w + refr1 * (1 - refr1.w);

    tex = letex.xy / letex.w;
    float4 refl = tex2D(reflectionmap, tex + distort.xy * 0.02f);
    
    float fresnel = 0.13f + 0.87f * pow((1.0f - dot(viewws, float3(0, 1, 0))), 4);
    fresnel = saturate(fresnel);
    
    // nem jo, a feny tangens terben van de a view vektor nem...
    viewws.yz = viewws.zy;
    
    float3 l = normalize(float3(0, -1, 1));
    float3 h = normalize(viewws + l);
    float3 n = normalize(distort.xyz);
    
    float diff = saturate(dot(l, n) + 0.3f);
    float specular = saturate(dot(n, h));

    specular = saturate(pow(specular, 200)) * 0.6f;
        
    color0 = lerp(refr, refl, fresnel) * diff + specular;
    color0.a = 1;
}

void vs_alpha(
    in out float4 pos : POSITION)
{
    pos = mul(mul(pos, matWorld), matViewProj);
}

void ps_alpha(
    out float4 color0 : COLOR)
{
    color0 = float4(0, 0, 0, 0);
}

technique water
{
    pass p0
    {
        vertexshader = compile vs_2_0 vs_water();
        pixelshader = compile ps_2_0 ps_water();
    }
}

technique alpha
{
    pass p0
    {
        vertexshader = compile vs_2_0 vs_alpha();
        pixelshader = compile ps_2_0 ps_alpha();
    }
}
