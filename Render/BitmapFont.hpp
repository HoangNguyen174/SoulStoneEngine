#ifndef BITMAPFONT_H
#define BITMAPFONT_H
#undef new
#include <vector>
//#define new new( __FILE__, __LINE__ )
#include "../Utilities/Vector2.hpp"
#include "../Utilities/Texture.hpp"

struct BitmapFontGlyph
{
	Vector2 texCoordMins;
	Vector2 texCoordMax;
	float ttfA;
	float ttfB;
	float ttfC;
};

class BitmapFont
{
	public:
		std::vector<BitmapFontGlyph> m_bitmapFontGlypdDataList;
		Texture* m_fontTextureSheet;
		
	public:
		BitmapFont() {};
		BitmapFont( const std::string& bitmapFontDataName,const std::string& m_fontSheet );
		~BitmapFont();

	private:
		Vector2 ParseMinMaxString(const std::string& myStr);
};

#endif