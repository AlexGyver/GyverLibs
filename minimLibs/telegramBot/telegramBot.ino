#define WIFI_SSID "login"
#define WIFI_PASS "pass"
#define BOT_TOKEN "2654326546:asjhAsfAsfkllgUsaOuiz_axfkj_AsfkjhB"
#define CHAT_ID "574578754"


#include <ESP8266WiFi.h>
#include "FastBot.h"
FastBot bot(BOT_TOKEN);

//FastBot bot(токен, лимит, порог, период);
// токен - уникальный код бота, берётся у BotFather
// лимит - количество сообщений, получаемое из одного запроса (по умолч. 10)
// порог - количество символов, при котором API запрос будет считаться слишком большим и будет пропущен (по умолч. 10000)
// период - период автоматического опроса бота в мс (по умолч. 1000)

void setup() {
  delay(2000);
  Serial.begin(115200);
  Serial.println();

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected");

  // установить ID чата, чтобы принимать сообщения только из него
  // узнать ID можно из ручного запроса в браузере
  //bot.setChatID(CHAT_ID); // передай "" (пустую строку) чтобы отключить проверку

  // подключаем функцию-обработчик
  bot.attach(parseHandler);
}

// создать свою функцию вида имя(String& строка)
void parseHandler(String& str) {
  // при вызове str примет значение сообщения в чате!
  Serial.println(str);
}

// можно вручную дёргать по одному сообщению при помощи tickManual
// тогда "лимит" при инициализации поставить 1
void loop() {
  // тикаем в луп
  bot.tick();
}
