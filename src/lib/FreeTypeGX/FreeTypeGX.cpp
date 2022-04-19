/*
 * FreeTypeGX for GRRLIB
 *
 * FreeTypeGX is a wrapper class for libFreeType which renders a compiled
 * FreeType parsable font into a GX texture for Wii homebrew development.
 * Copyright (C) 2008-2010 Armin Tamzarian
 *
 * This file is part of FreeTypeGX.
 *
 * FreeTypeGX is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * FreeTypeGX is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with FreeTypeGX.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <grrlib-mod.h>
#include "FreeTypeGX.h"

/**
 * Default constructor for the FreeTypeGX class.
 *
 * @param textureFormat	Optional format (GX_TF_*) of the texture as defined by the libogc gx.h header file. If not specified default value is GX_TF_RGBA8.
 */
FreeTypeGX::FreeTypeGX(uint8_t textureFormat) {
	FT_Init_FreeType(&this->ftLibrary);

	this->textureFormat = textureFormat;
}

/**
 * Default destructor for the FreeTypeGX class.
 */
FreeTypeGX::~FreeTypeGX() {
	this->unloadFont();
	FT_Done_FreeType(this->ftLibrary);
}

/**
 * Convert a short char string to a wide char string.
 *
 * This routine converts a supplied shot character string into a wide character string.
 * Note that it is the user's responsibility to clear the returned buffer once it is no longer needed.
 *
 * @param strChar	Character string to be converted.
 * @return Wide character representation of supplied character string.
 */
wchar_t* FreeTypeGX::charToWideChar(char* strChar) {
	wchar_t *strWChar = new wchar_t[strlen(strChar) + 1];

	int bt = mbstowcs(strWChar, strChar, strlen(strChar));
	if (bt) {
		strWChar[bt] = (wchar_t)'\0';
		return strWChar;
	}

	wchar_t *tempDest = strWChar;
	while((*tempDest++ = *strChar++));

	return strWChar;
}

/**
 *
 * \overload
 */
wchar_t* FreeTypeGX::charToWideChar(const char* strChar) {
	return FreeTypeGX::charToWideChar((char*) strChar);
}

/**
 * Sets the maximum video width
 *
 * This routine optionally sets the maximum video pixel width of the screen in order to improve performance.
 *
 * @param width	The pixel width of the video screen.
 * @return The new size of the video screen in pixels.
 */
int FreeTypeGX::maxVideoWidth = 0;
int FreeTypeGX::setMaxVideoWidth(int width) {
	return maxVideoWidth = width;
}

/**
 * Loads and processes a specified true type font buffer to a specific point size.
 *
 * This routine takes a precompiled true type font buffer and loads the necessary processed data into memory. This routine should be called before drawText will succeed.
 *
 * @param fontBuffer	A pointer in memory to a precompiled true type font buffer.
 * @param bufferSize	Size of the true type font buffer in bytes.
 * @param pointSize	The desired point size this wrapper's configured font face.
 * @param cacheAll	Optional flag to specify if all font characters should be cached when the class object is created. If specified as false the characters only become cached the first time they are used. If not specified default value is false.
 */
int FreeTypeGX::loadFont(uint8_t* fontBuffer, FT_Long bufferSize, FT_UInt pointSize, bool cacheAll) {
	int numCached = 0;

	//this->unloadFont(); // NEEDS FIX
	this->ftFontBuffer = (FT_Byte *)fontBuffer;
	this->ftFontBufferSize = bufferSize;
	this->ftPointSize = pointSize;

	FT_New_Memory_Face(this->ftLibrary, this->ftFontBuffer, this->ftFontBufferSize, 0, &this->ftFace);
	FT_Set_Pixel_Sizes(this->ftFace, 0, this->ftPointSize);

	this->ftKerningEnabled = FT_HAS_KERNING(this->ftFace);
	this->ftAscender = this->ftPointSize * this->ftFace->ascender / this->ftFace->units_per_EM;
	this->ftDescender = this->ftPointSize * this->ftFace->descender / this->ftFace->units_per_EM;

	if (cacheAll) {
		numCached = this->cacheGlyphDataComplete();
	}

	return numCached;
}

/**
 *
 * \overload
 */
int FreeTypeGX::loadFont(const uint8_t* fontBuffer, FT_Long bufferSize, FT_UInt pointSize, bool cacheAll) {
	return this->loadFont((uint8_t *)fontBuffer, bufferSize, pointSize, cacheAll);
}

/**
 * Clears all loaded font glyph data.
 *
 * This routine clears all members of the font map structure and frees all allocated memory back to the system.
 */
void FreeTypeGX::unloadFont() {
	for(std::map<wchar_t, ftgxCharData>::iterator i = this->fontData.begin(); i != this->fontData.end(); i++) {
		free(i->second.glyphDataTexture);
	}
	if(this->ftFace) {
		FT_Done_Face(this->ftFace);
	}

	this->cacheTextWidth.clear();
	this->fontData.clear();
}

/**
 * Enables or disables kerning of the output text.
 *
 * This routine enables or disables kerning of the output text only if kerning is supported by the font.
 * Note that by default kerning is enabled if it is supported by the font.
 *
 * @param enabled	The enabled state of the font kerning.
 * @return The resultant enabled state of the font kerning.
 */
bool FreeTypeGX::setKerningEnabled(bool enabled) {
	if(!enabled) {
		return this->ftKerningEnabled = false;
	}

	if(FT_HAS_KERNING(this->ftFace)) {
		return this->ftKerningEnabled = true;
	}

	return false;
}

/**
 * Gets the current enabled state of the font kerning mode.
 *
 * This routine gets the current enabled state of the font kerning mode.
 *
 * @return The enabled state of the font kerning.
 */
bool FreeTypeGX::getKerningEnabled() {
	return this->ftKerningEnabled;
}

/**
 * Enables or disables width value caching of the output text.
 *
 * This routine enables or disables width value caching of the output text in order avoid a repeated possible calculation of
 * textual width within the drawText function. If enabled FreeTypeGX will cache the values of all text strings drawn until such
 * time that clearTextWidthCache is called in order clear the current cache. Font width caching may be enabled or disabled at
 * any time by calling this function. Note that by default this option is disabled to conserve memory.
 *
 * @param enabled	The desired enabled state of the text width caching.
 * @return The resultant enabled state of the text width caching.
 */
bool FreeTypeGX::setTextWidthCachingEnabled(bool enabled) {
	return this->widthCachingEnabled = enabled;
}

/**
 * Gets the current enabled state of the text width caching mode.
 *
 * This routine gets the current enabled state of the text width caching mode.
 *
 * @return The enabled state of the text width caching.
 */
bool FreeTypeGX::getTextWidthCachingEnabled() {
	return this->widthCachingEnabled;
}

/**
 * Clears the current cache of text widths.
 *
 * This clears the cache of text widths regardless of the enabled state of the width caching flag.
 */
void FreeTypeGX::clearTextWidthCache() {
	this->cacheTextWidth.clear();
}

/**
 * Adjusts the texture data buffer to necessary width for a given texture format.
 *
 * This routine determines adjusts the given texture width into the required width to hold the necessary texture data for proper alignment.
 *
 * @param textureWidth	The initial guess for the texture width.
 * @param textureFormat	The texture format to which the data is to be converted.
 * @return The correctly adjusted texture width.
 */
int FreeTypeGX::adjustTextureWidth(int textureWidth, uint8_t textureFormat) {
	int alignment;

	switch(textureFormat) {
		case GX_TF_I4:		/* 8x8 Tiles - 4-bit Intensity */
		case GX_TF_I8:		/* 8x4 Tiles - 8-bit Intensity */
		case GX_TF_IA4:		/* 8x4 Tiles - 4-bit Intensity, , 4-bit Alpha */
			alignment = 8;
			break;

		case GX_TF_IA8:		/* 4x4 Tiles - 8-bit Intensity, 8-bit Alpha */
		case GX_TF_RGB565:	/* 4x4 Tiles - RGB565 Format */
		case GX_TF_RGB5A3:	/* 4x4 Tiles - RGB5A3 Format */
		case GX_TF_RGBA8:	/* 4x4 Tiles - RGBA8 Dual Cache Line Format */
		default:
			alignment = 4;
			break;
	}
	return textureWidth % alignment == 0 ? textureWidth : alignment + textureWidth - (textureWidth % alignment);

}

/**
 * Adjusts the texture data buffer to necessary height for a given texture format.
 *
 * This routine determines adjusts the given texture height into the required height to hold the necessary texture data for proper alignment.
 *
 * @param textureHeight	The initial guess for the texture height.
 * @param textureFormat	The texture format to which the data is to be converted.
 * @return The correctly adjusted texture height.
 */
int FreeTypeGX::adjustTextureHeight(int textureHeight, uint8_t textureFormat) {
	int alignment;

	switch(textureFormat) {
		case GX_TF_I4:		/* 8x8 Tiles - 4-bit Intensity */
			alignment = 8;
			break;

		case GX_TF_I8:		/* 8x4 Tiles - 8-bit Intensity */
		case GX_TF_IA4:		/* 8x4 Tiles - 4-bit Intensity, , 4-bit Alpha */
		case GX_TF_IA8:		/* 4x4 Tiles - 8-bit Intensity, 8-bit Alpha */
		case GX_TF_RGB565:	/* 4x4 Tiles - RGB565 Format */
		case GX_TF_RGB5A3:	/* 4x4 Tiles - RGB5A3 Format */
		case GX_TF_RGBA8:	/* 4x4 Tiles - RGBA8 Dual Cache Line Format */
		default:
			alignment = 4;
			break;
	}
	return textureHeight % alignment == 0 ? textureHeight : alignment + textureHeight - (textureHeight % alignment);

}

/**
 * Caches the given font glyph in the instance font texture buffer.
 *
 * This routine renders and stores the requested glyph's bitmap and relevant information into its own quickly addressable
 * structure within an instance-specific map.
 *
 * @param charCode	The requested glyph's character code.
 * @return A pointer to the allocated font structure.
 */
ftgxCharData *FreeTypeGX::cacheGlyphData(wchar_t charCode) {
	FT_UInt gIndex;
	int textureWidth = 0, textureHeight = 0;

	gIndex = FT_Get_Char_Index( this->ftFace, charCode );
	if (!FT_Load_Glyph(this->ftFace, gIndex, FT_LOAD_DEFAULT | FT_LOAD_FORCE_AUTOHINT | FT_LOAD_RENDER)) {

		if(this->ftFace->glyph->format == FT_GLYPH_FORMAT_BITMAP) {
			FT_Bitmap *glyphBitmap = &(this->ftFace->glyph->bitmap);

			textureWidth = adjustTextureWidth(glyphBitmap->width, this->textureFormat);
			textureHeight = adjustTextureHeight(glyphBitmap->rows, this->textureFormat);

			this->fontData[charCode] = (ftgxCharData){
				this->ftFace->glyph->advance.x >> 6,
				gIndex,
				textureWidth,
				textureHeight,
				this->ftFace->glyph->bitmap_top,
				this->ftFace->glyph->bitmap_top,
				textureHeight - this->ftFace->glyph->bitmap_top,
				NULL
			};
			this->loadGlyphData(glyphBitmap, &this->fontData[charCode]);

			return &this->fontData[charCode];
		}
	}

	return NULL;
}

/**
 * Locates each character in this wrapper's configured font face and process them.
 *
 * This routine locates each character in the configured font face and renders the glyph's bitmap.
 * Each bitmap and relevant information is loaded into its own quickly addressable structure within an instance-specific map.
 */
int FreeTypeGX::cacheGlyphDataComplete() {
	int i = 0;
	FT_UInt gIndex;
	FT_ULong charCode = FT_Get_First_Char( this->ftFace, &gIndex );
	while ( gIndex != 0 ) {

		if(this->cacheGlyphData(charCode) != NULL) {
			i++;
		}

		charCode = FT_Get_Next_Char( this->ftFace, charCode, &gIndex );
	}

	return i;
}

/**
 * Loads the rendered bitmap into the relevant structure's data buffer.
 *
 * This routine does a simple byte-wise copy of the glyph's rendered 8-bit grayscale bitmap into the structure's buffer.
 * Each byte is converted from the bitmap's intensity value into the a uint32_t RGBA value.
 *
 * @param bmp	A pointer to the most recently rendered glyph's bitmap.
 * @param charData	A pointer to an allocated ftgxCharData structure whose data represent that of the last rendered glyph.
 */
void FreeTypeGX::loadGlyphData(FT_Bitmap *bmp, ftgxCharData *charData) {

	unsigned int *glyphData = (unsigned int *)memalign(32, charData->textureWidth * charData->textureHeight * 4);
	memset(glyphData, 0x00, charData->textureWidth * charData->textureHeight * 4);

	uint8_t *src = (uint8_t *)bmp->buffer;
	unsigned int *dest = glyphData, *ptr = dest;

	for (unsigned int imagePosY = 0; imagePosY < bmp->rows; imagePosY++) {
		for (unsigned int imagePosX = 0; imagePosX < bmp->width; imagePosX++) {
			*ptr++ = EXPLODE_UINT8_TO_UINT32(*src);
			src++;
		}
		ptr = dest += charData->textureWidth;
	}

	switch(this->textureFormat) {
		case GX_TF_I4:
			charData->glyphDataTexture = Metaphrasis::convertBufferToI4(glyphData, charData->textureWidth, charData->textureHeight);
			break;
		case GX_TF_I8:
			charData->glyphDataTexture = Metaphrasis::convertBufferToI8(glyphData, charData->textureWidth, charData->textureHeight);
			break;
		case GX_TF_IA4:
			charData->glyphDataTexture = Metaphrasis::convertBufferToIA4(glyphData, charData->textureWidth, charData->textureHeight);
			break;
		case GX_TF_IA8:
			charData->glyphDataTexture = Metaphrasis::convertBufferToIA8(glyphData, charData->textureWidth, charData->textureHeight);
			break;
		case GX_TF_RGB565:
			charData->glyphDataTexture = Metaphrasis::convertBufferToRGB565(glyphData, charData->textureWidth, charData->textureHeight);
			break;
		case GX_TF_RGB5A3:
			charData->glyphDataTexture = Metaphrasis::convertBufferToRGB5A3(glyphData, charData->textureWidth, charData->textureHeight);
			break;
		case GX_TF_RGBA8:
		default:
			charData->glyphDataTexture = Metaphrasis::convertBufferToRGBA8(glyphData, charData->textureWidth, charData->textureHeight);
			break;
	}

	free(glyphData);
}

/**
 * Determines the x offset of the rendered string.
 *
 * This routine calculates the x offset of the rendered string based off of a supplied positional format parameter.
 *
 * @param width	Current pixel width of the string.
 * @param format	Positional format of the string.
 */
int FreeTypeGX::getStyleOffsetWidth(int width, int format) {
	switch(format & FTGX_JUSTIFY_MASK) {
		case FTGX_JUSTIFY_LEFT:
			return 0;
		case FTGX_JUSTIFY_CENTER:
			return width >> 1;
		case FTGX_JUSTIFY_RIGHT:
			return width;
		default:
			return 0;
	}
}

/**
 * Determines the y offset of the rendered string.
 *
 * This routine calculates the y offset of the rendered string based off of a supplied positional format parameter.
 *
 * @param format	Positional format of the string.
 */
int FreeTypeGX::getStyleOffsetHeight(int format) {
	switch(format & FTGX_ALIGN_MASK) {
		case FTGX_ALIGN_TOP:
			return -this->ftAscender;
		case FTGX_ALIGN_MIDDLE:
			return -(this->ftDescender + this->ftAscender) >> 1;
		case FTGX_ALIGN_BOTTOM:
			return this->ftDescender;
		default:
			return 0;
	}
}

/**
 * Returns the FreeTypeGX font character data structure.
 *
 * This routine locates the currently cached FreeTypeGX ftgxCharData structure for the supplied wide character. If the
 * structure has not been loaded and cached the routine initialized the loading and caching of the structure for that
 * data chatracter.
 *
 * @param character	Character whose information needs to be retrieved.
 * @return The font structure for the supplied character.
 */
ftgxCharData* FreeTypeGX::getCharacter(wchar_t character) {
	if( this->fontData.find(character) != this->fontData.end() ) {
		return &this->fontData[character];
	}

	return this->cacheGlyphData(character);
}

/**
 * Processes the supplied text string and prints the results at the specified coordinates.
 *
 * This routine processes each character of the supplied text string, loads the relevant preprocessed bitmap buffer,
 * a texture from said buffer, and loads the resultant texture into the EFB.
 *
 * @param x	Screen X coordinate at which to output the text.
 * @param y Screen Y coordinate at which to output the text. Note that this value corresponds to the text string origin and not the top or bottom of the glyphs.
 * @param text	NULL terminated string to output.
 * @param color	Optional color to apply to the text characters. If not specified default value is ftgxWhite: 0xffffffff
 * @param textStyle	Flags which specify any styling which should be applied to the rendered string.
 * @return The number of characters printed.
 */
int FreeTypeGX::drawText(float x, float y, wchar_t *text, float scaleX, float scaleY, float offsetX, float offsetY, float degrees, unsigned int color, int textStyle) {
	float x_pos = x, printed = 0;
	float x_offset = 0, y_offset = 0;
	GXTexObj glyphTexture;
	FT_Vector pairDelta;

	int textWidth = 0;

	if(this->widthCachingEnabled) {
		textWidth = this->cacheTextWidth.find(text) != this->cacheTextWidth.end() ? this->cacheTextWidth[text] : this->cacheTextWidth[text] = this->getWidth(text);
	}

	if(textStyle & FTGX_JUSTIFY_MASK) {
		x_offset = static_cast<float>(this->getStyleOffsetWidth(textWidth > 0 ? textWidth : this->getWidth(text), textStyle));
	}

	if(textStyle & FTGX_ALIGN_MASK) {
		y_offset = static_cast<float>(this->getStyleOffsetHeight(textStyle));
	}

	int i = 0;
	while(text[i]) {
		if(maxVideoWidth > 0 && (x_pos > maxVideoWidth)) {
			break;
		}

		ftgxCharData* glyphData = getCharacter(text[i]);

		if(glyphData != NULL) {
			if(this->ftKerningEnabled && i) {
				FT_Get_Kerning(this->ftFace, this->fontData[text[i - 1]].glyphIndex, glyphData->glyphIndex, FT_KERNING_DEFAULT, &pairDelta);
				x_pos += static_cast<float>(pairDelta.x >> 6) * scaleX;
			}

			GX_InitTexObj(&glyphTexture, glyphData->glyphDataTexture, glyphData->textureWidth, glyphData->textureHeight, this->textureFormat, GX_CLAMP, GX_CLAMP, GX_FALSE);
			this->copyTextureToFramebuffer(&glyphTexture, glyphData->textureWidth, glyphData->textureHeight, x_pos - x_offset, y - glyphData->renderOffsetY - y_offset, scaleX, scaleY, offsetX, offsetY, degrees, color);

			x_pos += static_cast<float>(glyphData->glyphAdvanceX) * scaleX;
			printed++;
		}

		i++;
	}

	if(textStyle & FTGX_STYLE_MASK) {
		this->drawTextFeature(x - x_offset, y - y_offset, textWidth > 0 ? textWidth : textWidth = this->getWidth(text), textStyle, scaleX, scaleY, offsetX, offsetY, degrees, color);
	}

	return printed;
}

/**
 * \overload
 */
int FreeTypeGX::drawText(float x, float y, wchar_t const *text, float scaleX, float scaleY, float offsetX, float offsetY, float degrees, unsigned int color, int textStyle) {
	return this->drawText(x, y, (wchar_t *)text, scaleX, scaleY, offsetX, offsetY, degrees, color, textStyle);
}

/**
 * Internal routine to draw the features for stylized text.
 *
 * This routine creates a simple feature for stylized text.
 *
 * @param x	Screen X coordinate of the text baseline.
 * @param y	Screen Y coordinate of the text baseline.
 * @param width	Pixel width of the text string.
 * @param textStyle	Flags which specify any styling which should be applied to the rendered string.
 * @param color	Color to be applied to the text feature.
 */
void FreeTypeGX::drawTextFeature(float x, float y, int width, int textStyle, float scaleX, float scaleY, float offsetX, float offsetY, float degrees, unsigned int color) {
	int featureHeight = this->ftPointSize >> 4 > 0 ? this->ftPointSize >> 4 : 1;

	if (textStyle & FTGX_STYLE_UNDERLINE ) {
		this->copyFeatureToFramebuffer(width, featureHeight, x, y + 1, scaleX, scaleY, offsetX, offsetY, degrees, color);
	}

	if (textStyle & FTGX_STYLE_STRIKE ) {
		this->copyFeatureToFramebuffer(width, featureHeight, x, y - static_cast<float>(this->ftAscender >> 2), scaleX, scaleY, offsetX, offsetY, degrees, color);
	}
}

/**
 * Processes the supplied string and return the width of the string in pixels.
 *
 * This routine processes each character of the supplied text string and calculates the width of the entire string.
 * Note that if precaching of the entire font set is not enabled any uncached glyph will be cached after the call to this function.
 *
 * @param text	NULL terminated string to calculate.
 * @return The width of the text string in pixels.
 */
int FreeTypeGX::getWidth(wchar_t *text) {
	int strWidth = 0;
	FT_Vector pairDelta;
	ftgxCharData* glyphData = NULL;

	int i = 0;
	while(text[i]) {

		glyphData = getCharacter(text[i]);

		if(glyphData != NULL) {
			if(this->ftKerningEnabled && (i > 0)) {
				FT_Get_Kerning( this->ftFace, this->fontData[text[i - 1]].glyphIndex, glyphData->glyphIndex, FT_KERNING_DEFAULT, &pairDelta );
				strWidth += pairDelta.x >> 6;
			}

			strWidth += glyphData->glyphAdvanceX;
		}

		i++;
	}

	return strWidth;
}

/**
 *
 * \overload
 */
int FreeTypeGX::getWidth(wchar_t const *text) {
	return this->getWidth((wchar_t *)text);
}

/**
 * Processes the supplied string and return the height of the string in pixels.
 *
 * This routine processes each character of the supplied text string and calculates the height of the entire string.
 * Note that if precaching of the entire font set is not enabled any uncached glyph will be cached after the call to this function.
 *
 * @param text	NULL terminated string to calculate.
 * @return The height of the text string in pixels.
 */
int FreeTypeGX::getHeight(wchar_t *text) {
	int strMax = 0, strMin = 0;

	int i = 0;
	while(text[i]) {

		ftgxCharData* glyphData = getCharacter(text[i]);

		if(glyphData != NULL) {
			strMax = glyphData->renderOffsetMax > strMax ? glyphData->renderOffsetMax : strMax;
			strMin = glyphData->renderOffsetMin > strMin ? glyphData->renderOffsetMin : strMin;
		}

		i++;
	}

	return strMax + strMin;
}

/**
 *
 * \overload
 */
int FreeTypeGX::getHeight(wchar_t const *text) {
	return this->getHeight((wchar_t *)text);
}

/**
 * Copies the supplied texture quad to the EFB.
 *
 * This routine uses the in-built GX quad builder functions to define the texture bounds and location on the EFB target.
 *
 * @param texObj	A pointer to the glyph's initialized texture object.
 * @param texWidth	The pixel width of the texture object.
 * @param texHeight	The pixel height of the texture object.
 * @param screenX	The screen X coordinate at which to output the rendered texture.
 * @param screenY	The screen Y coordinate at which to output the rendered texture.
 * @param color	Color to apply to the texture.
 */
void FreeTypeGX::copyTextureToFramebuffer(GXTexObj *texObj, float texWidth, float texHeight, float screenX, float screenY, float scaleX, float scaleY, float offsetX, float offsetY, float degrees, unsigned int color) {
	// Backup matrix
	GRRLIB_matrix matrixObject = GRRLIB_GetMatrix();

	GRRLIB_Translate(screenX, screenY);

	GRRLIB_Scale(scaleX, scaleY);
	GRRLIB_Translate(-offsetX, -offsetY);
	GRRLIB_Rotate(degrees);

	GX_LoadTexObj(texObj, GX_TEXMAP0);
	GX_SetTevOp(GX_TEVSTAGE0, GX_MODULATE);
	GX_SetVtxDesc(GX_VA_TEX0, GX_DIRECT);

	// Follow GRRLIB anti-aliasing settings
	if (GRRLIB_GetAntiAliasing() == false) {
        GX_InitTexObjLOD(texObj, GX_NEAR, GX_NEAR,
                         0.0, 0.0, 0.0, 0, 0, GX_ANISO_1);
        GX_SetCopyFilter(GX_FALSE, rmode->sample_pattern, GX_FALSE, rmode->vfilter);
    }
    else {
        GX_SetCopyFilter(rmode->aa, rmode->sample_pattern, GX_TRUE, rmode->vfilter);
    }

	GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
		GX_Position3f32(0.0, 0.0, 0.0);
		GX_Color1u32(color);
		GX_TexCoord2f32(0.0, 0.0);

		GX_Position3f32(texWidth, 0.0, 0.0);
		GX_Color1u32(color);
		GX_TexCoord2f32(1.0, 0.0);

		GX_Position3f32(texWidth, texHeight, 0.0);
		GX_Color1u32(color);
		GX_TexCoord2f32(1, 1);

		GX_Position3f32(0.0, texHeight, 0.0);
		GX_Color1u32(color);
		GX_TexCoord2f32(0, 1);
	GX_End();

	GX_SetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GX_SetVtxDesc(GX_VA_TEX0, GX_NONE);

	// Restore matrix
	GRRLIB_SetMatrix(&matrixObject);
}

/**
 * Creates a feature quad to the EFB.
 *
 * This function creates a simple quad for displaying stylized text.
 *
 * @param featureWidth	The pixel width of the quad.
 * @param featureHeight	The pixel height of the quad.
 * @param screenX	The screen X coordinate at which to output the quad.
 * @param screenY	The screen Y coordinate at which to output the quad.
 * @param color	Color to apply to the texture.
 */
void FreeTypeGX::copyFeatureToFramebuffer(float featureWidth, float featureHeight, float screenX, float screenY, float scaleX, float scaleY, float offsetX, float offsetY, float degrees, unsigned int color) {
	// Backup matrix
	GRRLIB_matrix matrixObject = GRRLIB_GetMatrix();

	GRRLIB_Translate(screenX, screenY);
	GRRLIB_Transform(scaleX, scaleY, degrees, offsetX, offsetY);

	GX_SetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
	GX_SetVtxDesc(GX_VA_TEX0, GX_NONE);

	GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
		GX_Position3f32(0.0, 0.0, 0.0);
		GX_Color1u32(color);

 		GX_Position3f32(featureWidth, 0.0, 0.0);
		GX_Color1u32(color);

		GX_Position3f32(featureWidth, featureHeight, 0.0);
		GX_Color1u32(color);

		GX_Position3f32(0.0, featureHeight, 0.0);
		GX_Color1u32(color);
	GX_End();

	GX_SetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GX_SetVtxDesc(GX_VA_TEX0, GX_NONE);

	// Revert matrix
	GRRLIB_SetMatrix(&matrixObject);
}
