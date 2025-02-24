#define factor 1000000
#define sleep_time  6
#define RED 2
#define GREEN 4
RTC_DATA_ATTR int boot_number = 0;
void setup()
{
  Serial.begin(115200);
  pinMode(RED,OUTPUT);
  pinMode(GREEN,OUTPUT);
  delay(500);  
  if(boot_number == 0)
  {
      digitalWrite(RED,HIGH);
      boot_number++;
  }
  else
  {
      digitalWrite(GREEN,HIGH);
  }
  delay(4000);
  digitalWrite(GREEN,LOW);
  digitalWrite(RED,LOW);

  Serial.println("ESP32 going to Deep Sleep");
  esp_sleep_enable_timer_wakeup(sleep_time * factor);
  esp_deep_sleep_start();
}
void loop()
{
}