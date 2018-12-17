/*
 * DR_SD.h
 *
 *  Created on: Dec 13, 2018
 *      Author: matias
 */

#ifndef DRIVERS_DR_SD_H_
#define DRIVERS_DR_SD_H_

#include "RegsLPC17xx.h"

/* SPI0 */
#define	S0SPCR		(*(volatile uint32_t*)0x40020000)
#define	S0SPSR		(*(volatile uint32_t*)0x40020004)
#define	S0SPDR		(*(volatile uint32_t*)0x40020008)
#define	S0SPCCR		(*(volatile uint32_t*)0x4002000C)
#define	S0SPINT		(*(volatile uint32_t*)0x4002001C)

/* SSP0,SSP1 */
#define	SSP0CR0		(*(volatile uint32_t*)0x40088000)
#define	SSP0CR1		(*(volatile uint32_t*)0x40088004)
#define	SSP0DR		(*(volatile uint32_t*)0x40088008)
#define	SSP0SR		(*(volatile uint32_t*)0x4008800C)
#define	SSP0CPSR	(*(volatile uint32_t*)0x40088010)
#define	SSP0IMSC	(*(volatile uint32_t*)0x40088014)
#define	SSP0RIS		(*(volatile uint32_t*)0x40088018)
#define	SSP0MIS		(*(volatile uint32_t*)0x4008801C)
#define	SSP0ICR		(*(volatile uint32_t*)0x40088020)
#define	SSP0DMACR	(*(volatile uint32_t*)0x40088024)
#define	SSP1CR0		(*(volatile uint32_t*)0x40030000)
#define	SSP1CR1		(*(volatile uint32_t*)0x40030004)
#define	SSP1DR		(*(volatile uint32_t*)0x40030008)
#define	SSP1SR		(*(volatile uint32_t*)0x4003000C)
#define	SSP1CPSR	(*(volatile uint32_t*)0x40030010)
#define	SSP1IMSC	(*(volatile uint32_t*)0x40030014)
#define	SSP1RIS		(*(volatile uint32_t*)0x40030018)
#define	SSP1MIS		(*(volatile uint32_t*)0x4003001C)
#define	SSP1ICR		(*(volatile uint32_t*)0x40030020)
#define	SSP1DMACR	(*(volatile uint32_t*)0x40030024)

void Init_SD(void);

#endif /* DRIVERS_DR_SD_H_ */
