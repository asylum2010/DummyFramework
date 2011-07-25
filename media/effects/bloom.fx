
uniform float2 texelSize;

uniform float bloomtreshold   = 0.45f;
uniform float baseintensity   = 1.0f;
uniform float bloomintensity  = 0.2f;
uniform float basesaturation  = 1.0f;
uniform float bloomsaturation = 0.4f;

static const float2 offsetsx[9] =
{
    { -4, 0 },
    { -3, 0 },
    { -2, 0 }, 
    { -1, 0 },
    { 0,  0 },
    { 1,  0 },
    { 2,  0 },
    { 3,  0 }, 
    { 4,  0 },
};

static const float2 offsetsy[9] =
{
    { 0, -4 },
    { 0, -3 },
    { 0, -2 }, 
    { 0, -1 },
    { 0,  0 },
    { 0,  1 },
    { 0,  2 },
    { 0,  3 }, 
    { 0,  4 },
};

static const float weights[9] =
{
    1.85548e-006f,
    0.000440052f,
    0.0218759f,
    0.227953f,
    0.497894f,
    0.227953f,
    0.0218759f,
    0.000440052f,
    1.85548e-006f,
};

uniform sampler2D sampler0 : register(s0);
uniform sampler2D sampler1 : register(s1);
uniform sampler2D sampler2 : register(s2);
uniform sampler2D sampler3 : register(s3);
uniform sampler2D sampler4 : register(s4);
uniform sampler2D sampler5 : register(s5);

/**
 * \brief Extract high luminance regions
 */
void ps_brightpass(
    in float2 tex    : TEXCOORD0,
   out float4 color0 : COLOR0)
{
    color0 = tex2D(sampler0, tex);
    color0 = saturate((color0 - bloomtreshold) / (1 - bloomtreshold));
    color0.a = 1.0f;
}

/**
 * \brief Downsample render target
 */
void ps_downsample(
    in float2 tex    : TEXCOORD0,
   out float4 color0 : COLOR0)
{
    color0 = 0;

    color0 += tex2D(sampler0, tex + texelSize * float2(-1, -1));
    color0 += tex2D(sampler0, tex + texelSize * float2(-1, 1));
    color0 += tex2D(sampler0, tex + texelSize * float2(1, -1));
    color0 += tex2D(sampler0, tex + texelSize * float2(1, 1));

    color0 *= 0.25f;
    color0.a = 1;
}

/**
 * \brief Gaussian blur in x direction
 */
void ps_blurx(
    in float2 tex    : TEXCOORD0,
   out float4 color0 : COLOR0)
{
    color0 = 0;

    for( int i = 0; i < 9; ++i )
    {
        color0 += tex2D(sampler0, tex + texelSize * offsetsx[i]) * weights[i];
    }
    
    color0.a = 1;
}

/**
 * \brief Gaussian blur in y direction
 */
void ps_blury(
    in float2 tex    : TEXCOORD0,
   out float4 color0 : COLOR0)
{
    color0 = 0;

    for( int i = 0; i < 9; ++i )
    {
        color0 += tex2D(sampler0, tex + texelSize * offsetsy[i]) * weights[i];
    }
    
    color0.a = 1;
}

/**
 * \brief Adjust color
 */
float4 adjust(float4 color, float4 saturation)
{
    float grey = dot(color.rgb, float3(0.3, 0.59, 0.11));
    return lerp(grey, color, saturation);
}

/**
 * \brief Adds glow to the scene
 */
void ps_final(
    in float2 tex    : TEXCOORD0,
   out float4 color0 : COLOR0)
{
    float4 base = tex2D(sampler0, tex);
    float4 bloom = tex2D(sampler1, tex);
    
    bloom += tex2D(sampler2, tex);
    bloom += tex2D(sampler3, tex) * 2;
    bloom += tex2D(sampler4, tex) * 3;
    bloom += tex2D(sampler5, tex) * 4;
    
    base = adjust(base, basesaturation) * baseintensity;
    bloom = adjust(bloom, bloomsaturation) * bloomintensity;
    
    color0 = (base + bloom);
    
    tex -= 0.5f;
    float v = 1 - dot(tex, tex);
    
    color0 *= v * v;
    color0.a = 1;
}

technique brightpass
{
    pass p0
    {
        pixelshader = compile ps_2_0 ps_brightpass();
    }
}

technique downsample
{
    pass p0
    {
        pixelshader = compile ps_2_0 ps_downsample();
    }
}

technique blurx
{
    pass p0
    {
        pixelshader = compile ps_2_0 ps_blurx();
    }
}

technique blury
{
    pass p0
    {
        pixelshader = compile ps_2_0 ps_blury();
    }
}

technique final
{
    pass p0
    {
        pixelshader = compile ps_2_0 ps_final();
    }
}


