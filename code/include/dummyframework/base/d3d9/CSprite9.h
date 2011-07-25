//=============================================================================================================
#ifndef _SPRITE9_H_
#define _SPRITE9_H_

#include <d3dx9.h>

namespace DummyFramework
{
    /**
     * \brief Represents a 2D object
     *
     * For smaller textures it is more efficient to use
	 * a pre-calculated mask for collision detection
     * than locking the texture.
     */
    class CSprite9
    {
    private:
        char*    mask;                     /*!< collision mask */
        tagSIZE  masksize;                 /*!< mask size (real image size) */

    public:
        D3DXVECTOR2         OriginalSize;  /*!< original size (as in the atlas) */
		D3DXVECTOR2         Position;      /*!< position for collision detection */
        D3DXVECTOR2         Size;          /*!< size of the sprite object */
        LPDIRECT3DTEXTURE9  Texture;       /*!< texture or texture atlas which contains the image */
        float               Texcoords[4];  /*!< texture coordinates */
        
        CSprite9();
        ~CSprite9();

        CSprite9& operator =(const CSprite9& other);
        void GenerateMask();

        static const unsigned long Tolerance = 10;
        static bool Collide(D3DXVECTOR2& out, const CSprite9& a, const CSprite9& b);
    };
}

#endif
//=============================================================================================================

