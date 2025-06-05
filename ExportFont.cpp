#include "ExportFont.hpp"

int main(int argc, char *argv[])
{
    if (TTF_Init() == -1)
    {
        std::cerr << "Failed to initialize SDL_ttf: " << TTF_GetError() << std::endl;
        return false;
    }
    if (!parseArgs(argc, argv))
    {
        TTF_Quit();
        return EXIT_FAILURE;
    }
    TTF_Quit();
    return EXIT_SUCCESS;
}

bool parseArgs(int argc, char *argv[])
{
    if (argc < 4)
    {
        printHelp();
        return false;
    }
    try
    {
        const char *font_path = argv[1];
        for (int i = 2; i < argc;)
        {
            std::string option = argv[i++];
            int32_t pixel_size = std::stoi(argv[i++]);
            TTF_Font *font = TTF_OpenFont(font_path, pixel_size);
            if (!font)
            {
                std::cerr << "Failed to open font: " << TTF_GetError() << std::endl;
                return false;
            }
            if (pixel_size <= 0 || pixel_size >= border_long)
            {
                std::cerr << "Invalid pixel size: " << pixel_size << std::endl;
                TTF_CloseFont(font);
                return false;
            }
            if (option == "-s")
            {
                if (i + 1 >= argc)
                {
                    std::cerr << "Missing arguments for -s option" << std::endl;
                    std::cout << "\t-s <pixel size> <single chacter> <output_dir>:\n\t\tExport a single glyph to a PNG file" << std::endl;
                    TTF_CloseFont(font);
                    return false;
                }
                std::string characters = argv[i++];
                UnicodeChar unicode_char = utf8_to_unicode(characters);
                const char *output_dir = argv[i++];
                if (!DrawSingleGlyph(font, unicode_char, output_dir))
                {
                    std::cerr << "Failed to draw single glyph" << std::endl;
                    TTF_CloseFont(font);
                    return false;
                }
            }
            else if (option == "-as")
            {
                if (i >= argc)
                {
                    std::cerr << "Missing arguments for -a option" << std::endl;
                    std::cout << "\t-as <pixel size> <output_dir>:\n\t\tExport all glyphs to a directory" << std::endl;
                    TTF_CloseFont(font);
                    return false;
                }
                const char *output_dir = argv[i++];
                std::vector<UnicodeChar> unicode_chars;
                if (!GetUnicodeCharsFromTTF(font, unicode_chars))
                {
                    std::cerr << "Failed to get unicode characters from TTF" << std::endl;
                    TTF_CloseFont(font);
                    return false;
                }
                if (!DrawSingleGlyph(font, unicode_chars, output_dir))
                {
                    std::cerr << "Failed to export all glyphs" << std::endl;
                    TTF_CloseFont(font);
                    return false;
                }
            }
            else if (option == "-am")
            {
                if (i >= argc)
                {
                    std::cerr << "Missing arguments for -am option" << std::endl;
                    std::cout << "\t-am <pixel size> <output_png>:\n\t\tExport and merge all glyphs into a single PNG file" << std::endl;
                    TTF_CloseFont(font);
                    return false;
                }
                const char *output_png = argv[i++];
                std::vector<UnicodeChar> unicode_chars;
                if (!GetUnicodeCharsFromTTF(font, unicode_chars))
                {
                    std::cerr << "Failed to get unicode characters from TTF" << std::endl;
                    TTF_CloseFont(font);
                    return false;
                }
                if (!DrawAndMergeGlyphs(font, unicode_chars, output_png))
                {
                    std::cerr << "Failed to draw and merge glyphs" << std::endl;
                    TTF_CloseFont(font);
                    return false;
                }
            }
            else if (option == "-cs")
            {
                if (i + 1 >= argc)
                {
                    std::cerr << "Missing arguments for -cs option" << std::endl;
                    std::cout << "\t-cs <pixel_size> <charmap_path> <output_dir>:\n\t\tExport single glyphs to a directory from a charmap file" << std::endl;
                    TTF_CloseFont(font);
                    return false;
                }
                const char *charmap_path = argv[i++];
                const char *output_dir = argv[i++];
                std::vector<UnicodeChar> unicode_chars;
                if (!GetUnicodeCharsFromCharmap(charmap_path, unicode_chars))
                {
                    std::cerr << "Failed to get unicode characters from file" << std::endl;
                    TTF_CloseFont(font);
                    return false;
                }
                if (!DrawSingleGlyph(font, unicode_chars, output_dir))
                {
                    std::cerr << "Failed to draw single glyphs" << std::endl;
                    TTF_CloseFont(font);
                    return false;
                }
            }
            else if (option == "-cm")
            {
                if (i + 1 >= argc)
                {
                    std::cerr << "Missing arguments for -cm option" << std::endl;
                    std::cout << "\t-cm <pixel_size> <charmap_path> <output_png>:\n\t\tExport and merge glyphs into a single PNG file from a charmap file" << std::endl;
                    TTF_CloseFont(font);
                    return false;
                }
                const char *charmap_path = argv[i++];
                const char *output_png = argv[i++];
                std::vector<UnicodeChar> unicode_chars;
                if (!GetUnicodeCharsFromCharmap(charmap_path, unicode_chars))
                {
                    std::cerr << "Failed to get unicode characters from file" << std::endl;
                    TTF_CloseFont(font);
                    return false;
                }
                if (!DrawAndMergeGlyphs(font, unicode_chars, output_png))
                {
                    std::cerr << "Failed to draw and merge glyphs" << std::endl;
                    TTF_CloseFont(font);
                    return false;
                }
            }
            else if (option == "-h" || option == "--help" || option == "?")
            {
                printHelp();
                TTF_CloseFont(font);
                return true;
            }
            else
            {
                std::cerr << "Unknown option: " << option << std::endl;
                TTF_CloseFont(font);
                return false;
            }
            TTF_CloseFont(font);
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error parsing arguments: " << e.what() << std::endl;
        return false;
    }
    return true;
}

void printHelp()
{
    std::cout << "Usage: ExportFont <font_path> <option>" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "\t-s <pixel size> <single chacter> <output_png>:\n\t\tExport a single glyph to a PNG file" << std::endl;
    std::cout << "\t-as <pixel size> <output_dir>:\n\t\tExport all glyphs to a directory" << std::endl;
    std::cout << "\t-am <pixel size> <output_png>:\n\t\tExport and merge all glyphs into a single PNG file" << std::endl;
    std::cout << "\t-cs <pixel_size> <charmap_path> <output_dir>:\n\t\tExport single glyphs to a directory from a charmap file" << std::endl;
    std::cout << "\t-cm <pixel_size> <charmap_path> <output_png>:\n\t\tExport and merge glyphs into a single PNG file from a charmap file" << std::endl;
    std::cout << "\t-h, -help, ?:\n\t\tShow this help message" << std::endl;
}

UnicodeChar utf8_to_unicode(const std::string &utf8_char)
{
    const uint8_t *bytes = reinterpret_cast<const uint8_t *>(utf8_char.c_str());
    size_t length = utf8_char.size();

    if (bytes[0] < 0x80) // 单字节字符 (ASCII)
    {
        return {static_cast<uint32_t>(bytes[0]), utf8_char};
    }
    else if (length >= 2 && (bytes[0] & 0xE0) == 0xC0) // 两字节字符
    {
        return {static_cast<uint32_t>((bytes[0] & 0x1F) << 6) | (bytes[1] & 0x3F), utf8_char};
    }
    else if (length >= 3 && (bytes[0] & 0xF0) == 0xE0) // 三字节字符
    {
        return {static_cast<uint32_t>((bytes[0] & 0x0F) << 12) | ((bytes[1] & 0x3F) << 6) | (bytes[2] & 0x3F), utf8_char};
    }
    else
    {
        std::cerr << "Invalid UTF-8 sequence or unsupported character length" << std::endl;
        return {0, ""}; // 返回一个无效的Unicode字符
    }
}

UnicodeChar unicode_to_utf8(uint32_t unicode)
{
    UnicodeChar unicode_char;
    unicode_char.unicode = unicode;
    if (unicode < 0x80)
    {
        unicode_char.utf8 += static_cast<char>(unicode);
    }
    else if (unicode < 0x800)
    {
        unicode_char.utf8 += static_cast<char>(0xC0 | (unicode >> 6));
        unicode_char.utf8 += static_cast<char>(0x80 | (unicode & 0x3F));
    }
    else if (unicode < 0x10000)
    {
        unicode_char.utf8 += static_cast<char>(0xE0 | (unicode >> 12));
        unicode_char.utf8 += static_cast<char>(0x80 | ((unicode >> 6) & 0x3F));
        unicode_char.utf8 += static_cast<char>(0x80 | (unicode & 0x3F));
    }
    return unicode_char;
}

bool GetUnicodeCharsFromCharmap(const char *charmap_path, std::vector<UnicodeChar> &unicode_chars)
{
    std::ifstream file(charmap_path);
    if (!file.is_open())
    {
        std::cerr << "Failed to open input file: " << charmap_path << std::endl;
        return false;
    }
    std::string line;
    while (std::getline(file, line))
    {
        if (!line.empty())
        {
            unicode_chars.push_back(utf8_to_unicode(line));
        }
    }
    file.close();
    return !unicode_chars.empty();
}

bool GetUnicodeCharsFromTTF(TTF_Font *font, std::vector<UnicodeChar> &unicode_chars)
{
    for (uint32_t unicode = 0; unicode < 0x10000; ++unicode)
    {
        if (!TTF_GlyphIsProvided(font, unicode))
        {
            continue; // 跳过字体中未提供的字符
        }
        unicode_chars.push_back(unicode_to_utf8(unicode));
    }
    return !unicode_chars.empty();
}

bool DrawSingleGlyph(TTF_Font *font, const std::vector<UnicodeChar> &unicodechars, const char *output_dir)
{
    if (!std::filesystem::exists(output_dir) && !std::filesystem::create_directories(output_dir))
    {
        std::cerr << "Failed to create output directory: " << output_dir << std::endl;
        return false;
    }
    SDL_Surface *output_surface = SDL_CreateRGBSurfaceWithFormat(0, border_long, border_long, 24, SDL_PIXELFORMAT_RGB888);
    if (!output_surface)
    {
        std::cerr << "Failed to create output surface: " << SDL_GetError() << std::endl;
        return false;
    }
    SDL_FillRect(output_surface, nullptr, SDL_MapRGB(output_surface->format, background_color.r, background_color.g, background_color.b));
    if (!output_surface)
    {
        return false;
    }
    for (const auto &unicode_char : unicodechars)
    {
        if (!DrawGlyph(font, unicode_char.unicode, output_surface, 0, 0))
        {
            SDL_FreeSurface(output_surface);
            return false;
        }
        std::stringstream ss;
        ss << output_dir << "/U+" << std::hex << unicode_char.unicode << ".png";
        const char *output_png = ss.str().c_str();
        save_surface_to_png(output_surface, output_png);
        SDL_FillRect(output_surface, nullptr, SDL_MapRGB(output_surface->format, background_color.r, background_color.g, background_color.b));
    }
    SDL_FreeSurface(output_surface);
    return true;
}

bool DrawSingleGlyph(TTF_Font *font, const UnicodeChar &unicodechar, const char *output_png)
{
    SDL_Surface *output_surface = SDL_CreateRGBSurfaceWithFormat(0, border_long, border_long, 24, SDL_PIXELFORMAT_RGB888);
    if (!output_surface)
    {
        std::cerr << "Failed to create output surface: " << SDL_GetError() << std::endl;
        return false;
    }
    SDL_FillRect(output_surface, nullptr, SDL_MapRGB(output_surface->format, background_color.r, background_color.g, background_color.b));
    if (!output_surface)
    {
        return false;
    }
    if (!DrawGlyph(font, unicodechar.unicode, output_surface, 0, 0))
    {
        SDL_FreeSurface(output_surface);
        return false;
    }
    save_surface_to_png(output_surface, output_png);
    SDL_FreeSurface(output_surface);
    return true;
}

bool DrawAndMergeGlyphs(TTF_Font *font, const std::vector<UnicodeChar> &unicodechars, const char *output_png)
{
    int unisize = unicodechars.size();
    int modichars = unisize % section_characters;
    int lines = (unisize / section_characters * border_long) + (modichars / line_Characters) + (modichars % line_Characters > 0);
    SDL_Surface *output_surface = SDL_CreateRGBSurfaceWithFormat(0, line_width, lines * border_long, 24, SDL_PIXELFORMAT_RGB888);
    if (!output_surface)
    {
        std::cerr << "Failed to create output surface: " << SDL_GetError() << std::endl;
        return false;
    }
    SDL_FillRect(output_surface, nullptr, SDL_MapRGB(output_surface->format, background_color.r, background_color.g, background_color.b));
    uint32_t index = 0;
    for (const auto &unicode_char : unicodechars)
    {
        int32_t x = (index % line_Characters) * border_long;
        int32_t y = (index / line_Characters) * border_long;
        if (!DrawGlyph(font, unicode_char.unicode, output_surface, x, y))
        {
            std::cerr << "Failed on drawing glyph: " << unicode_char.unicode << std::endl;
            continue;
        }
        if ((++index) % line_width == section_characters)
        {
            index += 9;
        }
    }
    save_surface_to_png(output_surface, output_png);
    SDL_FreeSurface(output_surface);
    return true;
}

bool DrawGlyph(TTF_Font *font, const uint32_t &unicode_char, SDL_Surface *&output_surface, int32_t x, int32_t y)
{
    SDL_Surface *glyph_surface = TTF_RenderGlyph_Blended(font, unicode_char, glyph_color);
    if (!glyph_surface)
    {
        std::cerr << "Failed to render glyph: " << TTF_GetError() << std::endl;
        return false;
    }

    SDL_Surface *shadow_surface = TTF_RenderGlyph_Blended(font, unicode_char, shadow_color);
    if (!shadow_surface)
    {
        std::cerr << "Failed to render shadow: " << TTF_GetError() << std::endl;
        SDL_FreeSurface(glyph_surface);
        return false;
    }

    // 绘制阴影到输出表面（偏移位置）
    for (int i = 1; i < 4; i++)
    {
        SDL_Rect shadow_rect = {x + (i & 1), y + (i >> 1), shadow_surface->w, shadow_surface->h};
        if (SDL_BlitSurface(shadow_surface, nullptr, output_surface, &shadow_rect) != 0)
        {
            std::cerr << "Failed to blit shadow onto output surface: " << SDL_GetError() << std::endl;
            SDL_FreeSurface(shadow_surface);
            continue;
        }
    }
    // 绘制字形到输出表面
    SDL_Rect glyph_rect = {x, y, glyph_surface->w, glyph_surface->h};
    if (SDL_BlitSurface(glyph_surface, nullptr, output_surface, &glyph_rect) != 0)
    {
        std::cerr << "Failed to blit glyph onto output surface: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(shadow_surface);
        SDL_FreeSurface(glyph_surface);
        return false;
    }

    SDL_FreeSurface(shadow_surface);
    SDL_FreeSurface(glyph_surface);
    return true;
}

void save_surface_to_png(SDL_Surface *surface, const char *&filename)
{
    SDL_Surface *indexed_surface = SDL_CreateRGBSurfaceWithFormat(0, surface->w, surface->h, 8, SDL_PIXELFORMAT_INDEX8);
    if (!indexed_surface)
    {
        std::cerr << "Failed to create indexed surface: " << SDL_GetError() << std::endl;
        return;
    }

    if (SDL_SetPaletteColors(indexed_surface->format->palette, palette, 0, sizeof(palette) / sizeof(SDL_Color)) != 0)
    {
        std::cerr << "Failed to set palette colors: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(indexed_surface);
        return;
    }

    // 使用指针直接访问像素数据以提高性能
    uint32_t *src_pixels = static_cast<uint32_t *>(surface->pixels);
    uint8_t *dst_pixels = static_cast<uint8_t *>(indexed_surface->pixels);
    int src_pitch = surface->pitch / 4;     // 每行像素数 (32位像素)
    int dst_pitch = indexed_surface->pitch; // 每行像素数 (8位像素)

    for (int y = 0; y < surface->h; ++y)
    {
        uint32_t *src_row = src_pixels + y * src_pitch;
        uint8_t *dst_row = dst_pixels + y * dst_pitch;

        for (int x = 0; x < surface->w; ++x)
        {
            uint32_t pixel = src_row[x];
            uint8_t r, g, b;
            SDL_GetRGB(pixel, surface->format, &r, &g, &b);

            if (r == transparent_color.r && g == transparent_color.g && b == transparent_color.b)
                dst_row[x] = 0; // 透明色
            else if (r == glyph_color.r && g == glyph_color.g && b == glyph_color.b)
                dst_row[x] = 1; // 字形色
            else if (r == shadow_color.r && g == shadow_color.g && b == shadow_color.b)
                dst_row[x] = 2; // 阴影色
            else
                dst_row[x] = 3; // 背景色
        }
    }

    if (IMG_SavePNG(indexed_surface, filename) != 0)
    {
        std::cerr << "Failed to save PNG: " << IMG_GetError() << std::endl;
    }

    SDL_FreeSurface(indexed_surface);
}
