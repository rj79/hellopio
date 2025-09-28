#include <Arduino.h>

#undef RGB_BUILTIN
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
    #ifndef DISABLE_SERIAL
    Serial.begin(MONITOR_SPEED);
    while (!Serial) {
        delay(1000);
    };
    delay(2000);
    Serial.println();
    Serial.flush();
    Serial.println("\nBlink setup()");
    #endif
    #ifndef RGB_BUILTIN
    #ifndef DISABLE_SERIAL
    Serial.print("LED_PIN=");
    Serial.println(LED_PIN);
    #endif
    pinMode(LED_PIN, OUTPUT);
    #else
    #ifndef DISABLE_SERIAL
    Serial.print("RGB_BUILTIN=");
    Serial.println(RGB_BUILTIN);
    Serial.print("RGB_POWER=");
    Serial.println(RGB_POWER);
    Serial.print("RGB_DATA=");
    Serial.println(RGB_DATA);
    #endif
    pinMode(RGB_POWER, OUTPUT);
    pinMode(RGB_DATA, OUTPUT);
    digitalWrite(RGB_POWER, HIGH);
    #endif
    NextChange = millis();
}

void led_toggle() {
    #ifndef DISABLE_SERIAL
    Serial.print("Toggle ");
    Serial.print(Value);
    #endif
    Value = !Value;
    digitalWrite(LED_PIN, Value);
}

#ifdef RGB_BUILTIN
void rgb_toggle () 
{
    #ifndef DISABLE_SERIAL
    Serial.print("RgbState ");
    Serial.print(RgbState);
    #endif
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
        #ifndef DISABLE_SERIAL
        Serial.print(NextChange);
        Serial.print(": ");
        #endif
        #ifndef RGB_BUILTIN
        led_toggle();
        #else
        rgb_toggle();
        #endif
        #ifndef DISABLE_SERIAL
        Serial.println();
        #endif
        NextChange += 1000;
    }
}