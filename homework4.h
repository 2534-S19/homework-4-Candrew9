/*
 * homework4.h
 *
 *  Created on: 4/5/20
 *      Author: Andrew Caldwell
 *
 */

#ifndef HOMEWORK4_H_
#define HOMEWORK4_H_

// This function initializes the board by turning off the Watchdog Timer.
void initBoard();
bool charFSM(char rChar);


// TODO: Define any constants that are local to homework.c using #define
typedef enum myStates{first, second, third, fourth} charStates;
charStates currState = first;

#endif /* HOMEWORK4_H_ */
