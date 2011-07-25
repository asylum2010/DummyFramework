//=============================================================================================================
#include <dummyframework/template/defines.h>
#include <dummyframework/base/d3d9/CSprite9.h>
#include <cstring>
#include <cmath>

namespace DummyFramework
{
    CSprite9::CSprite9()
    {
        Texture = 0;
        mask = 0;
    }
    //=============================================================================================================
    CSprite9::~CSprite9()
    {
        if( mask )
            delete[] mask;

        mask = NULL;
    }
    //=============================================================================================================
    CSprite9& CSprite9::operator =(const CSprite9& other)
    {
        if( &other != this )
        {
            Texture = other.Texture;
            Size = other.Size;

            memcpy(Texcoords, other.Texcoords, 4 * sizeof(float));

            if( other.mask )
            {
                masksize.cx = other.masksize.cx;
                masksize.cy = other.masksize.cy;

                mask = new char[other.masksize.cx * other.masksize.cy];
                memcpy(mask, other.mask, other.masksize.cx * other.masksize.cy * sizeof(char));
            }
        }

        return *this;
    }
    //=============================================================================================================
    void CSprite9::GenerateMask()
    {
        D3DLOCKED_RECT    rect;
        D3DSURFACE_DESC   desc;
        D3DXVECTOR4       realrect;
        HRESULT           hr;
        DWORD             stride;
        unsigned long     color;
        unsigned long     alpha;
        unsigned long     k = 0;
        unsigned long*    ptr;

        hr = Texture->GetLevelDesc(0, &desc);
        dnerror(, "Sprite::GenerateMask(): Could not get level desc", FAILED(hr));

        hr = Texture->LockRect(0, &rect, NULL, 0);
        dnerror(, "Sprite::GenerateMask(): Could not lock texture", FAILED(hr));

        realrect.x = Texcoords[0] * (float)desc.Width;  // left
        realrect.y = Texcoords[1] * (float)desc.Height; // top
        realrect.z = Texcoords[2] * (float)desc.Width;  // right
        realrect.w = Texcoords[3] * (float)desc.Height; // bottom

        masksize.cx = (long)(realrect.z - realrect.x);
        masksize.cy = (long)(realrect.w - realrect.y);

        safe_delete(mask);
        mask = new char[masksize.cx * masksize.cy];

        stride = rect.Pitch / desc.Width;
        dnerror(, "Sprite::GenerateMask(): Only 32 bit textures supported", stride != 4);

        ptr = (unsigned long*)rect.pBits;
        
        long xoff = (long)realrect.x;
        long yoff = (long)realrect.y;
        long x, y;

        for( long i = 0; i < masksize.cy; ++i )
        {
            for( long j = 0; j < masksize.cx; ++j )
            {
                x = xoff + j;
                y = yoff + i;

                color = *(ptr + y * desc.Width + x);
                alpha = (color >> 24) & 0xff;
                mask[i * masksize.cx + j] = (alpha < Tolerance ? 0 : 1);
            }
        }

        Texture->UnlockRect(0);
    }
    //=============================================================================================================
	bool CSprite9::Collide(D3DXVECTOR2& out, const CSprite9& a, const CSprite9& b)
	{
		dnerror(false, "Sprite::Collide(): One of the sprites has no collision mask", !a.mask || !b.mask);
	    
		D3DXVECTOR2 topleft(
			max(a.Position.x, b.Position.x),
			max(a.Position.y, b.Position.y));

		D3DXVECTOR2 bottomright(
			min(a.Position.x + a.Size.x, b.Position.x + b.Size.x),
			min(a.Position.y + a.Size.y, b.Position.y + b.Size.y));

		// gyorsteszt; ha forgatás is lesz akkor már nem fog müködni
		if( topleft.x >= bottomright.x || topleft.y >= bottomright.y )
			return false;
        
        // lassu teszt
        D3DXVECTOR4 scale(
            a.Size.x / (float)a.masksize.cx,
            a.Size.y / (float)a.masksize.cy,
            b.Size.x / (float)b.masksize.cx,
            b.Size.y / (float)b.masksize.cy);

        RECT reala;
        float ax, ay;
        int bx, by;
        char u, v;

        // a transzformáciojának inverzével áttranszformáljuk az identitás térbe
        reala.left = (long)floor((topleft.x - a.Position.x) / scale.x + 0.5f);
        reala.top = (long)floor((topleft.y - a.Position.y) / scale.y + 0.5f);
        reala.right = (long)floor((bottomright.x - a.Position.x) / scale.x + 0.5f);
        reala.bottom = (long)floor((bottomright.y - a.Position.y) / scale.y + 0.5f);
        
        // minden texelre a közös téglalapban
        for( long i = reala.left; i < reala.right; ++i )
        {
            for( long j = reala.top; j < reala.bottom; ++j )
            {
                ax = i * scale.x + a.Position.x;
                ay = j * scale.y + a.Position.y;

                bx = (int)floor((ax - b.Position.x) / scale.z + 0.5f);
                by = (int)floor((ay - b.Position.y) / scale.w + 0.5f);

                if( bx >= 0 && bx < b.masksize.cx &&
                    by >= 0 && by < b.masksize.cy )
                {
                    u = a.mask[j * a.masksize.cx + i];
                    v = b.mask[by * b.masksize.cx + bx];

                    if( u + v == 2 )
                    {
                        out.x = ax;
                        out.y = ay;

                        return true;
                    }
                }
            }
        }

        return false;
    }
    //=============================================================================================================
}
