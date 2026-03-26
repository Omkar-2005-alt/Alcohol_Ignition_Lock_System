## 💻 Final Arduino Code (Ready for GitHub)

```cpp id="finalcode001"
#define BLYNK_TEMPLATE_ID "TMPL3S4uJkHNU"
#define BLYNK_TEMPLATE_NAME "ALCOHOL IGNITION LOCK"
#define BLYNK_AUTH_TOKEN "YOUR_BLYNK_TOKEN"   // Replace with your token

#ifdef ESP32
  #include <WiFi.h>
  #include <BlynkSimpleEsp32.h>
#else
  #include <ESP8266WiFi.h>
  #include <BlynkSimpleEsp8266.h>
#endif

#define BLYNK_PRINT Serial

// 🔐 Replace with your WiFi credentials
char ssid[] = "YOUR_WIFI";
char pass[] = "YOUR_PASSWORD";

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define gas A0
#define motor D5
#define buzzer D6

int gas_sense;
int a = 0;
int b = 0;
String status = "NORMAL";

void setup()
{
  Serial.begin(9600);

  pinMode(gas, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(motor, OUTPUT);
  pinMode(D0, OUTPUT);

  digitalWrite(buzzer, LOW);
  digitalWrite(motor, LOW);
  digitalWrite(D0, LOW);

  lcd.init();
  lcd.backlight();

  lcd.clear();
  lcd.print("CONNECTING TO...");
  lcd.setCursor(0,1);
  lcd.print(ssid);
  delay(1000);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  while(WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println(WiFi.localIP());

  Blynk.config(BLYNK_AUTH_TOKEN);
  delay(500);

  lcd.clear();
  lcd.print("WIFI CONNECTED");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
  delay(1500);

  lcd.clear();
  lcd.print("ALCOHOL SYSTEM");
  lcd.setCursor(0,1);
  lcd.print("IGNITION LOCK");
  delay(1500);
}

void beep()
{
  digitalWrite(buzzer, HIGH); delay(300);
  digitalWrite(buzzer, LOW); delay(200);
  digitalWrite(buzzer, HIGH); delay(300);
  digitalWrite(buzzer, LOW); delay(200);
}

void loop()
{
  Blynk.run();

  b++;
  if(b > 50) b = 0;

  gas_sense = analogRead(gas);
  gas_sense = map(gas_sense, 0, 1023, 0, 100);

  Serial.print("ALCOHOL LEVEL: ");
  Serial.println(gas_sense);

  lcd.clear();
  lcd.print("ALCOHOL: ");
  lcd.print(gas_sense);
  lcd.print("%");

  if(gas_sense >= 30)
  {
    status = "ALERT";
    a++;
    if(a >= 10) a = 10;

    beep();
    digitalWrite(motor, LOW);   // Engine OFF
  }
  else
  {
    status = "NORMAL";
    digitalWrite(motor, HIGH);  // Engine ON
  }

  lcd.setCursor(0,1);
  lcd.print("STATUS: ");
  lcd.print(status);

  if(a == 2)
  {
    Blynk.logEvent("gas_alert");
    Blynk.virtualWrite(V0, gas_sense);
  }

  if(b == 50)
  {
    Blynk.virtualWrite(V0, gas_sense);
  }

  digitalWrite(D0, LOW); delay(300);
  digitalWrite(D0, HIGH); delay(200);
}
```
