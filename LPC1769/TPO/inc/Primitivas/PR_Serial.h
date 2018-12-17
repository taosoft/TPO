/*
 * PR_Serie.h
 *
 *  Created on: Dec 2, 2018
 *      Author: matias
 */

#ifndef PR_SERIAL_H_
#define PR_SERIAL_H_

#include "DR_UART.h"

#define TXBUFFER_SIZE	256
#define RXBUFFER_SIZE	256

uint8_t Serial_PushTx(uint8_t);
uint8_t Serial_PopRx(uint8_t *);
uint8_t Serial_Send(const uint8_t *, uint8_t);
void Serial_RxIntHandler(void);
void Serial_TxIntHandler(void);

#endif /* PR_SERIAL_H_ */
