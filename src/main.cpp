#include <Arduino.h>

#ifdef RGB_BUILTIN
#include <Adafruit_NeoPixel.h>
#endif

#ifndef LED_PIN
#define LED_PIN LED_BUILTIN
#endif

#ifndef MONITOR_SPEED
#define MONITOR_SPEED 9600
#endif


unsigned long NextChange = 0;
bool Value = false;

#ifdef RGB_BUILTIN
unsigned int RgbState = 0;
#endif

void setup()
{
    Serial.begin(MONITOR_SPEED);
    while (!Serial) {
        delay(1000);
    };
    delay(1000);
    Serial.flush();
    Serial.println("\nBlink setup()");
    #ifndef RGB_BUILTIN
    Serial.print("LED_PIN=");
    Serial.println(LED_PIN);
    pinMode(LED_PIN, OUTPUT);
    #else
    Serial.print("RGB_BUILTIN=");
    Serial.println(RGB_BUILTIN);
    Serial.print("RGB_POWER=");
    Serial.println(RGB_POWER);
    Serial.print("RGB_DATA=");
    Serial.println(RGB_DATA);
    pinMode(RGB_POWER, OUTPUT);
    pinMode(RGB_DATA, OUTPUT);
    digitalWrite(RGB_POWER, HIGH);
    #endif
    NextChange = millis();
}

void led_toggle() {
    Serial.print("Toggle ");
    Serial.print(Value);
    Value = !Value;
    digitalWrite(LED_PIN, Value);
}

#ifdef RGB_BUILTIN
void rgb_toggle () 
{
    Serial.print("RgbState ");
    Serial.print(RgbState);
    
    if (RgbState == 0) {
        neopixelWrite(RGB_DATA, 0, 255, 0);
        RgbState = 1;
    }
    else if (RgbState == 1) { 
        neopixelWrite(RGB_DATA, 255, 0, 0);
        RgbState = 2;
    }
    else if (RgbState == 2) {
        neopixelWrite(RGB_DATA, 0, 0, 255);
        RgbState = 0;
    }
}
#endif

void loop()
{
    if (millis() >= NextChange) {
        Serial.print(NextChange);
        Serial.print(": ");
        #ifndef RGB_BUILTIN
        led_toggle();
        #else
        rgb_toggle();
        #endif
        Serial.println();
        NextChange += 1000;
    }
}