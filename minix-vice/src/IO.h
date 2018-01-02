#ifndef IO_H
#define IO_H

#include "MinixVice.h"
#include "state_machines.h"

/** @defgroup IO IO
 * @{
 *
 * Input/Output basic functions. Interrupt handling for mouse, keyboard and timer and polling for Real-Time Clock (RTC).
 */

/**
 * @brief Subscribes all interrupts (keyboard, timer, mouse)
 */
void subscribeInterrupts();

/**
 * @brief Unsubscribes all interrupts (keyboard, timer, mouse)
 */
void unsubscribeInterrupts();

/**
 * @brief Mouse interrupt handler
 */
void mouseIH();

/**
 * @brief Timer interrupt handler
 */
void timerIH();

/**
 * @brief Keyboard interrupt handler
 */
void kbdIH();

/**
 * @brief Read date and hour from the RTC (Real-Time Clock) via polling.
 */
void readRTC();

/**
 * @brief All interrupts handler. Calls appropriate handler for each interrupt.
 */
void interruptsHandler();

#endif /* IO_H */
