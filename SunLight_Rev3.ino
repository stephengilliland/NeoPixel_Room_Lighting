#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>  

#ifdef __AVR__
  #include <avr/power.h>
#endif

#define STRIP1_PIN 5
#define STRIP2_PIN 6
#define STRIP3_PIN 7
#define STRIP4_PIN 8

Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(148, STRIP1_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(148, STRIP2_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip3 = Adafruit_NeoPixel(148, STRIP3_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip4 = Adafruit_NeoPixel(148, STRIP4_PIN, NEO_GRB + NEO_KHZ800);


  int x = 0;
  int userColor = 0;
  byte command = 0;
  byte mode = 0;
  byte userR = 0;
  byte userG = 0;
  byte userB = 0;
  byte currR = 2;
  byte currG = 252;
  byte currB = 211;
  byte newR = 2;
  byte newG = 252;
  byte newB = 211;

  
void setup() {

  Serial.begin(9600);  // Begin the serial monitor at 9600bps

  Serial1.begin(115200);  // The Serial1 Mate defaults to 115200bps
  Serial1.print("$");  // Print three times individually
  Serial1.print("$");
  Serial1.print("$");  // Enter command mode
  delay(100);  // Short delay, wait for the Mate to send back CMD
  Serial1.println("U,115200,N");  // Temporarily Change the baudrate to 9600, no parity
  // 115200 can be too fast at times for NewSoftSerial to relay the data reliably
  Serial1.begin(115200);  // Start Serial1 serial at 9600
  
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
  swipeStripsBlue();
  delay(2000);

  // Set All Lights To Soft White Initially
  swipeStripsSoftWhite();
  
}

void loop() {

  if(Serial1.available())  // If the Serial1 sent any characters
  {
    // Send any characters the Serial1 prints to the serial monitor
    mode = Serial1.read();
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


// Mode 0 - All Lights Off, Standby Mode.
  while(mode == 0) 
  {
    swipeStripsOff();
    userR = 0;
    userG = 0;
    userB = 0;
    delay(100);
    if(Serial1.available())  // If the Serial1 sent any characters
    {
      mode = Serial1.read();
      Serial.print("Mode ="); 
      Serial.print(mode);
      Serial.print("\n");  
    }
  }


// Mode 1 - Soft White | All Lights Set to a Soft White Color
  while(mode == 1) 
  {
    swipeStripsSoftWhite();
    userR = 255;
    userG = 70;
    userB = 10;
    delay(200);
    if(Serial1.available())  // If the Serial1 sent any characters
    {
      mode = Serial1.read();
      Serial.print("Mode ="); 
      Serial.print(mode);
      Serial.print("\n");  
    }
  }


// Mode 2 - Custom Color | User Chooses Color Through Interface
  while(mode == 2) 
  {
    setStrips(userR, userG, userB); // Red
    delay(250);
    
    if(Serial1.available())  // If the Serial1 sent any characters
    {
      command = Serial1.read();
      if(command != 0 && command != 1 && command != 2 && command != 3 && command != 4)
      {
        userR = command;
        Serial.print("User Red: ");
        Serial.println(userR);
        
        while(!Serial1.available()){}
        if(Serial1.available())  // If the Serial1 sent any characters
          userG = Serial1.read();
          Serial.print("User Green: ");
          Serial.println(userG);
        
        while(!Serial1.available()){}
        if(Serial1.available())  // If the Serial1 sent any characters
          userB = Serial1.read();
          Serial.print("User Blue: ");
          Serial.println(userB);
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


// Mode 3 - Fade | Fades Between Variety of Colors
  while(mode == 3) 
  {
    slowSwipeColor(currR, currG, currB, newR, newG, newB); 
    currR = newR;
    currG = newG;
    currB = newB;
    newR = random(255);
    newG = random(255);
    newB = random(255);

    if(mode == 3)
      delay(2000);
    if(Serial1.available())  // If the Serial1 sent any characters
    {
      mode = Serial1.read();
      Serial.print("Mode ="); 
      Serial.print(mode);
      Serial.print("\n");  
    }
    
    if(mode == 3)
      delay(2000);
    if(Serial1.available())  // If the Serial1 sent any characters
    {
      mode = Serial1.read();
      Serial.print("Mode ="); 
      Serial.print(mode);
      Serial.print("\n");  
    }
    
    if(mode == 3)
      delay(2000);
    if(Serial1.available())  // If the Serial1 sent any characters
    {
      mode = Serial1.read();
      Serial.print("Mode ="); 
      Serial.print(mode);
      Serial.print("\n");  
    }
    
    if(mode == 3)
      delay(2000);
        if(Serial1.available())  // If the Serial1 sent any characters
    {
      mode = Serial1.read();
      Serial.print("Mode ="); 
      Serial.print(mode);
      Serial.print("\n");  
    }
  }


// Mode 4 - Party | Color Changing / Rainbow and Color Swipes
  while(mode == 4) 
  {
    delay(100);  
    
    if(Serial1.available())  // If the Serial1 sent any characters
    {
      mode = Serial1.read();
      Serial.print("Mode ="); 
      Serial.print(mode);
      Serial.print("\n");  
    }
  } 
}

///
///
///
///

void slowSwipeColor(byte currRed, byte currGrn, byte currBlue, byte newRed, byte newGrn, byte newBlue)
{
  int done = 0;

  while(!done)
  {
    if(currRed < newRed)
        currRed += 1;
      if(currGrn < newGrn)
        currGrn += 1;
      if(currBlue < newBlue)
        currBlue += 1;   

      if(currRed > newRed)
        currRed -= 1;
      if(currGrn > newGrn)
        currGrn -= 1;
      if(currBlue > newBlue)
        currBlue -= 1;   
 
    for(uint16_t i=0; i<strip1.numPixels(); i++) {
      strip1.setPixelColor(i, strip1.Color(currRed, currGrn, currBlue));
      strip2.setPixelColor(i, strip2.Color(currRed, currGrn, currBlue));
      strip3.setPixelColor(i, strip3.Color(currRed, currGrn, currBlue));
    }
    
    if(currRed == newRed && currGrn == newGrn && currBlue == newBlue)
      done = 1;
    delay(10);
    strip1.show();
    strip2.show();
    strip3.show();
  }
}

void swipeStripsSoftWhite(void) {
  for(uint16_t i=0; i<strip1.numPixels(); i++) {
    strip1.setPixelColor(i, strip1.Color(255, 70, 10));
    strip1.show();
    strip2.setPixelColor(i, strip2.Color(255, 70, 10));
    strip2.show();
    strip3.setPixelColor(i, strip3.Color(255, 70, 10));
    strip3.show();
    //strip4.setPixelColor(i, strip4.Color(255, 70, 10));
    //strip4.show();
  }
}

void setStrips(byte red, byte green, byte blue) {
  for(uint16_t i=0; i<strip1.numPixels(); i++) {
    strip1.setPixelColor(i, strip1.Color(red, green, blue));
    strip1.show();
    strip2.setPixelColor(i, strip2.Color(red, green, blue));
    strip2.show();
    strip3.setPixelColor(i, strip3.Color(red, green, blue));
    strip3.show();
    //strip4.setPixelColor(i, strip4.Color(red, green, blue));
    //strip4.show();
  }
}

void swipeStripsOff(void) {
  for(uint16_t i=0; i<strip1.numPixels(); i++) {
    strip1.setPixelColor(i, strip1.Color(0, 0, 0));
    strip1.show();
    strip2.setPixelColor(i, strip2.Color(0, 0, 0));
    strip2.show();
    strip3.setPixelColor(i, strip3.Color(0, 0, 0));
    strip3.show();
    //strip4.setPixelColor(i, strip4.Color(0, 0, 0));
    //strip4.show();
  }
}

void swipeStripsBlue(void) {
  for(uint16_t i=0; i<strip1.numPixels(); i++) {
    strip1.setPixelColor(i, strip1.Color(0, 0, 255));
    strip1.show();
    strip2.setPixelColor(i, strip2.Color(0, 0, 255));
    strip2.show();
    strip3.setPixelColor(i, strip3.Color(0, 0, 255));
    strip3.show();
    //strip4.setPixelColor(i, strip4.Color(0, 0, 255));
    //strip4.show();
  }
}

// Fill the dots one after the other with a color
void setStrip1Color(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip1.numPixels(); i++) {
    strip1.setPixelColor(i, c);
    strip1.show();
  }
}
void setStrip2Color(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip2.numPixels(); i++) {
    strip2.setPixelColor(i, c);
  }
  strip2.show();
}
void setStrip3Color(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip3.numPixels(); i++) {
    strip3.setPixelColor(i, c);
  }
  strip3.show();
}
void setStrip4Color(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip4.numPixels(); i++) {
    strip4.setPixelColor(i, c);
  }
  strip4.show();
}
