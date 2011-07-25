
#ifndef _RAYTRACE_FXH_
#define _RAYTRACE_FXH_

uniform samplerCUBE sampler2 : register(s2); // generator belül
uniform samplerCUBE sampler3 : register(s3); // generator kivül
uniform samplerCUBE sampler4 : register(s4); // minden más
    
/**
 * \brief Ray-surface intersection
 */
float3 rayHitSurfaceAccurate(samplerCUBE samp, float3 start, float3 raydir)
{
    int linear_steps = 50;
	int secant_steps = 2;

    bool undershoot = false;
    bool overshoot = false;
    
    float dl, llp;
    float dp, ppp;
   
    float a = length(start) / length(raydir);
    float dt = 1.0f / (float)(linear_steps + 5);
    float t = 0.01f;
    float d, ra, rrp;
    float3 r;
  	    
    // linear search
    for( int j = 0; j < linear_steps; ++j )
    {
        if( overshoot && undershoot )
            break;
        
        d = a * t / (1 - t);
        r = start + raydir * d;
        
        // |r'|
        ra = texCUBElod(samp, float4(r, 0)).a;
        
        if( ra > 0 )
        {
            // |r|/|r'|
            rrp = length(r) / ra;
            
            // undershoot
            if( rrp < 1 )
            {
                dl = d;
                llp = rrp;  
                undershoot = true;
            }
            // overshoot
            else
            {
                dp = d;
                ppp = rrp;  
                overshoot = true;
            }
        }
        else
        {
            undershoot = false;
            overshoot = false;
        }
        
        t += dt;
    }
    
    if( !(overshoot && undershoot) )
  	    return float3(0, 0, 0);
  	
    float dnew = dl;
    
    // secant search
    for( int k = 0; k < secant_steps; ++k )
    {  
        dnew = dl + (dp - dl) * (1 - llp) / (ppp - llp);
        r = start + raydir * dnew;
        
        // |r|/|r'|
        rrp = length(r) / texCUBElod(samp, float4(r, 0)).a;
        
        if( rrp < 1.0f )
        {
            llp = rrp;
            dl = dnew; 
        }
        else
        {
            ppp = rrp;
            dp = dnew;
        }
    }
        
    return start + dnew * raydir;
}

/**
 * \brief Raytraces the caustic generator
 */
void rayTrace(out float3 i, out float3 p, in float3 n, in float3 v, in float3 x)
{
    float ri = 0.8f; //1.0f / 1.3f;  // index of refraction 
    float ec = 0.5f;         // extinction coefficient
    
    i = float3(1, 1, 1);
        
    float3 f0 = ((ri - 1) * (ri - 1) + ec * ec);
    float3 f1 = ((ri + 1) * (ri + 1) + ec * ec);
    float3 fresnel;
        
    // elsö érintkezés a felülettel (bemegy)
    fresnel = (f0 + 4 * ri * pow(1 - dot(n, -v), 5)) / f1; 
    p = refract(v, n, ri);

    if( dot(p, p) == 0 )
    {
        // teljes tükrözés
        p = v - 2 * dot(v, n) * n;
    }
    else
    {
        i *= (1 - fresnel);
        
        v = p;
        p = rayHitSurfaceAccurate(sampler2, x, v);

        // nem talált pontot
        if( dot(p, p) == 0 )
        {
            p = rayHitSurfaceAccurate(sampler4, x, v);
            
            if( dot(p, p) == 0 )
                p = v;
        }
        else
        {
			float3 nr = texCUBE(sampler2, p).xyz;
	        
			// üres-e a distancemap
			if( dot(nr, nr) == 0 )
			{
				p = v;
			}
			else
			{
				// második érintkezés: kimegy
				fresnel = (f0 + 4 * ri * pow(1 - dot(-nr, -v), 5)) / f1; 
		        
				x = p;
				p = refract(v, -nr, 1.0f / ri);
				
				if( dot(p, p) == 0 )
				{
					p = v + 2 * dot(v, -nr) * nr;
				}
				else
				{
					//i *= (1 - fresnel);
		            
					// végsö photon hit
					v = p;
					p = rayHitSurfaceAccurate(sampler4, x, v);
					
					if( dot(p, p) == 0 )
						p = v;
				}
				
			}
		}
    }
}

#endif


/*
    // first undershoot
    for( int i = 0; i < 10; ++i )
    {
        if( undershoot )
            break;
        
        d = a * t / (1 - t);
        r = start + raydir * d;
        
        ra = texCUBElod(samp, float4(r, 0)).a;
        
        if( ra > 0 )
        {
            rrp = length(r) / ra;
            
            if( rrp < 1 )
            {
                dl = d;
                llp = rrp;  
                undershoot = true;
            }
        }
        else
            undershoot = false;
            
        t += dt;
    }
    
    if( !undershoot )
  	    return float3(0, 0, 0);
  	    */