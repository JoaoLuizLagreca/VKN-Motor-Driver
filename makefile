CC := g++
LIBS_DRV := -l gpiod
LIBS_TOOLS := 
OUTPUT := build


all: driver tools

driver:
	- mkdir -p ${OUTPUT} 2> /dev/null
	${CC} src/*.cpp -I include/ ${LIBS_DRV} -o ${OUTPUT}/motor_driver

tools:
	- mkdir -p ${OUTPUT} 2> /dev/null
	${CC} src/tools/testemotor.cpp -I include/ ${LIBS_TOOLS} -o ${OUTPUT}/teste

clean:
	rm -rf ${OUTPUT}
