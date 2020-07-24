// rfid lib https://github.com/adafruit/Adafruit-PN532

// SETTINGS ///////////////////////////////////////////////////////////////
#define RGB_LED_BRIGHTNESS 100
#define SPEED_MULTIPLIER 0.03
#define INTRO_TIME 1000
#define ROUNDS 5
#define TURNS 4
///////////////////////////////////////////////////////////////////////////

#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <Adafruit_PN532.h>

#define PN532_IRQ_PIN   2
#define PN532_RESET_PIN 3
#define NEOPIXEL_PIN    12

#define RED_CARD_ID     0x89BA4B1F //0x994A5969 
#define GREEN_CARD_ID   0x89BA48D1 //0x99AF5769
#define YELLOW_CARD_ID  0x89BA4277 //0xA98B5D69
#define BLUE_CARD_ID    0x89BA430E //0xF92D6169

#define NEOPIXEL_COUNT  20

Adafruit_PN532 nfc(PN532_IRQ_PIN, PN532_RESET_PIN);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NEOPIXEL_COUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

uint32_t RED_COLOR = strip.Color(255, 0, 0);
uint32_t RED_DIM_COLOR = strip.Color(20, 0, 0);
uint32_t GREEN_COLOR = strip.Color(0, 255, 0);
uint32_t GREEN_DIM_COLOR = strip.Color(0, 20, 0);
uint32_t BLUE_COLOR = strip.Color(0, 0, 255);
uint32_t BLUE_DIM_COLOR = strip.Color(0, 0, 20);
uint32_t YELLOW_COLOR = strip.Color(225, 235, 52);
uint32_t YELLOW_DIM_COLOR = strip.Color(20, 16, 3);

unsigned long introStartTime = 0;
unsigned long elapsed;
byte appPhase = 0;

void setup()
{
    Serial.begin(9600);
    randomSeed(analogRead(0));
    LEDS_Init();
    RFID_Init();
    Serial.println("Started");
    introStartTime = millis();
}

void loop()
{
    elapsed = millis();

    switch (appPhase)
    {
    case 0:
        LEDS_RunIntro();

        if (elapsed > introStartTime + INTRO_TIME)
        {

            LEDS_SetAll(strip.Color(0, 0, 0));
            GAME_Init();
            appPhase = 1;
        }
        break;

    case 1:
        GAME_Update();
        break;
    }

    strip.show();
    delay(33);
}
