// распознавание количества хлопков

#include "VolAnalyzer.h"
#include "Clap.h"
VolAnalyzer vol(A0);	// микрофон на А0
Clap clap;
void setup() {
  Serial.begin(115200);
}

void loop() {
  vol.tick();
  clap.tick(vol.getRawMax());
  if (clap.isClap()) Serial.println("clap");
  if (clap.hasClaps(1)) Serial.println("1 clap");
  if (clap.hasClaps(2)) Serial.println("2 clap");
  if (clap.hasClaps(3)) Serial.println("3 clap");
}
