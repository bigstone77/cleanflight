
#pragma once

#define TARGET_BOARD_IDENTIFIER             "JDCODE"
#define USBD_PRODUCT_STRING                 "JDCode"

#define LED0_PIN                        PB14
#define LED0_INVERTED
#define LED1_PIN                        PB15
#define LED1_INVERTED

#define GYRO
#define USE_GYRO_MPU6050

#define ACC
#define USE_ACC_MPU6050

#define USE_EXTI
#define MPU_INT_EXTI            	PA1
#define USE_MPU_DATA_READY_SIGNAL

#define USB_IO

#define USE_VCP
#define USE_UART3
#define SERIAL_PORT_COUNT             	2

#define UART3_TX_PIN                    PC10
#define UART3_RX_PIN                    PC5

#define SERIALRX_UART                       SERIAL_PORT_USART3
#define SERIALRX_PROVIDER                   SERIALRX_SBUS

#define USE_I2C
#define USE_I2C_DEVICE_2
#define USE_I2C_PULLUP
#define I2C_DEVICE                      (I2CDEV_2) 

#define I2C2_SCL                        PB10
#define I2C2_SDA                        PC12

#define TARGET_IO_PORTA                     0xFCFF
#define TARGET_IO_PORTB                     0xF7FF
#define TARGET_IO_PORTC                     0xFFFF
#define TARGET_IO_PORTD                     (BIT(2))

#define USABLE_TIMER_CHANNEL_COUNT           4 
#define USED_TIMERS                         (TIM_N(3))
