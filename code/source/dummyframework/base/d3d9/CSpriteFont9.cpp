//=============================================================================================================
#include <dummyframework/base/d3d9/CSpriteFont9.h>
#include <dummyframework/base/d3d9/CGame9.h>
#include <dummyframework/base/CHelper.h>
#include <dummyframework/template/defines.h>

#include <iostream>
#include <fstream>
#include <sstream>

namespace DummyFramework
{
	CSpriteFont9::CSpriteFont9()
	{
		game = NULL;
		currentgroup = 0;
		initialized = false;
	}
	//=============================================================================================================
	void CSpriteFont9::Initialize(CGame9& mygame)
	{
		if( !initialized )
		{
			initialized = true;
			game = &mygame;
			quad.Initialize(mygame.Graphics, 200);

			mygame.lost.connect(&quad, &CDynamicQuadBuffer9::onlostdevice);
			mygame.reset.connect(&quad, &CDynamicQuadBuffer9::onresetdevice);
		}
	}
	//=============================================================================================================
	size_t CSpriteFont9::AddFont(const std::string& file)
	{
		std::ifstream infile(file.c_str());
		dnassert(0xffffffff, "CSpriteFont9::AddFont(): Could not open file", infile.fail());

		std::string			path, line, arg;
		std::string			key, value;
		std::stringstream	ss, conv;
		size_t				pos;
		scharset			font;
		unsigned short		charid;

		CHelper::GetPath(path, file);

		while( !infile.eof() )
		{
			std::getline(infile, line);

			ss.clear();
			ss.str(line);
			ss >> arg;

			if( arg == "char" )
			{
				while( !ss.eof() )
				{
					ss >> arg;

					pos = arg.find('=');
					key = arg.substr(0, pos);

					conv.clear();
					conv.str(arg.substr(pos + 1));

					if( key == "id" )
						conv >> charid;
					else if( key == "x" )
						conv >> font.chars[charid].x;
					else if( key == "y" )
						conv >> font.chars[charid].y;
					else if( key == "width" )
						conv >> font.chars[charid].width;
					else if( key == "height" )
						conv >> font.chars[charid].height;
					else if( key == "xoffset" )
						conv >> font.chars[charid].xoffset;
					else if( key == "yoffset" )
						conv >> font.chars[charid].yoffset;
					else if( key == "xadvance" )
						conv >> font.chars[charid].xadvance;
					else if( key == "page" )
						conv >> font.chars[charid].page;
				}
			}
			else if( arg == "common" )
			{
				while( !ss.eof() )
				{
					ss >> arg;

					pos = arg.find('=');
					key = arg.substr(0, pos);

					conv.clear();
					conv.str(arg.substr(pos + 1));

					if( key == "lineHeight" )
						conv >> font.lineheight;
					else if( key == "base" )
						conv >> font.base;
					else if( key == "scaleW" )
						conv >> font.width;
					else if( key == "scaleH" )
						conv >> font.height;
					else if( key == "pages" )
						conv >> font.pages;
				}
			}
			else if( arg == "page" )
			{
				while( !ss.eof() )
				{
					ss >> arg;

					pos = arg.find('=');
					key = arg.substr(0, pos);

					conv.clear();
					conv.str(arg.substr(pos + 1));

					if( key == "file" )
					{
						conv >> arg;

						font.texture = game->Content.LoadTexture(path + arg.substr(1, arg.length() - 2));
						dassert(0xffffffff, "CSpriteFont9::AddFont(): Could not load texture", font.texture);
					}
				}
			}
		}

		infile.close();
		fonts.push_back(font);

		return fonts.size() - 1;
	}
	//=============================================================================================================
	size_t CSpriteFont9::AddGroup(size_t fontid)
	{
		dnerror(0, "CSpriteFont9::AddGroup(): Invalid font id", fonts.size() <= fontid);

		groups.push_back(stextgroup(fontid));
		return groups.size() - 1;
	}
	//=============================================================================================================
	size_t CSpriteFont9::AddText(size_t groupid, const std::string& text)
	{
		dnerror(0xffffffff, "CSpriteFont9::AddText(): Invalid group id", groups.size() <= groupid);
		
		stextgroup& group = groups[groupid];
		stext t;

		t.alignment = DT_LEFT|DT_TOP;
		t.color = 0xffffffff;
		t.text = text;
		t.position.x = t.position.y = 0;
		t.scale = group.scale;

		group.texts.push_back(t);
		return group.texts.size() - 1;
	}
	//=============================================================================================================
	void CSpriteFont9::SetText(size_t groupid, size_t textid, const std::string& text)
	{
		dnerror(, "CSpriteFont9::SetText(): Invalid group id", groups.size() <= groupid);
		stextgroup& group = groups[groupid];

		dnerror(, "CSpriteFont9::SetText(): Invalid text id", group.texts.size() <= textid);
		group.texts[textid].text = text;
	}
	//=============================================================================================================
	void CSpriteFont9::SetTextPosition(size_t groupid, size_t textid, const D3DXVECTOR2& position)
	{
		dnerror(, "CSpriteFont9::SetTextPosition(): Invalid group id", groups.size() <= groupid);
		stextgroup& group = groups[groupid];

		dnerror(, "CSpriteFont9::SetTextPosition(): Invalid text id", group.texts.size() <= textid);
		group.texts[textid].position = position;
	}
	//=============================================================================================================
	void CSpriteFont9::SetTextColor(size_t groupid, size_t textid, unsigned int color)
	{
		dnerror(, "CSpriteFont9::SetTextColor(): Invalid group id", groups.size() <= groupid);
		stextgroup& group = groups[groupid];

		dnerror(, "CSpriteFont9::SetTextColor(): Invalid text id", group.texts.size() <= textid);
		group.texts[textid].color = color;
	}
	//=============================================================================================================
	void CSpriteFont9::SetTextAlignment(size_t groupid, size_t textid, unsigned int alignment)
	{
		dnerror(, "CSpriteFont9::SetTextAlignment(): Invalid group id", groups.size() <= groupid);
		stextgroup& group = groups[groupid];

		dnerror(, "CSpriteFont9::SetTextAlignment(): Invalid text id", group.texts.size() <= textid);
		group.texts[textid].alignment = alignment;
	}
	//=============================================================================================================
	void CSpriteFont9::SetTextVisibility(size_t groupid, size_t textid, bool visible)
	{
		dnerror(, "CSpriteFont9::SetTextVisibility(): Invalid group id", groups.size() <= groupid);
		stextgroup& group = groups[groupid];

		dnerror(, "CSpriteFont9::SetTextVisibility(): Invalid text id", group.texts.size() <= textid);
		group.texts[textid].visible = visible;
	}
	//=============================================================================================================
	void CSpriteFont9::SetTextScale(size_t groupid, size_t textid, float scale)
	{
		dnerror(, "CSpriteFont9::SetTextScale(): Invalid group id", groups.size() <= groupid);
		stextgroup& group = groups[groupid];

		dnerror(, "CSpriteFont9::SetTextScale(): Invalid text id", group.texts.size() <= textid);
		group.texts[textid].scale = scale;
	}
	//=============================================================================================================
	void CSpriteFont9::Set(size_t groupid, size_t textid, const std::string& text, const D3DXVECTOR2& position,
		unsigned int alignment, unsigned int color)
	{
		dnerror(, "CSpriteFont9::Set(): Invalid group id", groups.size() <= groupid);
		stextgroup& group = groups[groupid];

		dnerror(, "CSpriteFont9::Set(): Invalid text id", group.texts.size() <= textid);
		stext& t = group.texts[textid];

		t.alignment = alignment;
		t.color = color;
		t.position = position;
		t.text = text;
	}
	//=============================================================================================================
	void CSpriteFont9::SetGroupScale(size_t groupid, float scale)
	{
		dnerror(, "CSpriteFont9::SetGroupScale(): Invalid group id", groups.size() <= groupid);
		stextgroup& group = groups[groupid];

		group.scale = scale;

		for( size_t i = 0; i < group.texts.size(); ++i )
			group.texts[i].scale = scale;
	}
	//=============================================================================================================
	D3DXVECTOR2& CSpriteFont9::MeasureString(D3DXVECTOR2& out, size_t fontid, const std::string& str) const
	{
		dnerror(out, "CSpriteFont9::MeasureString(): Invalid font id", fonts.size() <= fontid);
		const scharset& font = fonts[fontid];

		out.y = font.lineheight;
		out.x = 0;
		float wmax = 0;

		for( size_t i = 0; i < str.length(); ++i )
		{
			const schardesc& desc = font.chars[(unsigned char)str[i]];
			out.x += desc.xadvance;

			if( str[i] == '\n' )
			{
				if( out.x > wmax )
					wmax = out.x;

				out.x = 0;
				out.y += font.lineheight;
			}
		}

		if( out.x > wmax )
			wmax = out.x;

		out.x = wmax;
		return out;
	}
	//=============================================================================================================
	D3DXVECTOR2& CSpriteFont9::MeasureText(D3DXVECTOR2& out, size_t groupid, size_t textid) const
	{
		dnerror(out, "CSpriteFont9::MeasureText(): Invalid group id", groups.size() <= groupid);
		const stextgroup& group = groups[groupid];

		dnerror(out, "CSpriteFont9::MeasureText(): Invalid font id", fonts.size() <= group.fontid);
		const scharset& font = fonts[group.fontid];

		dnerror(out, "CSpriteFont9::MeasureText(): Invalid text id", group.texts.size() <= textid);
		const stext& t = group.texts[textid];

		out.y = font.lineheight;
		out.x = 0;
		float wmax = 0;

		for( size_t i = 0; i < t.text.length(); ++i )
		{
			const schardesc& desc = font.chars[(unsigned char)t.text[i]];
			out.x += desc.xadvance;

			if( t.text[i] == '\n' )
			{
				if( out.x > wmax )
					wmax = out.x;

				out.x = 0;
				out.y += font.lineheight;
			}
		}

		if( out.x > wmax )
			wmax = out.x;

		out.x = wmax * t.scale;
		out.y *= t.scale;

		return out;
	}
	//=============================================================================================================
	size_t CSpriteFont9::Draw(size_t groupid)
	{
		dnerror(0, "CSpriteFont9::Write(): Invalid group id", groups.size() <= groupid);
        
		stextgroup& group = groups[groupid];
		dnerror(0, "CSpriteFont9::Write(): Invalid font id", fonts.size() <= group.fontid);

		D3DXVECTOR2		position, size;
		D3DXVECTOR2		textsize;
		scharset&		font = fonts[group.fontid];
		std::string		str;
		size_t			count = 0;
		size_t			start = 0, end = 0;
		size_t			lines;
		float			texcoords[4];
		char			ch;

		quad.Lock();

		for( textlist::iterator it = group.texts.begin(); it != group.texts.end(); ++it )
		{
			if( !it->visible )
				continue;

			if( it->alignment & DT_VCENTER )
			{
				lines = 1;

				for( size_t i = 0; i < it->text.length(); ++i )
				{
					if( it->text[i] == '\n' )
						++lines;
				}

				position.y = it->position.y - (float)(lines * font.lineheight) * it->scale * 0.5f;
			}
			else if( it->alignment & DT_BOTTOM )
			{
				lines = 1;

				for( size_t i = 0; i < it->text.length(); ++i )
				{
					if( it->text[i] == '\n' )
						++lines;
				}

				position.y = it->position.y - (float)(lines * font.lineheight * it->scale);
			}
			else
				position.y = it->position.y;

			do
			{
				end = it->text.find_first_of('\n', start);

				if( end == std::string::npos )
					str = it->text.substr(start);
				else
					str = it->text.substr(start, (end - start) + 1);

				if( it->alignment & DT_CENTER )
				{
					MeasureString(textsize, group.fontid, str);

					textsize *= it->scale;
					position.x = it->position.x - textsize.x * 0.5f;
				}
				else if( it->alignment & DT_RIGHT )
				{
					MeasureString(textsize, group.fontid, str);

					textsize *= it->scale;
					position.x = it->position.x - textsize.x;
				}
				else
					position.x = it->position.x;

				for( size_t i = 0; i < str.length(); ++i )
				{
					ch = str[i];
					schardesc& desc = font.chars[ch];

					size.x = (float)(desc.width) * it->scale;
					size.y = (float)(desc.height) * it->scale;

					// why that +1? dunno, but it works
					texcoords[0] = ((float)desc.x) / (float)font.width;
					texcoords[1] = ((float)desc.y) / (float)font.height;
					texcoords[2] = (float)(desc.x + desc.width) / (float)font.width;
					texcoords[3] = (float)(desc.y + desc.height) / (float)font.height;

					quad.WriteQuad(count,
						D3DXVECTOR2(position.x + (float)desc.xoffset * it->scale, position.y + (float)desc.yoffset * it->scale),
						size, texcoords, it->color);

					position.x += desc.xadvance * it->scale;
					++count;
				}

				start = end + 1;
				position.y += (float)font.lineheight * it->scale;
			}
			while( end != std::string::npos );
		}

		quad.Unlock();

		game->StateManager->SetTexture(0, font.texture);
		quad.Draw(count);

		return count;
	}
	//=============================================================================================================
	size_t CSpriteFont9::DrawCurrent()
	{
		return Draw(currentgroup);
	}
	//=============================================================================================================
	float CSpriteFont9::GetTextScale(size_t groupid, size_t textid)
	{
		dnerror(0, "CSpriteFont9::GetTextScale(): Invalid group id", groups.size() <= groupid);
		stextgroup& group = groups[groupid];

		dnerror(0, "CSpriteFont9::GetTextScale(): Invalid text id", group.texts.size() <= textid);
		return group.texts[textid].scale;
	}
	//=============================================================================================================
}

