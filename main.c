#include "main.h"

// SEI CERT: DCL31-C: Declare identifiers before using them
// Motor parameters should be declared before usage
MotorParameters motorParams = {0};  // Initializing with zero values

// SEI CERT: DCL30-C: Declare objects with appropriate storage durations
// ADC handle for potentiometer
ADC_HandleTypeDef hadc1;

// PWM handle for motor control
TIM_HandleTypeDef htim3;

int main(void)
{
    // SEI CERT: MSC37-C: Ensure that control never reaches the end of a non-void function
    HAL_Init();  // Initializes the HAL library
    SystemClock_Config();  // Configures the system clock
    GPIO_Init();  // Initializes GPIO pins
    ADC_Init();  // Initializes ADC
    PWM_Init();  // Initializes PWM
    
    LCD_Init();  // Initializes LCD for display

    // SEI CERT: MSC37-C: Ensure that control never reaches the end of a non-void function
    while (1)
    {
        // SEI CERT: EXP33-C: Do not read uninitialized memory
        // Reading the potentiometer value ensures initialization
        readPotentiometer();

        // SEI CERT: EXP33-C: Do not read uninitialized memory
        // Calculating motor parameters with initialized values
        motorParams.rpm = calculateRPM();
        motorParams.voltage = calculateVoltage();
        motorParams.current = calculateCurrent();
        motorParams.temperature = readMotorTemperature();
        motorParams.time = HAL_GetTick() / 1000;  // Time in seconds

        // SEI CERT: MSC37-C: Ensure that control never reaches the end of a non-void function
        displayMotorParameters(&motorParams);  // Display parameters on LCD
        
        HAL_Delay(1000);  // SEI CERT: EXP45-C: Do not perform assignments in selection statements
    }
}

// SEI CERT: DCL31-C: Declare identifiers before using them
// System clock configuration to match the required frequency for STM32F103T6
void SystemClock_Config(void)
{
    // Configuring system clock using either HSE or HSI, as per your configuration
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);
}

// SEI CERT: DCL30-C: Declare objects with appropriate storage durations
void GPIO_Init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();  // Enable clock for GPIOA

    // SEI CERT: DCL37-C: Do not declare or define a reserved identifier
    // Using non-reserved names for pin definitions
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = POTENTIOMETER_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_6;  // Pin for PWM (PA6)
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

// SEI CERT: DCL31-C: Declare identifiers before using them
void ADC_Init(void)
{
    __HAL_RCC_ADC1_CLK_ENABLE();  // Enable clock for ADC

    // SEI CERT: DCL30-C: Declare objects with appropriate storage durations
    hadc1.Instance = ADC1;
    hadc1.Init.Resolution = ADC_RESOLUTION_12B;
    hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
    hadc1.Init.ContinuousConvMode = ENABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.NbrOfConversion = 1;

    HAL_ADC_Init(&hadc1);
}

// SEI CERT: DCL31-C: Declare identifiers before using them
void PWM_Init(void)
{
    __HAL_RCC_TIM3_CLK_ENABLE();  // Enable clock for TIM3

    // SEI CERT: DCL30-C: Declare objects with appropriate storage durations
    htim3.Instance = TIM3;
    htim3.Init.Prescaler = 0;
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim3.Init.Period = 1000 - 1;  // PWM period for 20 kHz
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&htim3);

    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);  // Start PWM on channel 1
}

// SEI CERT: EXP33-C: Do not read uninitialized memory
// Ensure ADC value is read correctly before using it for PWM control
void readPotentiometer(void)
{
    HAL_ADC_Start(&hadc1);
    if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK)
    {
        uint32_t potValue = HAL_ADC_GetValue(&hadc1);
        uint32_t pwmValue = (potValue * PWM_FREQUENCY) / 4095;

        // SEI CERT: EXP45-C: Do not perform assignments in selection statements
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, pwmValue);  // Update PWM duty cycle
    }
}

// SEI CERT: MSC37-C: Ensure that control never reaches the end of a non-void function
void displayMotorParameters(MotorParameters *params)
{
    char displayBuffer[64];
    
    // SEI CERT: EXP35-C: Do not modify objects with temporary lifetime
    snprintf(displayBuffer, sizeof(displayBuffer), "RPM: %.2f\nVoltage: %.2fV\nCurrent: %.2fA\nTemp: %.2fC\nTime: %.2fs",
             params->rpm, params->voltage, params->current, params->temperature, params->time);
    LCD_DisplayString(displayBuffer);
}

// SEI CERT: EXP36-C: Do not cast pointers into more strictly aligned pointer types
// RPM calculation (example, should be replaced with actual sensor handling)
float calculateRPM(void)
{
    return 1500;  // Example value, actual calculation depends on motor sensor or encoder
}

// Example voltage calculation
float calculateVoltage(void)
{
    return 12.0f;  // Example value, adjust based on motor driver or sensor
}

// Example current calculation
float calculateCurrent(void)
{
    return 1.5f;  // Example value, adjust based on current sensor
}

// Example temperature reading
float readMotorTemperature(void)
{
    return 30.0f;  // Example, use temperature sensor for actual value
}
