#pragma once

#include "driver.hpp"
#include <stdint.h>

#define MVT_MODE_IDLE 0x00
#define MVT_MODE_STOP 0x01
#define MVT_MODE_FWRD 0x02
#define MVT_MODE_BACK 0x03
#define MVT_MODE_CLKW 0x04
#define MVT_MODE_CCKW 0x05


namespace motor {
    class controller {

        private:
            uint8_t speed;
            int8_t angle;
            uint8_t mvt_mode;
            motor::driver motor_drv;
            void update_wheels();
        public:
            controller(motor::driver motor_drv);
            void setMovementMode(uint8_t mvt_mode);
            uint8_t getMovementMode();
            void setAngle(int8_t angle);
            int8_t getAngle();
            void setSpeed(uint8_t speed);
            uint8_t getSpeed();
            void update();

    };
}
