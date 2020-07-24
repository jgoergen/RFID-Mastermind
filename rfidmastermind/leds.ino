float r;
float g;
float b;

void LEDS_Init()
{
    strip.begin();
    strip.setBrightness(RGB_LED_BRIGHTNESS);
    strip.show();
}

void LEDS_SetAll(uint32_t color)
{
    for (uint16_t i = 0; i < NEOPIXEL_COUNT; i++)
    {
        strip.setPixelColor(i, color);
        strip.show();
    }
}

void LEDS_RunIntro()
{
    for (float i = 0; i < float(NEOPIXEL_COUNT); i++)
    {
        r = byte(((pow(sin(((elapsed * SPEED_MULTIPLIER) + i) / 2.0), 1.0) + 1.0) * 80.0) + 70.0);
        g = byte(((pow(sin(((elapsed * SPEED_MULTIPLIER) + i * 2.0) / 4.0), 2.0) + 1.0) * 80.0) + 70.0);
        b = byte(((pow(cos(((elapsed * SPEED_MULTIPLIER) + i * 3.0) / 3.0), 3.0) + 1.0) * 80.0) + 70.0);

        strip.setPixelColor(i, strip.Color(floor(r), floor(g), floor(b)));
    }
}

void LEDS_SetColor(uint32_t color, byte index)
{
    strip.setPixelColor(index, strip.Color(r, g, b));
}

void LEDS_Flash(uint32_t color, byte count, int pause)
{
    for (byte i = 0; i < count; i++)
    {
        LEDS_SetAll(color);
        delay(pause);
        LEDS_SetAll(strip.Color(0, 0, 0));
        delay(pause);
    }
}
