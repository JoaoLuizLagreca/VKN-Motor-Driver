#include "libmotor.hpp"
#include <stdio.h>
#include <iostream>
#include <stdexcept>
#include <gpiod.h>
using namespace std;

#define	GPIO_B     "gpiochip1"
#define	GPIO_C     "gpiochip2"
/* GPIO Chip B */
#define LINE_ENAA  8
#define LINE_ENAB  10
#define NAME_ENAA  "Motor A Enable"
#define NAME_ENAB  "Motor B Enable"
/* GPIO Chip C*/
#define LINE_MOTA1 0
#define LINE_MOTA2 1
#define LINE_MOTB1 4
#define LINE_MOTB2 24
#define NAME_MOTA1 "Motor A Input 1"
#define NAME_MOTA2 "Motor A Input 2"
#define NAME_MOTB1 "Motor B Input 1"
#define NAME_MOTB2 "Motor B Input 2"

namespace motor {
	/* FIXME: Por razòes de segurança, as GPIOs não podem ser inicializadas no construtor */
	driver::driver() {
		speed_a = 255;
		speed_b = 255;
		
		return;
	}
	
	void driver::init() {
		int ret1, ret2, ret3;
		
		chip_b = gpiod_chip_open_by_name(GPIO_B);
		chip_c = gpiod_chip_open_by_name(GPIO_C);
			
		if (!chip_b || !chip_c) {
			free();
			throw std::runtime_error("Error initializing GPIO chips");
		}
		
		setup_mot1:
			mot1_in1 = gpiod_chip_get_line(chip_c, LINE_MOTA1);
			mot1_in2 = gpiod_chip_get_line(chip_c, LINE_MOTA2);
			ena_1    = gpiod_chip_get_line(chip_b, LINE_ENAA);
			if(!mot1_in1 || !mot1_in2 || !ena_1) {
				goto error_mot1;
			}
			ret1 = gpiod_line_request_output(mot1_in1, NAME_MOTA1, 0);
			ret2 = gpiod_line_request_output(mot1_in2, NAME_MOTA2, 0);
			ret3 = gpiod_line_request_output(ena_1,    NAME_ENAA, 0);
			if(ret1 != 0 || ret2 != 0 || ret3 != 0){
				goto error_mot1;
			}
			goto setup_mot2;
		error_mot1:
			free();
			throw std::runtime_error("Error initializing Motor A");
		
		setup_mot2:
			mot2_in1 = gpiod_chip_get_line(chip_c, LINE_MOTB1);
			mot2_in2 = gpiod_chip_get_line(chip_c, LINE_MOTB2);
			ena_2    = gpiod_chip_get_line(chip_b, LINE_ENAB);
			if(!mot2_in1 || !mot2_in2) {
				goto error_mot2;
			}
			ret1 = gpiod_line_request_output(mot2_in1, NAME_MOTB1, 0);
			ret2 = gpiod_line_request_output(mot2_in2, NAME_MOTB2, 0);
			ret3 = gpiod_line_request_output(ena_2,    NAME_ENAB, 0);
			if(ret1 != 0 || ret2 != 0 || ret3 != 0){
				goto error_mot2;
			}
			goto end_setup;
		error_mot2:
			free();
			throw std::runtime_error("Error initializing Motor B");
		end_setup:
			return;
	}
			
	void driver::free() {
		/* FIXME: Você não deve correr o risco de liberar ponteiros que não existem */
		gpiod_line_release(mot1_in1);
		gpiod_line_release(mot1_in2);
		gpiod_line_release(ena_1);
		gpiod_line_release(mot2_in1);
		gpiod_line_release(mot2_in2);
		gpiod_line_release(ena_2);
		gpiod_chip_close(chip_b);
		gpiod_chip_close(chip_c);
	}
	
	/* TODO: Movimentos devem alternar pela velocidade das rodas e não diretamente */
	
	void driver::forward() {
		printf("Moving forward\n");
		/* FIXME: Montar recuperação de erro */
		gpiod_line_set_value(mot1_in1, 1);
		gpiod_line_set_value(mot1_in2, 0);
		gpiod_line_set_value(mot2_in1, 1);
		gpiod_line_set_value(mot2_in2, 0);
	}
		
	void driver::backward() {
		printf("Moving backward\n");
		/* FIXME: Montar recuperação de erro */
		gpiod_line_set_value(mot1_in1, 0);
		gpiod_line_set_value(mot1_in2, 1);
		gpiod_line_set_value(mot2_in1, 0);
		gpiod_line_set_value(mot2_in2, 1);
	}
	
	void driver::clockwise() {
		printf("Turning clocwise\n");
		/* FIXME: Montar recuperação de erro */
		gpiod_line_set_value(mot1_in1, 1);
		gpiod_line_set_value(mot1_in2, 0);
		gpiod_line_set_value(mot2_in1, 0);
		gpiod_line_set_value(mot2_in2, 1);
	}
	
	void driver::counterclockwise(){
		printf("Turning counterclockwise\n");
		/* FIXME: Montar recuperação de erro */
		gpiod_line_set_value(mot1_in1, 0);
		gpiod_line_set_value(mot1_in2, 1);
		gpiod_line_set_value(mot2_in1, 1);
		gpiod_line_set_value(mot2_in2, 0);
	}
		
	void driver::stop() {
		printf("Breaking\n");
		/* FIXME: Montar recuperação de erro */
		gpiod_line_set_value(mot1_in1, 1);
		gpiod_line_set_value(mot1_in2, 1);
		gpiod_line_set_value(mot2_in1, 1);
		gpiod_line_set_value(mot2_in2, 1);
	}
			
	void driver::idle() {
		printf("Becomming idle\n");
		/* FIXME: Montar recuperação de erro */
		gpiod_line_set_value(mot1_in1, 0);
		gpiod_line_set_value(mot1_in2, 0);
		gpiod_line_set_value(mot2_in1, 0);
		gpiod_line_set_value(mot2_in2, 0);
	}
	
	void driver::setSpeed(int motor_id, unsigned char speed) {
		switch(motor_id) {
			case MOTOR_A:
				printf("Changing motor A speed to %d\n", speed);
				speed_a = speed;
			break;
			
			case MOTOR_B:
				printf("Changing motor B speed to %d\n", speed);
				speed_b = speed;
			break;
			
			default:
				throw std::runtime_error("Unindentified motor");
		}
	}
	
	unsigned char driver::getSpeed(int motor_id) {
		switch(motor_id) {
			case MOTOR_A:
				return speed_a;
			break;
			
			case MOTOR_B:
				return speed_b;
			break;
			
			default:
				throw std::runtime_error("Unindentified motor");
		}
	}
	  
}

