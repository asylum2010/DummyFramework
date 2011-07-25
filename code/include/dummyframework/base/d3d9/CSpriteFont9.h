//=============================================================================================================
#ifndef _CSPRITEFONT9_H_
#define _CSPRITEFONT9_H_

#include "CScreenQuad9.h"
#include <string>
#include <vector>

// TODO: path + texname -bol töltse a texturát

namespace DummyFramework
{
	class CGame9;

    /**
     * \brief Sprite based font drawing
     *
     * Supports BMFont files.
     */
    class CSpriteFont9
    {
        /**
         * \brief Character description
         */
        struct schardesc
        {
            unsigned short x, y;
            unsigned short width, height;
            unsigned short xadvance;
            unsigned short page;
            short xoffset, yoffset;

            schardesc()
                : x(0), y(0), width(0), height(0), xoffset(0), yoffset(0), xadvance(0), page(0)    {}
        };

        /**
         * \brief Character set (ASCII)
         *
         * For the time being only single page sets are supported.
         */
        struct scharset
        {
            LPDIRECT3DTEXTURE9 texture;
            unsigned short lineheight;
            unsigned short base;
            unsigned short width, height;
            unsigned short pages;
            schardesc chars[256];
        };
        
        /**
         * \brief A text object
         */
        struct stext
        {
            D3DXVECTOR2    position;
            std::string    text;
            unsigned long  color;
            unsigned long  alignment;
            float          scale;
            bool           visible;
        };
        
        typedef std::vector<scharset> charsetlist;
        typedef std::vector<stext> textlist;

        /**
         * \brief Group of texts
         *
         * It is more efficient to organize texts into groups, since
         * only the current form that has focus should be rendered.
         */
        struct stextgroup
        {
            size_t   fontid;
            float    scale;
            textlist texts;

            stextgroup(size_t id) : fontid(id), scale(1.0f) {}
        };

        typedef std::vector<stextgroup> grouplist;

    private:
        CGame9*              game;
        CDynamicQuadBuffer9  quad;
        charsetlist          fonts;
        grouplist            groups;
        size_t               currentgroup;
        bool                 initialized;

    public:
        CSpriteFont9();
		~CSpriteFont9() {}

        size_t AddFont(const std::string& file);
        size_t AddGroup(size_t fontid);
        size_t AddText(size_t groupid, const std::string& text);

        void Initialize(CGame9& mygame);
        void SetText(size_t groupid, size_t textid, const std::string& text);
        void SetTextPosition(size_t groupid, size_t textid, const D3DXVECTOR2& position);
        void SetTextColor(size_t groupid, size_t textid, unsigned long color);
        void SetTextAlignment(size_t groupid, size_t textid, unsigned long alignment);
        void SetTextVisibility(size_t groupid, size_t textid, bool visible);
        void SetTextScale(size_t groupid, size_t textid, float scale);
        void Set(size_t groupid, size_t textid, const std::string& text, const D3DXVECTOR2& position, unsigned long alignment, unsigned long color);
        void SetGroupScale(size_t groupid, float scale);
        
        D3DXVECTOR2& MeasureString(D3DXVECTOR2& out, size_t fontid, const std::string& str) const;
        D3DXVECTOR2& MeasureText(D3DXVECTOR2& out, size_t groupid, size_t textid) const;

		float GetTextScale(size_t groupid, size_t textid);

        size_t Draw(size_t groupid);
        size_t DrawCurrent();

        inline void SetCurrentGroup(size_t group) {
            currentgroup = group;
        }
    };
}

#endif
//=============================================================================================================


