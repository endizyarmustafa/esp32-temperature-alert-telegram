#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

// Wi-Fi bilgileri
const char* ssid = "**********";
const char* password = "*********";

// Telegram bot bilgileri
#define BOT_TOKEN "*****************************"
#define CHAT_ID "*********"

// Telegram bağlantısı
WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

// Sensör bağlantısı
int lm35Pin = A0; 
float sıcaklık;
bool mesajGonderildi = false;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi bağlandı.");

  // SSL sertifikasız bağlantı (Telegram API ile uyumlu)
  client.setInsecure();  // Bu satır olmadan mesaj gönderilemez
}

void loop() {
  int okunan = analogRead(lm35Pin);
  sıcaklık = (okunan * 3.3 / 1023.0) * 100.0;  // LM35 hesaplama

  Serial.print("Okunan analog: "); Serial.print(okunan);
  Serial.print("  => Sıcaklık: "); Serial.println(sıcaklık);

  if (sıcaklık > 35 && !mesajGonderildi) {
    Serial.println("🔥 Uyarı eşiği aşıldı. Mesaj gönderiliyor...");
    bool sonuc = bot.sendMessage(CHAT_ID, "UYARI: Haho em şewitin Kaşan! 🔥", "");

    if (sonuc) {
      Serial.println("✅ Mesaj başarıyla gönderildi.");
    } else {
      Serial.println("❌ Mesaj gönderilemedi!");
    }

    mesajGonderildi = true;
  } else if (sıcaklık <= 35) {
    mesajGonderildi = false;
  }

  delay(5000); // 5 saniyede bir kontrol
}
