// применение фильтров
PImage filtered(PImage image) {  
  image.resize(imageWidth, 0);
  ContrastAndBrightness(image, image, contrastValue, brightnessValue);
  if (thresholdState) image.filter(THRESHOLD, thresholdValue);
  if (grayscaleState) image.filter(GRAY);
  if (invertState) image.filter(INVERT);
  if (posterizeState) image.filter(POSTERIZE, posterizeValue);
  if (ditherState) ditherImage(image);  
  return image;
}

void mouseWheel(MouseEvent event) {
  if (mouseX > offsetWidth) {
    imageWidth -= event.getCount();
    imageWidth = constrain(imageWidth, 2, 1000);
    cp5.getController("img_width").setValue(imageWidth);
    changeFlag = true;
  }
}

// вывод изображения
void drawImage() {
  if (mousePressed && mouseButton == CENTER) {
    if (mouseX > offsetWidth) {
      if (!mouseState) {
        mouseState = true;  // фиксируем нажатие
        draggedX = mouseX;
        draggedY = mouseY;
      } else {
        draggedXadd = (mouseX - draggedX)/5;
        draggedYadd = (mouseY - draggedY)/5;
      }
    }
    changeFlag = true;
  } else {
    if (mouseState) {
      mouseState = false;  // фиксируем отпускание      
      imageXresult += draggedXadd;
      imageYresult += draggedYadd;
      draggedXadd = 0;
      draggedYadd = 0;
    }
  }

  if (changeFlag) {
    image = loadImage(imagePath);
    image = filtered(image);
    brightnessValue = sliderBC.getArrayValue()[0];
    contrastValue = sliderBC.getArrayValue()[1];

    imageXadd = int(sliderXY.getArrayValue()[0]);
    imageYadd = int(sliderXY.getArrayValue()[1]);

    imageXoffs = imageXresult+imageXadd+draggedXadd;
    imageYoffs = imageYresult+imageYadd+draggedYadd;

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
      rect(int(rectX + i * rectSize - rectSize*resultWidth/2), int(rectY + j * rectSize - rectSize*resultHeight/2), rectSize, rectSize);
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
void ContrastAndBrightness(PImage input, PImage output, float cont, float bright) {
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
    color inColor = input.pixels[i];

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
