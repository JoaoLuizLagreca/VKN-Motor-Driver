#include <gpiod.h>

#define MOTOR_A 0x00
#define MOTOR_B 0x01


namespace motor {
	class driver {
		
		private:
			struct gpiod_chip *chip_b, *chip_c;
			struct gpiod_line *mot1_in1, *mot1_in2, *ena_1,
							  *mot2_in1, *mot2_in2, *ena_2;
			unsigned char speed_a, speed_b;
			
		public:
			driver();
			
			void init();
			void free();
			void forward();
			void backward();
			void clockwise();
			void counterclockwise();
			void stop();
			void idle();
			void setSpeed(int motor, unsigned char speed);
			unsigned char getSpeed(int motor);
		  
	};
}
