#include <Arduino.h>

#ifndef DISABLE_SERIAL
void dbg_serial_println(const char* fmt, ...)
{
    char buffer[256];
    va_list args;
    va_start(args, fmt);
    int rc = vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    Serial.print(buffer);
}
#define PRINT(...) { dbg_serial_println(__VA_ARGS__); }
#define FLUSH() { Serial.flush(); }
#else
#define PRINT(...) ;
#define FLUSH(...) ;
#endif // ifdef SERIAL_DEBUG

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
    PRINT("\n")
    FLUSH();
    PRINT("\nBlink setup()\n");
    #endif

    #ifndef RGB_BUILTIN
    PRINT("LED_PIN=%d\n", LED_PIN);
    pinMode(LED_PIN, OUTPUT);
    #else
    PRINT("RGB_BUILTIN=%d\n", RGB_BUILTIN);
    PRINT("RGB_POWER=%d\n", RGB_POWER);
    PRINT("RGB_DATA=%d\n", RGB_DATA);

    pinMode(RGB_POWER, OUTPUT);
    pinMode(RGB_DATA, OUTPUT);
    digitalWrite(RGB_POWER, HIGH);
    #endif
    NextChange = millis();
}

void led_toggle() {
    PRINT("Toggle %d\n", Value);
    Value = !Value;
    digitalWrite(LED_PIN, Value);
}

#ifdef RGB_BUILTIN
void rgb_toggle () 
{    
    PRINT("RgbState %d\n", RgbState);
 
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
        PRINT("%d: ", NextChange);
        #ifndef RGB_BUILTIN
        led_toggle();
        #else
        rgb_toggle();
        #endif
        NextChange += 1000;
    }
}