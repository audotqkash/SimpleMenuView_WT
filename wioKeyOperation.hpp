#ifndef _KEYOPERATION_H_
#define _KEYOPERATION_H_
#include <stdint.h>
#include <Arduino.h>

#define WIO_5S_SHORTPRESS 0x01
#define WIO_5S_SHORTUP    0x02
#define WIO_5S_SHORTDOWN  0x03
#define WIO_5S_SHORTLEFT  0x04
#define WIO_5S_SHORTRIGHT 0x05
#define WIO_3S_SHORTA     0x06
#define WIO_3S_SHORTB     0x07
#define WIO_3S_SHORTC     0x08
#define WIO_5S_LONGPRESS  0x11
#define WIO_5S_LONGUP     0x12
#define WIO_5S_LONGDOWN   0x13
#define WIO_5S_LONGLEFT   0x14
#define WIO_5S_LONGRIGHT  0x15
#define WIO_3S_LONGA      0x16
#define WIO_3S_LONGB      0x17
#define WIO_3S_LONGC      0x18


static uint8_t  keyCode ;
static uint8_t  tmpKeyCode ;  /* 確定前 */
static uint32_t keyDownTime ;
class KeyOperation{
    private:
        bool _inited = false;
        static void keyDown(void);
        static void keyUp(void);

    
    public:
        KeyOperation(void){
            keyCode = 0;
            tmpKeyCode = 0;
            keyDownTime = 0;
        }
        void begin(void){
            if(_inited == true){
                return;
            }
            Serial.println(" |- attach interrupt!");
            pinMode(WIO_KEY_A, INPUT_PULLUP);
            pinMode(WIO_KEY_B, INPUT_PULLUP);
            pinMode(WIO_KEY_C, INPUT_PULLUP);
            pinMode(WIO_5S_PRESS, INPUT_PULLUP);
            pinMode(WIO_5S_UP,    INPUT_PULLUP);
            pinMode(WIO_5S_DOWN,  INPUT_PULLUP);
            pinMode(WIO_5S_LEFT,  INPUT_PULLUP);
            pinMode(WIO_5S_RIGHT, INPUT_PULLUP);
            //attachInterrupt(WIO_KEY_A, keyDown, FALLING); /*使用不可*/
            attachInterrupt(WIO_KEY_B,    keyDown, FALLING);
            attachInterrupt(WIO_KEY_C,    keyDown, FALLING);
            attachInterrupt(WIO_5S_PRESS, keyDown, FALLING);
            attachInterrupt(WIO_5S_UP,    keyDown, FALLING);
            attachInterrupt(WIO_5S_DOWN,  keyDown, FALLING);
            attachInterrupt(WIO_5S_LEFT,  keyDown, FALLING);
            attachInterrupt(WIO_5S_RIGHT, keyDown, FALLING);
        }

        void end()
        {
            detachInterrupt(WIO_KEY_B     );
            detachInterrupt(WIO_KEY_C     );
            detachInterrupt(WIO_5S_PRESS  );
            detachInterrupt(WIO_5S_UP     );
            detachInterrupt(WIO_5S_DOWN   );
            detachInterrupt(WIO_5S_LEFT   );
            detachInterrupt(WIO_5S_RIGHT  );
        }

        uint8_t getKeyOperation();
        bool getKeyOperation(uint8_t);
};








#endif