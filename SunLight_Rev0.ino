#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>  

#ifdef __AVR__
  #include <avr/power.h>
#endif

#define STRIP1_PIN 5
#define STRIP2_PIN 6
#define STRIP3_PIN 7
#define STRIP4_PIN 8

Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(60, STRIP1_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(60, STRIP2_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip3 = Adafruit_NeoPixel(60, STRIP3_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip4 = Adafruit_NeoPixel(60, STRIP4_PIN, NEO_GRB + NEO_KHZ800);

int bluetoothTx = 2;  // TX-O pin of bluetooth mate, Arduino D2
int bluetoothRx = 3;  // RX-I pin of bluetooth mate, Arduino D3

SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

  int x = 0;
  int userColor = 0;
  char command = '0';
  char mode = '0';
  int userR = 0;
  int userG = 0;
  int userB = 0;
  
void setup() {

  Serial.begin(9600);  // Begin the serial monitor at 9600bps

  bluetooth.begin(115200);  // The Bluetooth Mate defaults to 115200bps
  bluetooth.print("$");  // Print three times individually
  bluetooth.print("$");
  bluetooth.print("$");  // Enter command mode
  delay(100);  // Short delay, wait for the Mate to send back CMD
  bluetooth.println("U,9600,N");  // Temporarily Change the baudrate to 9600, no parity
  // 115200 can be too fast at times for NewSoftSerial to relay the data reliably
  bluetooth.begin(9600);  // Start bluetooth serial at 9600
  delay(100);
  
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif

  strip1.begin();
  strip1.show(); 
  strip2.begin();
  strip2.show();
  strip3.begin();
  strip3.show();  
  strip4.begin();
  strip4.show();

  // Flash lights 4 Times To Show Initialzation
  for(x=0; x<4; x++)
  {
    setStrip1Color(strip1.Color(255, 70, 10), 50); // Red
    setStrip2Color(strip2.Color(255, 70, 10), 50); // Red
    setStrip3Color(strip3.Color(255, 70, 10), 50); // Red
    setStrip4Color(strip4.Color(255, 70, 10), 50); // Red
    delay(500);
  }

  // Set All Lights To Soft White Initially
  setStripsSoftWhite();
  
}

void loop() {

  if(bluetooth.available())  // If the bluetooth sent any characters
  {
    // Send any characters the bluetooth prints to the serial monitor
    mode = bluetooth.read();
    Serial.print("Mode ="); 
    Serial.print(mode);
    Serial.print("\n");  
  }
  
  // Modes:
  // 0: Off
  // 1: Soft White
  // 2: Custom Color
  // 3: Fade
  // 4: Party

  while(mode == '0') // Mode 0 - All Lights Off, Standby Mode.
  {
    delay(100);
    if(bluetooth.available())  // If the bluetooth sent any characters
    {
      mode = bluetooth.read();
      Serial.print("Mode ="); 
      Serial.print(mode);
      Serial.print("\n");  
    }
  }
  
  while(mode == '1') // Mode 1 - Soft White | All Lights Set to a Soft White Color
  {
    setStripsSoftWhite();
    if(bluetooth.available())  // If the bluetooth sent any characters
    {
      mode = bluetooth.read();
      Serial.print("Mode ="); 
      Serial.print(mode);
      Serial.print("\n");  
    }
  }
  
  while(mode == '2') // Mode 2 - Custom Color | User Chooses Color Through Interface
  {
    setStrip1Color(strip1.Color(userR, userG, userB), 50); // Red
    setStrip2Color(strip2.Color(userR, userG, userB), 50); // Red
    setStrip3Color(strip3.Color(userR, userG, userB), 50); // Red
    setStrip4Color(strip4.Color(userR, userG, userB), 50); // Red
    delay(100);
    
    if(bluetooth.available())  // If the bluetooth sent any characters
    {
      command = bluetooth.read();
      if(command != '1' && command != '2' && command != '3' && command != '4')
      {
        userColor = command;
      }
      else
      {
        mode = command;
        Serial.print("Mode ="); 
        Serial.print(mode);
        Serial.print("\n");  
      }
    }
  }
  
  while(mode == '3') // Mode 3 - Fade | Fades Between Variety of Colors
  {
    delay(100);
    
    if(bluetooth.available())  // If the bluetooth sent any characters
    {
      mode = bluetooth.read();
      Serial.print("Mode ="); 
      Serial.print(mode);
      Serial.print("\n");  
    }
  }
  
  while(mode == '4') // Mode 4 - Party | Color Changing / Rainbow and Color Swipes
  {
    delay(100);  
    
    if(bluetooth.available())  // If the bluetooth sent any characters
    {
      mode = bluetooth.read();
      Serial.print("Mode ="); 
      Serial.print(mode);
      Serial.print("\n");  
    }
  }
  
}


void setStripsSoftWhite(void) {
  setStrip1Color(strip1.Color(255, 70, 10), 50); // Red
  setStrip2Color(strip1.Color(255, 70, 10), 50); // Red
  setStrip3Color(strip1.Color(255, 70, 10), 50); // Red
  setStrip4Color(strip1.Color(255, 70, 10), 50); // Red
}

// Fill the dots one after the other with a color
void setStrip1Color(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<8; i++) {
    strip1.setPixelColor(i, c);
  }
  strip1.show();
}
void setStrip2Color(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<8; i++) {
    strip2.setPixelColor(i, c);
  }
  strip2.show();
}
void setStrip3Color(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<8; i++) {
    strip3.setPixelColor(i, c);
  }
  strip3.show();
}
void setStrip4Color(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<8; i++) {
    strip4.setPixelColor(i, c);
  }
  strip4.show();
}
