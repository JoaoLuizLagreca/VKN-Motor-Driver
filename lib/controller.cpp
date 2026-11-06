#include <stdio.h>
#include <stdlib.h>
#include "controller.hpp"
#include "driver.hpp"
#include "servermacros.h"
#include <cmath>
#include <numbers>

namespace motor {
    controller::controller(motor::driver motor_drv) {
        speed = 255;
        angle = 0;
        mvt_mode = MVT_MODE_IDLE;
        this->motor_drv = motor_drv;

        update();
    }

    void controller::update_wheels(){
        float drift = cos(angle * std::numbers::pi / 180);
        uint8_t spa, spb;
        if(angle>0) {
            spb = speed * drift;
            spa = speed;
        } else {
            spa = speed * drift;
            spb = speed;
        }

        motor_drv.setWheelSpeed(MOTOR_A, spa);
        motor_drv.setWheelSpeed(MOTOR_B, spb);
    }

    void controller::update(){
        switch(mvt_mode){
            case MVT_MODE_IDLE:
                motor_drv.setWheelMode(MOTOR_A, WHEEL_MODE_IDLE);
                motor_drv.setWheelMode(MOTOR_B, WHEEL_MODE_IDLE);
            break;
            case MVT_MODE_STOP:
                motor_drv.setWheelMode(MOTOR_A, WHEEL_MODE_STOP);
                motor_drv.setWheelMode(MOTOR_B, WHEEL_MODE_STOP);
            break;
            case MVT_MODE_FWRD:
                motor_drv.setWheelMode(MOTOR_A, WHEEL_MODE_FORW);
                motor_drv.setWheelMode(MOTOR_B, WHEEL_MODE_FORW);
                update_wheels();
            break;
            case MVT_MODE_BACK:
                motor_drv.setWheelMode(MOTOR_A, WHEEL_MODE_BACK);
                motor_drv.setWheelMode(MOTOR_B, WHEEL_MODE_BACK);
                update_wheels();
            break;
            case MVT_MODE_CLKW:
                motor_drv.setWheelMode(MOTOR_A, WHEEL_MODE_FORW);
                motor_drv.setWheelMode(MOTOR_B, WHEEL_MODE_BACK);
                motor_drv.setWheelSpeed(MOTOR_A, speed);
                motor_drv.setWheelSpeed(MOTOR_B, speed);
            break;
            case MVT_MODE_CCKW:
                motor_drv.setWheelMode(MOTOR_A, WHEEL_MODE_BACK);
                motor_drv.setWheelMode(MOTOR_B, WHEEL_MODE_FORW);
                motor_drv.setWheelSpeed(MOTOR_A, speed);
                motor_drv.setWheelSpeed(MOTOR_B, speed);
            break;
        }
    }
}
