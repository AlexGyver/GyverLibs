#include <microLiquidCrystal_I2C.h>

// 0x27 или 0x3f
LiquidCrystal_I2C lcd(0x3f, 20, 4);

void setup()
{
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Hello, world!");
}

void loop()
{
}
