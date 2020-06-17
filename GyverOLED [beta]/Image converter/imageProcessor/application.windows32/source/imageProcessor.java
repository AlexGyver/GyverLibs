import processing.core.*; 
import processing.data.*; 
import processing.event.*; 
import processing.opengl.*; 

import controlP5.*; 
import processing.serial.*; 
import java.util.*; 
import java.awt.Toolkit; 
import java.awt.datatransfer.StringSelection; 
import java.awt.datatransfer.Clipboard; 

import java.util.HashMap; 
import java.util.ArrayList; 
import java.io.File; 
import java.io.BufferedReader; 
import java.io.PrintWriter; 
import java.io.InputStream; 
import java.io.OutputStream; 
import java.io.IOException; 

public class imageProcessor extends PApplet {

// *** imageProcessor ***
// Software for converting images into bitmaps for OLEDs and other displays
// AlexGyver, 2020, https://alexgyver.ru/

// constants
int nodeOffset = 5;        // мин. расстояние до следующего узла
int offsetWidth = 230;     // оффсет панели управления
int maxLines = 5000;       // макс. количество линий
int lastNodeWIndow = 10;   // окно последних повторяющихся узлов
int nailLength = 2;        // "длина" одного гвоздя для расчёта
int maxWidth = 650;
int maxHeight = 650;

// P5
// http://www.sojamo.de/libraries/controlP5/reference/index.html

ControlP5 cp5;
Slider2D sliderXY, sliderBC;
Slider sizeSlider;

// JAVA


Serial myPort;

// clipboard




// image
PImage image;
PGraphics brushLayer, hiddenLayer;
int sizeX, sizeY;
int imageWidth = 100;
int imageXoffs = 0, imageYoffs = 0;
String imagePath;
int centerX;
int centerY;

// settings
float rotAngle = 0;
boolean thresholdState = false;
float thresholdValue = 0.5f;
boolean grayscaleState = false;
boolean invertState = false;
int posterizeValue = 10;
boolean posterizeState = false;
float contrastValue = 1.0f;
float brightnessValue = 0;
boolean ditherState = false;
boolean frameState = false;
boolean changeFlag = true;
int overlapAmount;
float subtractWidth;
int subtractAlpha;
boolean brushState = false;
int brushSize = 10;
boolean helpState = false;
int brushColor = color(255);
int brushX;
int brushY;
boolean drawBrush = false;
int imgX, imgY;
int resultWidth, resultHeight, rectX, rectY;
float rectSize;
boolean progmem = true;
int saveMode = 0;
String saveLines;
boolean langulage = false;  // 0 рус, 1 англ

public void setup() {  
  
  frameRate(60);
  
  noStroke();

  imagePath = "noImage.jpg";
  image = loadImage(imagePath);
  imageWidth = image.width;

  GUIinit();
  sizeSlider.setValue(image.width);
  centerX = offsetWidth+150;
  centerY = height/2;
  rectX = (offsetWidth + width) / 2;
  rectY = height/2;
  brushLayer = createGraphics(width, height);
  hiddenLayer = createGraphics(width, height);
}

public void draw() {
  if (!helpState) drawImage();    // обработка и вывод картинки
  else showHelp();  // подсказки
  drawGUI();        // выводим интерфейс
  //println(mouseX + " " + mouseY);
}

// ===============================================================

public void drawGUI() {
  // панель управления
  fill(90);
  noStroke();
  rect(0, 0, offsetWidth, height);
  fill(255);
  rect(0, 225, offsetWidth, 3);
  rect(0, 440, offsetWidth, 3);
  rect(0, height-45, offsetWidth, 3);
}
// ================ EDITOR ==================

public void load_image() {
  selectInput("Select a file to process:", "fileSelected");
  changeFlag = true;
}

public void fileSelected(File selection) {
  if (selection == null) {
    println("Not selected");
  } else {
    imagePath = selection.getAbsolutePath();
    println("Select: " + imagePath);
    image = loadImage(imagePath);
    imageWidth = 256;
    image.resize(imageWidth, 0);    
    sizeSlider.setValue(image.width);
    cp5.getController("img_rotate").setValue(0);
    sliderXY.setValue(0, 0);
    sliderBC.setValue(0, 1);
  }
  changeFlag = true;
}

public void save_image() {
  PImage image = loadImage(imagePath);  
  image = filtered(image);
  image.save("outputImage.bmp");
}

public void img_width(int size) {
  imageWidth = size;
  changeFlag = true;
}

public void img_rotate(int val) {
  rotAngle = radians(val);
}

public void image_pos() {
  changeFlag = true;
}

public void br_contr() {
  changeFlag = true;
}

public void grayscale(boolean state) {
  grayscaleState = !state;
  changeFlag = true;
}

public void invert(boolean state) {
  invertState = !state;
  changeFlag = true;
}

public void dither(boolean state) {
  ditherState = !state;
  changeFlag = true;
}

public void frame(boolean state) {
  frameState = !state;
  changeFlag = true;
}

public void threshold(boolean state) {
  thresholdState = !state;
  changeFlag = true;
}

public void t_value(float val) {
  thresholdValue = val;
  changeFlag = true;
}

public void posterize(boolean state) {
  posterizeState = !state;
  changeFlag = true;
}

public void p_value(int val) {
  posterizeValue = val;
  changeFlag = true;
}

public void brush(boolean state) {
  changeFlag = true;  
  brushState = !state;
  brushLayer = createGraphics(width, height);
}

public void brush_size(int val) {
  brushSize = val;
  changeFlag = true;
}

public void result_width(int val) {
  resultWidth = val;
}
public void result_height(int val) {
  resultHeight = val;
}

public void bar(int val) {
  saveMode = val;
}

public void progmem(boolean val) {
  progmem = !val;
}

public void generateBitmap() {
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

public void save_bitmap() {
  generateBitmap();
  String[] lines = new String[1];
  lines[0] = saveLines;
  saveStrings("bitmap.h", lines);
}

public void copy_clipboard() {
  generateBitmap();
  StringSelection selection = new StringSelection(saveLines);
  Clipboard clipboard = Toolkit.getDefaultToolkit().getSystemClipboard();
  clipboard.setContents(selection, selection);
}

public int getPix(int x, int y) {
  if (x >= resultWidth || y >= resultHeight) return 0;
  int c = get(PApplet.parseInt(rectX + x * rectSize - rectSize*resultWidth/2 + rectSize/2), PApplet.parseInt(rectY + y * rectSize - rectSize*resultHeight/2 + rectSize/2));
  if (brightness(c) < 127) return 1;
  else return 0;
}

public int getPixRGB32(int x, int y) {
  if (x >= resultWidth || y >= resultHeight) return 0;
  return (get(PApplet.parseInt(rectX + x * rectSize - rectSize*resultWidth/2 + rectSize/2), PApplet.parseInt(rectY + y * rectSize - rectSize*resultHeight/2 + rectSize/2)));
}

public int getPixRGB16(int x, int y) {
  if (x >= resultWidth || y >= resultHeight) return 0;
  int col = get(PApplet.parseInt(rectX + x * rectSize - rectSize*resultWidth/2 + rectSize/2), PApplet.parseInt(rectY + y * rectSize - rectSize*resultHeight/2 + rectSize/2));
  int red = (col & 0x00FF0000) >> 16;
  int green = (col & 0x0000FF00) >> 8;
  int blue =  col & 0x000000FF;
  return (red >> 3 << 11) + (green >> 2 << 5) + (blue >> 3);
}

// ================ BOTTOM ==================

public void help_ru() {
  helpState = !helpState;
  langulage = false;
  if (!helpState) changeFlag = true;
}
public void help_en() {
  helpState = !helpState;
  langulage = true;
  if (!helpState) changeFlag = true;
}

public void showHelp() {
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

public void about() {
  link("http://alexgyver.ru/");
}
public void GUIinit() {
  cp5 = new ControlP5(this);  

  // ======== EDITOR ========

  cp5.addButton("load_image").setCaptionLabel("OPEN  IMAGE").setPosition(10, 10).setSize(100, 25);

  cp5.addButton("save_image").setCaptionLabel("SAVE  IMAGE").setPosition(120, 10).setSize(100, 25);

  sizeSlider = cp5.addSlider("img_width").setCaptionLabel("IMG  SIZE").setPosition(10, 40).setSize(210, 25).setRange(2, 1000).setValue(100).setNumberOfTickMarks(998-1).showTickMarks(false);
  cp5.getController("img_width").getCaptionLabel().setPaddingX(-40);

  sizeSlider = cp5.addSlider("img_rotate").setCaptionLabel("ROTATE").setPosition(10, 70).setSize(210, 25).setRange(0, 360).setValue(0);
  cp5.getController("img_rotate").getCaptionLabel().setPaddingX(-35);

  sliderXY = cp5.addSlider2D("image_pos").setPosition(10, 100).setSize(100, 100).setMinMax(-100, -100, 100, 100).setValue(0, 0);

  sliderBC = cp5.addSlider2D("br_contr").setPosition(120, 100).setSize(100, 100).setMinMax(-128, 0.0f, 128, 5.0f).setValue(0, 1);  

  cp5.addToggle("grayscale").setPosition(10, 240).setSize(45, 25).setMode(ControlP5.SWITCH).setValue(true);

  cp5.addToggle("invert").setPosition(65, 240).setSize(45, 25).setMode(ControlP5.SWITCH).setValue(true);

  cp5.addToggle("dither").setPosition(120, 240).setSize(45, 25).setMode(ControlP5.SWITCH).setValue(true);

  cp5.addToggle("frame").setPosition(175, 240).setSize(45, 25).setMode(ControlP5.SWITCH).setValue(false);

  cp5.addToggle("threshold").setPosition(10, 290).setSize(45, 25).setMode(ControlP5.SWITCH).setValue(true);

  cp5.addSlider("t_value").setCaptionLabel("VALUE").setPosition(65, 290).setSize(155, 25).setRange(0.0f, 1.0f).setValue(0.5f);
  cp5.getController("t_value").getCaptionLabel().setPaddingX(-30);

  cp5.addToggle("posterize").setPosition(10, 340).setSize(45, 25).setMode(ControlP5.SWITCH).setValue(true);

  cp5.addSlider("p_value").setCaptionLabel("VALUE").setPosition(65, 340).setSize(155, 25).setRange(2, 10).setValue(10).setNumberOfTickMarks(9);
  cp5.getController("p_value").getCaptionLabel().setPaddingX(-30);

  cp5.addToggle("brush").setPosition(10, 390).setSize(45, 25).setMode(ControlP5.SWITCH).setValue(true);

  cp5.addSlider("brush_size").setCaptionLabel("BRUSH  SIZE").setPosition(65, 390).setSize(155, 25).setRange(1, 10).setValue(1).setNumberOfTickMarks(9);
  cp5.getController("brush_size").getCaptionLabel().setPaddingX(-55);

  cp5.addSlider("result_width").setCaptionLabel("RESULT  WIDTH").setPosition(10, 460).setSize(210, 25).setRange(1, maxWidth).setValue(128).setNumberOfTickMarks(maxWidth-1).showTickMarks(false);
  cp5.getController("result_width").getCaptionLabel().setPaddingX(-65);

  cp5.addSlider("result_height").setCaptionLabel("RESULT  HEIGHT").setPosition(10, 490).setSize(210, 25).setRange(1, maxHeight).setValue(64).setNumberOfTickMarks(maxHeight-1).showTickMarks(false);
  cp5.getController("result_height").getCaptionLabel().setPaddingX(-65);

  cp5.addButtonBar("bar").setPosition(10, 520).setSize(155, 25).addItem("mono", "mono").addItem("rgb16", "rgb16").addItem("rgb32", "rgb32").addItem("oled", "oled"); 

  cp5.addToggle("progmem").setPosition(175, 520).setSize(45, 25).setMode(ControlP5.SWITCH).setValue(false);

  cp5.addButton("save_bitmap").setCaptionLabel("SAVE  BITMAP").setPosition(10, 570).setSize(100, 25);
  cp5.addButton("copy_clipboard").setCaptionLabel("COPY  BITMAP").setPosition(120, 570).setSize(100, 25);

  // ======== BOTTOM ========

  cp5.addButton("help_ru").setPosition(10, height-30).setSize(45, 25);  
  cp5.addButton("help_en").setPosition(65, height-30).setSize(45, 25); 
  cp5.addButton("about").setPosition(120, height-30).setSize(100, 25);
}
// 10,65,120,175
// ================== ФИЛЬТР DITHERING ==================

// https://gist.github.com/joshmurr/7998502
int[] colorPalette = new int[] {
  color(0, 0, 0), 
  color(0, 0, 255), 
  color(0, 255, 0), 
  color(0, 255, 255), 
  color(255, 0, 0), 
  color(255, 0, 255), 
  color(255, 255, 0), 
  color(255, 255, 255)
};

int[] greyPalette = new int[] {
  color(0), 
  color(51), 
  color(102), 
  color(153), 
  color(204), 
  color(255)
};

int[] bwPalette = new int[] {
  color(0, 0, 0), 
  color(255, 255, 255)
};

int[] rbPalette = new int[] {
  color(255, 0, 0), 
  color(0, 0, 255)
};

int[] rPalette = new int[] {
  color(156, 7, 201), 
  color(9, 245, 89)
};

public void ditherImage(PImage img) {
  img.loadPixels();
  int w = img.width;
  int h = img.height;

  int[][] p = new int[h][w];
  float s = 7.0f/16;
  float t = 3.0f/16;
  float f = 5.0f/16;
  float o = 1.0f/16;

  for (int y=0; y<h; y++) {
    for (int x=0; x<w; x++) {
      p[y][x] = img.pixels[y*w+x];
    }
  }

  for (int y=0; y<h; y++) {
    for (int x=0; x<w; x++) {
      int oldP = p[y][x];

      int newP = find_new_color(oldP, bwPalette);
      p[y][x] = newP;

      img.pixels[y*w+x] = p[y][x];

      float qr = red(oldP) - red(newP);
      float qg = green(oldP) - green(newP);
      float qb = blue(oldP) - blue(newP);

      if (x+1 < w) {
        float nr = (p[y][x+1] >> 16 & 0xFF) + s * qr;
        float ng = (p[y][x+1] >> 8 & 0xFF) + s * qg;
        float nb = (p[y][x+1] & 0xFF) + s * qb;
        p[y][x+1] = color(nr, ng, nb);
      }

      if (x-1>=0 && y+1<h) {
        float nr = (p[y+1][x-1] >> 16 & 0xFF) + t * qr;
        float ng = (p[y+1][x-1] >> 8 & 0xFF) + t * qg;
        float nb = (p[y+1][x-1] & 0xFF) + t * qb;
        p[y+1][x-1] = color(nr, ng, nb);
      }
      if (y+1 < h) {
        float nr = (p[y+1][x] >> 16 & 0xFF) + f * qr;
        float ng = (p[y+1][x] >> 8 & 0xFF) + f * qg;
        float nb = (p[y+1][x] & 0xFF) + f * qb;
        p[y+1][x] = color(nr, ng, nb);
      }
      if (x+1<w && y+1<h) {
        float nr = (p[y+1][x+1] >> 16 & 0xFF) + o * qr;
        float ng = (p[y+1][x+1] >> 8 & 0xFF) + o * qg;
        float nb = (p[y+1][x+1] & 0xFF) + o * qb;
        p[y+1][x+1] = color(nr, ng, nb);
      }
    }
  }

  img.updatePixels();
  image(img, 0, 0);
}

public int find_new_color(int c, int[] palette) {
  int bestIndex = 0;

  PVector[] vpalette = new PVector[palette.length];
  PVector vcolor = new PVector((c >> 16 & 0xFF), (c >> 8 & 0xFF), (c & 0xFF));
  float distance = vcolor.dist(new PVector(0, 0, 0));

  for (int i=0; i<palette.length; i++) {
    vpalette[i] = new PVector((palette[i] >> 16 & 0xFF), (palette[i] >> 8 & 0xFF), (palette[i] & 0xFF));
    float d = vcolor.dist(vpalette[i]);
    if (d < distance) {
      distance = d;
      bestIndex = i;
    }
  }
  return palette[bestIndex];
}
// применение фильтров
public PImage filtered(PImage image) {  
  image.resize(imageWidth, 0);
  ContrastAndBrightness(image, image, contrastValue, brightnessValue);
  if (thresholdState) image.filter(THRESHOLD, thresholdValue);
  if (grayscaleState) image.filter(GRAY);
  if (invertState) image.filter(INVERT);
  if (posterizeState) image.filter(POSTERIZE, posterizeValue);
  if (ditherState) ditherImage(image);  
  return image;
}

// вывод изображения
public void drawImage() {
  if (changeFlag) {
    image = loadImage(imagePath);
    image = filtered(image);
    brightnessValue = sliderBC.getArrayValue()[0];
    contrastValue = sliderBC.getArrayValue()[1]; 
    imageXoffs = PApplet.parseInt(sliderXY.getArrayValue()[0]);
    imageYoffs = PApplet.parseInt(sliderXY.getArrayValue()[1]);
    imgX = centerX - image.width/2 + imageXoffs;
    imgY = centerY - image.height/2 + imageYoffs; 
    changeFlag = false;
  }
  background(255);

  // рисуем картинку
  hiddenLayer.beginDraw();
  hiddenLayer.background(255);
  hiddenLayer.imageMode(CENTER);
  hiddenLayer.pushMatrix();
  hiddenLayer.translate(width/2, height/2);
  hiddenLayer.rotate(rotAngle);
  hiddenLayer.image(image, 0, 0);
  hiddenLayer.endDraw();    
  hiddenLayer.popMatrix();

  // заливка от дисера
  if (ditherState) {
    noStroke();
    fill(255);
    rect(0, 0, width, imgY);
    rect(0, 0, imgX, height);
  }

  int maxSide = max(resultHeight, resultWidth);
  rectSize = maxHeight / maxSide;
  noStroke();
  for (int i = 0; i < resultWidth; i++) {
    for (int j = 0; j < resultHeight; j++) {
      fill(hiddenLayer.get(width/2 - resultWidth/2+i-imageXoffs, centerY - resultHeight/2+j-imageYoffs));
      rect(PApplet.parseInt(rectX + i * rectSize - rectSize*resultWidth/2), PApplet.parseInt(rectY + j * rectSize - rectSize*resultHeight/2), rectSize, rectSize);
    }
  }

  int x, y;
  x = round((mouseX-brushSize*rectSize/2 - rectX + rectSize*resultWidth/2) / rectSize);
  y = round((mouseY-brushSize*rectSize/2 - rectY + rectSize*resultHeight/2) / rectSize);
  x = round(rectX + x * rectSize - rectSize*resultWidth/2);
  y = round(rectY + y * rectSize - rectSize*resultHeight/2);

  if (mouseY > (rectY-rectSize*resultHeight/2) && mouseY < (rectY+rectSize*resultHeight/2) &&  
    mouseX > (rectX-rectSize*resultWidth/2) && mouseX < (rectX+rectSize*resultWidth/2) &&  
    brushState && mousePressed) {
    if (mouseButton == LEFT) brushColor = 255;
    else if (mouseButton == RIGHT) brushColor = 0;      
    brushLayer.beginDraw();
    brushLayer.noStroke();      
    brushLayer.fill(brushColor);
    brushLayer.rect(x, y, rectSize*brushSize, rectSize*brushSize);      
    brushLayer.endDraw();
  }

  if (brushState) {
    image(brushLayer, 0, 0);
    stroke(255, 0, 0);
    strokeWeight(1);
    noFill();
    rect(x, y, rectSize*brushSize, rectSize*brushSize);
  }

  if (frameState) {
    noFill();
    stroke(255, 0, 0);
    strokeWeight(2);
    rect(rectX - rectSize*resultWidth/2, rectY - rectSize*resultHeight/2, rectSize*resultWidth, rectSize*resultHeight);
  }
}

// ================== ФИЛЬТР ЯРКОСТЬ/КОНТРАСТ ==================
// https://forum.processing.org/one/topic/increase-contrast-of-an-image.html
public void ContrastAndBrightness(PImage input, PImage output, float cont, float bright) {
  int w = input.width;
  int h = input.height;

  //our assumption is the image sizes are the same
  //so test this here and if it's not true just return with a warning
  if (w != output.width || h != output.height)
  {
    println("error: image dimensions must agree");
    return;
  }

  //this is required before manipulating the image pixels directly
  input.loadPixels();
  output.loadPixels();

  //loop through all pixels in the image
  for (int i = 0; i < w*h; i++)
  {  
    //get color values from the current pixel (which are stored as a list of type 'color')
    int inColor = input.pixels[i];

    //slow version for illustration purposes - calling a function inside this loop
    //is a big no no, it will be very slow, plust we need an extra cast
    //as this loop is being called w * h times, that can be a million times or more!
    //so comment this version and use the one below
    int r = (int) red(input.pixels[i]);
    int g = (int) green(input.pixels[i]);
    int b = (int) blue(input.pixels[i]);

    //here the much faster version (uses bit-shifting) - uncomment to try
    //int r = (inColor >> 16) & 0xFF; //like calling the function red(), but faster
    //int g = (inColor >> 8) & 0xFF;
    //int b = inColor & 0xFF;      

    //apply contrast (multiplcation) and brightness (addition)
    r = (int)(r * cont + bright); //floating point aritmetic so convert back to int with a cast (i.e. '(int)');
    g = (int)(g * cont + bright);
    b = (int)(b * cont + bright);

    //slow but absolutely essential - check that we don't overflow (i.e. r,g and b must be in the range of 0 to 255)
    //to explain: this nest two statements, sperately it would be r = r < 0 ? 0 : r; and r = r > 255 ? 255 : 0;
    //you can also do this with if statements and it would do the same just take up more space
    r = r < 0 ? 0 : r > 255 ? 255 : r;
    g = g < 0 ? 0 : g > 255 ? 255 : g;
    b = b < 0 ? 0 : b > 255 ? 255 : b;

    //and again in reverse for illustration - calling the color function is slow so use the bit-shifting version below
    output.pixels[i] = color(r, g, b);
    //output.pixels[i]= 0xff000000 | (r << 16) | (g << 8) | b; //this does the same but faster
  }

  //so that we can display the new image we must call this for each image
  input.updatePixels();
  output.updatePixels();
}
  public void settings() {  size(1200, 650);  smooth(); }
  static public void main(String[] passedArgs) {
    String[] appletArgs = new String[] { "imageProcessor" };
    if (passedArgs != null) {
      PApplet.main(concat(appletArgs, passedArgs));
    } else {
      PApplet.main(appletArgs);
    }
  }
}
