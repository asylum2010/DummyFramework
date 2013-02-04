//=============================================================================================================
#ifndef _BACKGROUND_H_
#define _BACKGROUND_H_

#include "gameelement.h"

#include <dummyframework/base/d3d9/CSprite9.h>
#include <vector>
#include <list>

/**
 * \brief Multi-layered background
 *
 * A scrolling background that supports multiple layers, each with different speed and size.
 */
class Background : public GameElement
{
	/**
	 * \brief An instance of a given texture
	 *
	 * We cannot pop the instance immediately, since the rendering
	 * needs its previous position; thus the 'countdown' member.
	 */
	struct instance
	{
		size_t index;
		size_t countdown;
		syncedfloat x;

		instance() { countdown = 3; }
		instance(size_t i, float p) : index(i), x(p) {}
	};

	typedef std::vector<DummyFramework::CSprite9> elementlist;
	typedef std::list<instance> instancelist;
	typedef instancelist::iterator instance_it;
	typedef instancelist::reverse_iterator instance_rit;

	/**
	 * \brief A layer
	 */
	struct layer
	{
		float speed;
		float height;
		elementlist elements;			/*!< \brief Useable textures */
		instancelist visible;			/*!< \brief Visible elements */
	};

private:
	typedef std::vector<layer> layerlist;

	LPDIRECT3DTEXTURE9	current;		/*!< \brief To minimize SetTexture() calls */
	D3DXMATRIX			transform;		/*!< \brief Transformation */
	layerlist			layers;			/*!< \brief Layers */

public:
	float Speed;						/*!< \brief Global speed */          

	Background();
	~Background() {}

	//! Adds a sprite to the given layer
	void AddElement(size_t index, const DummyFramework::CSprite9& sprite);

	//! Renders all layers
	void Draw();

	//! Reserves memory for n layers
	void Reset(size_t layercount);

	//! Resizes the sprites. Should be called after the device is reset
	void ResizeSprites();

	//! Sets the attributes of a given layer
	void SetLayer(size_t index, float speed, size_t elements);

	//! Fixed timestep scrolling
	void Update();
};

#endif
//=============================================================================================================
