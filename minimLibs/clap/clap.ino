// распознавание количества хлопков

#include "VolAnalyzer.h"
#include "Clap.h"
VolAnalyzer vol(A0);	// микрофон на А0
Clap clap;
void setup() {
  Serial.begin(9600);
}

void loop() {
  if (vol.tick());
  clap.tick(vol.getRawMax());
  if (clap.haveClaps()) {
    Serial.println(clap.getClaps());
  }
  if (clap.isClap()) Serial.println("clap");
}
