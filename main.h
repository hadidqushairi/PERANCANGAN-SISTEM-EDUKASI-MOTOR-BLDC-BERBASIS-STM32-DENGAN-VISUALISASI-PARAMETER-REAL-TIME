#ifndef MAIN_H
#define MAIN_H

#include "stm32f1xx_hal.h"
#include "lcd.h"  // LCD library for STM32
#include "motor_control.h"  // Custom motor control functions

#define PWM_FREQUENCY 20000  // PWM frequency in Hz
#define POTENTIOMETER_PIN GPIO_PIN_0  // Potentiometer pin (PA0)

// SEI CERT: DCL30-C: Declare objects with appropriate storage durations
// Declaring MotorParameters with appropriate storage duration (static storage duration)
typedef struct {
    float rpm;         // Motor speed in RPM
    float voltage;     // Motor voltage
    float current;     // Motor current
    float temperature; // Motor temperature
    float time;        // Time of operation
} MotorParameters;

// SEI CERT: DCL31-C: Declare identifiers before using them
// Prototypes of functions declared before usage to ensure clarity and maintainability.
void SystemClock_Config(void);
void GPIO_Init(void);
void ADC_Init(void);
void PWM_Init(void);
void readPotentiometer(void);
void displayMotorParameters(MotorParameters *params);

#endif // MAIN_H
