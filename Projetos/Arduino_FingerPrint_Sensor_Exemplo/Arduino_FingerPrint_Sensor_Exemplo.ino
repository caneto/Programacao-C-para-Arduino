  #include <Wire.h>
  #include <Adafruit_GFX.h>
  #include <Adafruit_SSD1306.h>
  #define OLED_RESET 4
  Adafruit_SSD1306 display(OLED_RESET);

  #include <Adafruit_Fingerprint.h>
  #include <SoftwareSerial.h>
  SoftwareSerial mySerial(2, 3);

  Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
  int fingerprintID = 0;
  String IDname;

  void setup(){
    //Configuração do módulo do sensor de impressão digital
    Serial.begin(9600);
    // definir a taxa de dados para a porta serial do sensor
    finger.begin(57600);
    
    if (finger.verifyPassword()) {
      Serial.println("Found fingerprint sensor!");
    } 
    else {
      Serial.println("Did not find fingerprint sensor :(");
      while (1) { delay(1); }
    }

    //Configuração da tela OLED
    Wire.begin();
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    //Configuração da tela OLED
    displayMainScreen();
  }

  void loop(){
    displayMainScreen();
    fingerprintID = getFingerprintIDez();
    delay(50);
    if(fingerprintID == 1 || fingerprintID == 3 || fingerprintID == 4 || fingerprintID == 5){
      IDname = "Sergio";
      displayUserGreeting(IDname);            
    } else if(fingerprintID == 2){
      IDname = "Julio";  
      displayUserGreeting(IDname);            
    }
  }

  // retorna -1 se houver falha, caso contrário, retorna ID #
  int getFingerprintIDez() {
    uint8_t p = finger.getImage();
    if (p != FINGERPRINT_OK)  return -1;

    p = finger.image2Tz();
    if (p != FINGERPRINT_OK)  return -1;

    p = finger.fingerFastSearch();
    if (p != FINGERPRINT_OK)  return -1;
    
    // encontrou uma correspondência!
    Serial.print("Found ID #"); 
    Serial.print(finger.fingerID); 
    Serial.print(" with confidence of "); 
    Serial.println(finger.confidence);
    return finger.fingerID; 
  }

  void displayMainScreen(){
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(7,5);
    display.println("Waiting fingerprint");
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(52,20);
    display.println("...");  
    display.display();
    delay(2000);
  }

  void displayUserGreeting(String Name){
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.setCursor(0,0);
    display.print("Hello");
    display.setCursor(0,15);
    display.print(Name); 
    display.display();
    delay(5000);
    fingerprintID = 0; 
  }