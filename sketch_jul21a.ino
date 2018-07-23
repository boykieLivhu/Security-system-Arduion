#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
 
#define SS_PIN 10
#define RST_PIN 9
#define LED_G 3 
#define LED_R 4
LiquidCrystal_I2C lcd(0x27,20,4); 
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
Servo myServo; 
 
void setup() 
{
  lcd.init();
  lcd.backlight();
  
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  myServo.attach(7); //servo pin
  myServo.write(0);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_R, OUTPUT);
  Serial.println("Put your card to the reader...");
  Serial.println();

}
void loop() 
{
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "5B 1A 67 C1") //change here the UID of the card/cards that you want to give access
  {
    lcd.clear();
    Serial.println("Authorized access");
    Serial.println();
    lcd.print("Authorized access");
    lcd.setCursor(0,1);
    lcd.print(content);
    delay(3000);
    myServo.write(180);
    digitalWrite(LED_G, HIGH);
    delay(5000);
    myServo.write(0);
  }
 
 else   {
    Serial.println(" Access denied");
    lcd.clear();
    lcd.print("Access denied");
    lcd.setCursor(0,1);
    lcd.print(content);
    digitalWrite(LED_R, HIGH);
    delay(500);
    digitalWrite(LED_R, LOW);
    delay(500);
    digitalWrite(LED_R, HIGH);
    delay(500);
    digitalWrite(LED_R, LOW);
    delay(400);
    digitalWrite(LED_R, HIGH);
    delay(3000);
    digitalWrite(LED_R, LOW);
  }
} 
