# ExportFont

- 依赖库

  ```tex
  libSDL2-dev libSDL2-image-dev libSDL2-ttf-dev
  ```
- 功能

  读取charmap.txt并从ttf中导出charmap包含的字的字体，生成png索引图
- 参数

  ```plaintext
  ExportFont <ttf path> <option>
  	-s <pixel size> <single chacter> <output_png>:
  		Export a single glyph to a PNG file
  	-as <pixel size> <output_dir>:
  		Export all glyphs to a directory
  	-am <pixel size> <output_png>:
  		Export and merge all glyphs into a single PNG file
  	-cs <pixel_size> <charmap_path> <output_dir>:
  		Export single glyphs to a directory from a charmap file
  	-cm <pixel_size> <charmap_path> <output_png>:
  		Export and merge glyphs into a single PNG file from a charmap file
  	-h, -help, ?:
  		Show help message
  ```
