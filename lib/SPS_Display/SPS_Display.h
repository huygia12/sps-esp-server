#ifndef SPS_Display_H
#define SPS_Display_H

#include <LiquidCrystal_I2C.h>

class SPS_Display
{
public:
    /**
     * @param   addr    I2C's address of LCD. Texas Instruments’ PCF8574 chip: 0100A2A1A0. NXP’s PCF8574 chip: 0111A2A1A0
     * @param   fps     Control the animation rate
     */
    SPS_Display(uint8_t addr, int fps);

    /**
     * Display content to LCD
     * @param   s1      slot 1 state, 0 means "Empty", 1 means "Full"
     * @param   s2      slot 2 state, 0 means "Empty", 1 means "Full"
     * @param   s3      slot 3 state, 0 means "Empty", 1 means "Full"
     * @param   s4      slot 4 state, 0 means "Empty", 1 means "Full"
     * @param   s5      slot 5 state, 0 means "Empty", 1 means "Full"
     * @param   s6      slot 6 state, 0 means "Empty", 1 means "Full"

     */
    void render(int s1, int s2, int s3, int s4, int s5, int s6);

    /**
     * Setup LCD connection. Must be called before using other functions
     */
    void init();

private:
    LiquidCrystal_I2C lcd;
    int nextAnimation;
    long lastAnimationTime;
    long timeWindow;

    void animate();
    void printSlot(int slot, int state);

    uint8_t Heart1[8] = {
        0b00000,
        0b00000,
        0b01010,
        0b11111,
        0b11111,
        0b01110,
        0b00100,
        0b00000};
    uint8_t Heart2[8] = {
        0b00000,
        0b01010,
        0b11111,
        0b11111,
        0b01110,
        0b00100,
        0b00000,
        0b00000};
    uint8_t Heart3[8] = {
        0b00000,
        0b00000,
        0b01010,
        0b11111,
        0b11111,
        0b01110,
        0b00100,
        0b00000};
    uint8_t Heart4[8] = {
        0b00000,
        0b00000,
        0b00000,
        0b01010,
        0b11111,
        0b11111,
        0b01110,
        0b00100};
};

#endif