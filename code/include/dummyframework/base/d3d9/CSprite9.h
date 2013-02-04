//=============================================================================================================
#ifndef _SPRITE9_H_
#define _SPRITE9_H_

#include <d3dx9.h>

namespace DummyFramework
{
	/**
	 * \brief Represents a 2D object
	 *
	 * This class uses a collision mask for
	 * collision detection. This part of the
	 * framework is yet to be improved.
	 */
	class CSprite9
	{
	private:
		char*		mask;						/*!< Collision mask */
		tagSIZE		masksize;					/*!< Mask size (real image size) */

	public:
		D3DXVECTOR2			OriginalSize;	/*!< Original size (as in the atlas) */
		D3DXVECTOR2			Position;		/*!< Position for collision detection */
		D3DXVECTOR2			Size;			/*!< Size of the sprite object */
		LPDIRECT3DTEXTURE9	Texture;		/*!< Texture or texture atlas which contains the image */
		float				Texcoords[4];	/*!< Texture coordinates */

		CSprite9();
		~CSprite9();

		CSprite9& operator =(const CSprite9& other);
		void GenerateMask();

		static const unsigned int Tolerance = 10;
		static bool Collide(D3DXVECTOR2& out, const CSprite9& a, const CSprite9& b);
	};
}

#endif
//=============================================================================================================

