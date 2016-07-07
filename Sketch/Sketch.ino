#include <Adafruit_NeoPixel.h>
#define PIN            6
#define PIXELSPERROW   8 //Column
#define ROWS   5
#define NUMPIXELS      PIXELSPERROW * ROWS// ROWS * PIXELSPERROW
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int pixelLife[NUMPIXELS];
int pixelLifeNext[NUMPIXELS];
int changes = 0;
int changesBefore = 0, count = 0;
uint32_t life = pixels.Color(30, 0, 0);
uint32_t dead = pixels.Color(0, 0, 10);

void setup() {
  randomSeed(analogRead(0));
  delay(20);
  pixels.begin();
  resetNeopixel();
}

void loop() {
  showNextStep();
  if (changesBefore == changes) {
    count++;
  } else {
    changesBefore = changes;
    count = 0;
  }
  if (count ==  50) {
    resetNeopixel();
    count = 0;
  }
}

void resetNeopixel() {
  for (int i = 0; i < NUMPIXELS ; i++) {
    int token = random(0, 2);
    if (token == 1) {
      pixelLifeNext[i] = 1;
    } else {
      pixelLifeNext[i] = 0;
    }
  }
  syncArrays();
}

void refreshNeopixel() {
  for (int i = 0; i < NUMPIXELS ; i++) {
    if (pixelLife[i] == 1) { //lebendige Zelle
      pixels.setPixelColor( i , life);
    } else {
      pixels.setPixelColor( i , dead);
    }
    pixels.show();
  }
}

void showNextStep() {
  changes = 0;
  for (int i = 0; i < NUMPIXELS ; i++) {
    int life = checkNeiberhood(i);
    if (life < 2 || life > 3) {
      pixelLifeNext[i] = 0;
      changes++;
    }
    if (life == 3) {
      pixelLifeNext[i] = 1;
      changes++;
    }
  }
  syncArrays();
  refreshNeopixel();
}

void syncArrays() {
  for (int i = 0; i < NUMPIXELS ; i++) {
    pixelLife[i] = pixelLifeNext[i];
  }
}

int checkNeiberhood(int pixel) {
  int lifeCount = 0;
  boolean checkLeft = true,
          checkRigth  = true,
          checkBottom  = true,
          checkTop  = true;

  if (PIXELSPERROW - pixel > 0 ) {
    checkLeft = false;
  }
  if (pixel + PIXELSPERROW >= NUMPIXELS) {
    checkRigth = false;
  }
  if (pixel % PIXELSPERROW == 0 ) {
    checkBottom = false;
  }
  if (pixel % PIXELSPERROW == 7 ) {
    checkTop = false;
  }
  if (checkTop && pixelLife[pixel + 1] == 1 ) {
    lifeCount++;
  }
  if (checkBottom && pixelLife[pixel - 1] == 1 ) {
    lifeCount++;
  }
  if (checkLeft  && pixelLife[pixel - PIXELSPERROW] == 1 ) {
    lifeCount++;
  }
  if (checkTop && checkLeft  && pixelLife[pixel - PIXELSPERROW + 1] == 1 ) {
    lifeCount++;
  }
  if (checkBottom && checkLeft && pixelLife[pixel - PIXELSPERROW - 1] == 1 ) {
    lifeCount++;
  }
  if ( checkRigth  &&  pixelLife[pixel + PIXELSPERROW] == 1 ) {
    lifeCount++;
  }
  if (checkTop && checkRigth  &&  pixelLife[pixel + PIXELSPERROW + 1] == 1 ) {
    lifeCount++;
  }
  if (checkBottom && checkRigth && pixelLife[pixel + PIXELSPERROW - 1] == 1 ) {
    lifeCount++;
  }
  return lifeCount;
}
