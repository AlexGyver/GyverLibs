// узнать токен бота можно у BotFather
// узнать chat id можно по запросу https://api.telegram.org/bot<token>/getUpdates
// искать "chat":{"id": тут id

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
  bot.setChatID(CHAT_ID); // передай "" (пустую строку) чтобы отключить проверку

  // подключаем функцию-обработчик
  bot.attach(parseHandler);

  bot.sendMessage("Hello, World!");   // работает, если указан CHAT_ID
}

// создать свою функцию вида имя(String& строка)
void parseHandler(String& name, String& msg) {
  // msg - сообщение в чате от name юзера
  Serial.print(name);
  Serial.print(", ");
  Serial.println(msg);
}

// можно вручную дёргать по одному сообщению при помощи tickManual
// тогда "лимит" при инициализации поставить 1
void loop() {
  // тикаем в луп
  bot.tick();
}
