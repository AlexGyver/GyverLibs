#include "NecDecoder.h"

NecDecoder ir;

void setup() {
  Serial.begin(115200);
  attachInterrupt(0, ir_handler, FALLING);
}

void loop() {
  if (ir.isDecoded()) {          // Если поймали пакет
    if (ir.packetIsValid()) {    // И он прошел проверку (еще есть ir.addressIsValid() и ir.commandIsValid())
      Serial.println("\nNEC packet is decoded & valid");
      Serial.print("Packet: ");
      Serial.println(ir.readPacket(), HEX);     // Пакет целиком
      Serial.print("Address: ");
      Serial.println(ir.readAddress(), HEX);    // Только адрес
      Serial.print("~Address: ");
      Serial.println(ir.readInvAddress(), HEX); // Только инвертированный адрес
      Serial.print("Command: ");
      Serial.println(ir.readCommand(), HEX);    // Только комманда
      Serial.print("~Command: ");
      Serial.println(ir.readInvCommand(), HEX); // Только инвертированная команда
    }
  } else if (ir.isRepeat()) {                   // Если поймали код повтора
    Serial.print("\nRepeat: ");                   // Выведем последнюю принятый пакет
    Serial.println(ir.readPacket(), HEX);
  }
}

void ir_handler() {
  ir.tick();  // обязательно FALLING фронт (можно и вручную отслеживать)
}
