/*
 * interrupt.h
 *
 *  Created on: Dec 27, 2023
 *      Author: Tran Thoi
 */

#ifndef ENC28J60_INTERRUPT_H_
#define ENC28J60_INTERRUPT_H_

#include "enc28j60.h"

#define EIE				0x1B
#define EIE_INTIE		0x80
#define EIE_PKTIE		0x40


void ENC_28J60_SET_INTERRUPT();
#endif /* ENC28J60_INTERRUPT_H_ */
