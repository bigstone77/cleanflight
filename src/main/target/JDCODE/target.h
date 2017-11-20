
#pragma once

#define TARGET_BOARD_IDENTIFIER             "JDCODE"
#define USBD_PRODUCT_STRING                 "JDCode"

#define LED0_PIN                        PB14
#define LED1_PIN                        PB15

#define GYRO
#define USE_GYRO_MPU6050

#define ACC
#define USE_ACC_MPU6050

#define USE_EXTI
#define MPU_INT_EXTI            PA15
#define USE_MPU_DATA_READY_SIGNAL

#define USB_IO

#define USE_VCP
#define USE_UART3
#define SERIAL_PORT_COUNT             	2

#define UART3_TX_PIN                    PB10
#define UART3_RX_PIN                    PB11

#define SERIALRX_UART                       SERIAL_PORT_USART3
#define SERIALRX_PROVIDER                   SERIALRX_SBUS

#define USE_I2C
#define USE_I2C_DEVICE_2
#define I2C_DEVICE                      (I2CDEV_2) 

#define I2C2_SCL                        PB10
#define I2C2_SDA                        PC12

#define VBAT_LPF_FREQ 0.25f                // The ADC in the F4NEO appears to be noisy, this stabilises it but at the cost of latency.

#define TARGET_IO_PORTA                     0xffff
#define TARGET_IO_PORTB                     0xffff
#define TARGET_IO_PORTC                     0xffff
#define TARGET_IO_PORTD                     (BIT(2))

#define USABLE_TIMER_CHANNEL_COUNT          14 // 4xPWM, 6xESC, 2xESC via UART3 RX/TX, 1xLED Strip, 1xIR.
#define USED_TIMERS                         (TIM_N(1) | TIM_N(2) | TIM_N(3) | TIM_N(8) | TIM_N(9))
