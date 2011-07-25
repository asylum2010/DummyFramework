//=============================================================================================================
#include "background.h"
#include <d3d9.h>

Background::Background()
    : GameElement()
{
    current = NULL;
    Speed = 30.0f;
}
//=============================================================================================================
void Background::AddElement(size_t index, const DummyFramework::CSprite9& sprite)
{
    dnerror(, "Background::AddElement(): Index out of range", index >= layers.size());
    derror(, "Background::AddElement(): The sprite has no texture", sprite.Texture);

    layer& lr = layers[index];
    
	// az elsöt mindig berakjuk a kirajzolandok közé
    if( lr.elements.size() == 0 )
        lr.visible.push_back(instance(0, 0));
    else
    {
        instance_rit& last = lr.visible.rbegin();

		// a többit csak akkor ha láthatoak
        if( last->x.current() + sprite.Size.x < GameVariables::ScreenWidth )
            lr.visible.push_back(instance(lr.elements.size(), last->x.current() + sprite.Size.x));
    }

    lr.elements.push_back(sprite);
}
//=============================================================================================================
void Background::Draw()
{
	// ezt nem lehet dinamikus vertexbufferrel, hiszen külön texturák
    float alpha = GameVariables::GetAlpha();
    float x;

    for( size_t i = 0; i < layers.size(); ++i )
    {
        layer& lr = layers[i];
        current = NULL;

        for( instance_it it = lr.visible.begin(); it != lr.visible.end(); ++it )
        {
			// elkerülendö a lyukakat
            if( current == NULL )
                x = floor(it->x.value);

            DummyFramework::CSprite9& sprite = lr.elements[it->index];
            it->x.smooth(alpha);

            transform._11 = sprite.Size.x;
            transform._22 = lr.height;
            transform._41 = x - 0.5f;
			transform._42 = -0.5f;

			// ne állitgassunk fölöslegesen
            if( sprite.Texture != current )
            {
                game->StateManager->SetTexture(0, sprite.Texture);
                current = sprite.Texture;
            }

            game->DrawImage(transform, sprite.Texcoords, 0xffffffff);
            x = floor(x + transform._11);
        }
    }
}
//=============================================================================================================
void Background::Reset(size_t layercount)
{
    layers.clear();
    layers.resize(layercount);

    D3DXMatrixIdentity(&transform);
}
//=============================================================================================================
void Background::ResizeSprites()
{
    for( size_t i = 0; i < layers.size(); ++i )
    {
        layer& lr = layers[i];

        for( size_t j = 0; j < lr.elements.size(); ++j )
        {
            DummyFramework::CSprite9& sprite = lr.elements[j];

            sprite.Size.x = GameVariables::CorrelateH(sprite.OriginalSize.x);
            sprite.Size.y = GameVariables::CorrelateH(sprite.OriginalSize.y);
        }
    }
}
//=============================================================================================================
void Background::SetLayer(size_t index, float speed, size_t elements)
{
    dnerror(, "Background::SetLayer(): Index out of range", index >= layers.size());

    layers[index].speed = speed;
    layers[index].height = GameVariables::ScreenHeight;
    layers[index].elements.reserve(elements);
}
//=============================================================================================================
void Background::Update()
{
    float tmp;

    for( size_t i = 0; i < layers.size(); ++i )
    {
        layer& lr = layers[i];

        for( instance_it it = lr.visible.begin(); it != lr.visible.end(); ++it )
            it->x.extend(-Speed * layers[i].speed);

        if( lr.visible.size() > 0 )
        {
            instance_rit& last = lr.visible.rbegin();
            instance_it& first = lr.visible.begin();

            DummyFramework::CSprite9& slast = lr.elements[last->index];
            DummyFramework::CSprite9& sfirst = lr.elements[first->index];

            // feltételezi, hogy a textura legalább akkora mint a képernyö szélessége
            tmp = last->x.current() + slast.Size.x;

            if( tmp < GameVariables::ScreenWidth )
            {
                instance inst;

                inst.index = (last->index + 1) % lr.elements.size();
                inst.x.current() = tmp;
                inst.x.previous() = last->x.previous() + slast.Size.x;

                lr.visible.push_back(inst);
            }

            if( first->x.current() < -sfirst.Size.x )
            {
                if( first->countdown == 0 )
                    lr.visible.pop_front();
                else
                    --first->countdown;
            }
        }
    }
}
//=============================================================================================================
