#ifndef EXPORTFONT_HPP
#define EXPORTFONT_HPP
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdint.h>
#include <filesystem>

const int32_t line_Characters = 16;
const int32_t border_long = 16;
const int32_t section_characters = 247;
const int32_t line_width = line_Characters * border_long;
const SDL_Color glyph_color = {56, 56, 56, 255}, shadow_color = {127, 127, 127, 255};
const SDL_Color background_color = {255, 255, 255, 255}, transparent_color = {0, 0, 0, 255};
const SDL_Color palette[4] =
{
    transparent_color,
    glyph_color,
    shadow_color,
    background_color
};

struct UnicodeChar
{
    uint32_t unicode;
    std::string utf8;
};

bool parseArgs(int argc, char *argv[]);
void printHelp();
UnicodeChar utf8_to_unicode(const std::string &utf8_char);
UnicodeChar unicode_to_utf8(uint32_t unicode);
bool GetUnicodeCharsFromCharmap(const char *charmap_path, std::vector<UnicodeChar> &unicode_chars);
bool GetUnicodeCharsFromTTF(TTF_Font *font, std::vector<UnicodeChar> &unicode_chars);
bool DrawSingleGlyph(TTF_Font *font, const std::vector<UnicodeChar> &unicodechars, const char *output_dir);
bool DrawSingleGlyph(TTF_Font *font, const UnicodeChar &unicodechar, const char *output_png);
bool DrawAndMergeGlyphs(TTF_Font *font, const std::vector<UnicodeChar> &unicodechars, const char *output_png);

bool DrawGlyph(TTF_Font *font, const uint32_t &unicode_char, SDL_Surface *&output_surface, int32_t x, int32_t y);
void save_surface_to_png(SDL_Surface *surface, const char *&filename);

#endif // EXPORTFONT_HPP
