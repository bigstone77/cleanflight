
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "platform.h"

#ifdef SERIAL_RX

#include "common/utils.h"

#include "drivers/time.h"

#include "io/serial.h"

#include "rx/rx.h"
#include "rx/jdcode.h"
#include "drivers/light_led.h"

#define RXBUF_SIZE    64
#define TXBUF_SIZE    64
uint8_t rx_head, rx_tail;
uint8_t tx_head, tx_tail;
uint16_t rxBuf[RXBUF_SIZE];
uint16_t txBuf[TXBUF_SIZE];
uint16_t jdcodeChannel[6];

static bool JDCodeFrameDone = false;

static uint8_t rxbuf_size()
{
	if(rx_head >= rx_tail)
		return rx_head-rx_tail;
	else  
		return rx_head+RXBUF_SIZE-rx_tail;
}

static int get_cmdblock(void)
{
	uint8_t buf[8];
	int n, qlen = rxbuf_size();

	for(n=0; n<qlen; n++){
		if(rxBuf[rx_tail] == 0xF0)
			break;
		else{
			if(++rx_tail >= RXBUF_SIZE)  
				rx_tail = 0;
		}
	}

	if((qlen-n) >= 8){
		for(n=0; n<8; n++){
			buf[n] = rxBuf[rx_tail++];
			if(rx_tail >= RXBUF_SIZE) rx_tail = 0;
		}

		int sum=0;
		uint8_t checksum;
		for(n=1; n<7; n++)
			sum += buf[n];
		checksum = ((sum&0xFF)==0xF0)? 0xEF : sum&0xFF;

		if(checksum == buf[7]){
			if(buf[1] == 0xA1){
				buf[2] = 100+(100-buf[2])*2/3;
				buf[3] = 100+(buf[3]-100)*2/3;
				buf[4] = 100+(buf[4]-100)*4/3;
				for(n=0;n<4; n++)
					jdcodeChannel[n] = buf[n+2];
				JDCodeFrameDone = true;
			}
			return 1;
		}
	}
	return 0;
}


static void DataReceive(uint16_t c)
{

	rxBuf[rx_head++] = c;
	if(rx_head >= RXBUF_SIZE) 
		rx_head = 0; 
	}

static uint8_t jdcodeFrameStatus(void)
{
    if (!JDCodeFrameDone)
        return RX_FRAME_PENDING;
    JDCodeFrameDone = false;

    return RX_FRAME_COMPLETE;
}

static uint16_t jdcodeReadRawRC(const rxRuntimeConfig_t *rxRuntimeConfig, uint8_t chan)
{
	UNUSED(rxRuntimeConfig);
	if(rxbuf_size()>=8)
		get_cmdblock();

	if(chan>4)
		chan = 4;
	return 885 + 1230*jdcodeChannel[chan]/200;
}

bool jdcodeInit(const rxConfig_t *rxConfig, rxRuntimeConfig_t *rxRuntimeConfig)
{
	UNUSED(rxConfig);
	rxRuntimeConfig->channelCount = 4;

	rxRuntimeConfig->rcReadRawFn = jdcodeReadRawRC;
	rxRuntimeConfig->rcFrameStatusFn = jdcodeFrameStatus;

	const serialPortConfig_t *portConfig = findSerialPortConfig(FUNCTION_RX_SERIAL);
		if (!portConfig) {
		return false;
	}

	
	serialPort_t *port = openSerialPort(portConfig->identifier,
			FUNCTION_RX_SERIAL,
			DataReceive,
			38400,
			MODE_RXTX,
			SERIAL_STOPBITS_1 | SERIAL_PARITY_NO);
	return port != NULL;
}
#endif
