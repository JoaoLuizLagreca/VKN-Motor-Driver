#include <gpiod.h>

#define MOTOR_A 0x00
#define MOTOR_B 0x01

#define WHEEL_MODE_IDLE  0x00
#define WHEEL_MODE_STOP  0x01 
#define WHEEL_MODE_FORW  0x02
#define WHEEL_MODE_BACK  0x03

namespace motor {
	class driver {
		
		private:
			struct gpiod_chip *chip_b, *chip_c;
			struct gpiod_line *mot1_in1, *mot1_in2, *ena_1,
							  *mot2_in1, *mot2_in2, *ena_2;
			unsigned char w_speed[2], w_mode[2];
			
		public:
			driver();
			
			void init();
			void free();
			void setWheelMode(int motor_id, int mode);
			unsigned char getWheelMode(int motor_id);
			void setWheelSpeed(int motor_id, unsigned char speed);
			unsigned char getWheelSpeed(int motor_id);
		  
	};
}
