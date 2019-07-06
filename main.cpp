#include "mbed.h"
#include "pindef.h"
#include <vector>
/*
Define the PWM speaker output
Define analog inputs
*/

Serial device(UART_TX, UART_RX);
BusOut lowLed(PA_10, PB_3, PB_5);
BusOut midLed(PA_8, PA_9, PC_7);
BusOut highLed(PA_7, PA_6, PA_5);
DigitalIn button(PB_9);

PwmOut speaker(PB_6);
PwmOut bass(PB_10);
AnalogIn volume(PA_0);
AnalogIn pitch(PA_1);

#include "variables.h"

#include "despacito.h"
#include "cascada.h"

int song = 0;

/*----------------------------------------------------------------------------
 MAIN function
 *----------------------------------------------------------------------------*/

float noteLength(int bpm, float noteType)
{
    return 60.0 / (float)bpm * noteType - NOTE_END;
}

void alexaPlayDespacito()
{
    Timer t;
    for (int i = 0; i < despacitoSize; ++i) {
        midLed = rand() % 7 + 1;
        highLed = rand() % 7 + 1;
        t.start();
        speaker.period(1.0/(despacito[i].second * 4.0));
        while (t.read() < noteLength(despacitoBPM, despacito[i].first)) {
            for(float j = 0.0; j < 1.0; j += SAW_STEP) {
                speaker = j;
                wait(DELAY);
            }
            for(float j = 1.0; j > 0.0; j -= SAW_STEP) {
                speaker = j;
                wait(DELAY);
            }
        }
        speaker.period(1.0);
        wait(NOTE_END);
        t.reset();
    }
}

void alexaPlayDespacitoBass()
{
    Timer t;
    for (int i = 0; i < despacitoBassSize; ++i) {
        lowLed = despacitoBass[i].second == 1 ? 0x0 : 0x2;
        t.start();
        bass.period(1.0/(despacitoBass[i].second * 32.0));
        while (t.read() < noteLength(despacitoBPM, despacitoBass[i].first)) {
            for(float j = 0.0; j < 1.0; j += SAW_STEP) {
                bass = j;
                wait(DELAY);
            }
            for(float j = 1.0; j > 0.0; j -= SAW_STEP) {
                bass = j * 2;
                wait(DELAY);
            }
        }
        bass.period(1.0);
        wait(NOTE_END);
        t.reset();
    }
}


void ladyMelody()
{

    Timer t;
    for (int i = 0; i < cascadaSize; ++i) {
        midLed = rand() % 7 + 1;
        highLed = rand() % 7 + 1;
        t.start();
        speaker.period(1.0/(cascada[i].second * 4.0));
        while (t.read() < noteLength(cascadaBPM, cascada[i].first)) {
            for(float j = 0.0; j < 1.0; j += SAW_STEP) {
                speaker = j;
                wait(DELAY);
            }
            for(float j = 1.0; j > 0.0; j -= SAW_STEP) {
                speaker = j;
                wait(DELAY);
            }
        }
        speaker.period(1.0);
        wait(NOTE_END);
        t.reset();
    }
}

void misterBass()
{

    Timer t;
    for (int i = 0; i < cascadaBassSize; ++i) {
        lowLed = cascadaBass[i].second == 1 ? 0x0 : 0x2;
        t.start();
        bass.period(1.0/(cascadaBass[i].second * 32.0));
        while (t.read() < noteLength(cascadaBPM, cascadaBass[i].first)) {
            for(float j = 0.0; j < 1.0; j += SAW_STEP) {
                bass = j;
                wait(DELAY);
            }
            for(float j = 1.0; j > 0.0; j -= SAW_STEP) {
                bass = j * 2;
                wait(DELAY);
            }
        }
        bass.period(1.0);
        wait(NOTE_END);
        t.reset();
    }
}

void buttonHandler()
{
    bool standby = false;
    while(1) {
        //device.printf("%d\n\r", (int)button);
        if (standby && button != 0x0) {
            standby = false;
        }
        if (!standby && button == 0x0) {

            song == 0 ? song = 1 : song = 0;
            standby = true;
            wait(0.02);
        }
        wait(0.02);

    }

}

int main()
{

    Thread buttonThread;
    buttonThread.start(buttonHandler);

    while(1) {

        //device.printf("%d\n\r", song);
        if (song == 0) {
            Thread despacitoThread;
            Thread despacitoBassThread;
            despacitoThread.start(alexaPlayDespacito);
            despacitoBassThread.start(alexaPlayDespacitoBass);
            despacitoThread.join();
            despacitoBassThread.join();
        } else if (song == 1) {
            Thread melodyThread;
            Thread bassThread;
            melodyThread.start(ladyMelody);
            bassThread.start(misterBass);
            melodyThread.join();
            bassThread.join();
        }
    }
}
