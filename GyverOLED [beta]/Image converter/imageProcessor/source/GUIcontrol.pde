// ================ EDITOR ==================

void load_image() {
  selectInput("Select a file to process:", "fileSelected");
  changeFlag = true;
}

void fileSelected(File selection) {
  if (selection == null) {
    println("Not selected");
  } else {
    imagePath = selection.getAbsolutePath();
    println("Select: " + imagePath);
    image = loadImage(imagePath);
    imageWidth = 256;
    image.resize(imageWidth, 0);    
    sizeSlider.setValue(image.width);
    sliderXY.setValue(0, 0);
    sliderBC.setValue(0, 1);
  }
  changeFlag = true;
}

void save_image() {
  PImage image = loadImage(imagePath);  
  image = filtered(image);
  image.save("outputImage.bmp");
}

void img_width(int size) {
  imageWidth = size;
  changeFlag = true;
}

void img_rotate(int val) {
  rotAngle = radians(val);
}

void image_pos() {
  changeFlag = true;
}

void br_contr() {
  changeFlag = true;
}

void grayscale(boolean state) {
  grayscaleState = !state;
  changeFlag = true;
}

void invert(boolean state) {
  invertState = !state;
  changeFlag = true;
}

void dither(boolean state) {
  ditherState = !state;
  changeFlag = true;
}

void frame(boolean state) {
  frameState = !state;
  changeFlag = true;
}

void threshold(boolean state) {
  thresholdState = !state;
  changeFlag = true;
}

void t_value(float val) {
  thresholdValue = val;
  changeFlag = true;
}

void posterize(boolean state) {
  posterizeState = !state;
  changeFlag = true;
}

void p_value(int val) {
  posterizeValue = val;
  changeFlag = true;
}

void brush(boolean state) {
  changeFlag = true;  
  brushState = !state;
  brushLayer = createGraphics(width, height);
}

void brush_size(int val) {
  brushSize = val;
  changeFlag = true;
}

void result_width(int val) {
  resultWidth = val;
}
void result_height(int val) {
  resultHeight = val;
}

void bar(int val) {
  saveMode = val;
}

void progmem(boolean val) {
  progmem = !val;
}

void generateBitmap() {
  saveLines = "";
  switch (saveMode) {
  case 0:  
    // линейный битмап
    saveLines = "const uint8_t bitmap_"+resultWidth+"x"+resultHeight+"[]" + (progmem ? " PROGMEM" : "") + " = {\n";
    for (byte h = 0; h < resultHeight; h++) {
      saveLines += "\t";
      for (int i = 0; i < resultWidth; i+=8) {      
        byte thisByte = 0;
        for (byte b = 0; b < 8; b++) thisByte |= getPix(b+i, h) << (7-b);          
        saveLines += "0x" + hex(thisByte, 2);
        saveLines += ", ";
      }
      saveLines += "\n";
    }
    saveLines += "};";
    break;
  case 1:
    // ==== rgb16 ====
    saveLines = "const uint16_t bitmap_"+resultWidth+"x"+resultHeight+"[]" + (progmem ? " PROGMEM" : "") + " = {\n";
    for (int x = 0; x < resultWidth; x++) {
      for (int y = 0; y < resultHeight; y++) {
        saveLines += "0x" + hex(getPixRGB32(x, y), 4) + ", ";
      }
      saveLines += "\n";
    }
    saveLines += "};";
    break;
  case 2:
    // ==== rgb32 ====
    saveLines = "const uint32_t bitmap_"+resultWidth+"x"+resultHeight+"[]" + (progmem ? " PROGMEM" : "") + " = {\n";
    for (int x = 0; x < resultWidth; x++) {
      for (int y = 0; y < resultHeight; y++) {
        saveLines += "0x" + hex(getPixRGB32(x, y), 6) + ", ";
      }
      saveLines += "\n";
    }
    saveLines += "};";
    break;
  case 3:
    // ==== битмап для оледов ====
    int numRows = ceil(resultHeight / 8);
    saveLines = "const uint8_t bitmap_"+resultWidth+"x"+resultHeight+"[]" + (progmem ? " PROGMEM" : "") + " = {\n";

    for (byte r = 0; r < numRows; r++) {
      saveLines += "\t";
      for (int j = 0; j < resultWidth; j++) {     
        byte thisByte = 0;
        for (byte b = 0; b < 8; b++) thisByte |= getPix(j, r*8+7-b) << (7-b);          
        saveLines += "0x" + hex(thisByte, 2);
        saveLines += ", ";
      }
      saveLines += "\n";
    }
    saveLines += "};";
    break;
  }
}

void save_bitmap() {
  generateBitmap();
  String[] lines = new String[1];
  lines[0] = saveLines;
  saveStrings("bitmap.h", lines);
}

void copy_clipboard() {
  generateBitmap();
  StringSelection selection = new StringSelection(saveLines);
  Clipboard clipboard = Toolkit.getDefaultToolkit().getSystemClipboard();
  clipboard.setContents(selection, selection);
}

int getPix(int x, int y) {
  if (x >= resultWidth || y >= resultHeight) return 0;
  color c = get(int(rectX + x * rectSize - rectSize*resultWidth/2 + rectSize/2), int(rectY + y * rectSize - rectSize*resultHeight/2 + rectSize/2));
  if (brightness(c) < 127) return 1;
  else return 0;
}

int getPixRGB32(int x, int y) {
  if (x >= resultWidth || y >= resultHeight) return 0;
  return (get(int(rectX + x * rectSize - rectSize*resultWidth/2 + rectSize/2), int(rectY + y * rectSize - rectSize*resultHeight/2 + rectSize/2)));
}

int getPixRGB16(int x, int y) {
  if (x >= resultWidth || y >= resultHeight) return 0;
  int col = get(int(rectX + x * rectSize - rectSize*resultWidth/2 + rectSize/2), int(rectY + y * rectSize - rectSize*resultHeight/2 + rectSize/2));
  int red = (col & 0x00FF0000) >> 16;
  int green = (col & 0x0000FF00) >> 8;
  int blue =  col & 0x000000FF;
  return (red >> 3 << 11) + (green >> 2 << 5) + (blue >> 3);
}

// ================ BOTTOM ==================

void help_ru() {
  helpState = !helpState;
  langulage = false;
  if (!helpState) changeFlag = true;
}
void help_en() {
  helpState = !helpState;
  langulage = true;
  if (!helpState) changeFlag = true;
}

void showHelp() {
  background(255);
  PFont myFont;
  myFont = createFont("Ubuntu", 17);
  textFont(myFont);
  fill(0);
  if (!langulage) {
    text("Открыть / Сохранить отредактированное изображение", 240, 30);
    text("Размер (ширина) исходное изображение", 240, 60);
    text("Вращение изображения", 240, 90);
    text("Позиция изображения по XY (слева)", 240, 130);
    text("Яркость и контраст (справа)", 240, 150);
    text("Преобразовать в серые тона / Инверсия / Фильтр dithering для OLED / Рамка ", 240, 260);
    text("Включить и настроить “порог” (преобразует в чёткое чёрно-белое изображение)", 240, 310);
    text("Включить и настроить постеризацию (уменьшение количества оттенков)", 240, 360);
    text("Кисть и её размер (ЛКМ - белый, ПКМ - правый)", 240, 410);

    text("Ширина битмапа. Можно колёсиком мыши более точно настроить!", 240, 480);
    text("Высота битмапа. Можно колёсиком мыши более точно настроить!", 240, 510);
    text("Линейный ЧБ битмап (8 бит в байте) / RGB16 / RGB32 / Вывод для OLED (вертикальный байт) / PROGMEM", 240, 540);
    text("Сохранить в bitmap.h / Копировать в буфер обмена", 240, 590);

    text("Помощь (нажми, чтобы закрыть) / О программе", 240, height - 10);
  } else {
    text("Open image / Save edited image", 240, 30);
    text("Size (width) of source image", 240, 60);
    text("Rotate", 240, 90);
    text("XY position", 240, 130);
    text("Brightness & contrast", 240, 150);
    text("Grayscale / Invert / Dithering (for OLED) / Show frame ", 240, 260);
    text("Threshold", 240, 310);
    text("Posterisation", 240, 360);
    text("Brush mode / Size (LMB - white, RMB - black)", 240, 410);

    text("Bitmap width (use mouse wheel for fine tuning)", 240, 480);
    text("Bitmap height (use mouse wheel for fine tuning)", 240, 510);
    text("Linear BW mode (8 bit in byte) / RGB16 / RGB32 / OLED (vertical byte) / PROGMEM", 240, 540);
    text("Save into bitmap.h / Copy to clipboard", 240, 590);

    text("Help (press again to close) / About", 240, height - 10);
  }
}

void about() {
  link("http://alexgyver.ru/");
}
