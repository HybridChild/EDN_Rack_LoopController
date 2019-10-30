
#ifndef MAIN_H_
#define MAIN_H_

#define SWITCH_INDICATOR_ADDR	0b000
#define DIGIT_0_1_ADDR			0b001
#define DIGIT_2_3_ADDR			0b010
#define TUNER_DISPLAY_ADDR		0b100

enum SystemState {PRESET_CTRL = 0, LOOP_CTRL = 1, TUNER = 2};

volatile unsigned char StateOfSystem;

#endif /* MAIN_H_ */