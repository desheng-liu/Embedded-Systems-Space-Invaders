// SpaceInvaders.c
// Runs on TM4C123
// Jonathan Valvano and Daniel Valvano
// This is a starter project for the EE319K Lab 10

// Last Modified: 1/12/2022 
// http://www.spaceinvaders.de/
// sounds at http://www.classicgaming.cc/classics/spaceinvaders/sounds.php
// http://www.classicgaming.cc/classics/spaceinvaders/playguide.php

// ******* Possible Hardware I/O connections*******************
// Slide pot pin 1 connected to ground
// Slide pot pin 2 connected to PD2/AIN5
// Slide pot pin 3 connected to +3.3V 
// buttons connected to PE0-PE3

// 32*R resistor DAC bit 0 on PB0 (least significant bit)
// 16*R resistor DAC bit 1 on PB1
// 8*R resistor DAC bit 2 on PB2 
// 4*R resistor DAC bit 3 on PB3
// 2*R resistor DAC bit 4 on PB4
// 1*R resistor DAC bit 5 on PB5 (most significant bit)

// LED on PD1
// LED on PD0


#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "ST7735.h"
#include "Print.h"
#include "Random.h"
#include "TExaS.h"
#include "ADC.h"
#include "Images.h"
#include "Sound.h"
#include "Timer1.h"


//images 
const unsigned short mainship[] = {
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xDEDB, 0xEF7D, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x632C, 0xAD75, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xB5B6, 0xAD55, 0x9CF3, 0xDEDB, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0xAF52, 0xDE9B, 0x9492, 0x9492, 0xB5B6, 0xD6BA, 0xEF5D, 0xDEFB, 0xDEFB, 0x9CF3, 0x94B2, 0x8C71, 0xF7DE, 0x9C5A, 0x0000, 0x0000,
 0x0000, 0xE73C, 0xE73C, 0xE73C, 0xEF5D, 0xBDD7, 0xE71C, 0xA514, 0xEF5D, 0xE73C, 0xE73C, 0xEF5D, 0xCE79, 0x0000, 0x0000, 0x0000,
 0x0000, 0xC638, 0xB596, 0x94B2, 0xEF7D, 0xD6BA, 0xAD75, 0xA534, 0xEF7D, 0xD69A, 0x94B2, 0xCE59, 0xC638, 0x0000, 0x0000, 0x0000,
 0x0000, 0xE73C, 0xCE79, 0xE73C, 0xBDF7, 0xC618, 0xDEFB, 0xEF5D, 0xA514, 0xDEFB, 0xDEFB, 0xCE99, 0xF7BE, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0xCE3D, 0xD69A, 0xD69A, 0xB596, 0xD69A, 0xB596, 0xD69A, 0xC618, 0xEF7C, 0x8BF9, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0xDEBD, 0x0000, 0x0000, 0xD69A, 0xE71C, 0xEF5D, 0xCE59, 0x0000, 0x0000, 0xB53B, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xC659, 0x6BF0, 0x7C51, 0xDEFB, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xB4F2, 0xCA23, 0xC1E2, 0xBE9A, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0xC618, 0xFFFF, 0xEBCA, 0xE1E0, 0xE9E0, 0xEDD4, 0xE77D, 0xE71C, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x2124, 0xEEFA, 0xFAE4, 0xFB88, 0xB618, 0x632C, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xF7FF, 0xDE38, 0xD699, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xAD55, 0xD6BA, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,

};



const unsigned short enemy[] = {
	
	
	
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x1091, 0x1091, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x1091, 0x1091, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x1091, 0x1091, 0x0000, 0x1091, 0x1091, 0x0000, 0x1091, 0x1091, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x1091, 0x0000, 0x1091, 0x1091, 0x0000, 0x1091, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x1091, 0x1091, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x1091, 0x1091, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};






const unsigned short missile[] = {
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x97DF, 0x0000, 0x0000, 0x0000, 0x0000, 0x27BF, 0x0000, 0x0000, 0x0000,
 0x0000, 0x4FBF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,

};
const unsigned short golden_trophy[] = {
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0167, 0x0146, 0x0146, 0x0167, 0x0188, 0x0146, 0x0126, 0x29A6, 0x39C7, 0x39C7, 0x31A7, 0x0126, 0x0146,
 0x0146, 0x0146, 0x0146, 0x0146, 0x0167, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0D9B, 0x0D5A, 0x0D5A, 0x02EE, 0x01E9,
 0x0CF8, 0x053A, 0xAE7B, 0xE6FB, 0xE6FB, 0xC6BB, 0x0539, 0x0D59, 0x0D5A, 0x0D5A, 0x0D59, 0x0D59, 0x0DDC, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x73AE, 0x0000,
 0x0357, 0x869F, 0x0C3C, 0x022F, 0x0AF3, 0x0BFA, 0x0B56, 0x0C1A, 0x0000, 0x5AAA, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x8C51, 0x0000, 0x3C79, 0xFFFF, 0x03FB, 0x0C1B, 0x0C1A, 0x0C1B, 0x0000,
 0x632C, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x4A28, 0x0000, 0xCF9F, 0x7DDC, 0x03B9, 0x0C7D, 0x0063, 0x2904, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x2312, 0xCF5F, 0x03BA, 0x0B35, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0xC75F, 0x03FC, 0x00A5, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x6DBD, 0x043C, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x1B34, 0x0B97, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x18C2, 0x0000, 0x0000, 0x0000, 0x024F, 0x0AD2, 0x0000, 0x0000, 0x0000,
 0x1060, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0108,
 0x0378, 0x65FF, 0x249D, 0x0C1B, 0x0C1B, 0x0C5C, 0x0C7D, 0x0BD9, 0x018A, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x02F6, 0x8E5F, 0xE75F, 0x13D9, 0x03D9, 0x0BFA, 0x0BFA, 0x0BFA, 0x0BB9, 0x0BBA,
 0x0C1C, 0x0BB9, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0317, 0xEFBF, 0xBF9F,
 0x055C, 0x0E5E, 0x0E7E, 0x0E5E, 0x0E5E, 0x0E7E, 0x0E7E, 0x0DFD, 0x0D1C, 0x0BFA, 0x0BDA, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x018D, 0xDF9F, 0xBFBF, 0x05DD, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E,
 0x0E5E, 0x0E5E, 0x0CBC, 0x0292, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x761F, 0xFFFF, 0x05DD, 0x0E1E,
 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E7E, 0x0D1E, 0x0042, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x024F,
 0x0C5D, 0x151F, 0x0000, 0x022F, 0xFFFF, 0x0E3E, 0x0E1E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E,
 0x0E3E, 0x0E3E, 0x0E3E, 0x0E9F, 0x0B13, 0x0000, 0x14DF, 0x0C9E, 0x0291, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x00E6, 0x0CDF, 0x0B97, 0x00A5, 0x0000, 0x0000, 0x8EFF, 0x9F5F, 0x05FD, 0x0E3E, 0x0E3E,
 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x167F, 0x0000, 0x0000, 0x0064,
 0x0000, 0x14FF, 0x016A, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x00E7, 0x157F, 0x01A9, 0x0000,
 0x0000, 0x0000, 0x00C5, 0xFFFF, 0x05FD, 0x0E1E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E,
 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x16BF, 0x020A, 0x2903, 0x0000, 0x0000, 0x0126, 0x157F, 0x018A, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x159F, 0x0209, 0x0000, 0x0000, 0x0000, 0x0820, 0x1BB1, 0xA79F, 0x05FE, 0x0E3E, 0x0E3E, 0x0E3E,
 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x169F, 0x02AD, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0125, 0x15FF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0B13, 0x0D59, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x863A, 0x1E5E, 0x0E1E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E,
 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E7F, 0x032F, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0C75, 0x0BF7, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x171F, 0x0126, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x6F7F, 0x061E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E,
 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E5F, 0x0C96, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x177F, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x01A8, 0x177F, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x2EFF, 0x0E1E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E,
 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E1E, 0x0EFF, 0x0083, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x171F, 0x028C, 0x1041, 0x0000,
 0x0000, 0x0000, 0x0391, 0x0EDF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0146, 0x0EBF, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E,
 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x061E, 0x5F3F, 0x01E9, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0DFD, 0x0475, 0x0000, 0x0000, 0x0000, 0x0000, 0x3D99, 0x4F7F, 0x4E1B, 0x4E3C, 0x4E3C, 0x4E9E,
 0x11E9, 0x0105, 0x16BF, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E,
 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x061E, 0x9F9F, 0x026C, 0x0905, 0x4EBE, 0x4E3C, 0x4E3C, 0x4E1B, 0x4F5F, 0x467D, 0x0000, 0x0000,
 0x0000, 0x0000, 0x20A2, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0820, 0x0000, 0x16BF, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E,
 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x05FE, 0xBFDF, 0x0330, 0x0000,
 0x3124, 0x2903, 0x2903, 0x2903, 0x28E3, 0x28C2, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x1041, 0x02CE, 0x169F, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E,
 0x0E3E, 0x0E3E, 0x0E3E, 0x0E3E, 0x05FE, 0xB7BF, 0x0392, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x2903, 0x032F, 0x16BF, 0x0E7E, 0x0E7E, 0x0E7E, 0x0E7E, 0x0E7E,
 0x0E7E, 0x0E7E, 0x0E7E, 0x0E7E, 0x0E7E, 0x0E7E, 0x0E7E, 0x0E7E, 0x0E7E, 0x0E7E, 0x0E7E, 0x0E7E, 0x065E, 0x97BF, 0x03F3, 0x0800,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x2903, 0x01ED, 0x0C5C, 0x0C1B, 0x0C1B, 0x0C1B, 0x0C1B, 0x0C1B, 0x0C1B, 0x0C1B, 0x0C1B, 0x0C1B, 0x0C1B, 0x0C1B, 0x0C1B, 0x0C1B,
 0x0C1B, 0x0C1B, 0x0C1B, 0x0C1B, 0x0BFB, 0x3D1D, 0x0271, 0x0800, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x18A2, 0x0128, 0x0AD2, 0x0AB1, 0x0AB1, 0x0AB1, 0x0AB1, 0x0AB1,
 0x0AB1, 0x0AB1, 0x0AB1, 0x0AB1, 0x0AB1, 0x0AB1, 0x0AB1, 0x0AB2, 0x0AB2, 0x0AB2, 0x0AB2, 0x0AB2, 0x0AB2, 0x02B2, 0x018B, 0x0800,
};

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void spriteMovement(void);
void bulletCollission(void);
void movement(void);
void Delay100ms(uint32_t count); // time delay in 0.1 seconds
int english = 0;
int spanish = 0;
int gamestatus = 0;
int score = 0;
int clearing = 0;

struct EnemyInvader {
    int8_t x;
    int y;
    int8_t xold; //only for mainship
    uint8_t yold;
    int8_t dy;
    int w,h;
    const uint16_t *image;
    uint8_t life;
};

struct Bullet {
	uint32_t x;
	int y;
	uint8_t dy;
	const uint16_t *image;
	uint8_t w,h, yold, life;
	//x direction should be the same as the ship
	//y direction should start at the tip of the ship
	//dx should be 0, since not moving to the left or right 
	//dy should be constant speed
};
typedef struct EnemyInvader p_type;
p_type enemy1 = {20, 20, 0, 20, 1,16, 16, enemy, 1};
p_type enemy2 = {40, 20, 0, 20, 1, 16, 16, enemy, 1};
p_type enemy3 = {60, 20, 0, 20, 1, 16, 16, enemy, 1};
p_type enemy4 = {80, 20, 0, 20, 1,16, 16, enemy, 1};
p_type enemy5 = {100, 20, 0, 20, 1, 16, 16, enemy, 1};
p_type ship = {20, 140, 20, 140, 0, 16, 16, mainship, 1};
p_type trophy = {0, 0, 0, 0, 0, 0, 0, golden_trophy, 0};
typedef struct Bullet t_type;
t_type bullet = {0, 140, 8, missile, 5, 5, 0, 0};

void SysTick_Init(uint32_t period){
  //now init the systick timer
	NVIC_ST_CTRL_R = 0;               // disable SysTick during setup
	NVIC_ST_RELOAD_R = period-1;
	NVIC_ST_CURRENT_R = 0;
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0xEFFFFFFF) | 0x40000000; // priority 2
	NVIC_ST_CTRL_R = 0x07;      // enable SysTick with core clock	
}

void SysTick_Handler(void){
	spriteMovement();
}
uint32_t pastpause = 0;
uint8_t pausestatus;
uint32_t pause, fire, bulletlife = 0;
void spriteMovement(void){
	fire = GPIO_PORTE_DATA_R & 0x01;
	pastpause = pause;
	pause = GPIO_PORTE_DATA_R & 0x02;
	
	if(pause == 2 && pastpause == 0)
	{
		pausestatus ^= 0x01;
	}

	if(pausestatus == 0){
	
	if(fire == 1 && bullet.life == 0){
		bullet.x = ship.x;
		bullet.y = 132;
		bulletlife = 1;
		bullet.life = 1;
		Sound_Shoot();
	}
	
	if(bullet.y <= 15){
		bullet.life = 0;
		ST7735_FillRect(bullet.x+4, 5, bullet.w, 10, ST7735_BLACK);
	}
	else{
		bullet.yold = bullet.y;
		bullet.y -= bullet.dy;
	}
	
	if(enemy1.life == 1){
		enemy1.yold = enemy1.y;
		enemy1.y += enemy1.dy;
		if(enemy1.y+ship.h > ship.y){
			gamestatus = 0;
		}
	}
	if(enemy1.life == 0){
		//Random_Init(100);
		enemy1.x = 20;
		enemy1.y = 20;
		enemy1.dy = 1;
		enemy1.life = 1;
	}
	if(enemy2.life == 1){
		enemy2.yold = enemy2.y;
		enemy2.y += enemy2.dy;
		if(enemy2.y+ship.h > ship.y){
			gamestatus = 0;
		}
	}
	if(enemy2.life == 0){
		//Random_Init(100);
		enemy2.x = 40;
		enemy2.y = 20;
		enemy2.dy = 1;
		enemy2.life = 1;
	}
	if(enemy3.life == 1){
		enemy3.yold = enemy3.y;
		enemy3.y += enemy3.dy;
		if(enemy3.y+ship.h > ship.y){
			gamestatus = 0;
		}
	}
	if(enemy3.life == 0){
		//Random_Init(100);
		enemy3.x = 60;
		enemy3.y = 20;
		enemy3.dy = 1;
		enemy3.life = 1;
	}	
	
		if(enemy4.life == 1){
		enemy4.yold = enemy4.y;
		enemy4.y += enemy4.dy;
		if(enemy4.y+ship.h > ship.y){
			gamestatus = 0;
		}
	}
	if(enemy4.life == 0){
		//Random_Init(100);
		enemy4.x = 80;
		enemy4.y = 20;
		enemy4.dy = 1;
		enemy4.life = 1;
	}
			if(enemy5.life == 1){
		enemy5.yold = enemy5.y;
		enemy5.y += enemy5.dy;
		if(enemy5.y+ship.h > ship.y){
			gamestatus = 0;
		}
	}
	if(enemy5.life == 0){
		//Random_Init(100);
		enemy5.x = 100;
		enemy5.y = 20;
		enemy5.dy = 1;
		enemy5.life = 1;
	}
	ship.xold = ship.x;
	
	
	int value = ADC_In();
	value = ((120*ADC_In())/4095);
	if(value<40){//converts adv values to x coord movements for spaceship
		ship.x = 40;
	}		
	if(value>110){
		ship.x = 110;
	}
	else{
		ship.x = value;
	}
}
}

//port initalization method 
void portInit(void){ volatile int delay;
	SYSCTL_RCGCGPIO_R |= 0x10;  //turn on clock port e
	
	delay = SYSCTL_RCGCGPIO_R;
	delay = SYSCTL_RCGCGPIO_R; //delays
	
	
	GPIO_PORTE_DEN_R |= 0x03; //turns on pin 0, 1
	GPIO_PORTE_DIR_R &= ~0x03; //makes pin 0 and 1 an input (bic)
}
int main1(void){
  DisableInterrupts();
  TExaS_Init(NONE);       // Bus clock is 80 MHz 
  Random_Init(1);

  Output_Init();
  ST7735_FillScreen(0x0000);            // set screen to black
  Delay100ms(50); //
  ST7735_DrawBitmap(22, 159, PlayerShip0, 18,8); // player ship bottom
  ST7735_DrawBitmap(53, 151, Bunker0, 18,5);
  ST7735_DrawBitmap(42, 159, PlayerShip1, 18,8); // player ship bottom
  ST7735_DrawBitmap(62, 159, PlayerShip2, 18,8); // player ship bottom
  ST7735_DrawBitmap(82, 159, PlayerShip3, 18,8); // player ship bottom

  ST7735_DrawBitmap(0, 9, SmallEnemy10pointA, 16,10);
  ST7735_DrawBitmap(20,9, SmallEnemy10pointB, 16,10);
  ST7735_DrawBitmap(40, 9, SmallEnemy20pointA, 16,10);
  ST7735_DrawBitmap(60, 9, SmallEnemy20pointB, 16,10);
  ST7735_DrawBitmap(80, 9, SmallEnemy30pointA, 16,10);
  ST7735_DrawBitmap(100, 9, SmallEnemy30pointB, 16,10);

  Delay100ms(50);              // delay 5 sec at 80 MHz

  ST7735_FillScreen(0x0000);   // set screen to black
  ST7735_SetCursor(1, 1);
  ST7735_OutString("GAME OVER");
  ST7735_SetCursor(1, 2);
  ST7735_OutString("Nice try,");
  ST7735_SetCursor(1, 3);
  ST7735_OutString("Earthling!");
  ST7735_SetCursor(2, 4);
  LCD_OutDec(1234);
  while(1){
  }

}


// You can't use this timer, it is here for starter code only 
// you must use interrupts to perform delays
void Delay100ms(uint32_t count){uint32_t volatile time;
  while(count>0){
    time = 727240;  // 0.1sec at 80 MHz
    while(time){
      time--;
    }
    count--;
  }
}
typedef enum {English, Spanish} Language_t;
Language_t myLanguage=English;
typedef enum {SpaceInvaders, SwitchQuestion1, SwitchQuestion2, end1, end2} phrase_t;
const char SpaceInvaders_English[] = "Space Invaders";
const char SpaceInvaders_Spanish[] = "Invasores Espaciales";
const char SwitchQuestion1_English[] ="Press SW1:";
const char SwitchQuestion2_English[] ="for English";
const char SwitchQuestion1_Spanish[] ="Presione SW2:";
const char SwitchQuestion2_Spanish[] ="para Espa\xA4ol";
const char end1_English[] ="Game Over!";
const char end2_English[] ="Score:";
const char end1_Spanish[] ="Buen Intento!";
const char end2_Spanish[] ="Puntaje:";
//const char PlayAgain_English[] = "Play Again?";
//const char PlayAgain_Spanish[] = "�juega de nuevo?";
const char *Phrases[5][2]={
	{SpaceInvaders_English, SpaceInvaders_Spanish},
  {SwitchQuestion1_English, SwitchQuestion1_Spanish}, 
	{SwitchQuestion2_English, SwitchQuestion2_Spanish},
	{end1_English, end1_Spanish},
	{end2_English, end2_Spanish}
};


void displayBeginning(){
		ST7735_SetCursor(0,0);
		ST7735_OutString((char *)Phrases[SpaceInvaders][English]);
		ST7735_SetCursor(0,1);
		ST7735_OutString((char *)Phrases[SpaceInvaders][Spanish]);
	
		ST7735_DrawBitmap(50, 60, ship.image, ship.w, ship.h );
		ST7735_DrawBitmap(49, 40, enemy1.image, enemy1.w, enemy1.h );
	
		
		ST7735_SetCursor(0,8);
		ST7735_OutString((char *)Phrases[SwitchQuestion1][English]);
		ST7735_SetCursor(0,9);
		ST7735_OutString((char *)Phrases[SwitchQuestion2][English]);
	
		ST7735_SetCursor(0,11);
		ST7735_OutString((char *)Phrases[SwitchQuestion1][Spanish]);
		ST7735_SetCursor(0,12);
		ST7735_OutString((char *)Phrases[SwitchQuestion2][Spanish]);
}

void movement(){
				if(enemy1.life == 1){
					ST7735_FillRect(enemy1.x, enemy1.yold-enemy1.h, enemy1.w, enemy1.h, ST7735_BLACK); //erasing overwritten pixels
					ST7735_DrawBitmap(enemy1.x, enemy1.y, enemy1.image, enemy1.w, enemy1.h); //writing new images
				}
				
				if(enemy2.life == 1){
					ST7735_FillRect(enemy2.x, enemy2.yold-enemy2.h, enemy2.w, enemy2.h, ST7735_BLACK);
					ST7735_DrawBitmap(enemy2.x, enemy2.y, enemy2.image, enemy2.w, enemy2.h);
				}
				
				if(enemy3.life == 1){
					ST7735_FillRect(enemy3.x, enemy3.yold-enemy3.h, enemy3.w, enemy3.h, ST7735_BLACK);
					ST7735_DrawBitmap(enemy3.x, enemy3.y, enemy3.image, enemy3.w, enemy3.h);
				}
				
				if(enemy4.life == 1){
					ST7735_FillRect(enemy4.x, enemy4.yold-enemy4.h, enemy4.w, enemy4.h, ST7735_BLACK);
					ST7735_DrawBitmap(enemy4.x, enemy4.y, enemy4.image, enemy4.w, enemy4.h);
				}
				
				if(enemy5.life == 1){
					ST7735_FillRect(enemy5.x, enemy5.yold-enemy5.h, enemy5.w, enemy5.h, ST7735_BLACK);
					ST7735_DrawBitmap(enemy5.x, enemy5.y, enemy5.image, enemy5.w, enemy5.h);
				}
				
				//for mainship
				if(clearing % 3 == 0){
          ST7735_FillRect(0, 125, 127, 35, ST7735_BLACK); //erasing old ship image
				}
          ST7735_DrawBitmap(ship.x, ship.y, ship.image, ship.w, ship.h); //writing new ship image
					//bullet display movement
				if(bullet.life == 1){
					ST7735_FillRect(bullet.x+6, bullet.yold-bullet.h, bullet.w, bullet.h, ST7735_BLACK); //erasing overwritten pixels
					ST7735_DrawBitmap(bullet.x+6, bullet.y, bullet.image, bullet.w, bullet.h);
				}	
				
				clearing++;
}
void displayEnd(){
		ST7735_FillScreen(0x0000);
		if(english == 1){
		ST7735_SetCursor(0,0);
		ST7735_OutString((char *)Phrases[end1][English]);
		ST7735_SetCursor(0,1);
		ST7735_OutString((char *)Phrases[end2][English]);
		int count = 3;
		while(score > 0)
		{
			ST7735_SetCursor(count,2);
			ST7735_OutChar((score%10)+0x30);
			score = score/10;
			count--;
			
		}
		}
		if(spanish == 1){
		ST7735_SetCursor(0,0);
		ST7735_OutString((char *)Phrases[end1][Spanish]);
		ST7735_SetCursor(0,1);
		ST7735_OutString((char *)Phrases[end2][Spanish]);
		int count = 3;
		while(score > 0)
		{
			ST7735_SetCursor(count,2);
			ST7735_OutChar((score%10)+0x30);
			score = score/10;
			count--;
			
		}
		}
		
		ST7735_DrawBitmap(40, 120, trophy.image, 40, 36); //writing new ship image
}
void bulletCollission(){
	if(enemy1.x<(bullet.x+8) && (bullet.x+8)<(enemy1.x+16)){					
					if(((bullet.y-bullet.h) - enemy1.y)<0) { //if colliding
						if(bullet.life == 1 && enemy1.life == 1){
							Sound_Explosion();
							bullet.life = 0;
							enemy1.life = 0;
							ST7735_FillRect(enemy1.x, enemy1.y-enemy1.h, enemy1.w, enemy1.h+bullet.h+2, ST7735_BLACK);
							score += 100;
							
						}
					}
				}
				
				if(enemy2.x<(bullet.x+8) && (bullet.x+8)<(enemy2.x+16)){					
					if(((bullet.y-bullet.h) - enemy2.y)<0) { //if colliding
						if(bullet.life == 1 && enemy2.life == 1){
							Sound_Explosion();
							bullet.life = 0;
							enemy2.life = 0;
							ST7735_FillRect(enemy2.x, enemy2.y-enemy2.h, enemy2.w, enemy2.h+bullet.h+2, ST7735_BLACK);
							score += 100;
						}
					}
				}
				
				if(enemy3.x<(bullet.x+8) && (bullet.x+8)<(enemy3.x+16)){					
					if(((bullet.y-bullet.h) - enemy3.y)<0) { //if colliding
						if(bullet.life == 1 && enemy3.life == 1){
							Sound_Explosion();
							bullet.life = 0;
							enemy3.life = 0;
							ST7735_FillRect(enemy3.x, enemy3.y-enemy3.h, enemy3.w, enemy3.h+bullet.h+2, ST7735_BLACK);
							score += 100;
						}
					}
				}
		
		if(enemy4.x<(bullet.x+8) && (bullet.x+8)<(enemy4.x+16)){					
					if(((bullet.y-bullet.h) - enemy4.y)<0) { //if colliding
						if(bullet.life == 1 && enemy4.life == 1){
							Sound_Explosion();
							bullet.life = 0;
							enemy4.life = 0;
							ST7735_FillRect(enemy4.x, enemy4.y-enemy4.h, enemy4.w, enemy4.h+bullet.h+2, ST7735_BLACK);
							score += 100;
						}
					}
				}
				
	
		if(enemy5.x<(bullet.x+8) && (bullet.x+8)<(enemy5.x+16)){					
					if(((bullet.y-bullet.h) - enemy5.y)<0) { //if colliding
						if(bullet.life == 1 && enemy5.life == 1){
							Sound_Explosion();
							bullet.life = 0;
							enemy5.life = 0;
							ST7735_FillRect(enemy5.x, enemy5.y-enemy5.h, enemy5.w, enemy5.h+bullet.h+2, ST7735_BLACK);
							score += 100;
						}
					}
				}
}

int main(void){ //char l;
  DisableInterrupts();
  TExaS_Init(NONE);       // Bus clock is 80 MHz 
  Sound_Init();
	Output_Init();
	ST7735_InvertDisplay(1);
  ST7735_FillScreen(0x0000);	// set screen to black
	portInit();
	ADC_Init();
	SysTick_Init(10000000);	
  Delay100ms(5);
  ST7735_FillScreen(0x0000);       // set screen to black
	
	while(1){
		displayBeginning();
    //display start screen, as long as switch is not pressed, once switch pressed, move onto language screen
		while((GPIO_PORTE_DATA_R&0x01)== 0 && (GPIO_PORTE_DATA_R&0x02) == 0){}
			
		if((GPIO_PORTE_DATA_R&0x01) == 1){  //if SW1, set english
			english = 1;
			spanish = 0;
		}
		
		if((GPIO_PORTE_DATA_R&0x02)== 2){  //if SW2, set spanish
			spanish = 1;
			english = 0;
		}
		
		gamestatus = 1;
		ST7735_FillScreen(0x0000);	// set screen to black
		Delay100ms(10);
		EnableInterrupts();
		//----------------------------------------------------------------------------
		//all in-game content code
		while(gamestatus == 1){
				movement(); //movement
				bulletCollission(); //collision
		}
		//once game ends, new screen with score and some other random graphics

		displayEnd();
		while(1){}
		
		//ask to play again? possibly?
		
  }  
}

