#include <gpiod.h>

namespace motor {
	class driver {
		
		private:
			struct gpiod_chip *chip_c;
			struct gpiod_line *mot1_in1, *mot1_in2,
							  *mot2_in1, *mot2_in2;
			
		public:
			driver();
			
			void free();
			void forward();
			void backward();
			void stop();
			void idle();
		  
	};
}
