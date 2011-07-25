
uniform matrix matWorld;
uniform matrix matWorldInv;
uniform matrix matViewProj;

uniform float4 lightPos = { -10, 10, -10, 1 };
uniform float2 uv = { 1, 1 };
uniform float4 eyePos;

uniform float heightScale = 0.08f;
uniform float specularExp = 80;
uniform float specularIntensity = 0.3f;

uniform sampler2D sampler0 : register(s0);
uniform sampler2D sampler1 : register(s1);

float rayIntersectHeightfield(in float2 dp, in float2 ds)
{
    const int linear_search_steps = 15;
    const int binary_search_steps = 15;
    
    float4 t;
    float depth_step = 1.0f / binary_search_steps;
    float size = depth_step;  // ennyit lépünk egy iterácioban
    float depth = 0.0f;       // hol tartunk éppen
    float best_depth = 1.0f;  // az eddigi legjobb mélység
    
    // megkeressük az elsö olyan pontot, ami már a felületen belül van
    for( int i = 0; i < linear_search_steps - 1; ++i )
    {
        depth += size;
        t = tex2D(sampler1, dp + ds * depth);
        
        if( best_depth > 0.996f )
        {
            if( depth >= (1.0f - t.w) )
                best_depth = depth;
        }
    }

    depth = best_depth;
    
    // a talált pont környezetében keressük a pontos metszést
    for( int i = 0; i < binary_search_steps; ++i )
    {
        size *= 0.5;
        t = tex2D(sampler1, dp + ds * depth);
        
        if( depth >= (1.0f - t.w) )
        {
            best_depth = depth;
            depth -= 2 * size;
        }
        
        depth += size;
    }
    
    return best_depth;
}

void vs_main(
    in out float4 pos    : POSITION,
        in float3 norm   : NORMAL,
        in float3 tang   : TANGENT,
        in float3 bin    : BINORMAL,
    in out float2 tex    : TEXCOORD0,
       out float3 vpos   : TEXCOORD1,
       out float3 wtan   : TEXCOORD2,
       out float3 wbin   : TEXCOORD3,
       out float3 wnorm  : TEXCOORD4)
{
    wnorm = normalize(mul(matWorldInv, float4(norm, 0)).xyz);
    wtan = normalize(mul(float4(tang, 0), matWorld).xyz);
    wbin = normalize(mul(float4(bin, 0), matWorld).xyz);
    
    pos = mul(pos, matWorld);
    vpos = pos.xyz - eyePos.xyz;
    
    pos = mul(pos, matViewProj);
}

void ps_main(
    in float2 tex    : TEXCOORD0,
    in float3 vpos   : TEXCOORD1,
    in float3 wtan   : TEXCOORD2,
    in float3 wbin   : TEXCOORD3,
    in float3 wnorm  : TEXCOORD4,
   out float4 color  : COLOR0)
{
    float3 p = vpos;
    float3 v = normalize(p);
    
    float3x3 tbn = { wtan, -wbin, wnorm };
    float3 s = normalize(mul(tbn, v));
    
    // metszetkeresés
    float2 dp = tex * uv;
    float2 ds = float2(-s.x, s.y) * heightScale / s.z;
    
    float d = rayIntersectHeightfield(dp, ds);
    
    tex = dp + ds * d;
    p += v * d * s.z;
    
    // fényvektor + átrakás tangent spacebe
    float3 l = normalize(lightPos.xyz - p);
    l = normalize(mul(tbn, l));
    
    float3 n = tex2D(sampler1, tex).rgb * 2 - 1;
    float3 h = normalize(l - s);
    
    // világitás
    float diffuse = saturate(dot(n, l));
    float specular = saturate(dot(n, h));
    
    diffuse = diffuse * 0.8f + 0.2f;
    specular = pow(specular, specularExp) * specularIntensity;

#if 1
    // soft shadows
    l.xy *= heightScale;
    l.y *= -1;

    float sh0 =  tex2D(sampler1, tex).a;
    float softy = 0.58f;
    
    float shA = (tex2D(sampler1, tex + l.xy * 0.88).a - sh0) *  1 * softy;
    float sh9 = (tex2D(sampler1, tex + l.xy * 0.77).a - sh0) *  2 * softy;
    float sh8 = (tex2D(sampler1, tex + l.xy * 0.66).a - sh0) *  4 * softy;
    float sh7 = (tex2D(sampler1, tex + l.xy * 0.55).a - sh0) *  6 * softy;
    float sh6 = (tex2D(sampler1, tex + l.xy * 0.44).a - sh0) *  8 * softy;
    float sh5 = (tex2D(sampler1, tex + l.xy * 0.33).a - sh0) * 10 * softy;
    float sh4 = (tex2D(sampler1, tex + l.xy * 0.22).a - sh0) * 12 * softy;
    
    float shadow = 1 - max(max(max(max(max(max(shA, sh9), sh8), sh7), sh6), sh5), sh4);

    shadow = shadow * 0.3 + 0.7;
    diffuse *= shadow;
#endif
    
    // végsö szin
    color = tex2D(sampler0, tex) * diffuse + specular;
    color.a = 1;
}

technique relief
{
    pass p0
    {
        vertexshader = compile vs_3_0 vs_main();
        pixelshader = compile ps_3_0 ps_main();
    }
}
