#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
#define LED1 4
#define LED2 3
#define LED3 2


MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

byte led1 = 0;
byte led2 = 0;
byte led3 = 0;
MFRC522::MIFARE_Key key;


int code[] = {163, 48, 154, 159}; //This is the stored UID (Unlock Card)
int code1[] = {231, 177, 143, 67};

int codeRead = 0;

String uidString;

void setup() {

  Serial.begin(9600);

  Serial.println("Place your Tag");

  SPI.begin(); // Init SPI bus

  rfid.PCD_Init(); // Init MFRC522

  Serial.println(F("RFID Project"));

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
}



void loop() {

  if(  rfid.PICC_IsNewCardPresent())

  {

      readRFID();

  }

  delay(100);



}



void readRFID()

{



  rfid.PICC_ReadCardSerial();

  Serial.print(F("\nPICC type: "));

  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);

  Serial.println(rfid.PICC_GetTypeName(piccType));



  // Check is the PICC of Classic MIFARE type

  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&

    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&

    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {

    Serial.println(F("Your tag is not of type MIFARE Classic."));

    return;

  }



   

    Serial.println("Scanned PICC's UID:");

    printDec(rfid.uid.uidByte, rfid.uid.size);



    uidString = String(rfid.uid.uidByte[0])+" "+String(rfid.uid.uidByte[1])+" "+String(rfid.uid.uidByte[2])+ " "+String(rfid.uid.uidByte[3]);



    int i = 0;

    boolean match = false;
    boolean match1 = false;
    boolean match2 = false;

    while(i<rfid.uid.size)

    {

 

      if(int(rfid.uid.uidByte[i]) == int(code[i]))

      {

           match = true;
           match1 = false;
           match2 = false; 

      }
      else if(int(rfid.uid.uidByte[i]) == int(code1[i]))
      {

           match1 = true;
           match = false;
           match2 = false;

      }
      else
      {

        match , match1 = false;
        match2 = true;

      }
      i++;

    }



    if(match)

    {

      Serial.println("\n*** Bismillah ***");
      digitalWrite(LED1, HIGH);
      led1 = 1;
      digitalWrite(LED3, LOW);
      led3 = 0;
      digitalWrite(LED2, LOW);
      led2 = 0; 
      delay(10000);
      digitalWrite(LED1, LOW);
      led1 = 0;

      delay(8000);
      digitalWrite(LED1, LOW);
      led1 = 0;
    }
    else if(match1)

    {

      Serial.println("\n*** Bismillah ***");
      digitalWrite(LED2, HIGH);
      led2 = 1;
      digitalWrite(LED3, LOW);
      led3 = 0;
      digitalWrite(LED1, LOW);
      led1 = 0; 

    }
    
    else

    {

      Serial.println("\nAccess Denied");
      digitalWrite(LED1, LOW);
      led2 = 0;
      digitalWrite(LED2, LOW);
      led2 = 0;
      digitalWrite(LED3, HIGH);
      led3 = 1;      

    }

   Serial.println("============================");



    // Halt PICC

  rfid.PICC_HaltA();



  // Stop encryption on PCD

  rfid.PCD_StopCrypto1();

}



void printDec(byte *buffer, byte bufferSize) {

  for (byte i = 0; i < bufferSize; i++) {

    Serial.print(buffer[i] < 0x10 ? " 0" : " ");

    Serial.print(buffer[i], DEC);

  }

}                            