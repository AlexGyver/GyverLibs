// очень простая и быстрая библиотека для телеграм бота
// оптимизирована для большой нагрузки (спокойно принимает 50 сообщ в секунду)

#ifndef FastBot_h
#define FastBot_h
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
static std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
static HTTPClient https;

/*
  Статусы tick:
  0 - ожидание
  1 - ОК
  2 - Переполнен по ovf
  3 - Ошибка телеграм
  4 - Ошибка подключения
*/

class FastBot {
  public:
    FastBot(String token, int limit = 10, int ovf = 10000, int period = 1000) {
      _request = (String)"https://api.telegram.org/bot" + token;
      _ovf = ovf;
      _limit = limit;
      _period = period;
      client->setInsecure();
    }
    void setChatID(String chatID) {
      _chatID = chatID;
    }
    void setChatID(const char* chatID) {
      _chatID = chatID;
    }

    void attach(void (*handler)(String&, String&)) {
      _callback = handler;
    }

    byte tickManual() {
      byte status = 1;
      if (https.begin(*client, _request + "/getUpdates?limit=" + _limit + "&offset=" + ID)) {
        if (https.GET() == HTTP_CODE_OK) {
          if (https.getSize() > _ovf) {
            int IDpos = https.getString().indexOf("{\"update_id\":", IDpos);
            if (IDpos > 0) ID = https.getString().substring(IDpos + 13, https.getString().indexOf(',', IDpos)).toInt();
            ID++;
            https.end();
            return 2;
          }

          // считаем, сколько в пакете данных (метка update_id)
          int count = 0;
          int IDpos = 0;
          while (1) {
            IDpos = https.getString().indexOf("{\"update_id\":", IDpos);
            if (IDpos > 0) {
              ID = https.getString().substring(IDpos + 13, https.getString().indexOf(',', IDpos)).toInt() + 1;  // сразу ++ для следующего пакета
              IDpos++;
              count++;
            } else break;
          }

          // считаем и парсим сообщения
          if (count) {
            int textPos = 0;
            while (1) {
              int endPos;
              if (_chatID.length() > 0) {
                textPos = https.getString().indexOf("\"chat\":{\"id\":", textPos);
                if (textPos < 0) break;
                endPos = https.getString().indexOf(",\"", textPos);
                String chatID = https.getString().substring(textPos + 13, endPos);
                textPos = endPos;
                if (!chatID.equals(_chatID)) continue;
              }
              textPos = https.getString().indexOf("\"username\":\"", textPos);
              if (textPos < 0) break;
              endPos = https.getString().indexOf("\",\"", textPos);
              String name = https.getString().substring(textPos + 12, endPos);
              textPos = https.getString().indexOf(",\"text\":\"", textPos);
              if (textPos < 0) break;
              endPos = https.getString().indexOf("\"}}", textPos);
              int endPos2 = https.getString().indexOf("\",\"entities", textPos);
              if (endPos > 0 && endPos2 > 0) endPos = min(endPos, endPos2);
              else if (endPos < 0) endPos = endPos2;
              if (https.getString()[textPos + 9] == '/') textPos++;
              String msg = https.getString().substring(textPos + 9, endPos);
              if (*_callback) _callback(name, msg);
              textPos = endPos;
              yield();
            }
          }
        } else status = 3;
        https.end();
      } else status = 4;
      return status;
    }

    byte tick() {
      if (millis() - tmr >= _period) {
        tmr = millis();
        return tickManual();
      }
      return 0;
    }

    byte sendMessage(const char* msg) {
      String request = _request + "/sendMessage?chat_id=" + _chatID + "&text=" + msg;
      return sendRequest(request);
    }

    byte sendMessage(String msg) {
      return sendMessage(msg.c_str());
    }

    byte showMenu(const char* str) {
      String request = _request + "/sendMessage?chat_id=" + _chatID + "&text=Show Menu&reply_markup={\"keyboard\":[[\"";
      for (int i = 0; i < strlen(str); i++) {
        char c = str[i];
        if (c == '\t') request += "\",\"";
        else if (c == '\n') request += "\"],[\"";
        else request += c;
      }
      request += "\"]],\"resize_keyboard\":true}";
      return sendRequest(request);
    }

    byte showMenu(String str) {
      return showMenu(str.c_str());
    }

    byte closeMenu() {
      String request = _request + "/sendMessage?chat_id=" + _chatID + "&text=Close Menu&reply_markup={\"remove_keyboard\":true}";
      return sendRequest(request);
    }

    byte sendRequest(String& req) {
      byte status = 1;
      if (https.begin(*client, req)) {
        if (https.GET() != HTTP_CODE_OK) status = 3;
        https.end();
      } else status = 4;
      return status;
    }

  private:
    void (*_callback)(String& name, String& msg) = NULL;
    String _request;
    int _ovf = 5000, _period = 1000, _limit = 10;
    long ID = 0;
    uint32_t tmr = 0;
    String _chatID = "";
};
#endif
