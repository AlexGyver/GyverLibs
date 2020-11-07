// ================ MOVE ==================

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
    sizeSlider.setValue(128);
    cp5.getController("img_rotate").setValue(0);
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

// ============== EFFECTS ==============

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

// ============== ENCODING ==============

void result_width(int val) {
  resultWidth = val;
}
void result_height(int val) {
  resultHeight = val;
}

void dropdown(int val) {
  saveMode = val;
  cp5.getController("save_bitmap").setVisible(true);
  cp5.getController("copy_clipboard").setVisible(true);
  cp5.getController("progmem").setVisible(true);
  cp5.getController("invert_result").setVisible(true);
  cp5.getController("flip_x").setVisible(true);
  cp5.getController("flip_y").setVisible(true);
}

void b_dimension(boolean val) {
  dimension = !val;
  cp5.getController("b_dimension").setCaptionLabel(dimension ? "2D ARRAY" : "1D ARRAY");
}

void progmem(boolean val) {
  progmem = !val;
}
void invert_result(boolean val) {
  invert = !val;
}
void flip_x(boolean val) {
  flipX = !val;
}
void flip_y(boolean val) {
  flipY = !val;
}

String generateName(int arrayH, int arrayW) {
  return ("_"+resultWidth+"x"+resultHeight+(dimension ? "["+ arrayH +"]["+ arrayW +"]" : "[]") + (progmem ? " PROGMEM" : "") + " = {\n");
}


void generateBitmap() {
  String thisName = cp5.get(Textfield.class,"name").getText();
  saveLines = "";
  switch (saveMode) {
  case 0:  
    // ==== битмап для оледов ====
    int numRows = ceil(resultHeight / 8.0);    
    saveLines = "const uint8_t " + thisName + generateName(numRows, resultWidth);

    for (byte r = 0; r < numRows; r++) {
      saveLines += "\t";
      if (dimension) saveLines += "{";
      for (int j = 0; j < resultWidth; j++) {     
        byte thisByte = 0;
        for (byte b = 0; b < 8; b++) thisByte |= getPixBW(j, r*8+7-b) << (7-b);          
        saveLines += "0x" + hex(thisByte, 2);
        saveLines += ", ";
      }
      if (dimension) saveLines += "},";
      saveLines += "\n";
    }
    saveLines += "};";
    break;
  case 1:
    // линейный битмап
    saveLines = "const uint8_t " + thisName + generateName(resultHeight, resultWidth/8);
    for (byte h = 0; h < resultHeight; h++) {
      saveLines += "\t";
      if (dimension) saveLines += "{";
      for (int i = 0; i < resultWidth; i+=8) {      
        byte thisByte = 0;
        for (byte b = 0; b < 8; b++) thisByte |= getPixBW(b+i, h) << (7-b);          
        saveLines += "0x" + hex(thisByte, 2);
        saveLines += ", ";
      }
      if (dimension) saveLines += "},";
      saveLines += "\n";
    }
    saveLines += "};";
    break;
  case 2:
    // 1 pix/byte, BW
    saveLines = "const uint8_t " + thisName + generateName(resultHeight, resultWidth);
    for (int y = 0; y < resultHeight; y++) {
      saveLines += "\t";
      if (dimension) saveLines += "{";
      for (int x = 0; x < resultWidth; x++) {
        saveLines += getPixBW(x, y) + ", ";
      }
      if (dimension) saveLines += "},";
      saveLines += "\n";
    }
    saveLines += "};";
    break;
  case 3:
    // ==== 1 pix/byte, Gray ====
    saveLines = "const uint8_t " + thisName + generateName(resultHeight, resultWidth);
    for (int y = 0; y < resultHeight; y++) {
      saveLines += "\t";
      if (dimension) saveLines += "{";
      for (int x = 0; x < resultWidth; x++) {
        saveLines += "0x" + hex(getPixGray(x, y), 2) + ", ";
      }
      if (dimension) saveLines += "},";
      saveLines += "\n";
    }
    saveLines += "};";
    break;
  case 4:
    // ==== rgb16 ====
    saveLines = "const uint16_t " + thisName + generateName(resultHeight, resultWidth);
    for (int y = 0; y < resultHeight; y++) {
      saveLines += "\t";
      if (dimension) saveLines += "{";
      for (int x = 0; x < resultWidth; x++) {
        saveLines += "0x" + hex(getPixRGB32(x, y), 4) + ", ";
      }
      if (dimension) saveLines += "},";
      saveLines += "\n";
    }
    saveLines += "};";
    break;
  case 5:
    // ==== rgb32 ====
    saveLines = "const uint32_t " + thisName + generateName(resultHeight, resultWidth);
    for (int y = 0; y < resultHeight; y++) {
      saveLines += "\t";
      if (dimension) saveLines += "{";
      for (int x = 0; x < resultWidth; x++) {
        saveLines += "0x" + hex(getPixRGB32(x, y), 6) + ", ";
      }
      if (dimension) saveLines += "},";
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

// ============== COLOR PICKERS ==============
color getColor(int x, int y) {
  if (flipX) x = resultWidth-1 - x;
  if (flipY) y = resultHeight-1 - y;
  color c = get(int(rectX + x * rectSize - rectSize*resultWidth/2 + rectSize/2), int(rectY + y * rectSize - rectSize*resultHeight/2 + rectSize/2));
  return (invert ? ~c : c);
}

int getPixBW(int x, int y) {
  if (x >= resultWidth || y >= resultHeight) return 0;
  if (brightness(getColor(x, y)) < 127) return 0;
  else return 1;
}

byte getPixGray(int x, int y) {
  if (x >= resultWidth || y >= resultHeight) return 0;
  return byte(brightness(getColor(x, y)));
}

int getPixRGB32(int x, int y) {
  if (x >= resultWidth || y >= resultHeight) return 0;
  return getColor(x, y);
}

int getPixRGB16(int x, int y) {
  if (x >= resultWidth || y >= resultHeight) return 0;
  int col = getColor(x, y);
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

void about() {
  link("https://github.com/AlexGyver/imageProcessor");
}

void showHelp() {
  background(255);
  int[] textPos = new int[15];
  textPos[0] = 30; 
  textPos[1] = 60;
  textPos[2] = 90; 
  textPos[3] = 130;
  textPos[4] = 150; 
  textPos[5] = 260;
  textPos[6] = 310;
  textPos[7] = 360;
  textPos[8] = 410; 
  textPos[9] = 480;
  textPos[10] = 510; 
  textPos[11] = 540;
  textPos[12] = 590; 
  textPos[13] = 650;
  textPos[14] = height - 10; 

  noFill();
  stroke(0);
  strokeWeight(3);
  rect(840, 3, 300, 520);

  PFont myFont;
  myFont = createFont("Ubuntu", 15);
  textFont(myFont);
  fill(0);
  String[] helpText;

  if (!langulage) helpText = loadStrings("langRU");
  else helpText = loadStrings("langEN");
  for (byte i = 0; i < 15; i++) {
    text(helpText[i], 240, textPos[i]);
  }
  for (byte i = 0; i < 34; i++) {
    text(helpText[i+16], 850, 20+i*15);
  }
}
