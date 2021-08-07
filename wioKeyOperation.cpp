#include "wioKeyOperation.hpp"

void KeyOperation::keyDown(void){
    tmpKeyCode = 0;
    if(digitalRead(WIO_KEY_A) == LOW){
        //Serial.println("inst A");
        tmpKeyCode = WIO_3S_SHORTA;
    }
    if(digitalRead(WIO_KEY_B) == LOW){
        //Serial.println("inst B");
        tmpKeyCode = WIO_3S_SHORTB;
    }
    if(digitalRead(WIO_KEY_C) == LOW){
        //Serial.println("inst C");
        tmpKeyCode = WIO_3S_SHORTC;
    }
    if(digitalRead(WIO_5S_PRESS) == LOW){
        //Serial.println("inst ・");
        tmpKeyCode = WIO_5S_SHORTPRESS;
    }
    if(digitalRead(WIO_5S_UP) == LOW){
        //Serial.println("inst ↑");
        tmpKeyCode = WIO_5S_SHORTUP;
    }
    if(digitalRead(WIO_5S_DOWN) == LOW){
        //Serial.println("inst ↓");
        tmpKeyCode = WIO_5S_SHORTDOWN;
    }
    if(digitalRead(WIO_5S_LEFT) == LOW){
        //Serial.println("inst ←");
        tmpKeyCode = WIO_5S_SHORTLEFT;
    }
    if(digitalRead(WIO_5S_RIGHT) == LOW){
        //Serial.println("inst →");
        tmpKeyCode = WIO_5S_SHORTRIGHT;
    }

    if(tmpKeyCode == 0){
        return;
    }

    //attachInterrupt(WIO_KEY_A,    keyUp, RISING);
    attachInterrupt(WIO_KEY_B,    keyUp, RISING);
    attachInterrupt(WIO_KEY_C,    keyUp, RISING);
    attachInterrupt(WIO_5S_PRESS, keyUp, RISING);
    attachInterrupt(WIO_5S_UP,    keyUp, RISING);
    attachInterrupt(WIO_5S_DOWN,  keyUp, RISING);
    attachInterrupt(WIO_5S_LEFT,  keyUp, RISING);
    attachInterrupt(WIO_5S_RIGHT, keyUp, RISING);

    keyDownTime = micros();
}

void KeyOperation::keyUp(void){
    if(micros() - keyDownTime < 800000){
        keyCode = tmpKeyCode;
    }else{
        keyCode = tmpKeyCode + 0x10;
    }

    //attachInterrupt(WIO_KEY_A, keyDown, FALLING);
    attachInterrupt(WIO_KEY_B, keyDown, FALLING);
    attachInterrupt(WIO_KEY_C, keyDown, FALLING);
    attachInterrupt(WIO_5S_PRESS, keyDown, FALLING);
    attachInterrupt(WIO_5S_UP,    keyDown, FALLING);
    attachInterrupt(WIO_5S_DOWN,  keyDown, FALLING);
    attachInterrupt(WIO_5S_LEFT,  keyDown, FALLING);
    attachInterrupt(WIO_5S_RIGHT, keyDown, FALLING);
}

uint8_t KeyOperation::getKeyOperation(void){
    uint8_t ret = keyCode;
    keyCode = 0;

    if(micros() - keyDownTime > 5000000){
        return 0;
    }
    return ret;
}


bool KeyOperation::getKeyOperation(uint8_t key){
    if(keyCode != key)
    {
        return false;
    }

    if(micros() - keyDownTime > 5000000){
        return false;
    }
    keyCode = 0;
    return true;
}