#include "SPS_Display.h"
#include <Arduino.h>

SPS_Display::SPS_Display(uint8_t addr, int fps) : lcd(addr, 20, 4), nextAnimation(0), timeWindow(1000 / fps), lastAnimationTime(-1)
{
}

void SPS_Display::init()
{
    lcd.init();
    lcd.backlight();

    lcd.createChar(0, Heart1);
    lcd.createChar(1, Heart2);
    lcd.createChar(2, Heart3);
    lcd.createChar(3, Heart4);

    lcd.clear();
}

void SPS_Display::render(int s1, int s2, int s3, int s4, int s5, int s6)
{
    long now = millis();
    if (lastAnimationTime > -1)
    {
        long elapsed = now - lastAnimationTime;
        long timeToWait = timeWindow - elapsed;
        if (timeToWait > 0)
        {
            return;
        }
    }

    const int slotsLeft = 6 - s1 - s2 - s3 - s4 - s5 - s6;

    animate();
    lastAnimationTime = now;

    lcd.setCursor(4, 0);
    lcd.print("Have slot: " + String(slotsLeft));

    lcd.setCursor(0, 1);
    printSlot(1, s1);
    lcd.setCursor(12, 1);
    printSlot(2, s2);

    lcd.setCursor(0, 2);
    printSlot(3, s3);
    lcd.setCursor(12, 2);
    printSlot(4, s4);

    lcd.setCursor(0, 3);
    printSlot(5, s5);
    lcd.setCursor(12, 3);
    printSlot(6, s6);
}

void SPS_Display::animate()
{
    lcd.setCursor(0, 0);
    lcd.write(nextAnimation);
    lcd.setCursor(2, 0);
    lcd.write(nextAnimation);
    lcd.setCursor(17, 0);
    lcd.write(nextAnimation);
    lcd.setCursor(19, 0);
    lcd.write(nextAnimation);
    nextAnimation = (nextAnimation + 1) % 4;
}

void SPS_Display::printSlot(int slot, int state)
{
    char *buf = (char *)malloc(9 * sizeof(char));
    snprintf(buf, 9, "S%d:%s", slot, state == 1 ? "Fill " : "Empty");

    lcd.print(buf);
    free(buf);
};