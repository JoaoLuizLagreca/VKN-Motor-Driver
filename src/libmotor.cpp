#include "libmotor.hpp"
#include <stdio.h>
#include <iostream>
#include <stdexcept>
#include <gpiod.h>
using namespace std;

#define	GPIO_C     "gpiochip2"
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
		return;
	}
	
	void driver::init() {
		int ret1, ret2;
		
		chip_c = gpiod_chip_open_by_name(GPIO_C);
			
		if (!chip_c) {
			free();
			throw std::runtime_error("Error initializing GPIO chips");
		}
		
		setup_mot1:
			mot1_in1 = gpiod_chip_get_line(chip_c, LINE_MOTA1);
			mot1_in2 = gpiod_chip_get_line(chip_c, LINE_MOTA2);
			if(!mot1_in1 || !mot1_in2) {
				goto error_mot1;
			}
			ret1 = gpiod_line_request_output(mot1_in1, NAME_MOTA1, 0);
			ret2 = gpiod_line_request_output(mot1_in2, NAME_MOTA2, 0);
			if(ret1 != 0 || ret2 != 0){
				goto error_mot1;
			}
			goto setup_mot2;
		error_mot1:
			free();
			throw std::runtime_error("Error initializing Motor A");
		
		setup_mot2:
			mot2_in1 = gpiod_chip_get_line(chip_c, LINE_MOTB1);
			mot2_in2 = gpiod_chip_get_line(chip_c, LINE_MOTB2);
			if(!mot2_in1 || !mot2_in2) {
				goto error_mot2;
			}
			ret1 = gpiod_line_request_output(mot2_in1, NAME_MOTB1, 0);
			ret2 = gpiod_line_request_output(mot2_in2, NAME_MOTB2, 0);
			if(ret1 != 0 || ret2 != 0){
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
		gpiod_line_release(mot2_in1);
		gpiod_line_release(mot2_in2);
		gpiod_chip_close(chip_c);
	}
	
	void driver::forward() {
		/* FIXME: Montar recuperação de erro */
		gpiod_line_set_value(mot1_in1, 1);
		gpiod_line_set_value(mot1_in2, 0);
		gpiod_line_set_value(mot2_in1, 1);
		gpiod_line_set_value(mot2_in2, 0);
	}
		
	void driver::backward() {
		/* FIXME: Montar recuperação de erro */
		gpiod_line_set_value(mot1_in1, 0);
		gpiod_line_set_value(mot1_in2, 1);
		gpiod_line_set_value(mot2_in1, 0);
		gpiod_line_set_value(mot2_in2, 1);
	}
		
	void driver::stop() {
		/* FIXME: Montar recuperação de erro */
		gpiod_line_set_value(mot1_in1, 1);
		gpiod_line_set_value(mot1_in2, 1);
		gpiod_line_set_value(mot2_in1, 1);
		gpiod_line_set_value(mot2_in2, 1);
	}
			
	void driver::idle() {
		/* FIXME: Montar recuperação de erro */
		gpiod_line_set_value(mot1_in1, 0);
		gpiod_line_set_value(mot1_in2, 0);
		gpiod_line_set_value(mot2_in1, 0);
		gpiod_line_set_value(mot2_in2, 0);
	}
	  
}

