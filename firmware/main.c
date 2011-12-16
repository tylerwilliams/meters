#define LED_PORT_DDR        DDRB
#define LED_PORT_OUTPUT     PORTB
#define LED_BIT             0

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "oddebug.h"

#include <avr/pgmspace.h>
#include "usbdrv.h"
#include "requests.h"
#include "lcd.h"

static uint8_t meter_value;
static uint8_t itoa_buffer[7];

/* The following variables store the status of the current data transfer */
static uchar    currentAddress;
static uchar    bytesRemaining;

void ioinit( void )
{
    cli();
    DDRB = 0;
    DDRB |= (1<<PB0) | (1<<PB3);
    setup_pwm();
    lcd_init(LCD_DISP_ON);          /* initialize display, cursor off */
    sei();                          /* enable interrupts */
}

void setup_pwm ( void )
{
    OCR2 = 128;                 // set PWM for 50% duty cycle
    TCCR2 |= (1 << COM21);      // set none-inverting mode
    TCCR2 |= (1 << WGM21) | (1 << WGM20);   // set fast PWM Mode
    TCCR2 |= (1 << CS21);       // set prescaler to 8 and starts PWM
}

/* ------------------------------------------------------------------------- */
/* ----------------------------- USB interface ----------------------------- */
/* ------------------------------------------------------------------------- */

uchar usbFunctionWrite(uchar *data, uchar len)
{
    uchar i;
    if(len > bytesRemaining)                // if this is the last incomplete chunk
        len = bytesRemaining;               // limit to the amount we can store
    bytesRemaining -= len;
    for(i = 0; i < len; i++)
        lcd_putc(data[i]);
    return bytesRemaining == 0;             // return 1 if we have all data
}

usbMsgLen_t usbFunctionSetup(uchar data[8])
{
  usbRequest_t    *rq = (void *)data;
  static uchar    dataBuffer[6];  /* buffer must stay valid when usbFunctionSetup returns */

    if(rq->bRequest == CUSTOM_RQ_ECHO){ /* echo -- used for reliability tests */
        dataBuffer[0] = rq->wValue.bytes[0];
        dataBuffer[1] = rq->wValue.bytes[1];
        dataBuffer[2] = rq->wIndex.bytes[0];
        dataBuffer[3] = rq->wIndex.bytes[1];
        usbMsgPtr = dataBuffer;         /* tell the driver which data to return */
        return 4;
    }else if(rq->bRequest == CUSTOM_RQ_SET_STATUS){
        if(rq->wValue.bytes[0] & 1){    /* set LED */
            LED_PORT_OUTPUT |= _BV(LED_BIT);
        }else{                          /* clear LED */
            LED_PORT_OUTPUT &= ~_BV(LED_BIT);
        }
    }else if(rq->bRequest == CUSTOM_RQ_GET_STATUS){
        dataBuffer[0] = ((LED_PORT_OUTPUT & _BV(LED_BIT)) != 0);
        usbMsgPtr = dataBuffer;
        return 1;
    }else if(rq->bRequest == CUSTOM_RQ_GET_METER) {
        dataBuffer[0] = meter_value;
        usbMsgPtr = dataBuffer;
        return 1;
    } else if(rq->bRequest == CUSTOM_RQ_SET_METER) {
        meter_value = rq->wIndex.bytes[0];
        lcd_clrscr();
    } else if(rq->bRequest == CUSTOM_RQ_LCD_CLEAR) {
        lcd_clrscr();
    } else if(rq->bRequest == CUSTOM_RQ_LCD_GOTO) {
        lcd_gotoxy((rq->wIndex.bytes[0] & 15), (rq->wIndex.bytes[0] & 240)>>4);
    }else if(rq->bRequest == CUSTOM_RQ_LCD_COMMAND) {
        lcd_command(rq->wIndex.bytes[0]);
    } else if(rq->bRequest == CUSTOM_RQ_LCD_PUTS) {
        bytesRemaining = rq->wIndex.bytes[0];
        currentAddress = 0;
        // itoa( bytesRemaining , itoa_buffer, 10);
        // lcd_puts(itoa_buffer);
        return USB_NO_MSG;
    }

    return 0;   /* default for not implemented requests: return no data back to host */
}

/* ------------------------------------------------------------------------- */


int __attribute__((noreturn)) main(void)
{
uchar   i;

    wdt_enable(WDTO_1S);
    /* Even if you don't use the watchdog, turn it off here. On newer devices,
     * the status of the watchdog (on/off, period) is PRESERVED OVER RESET!
     */
    /* RESET status: all port bits are inputs without pull-up.
     * That's the way we need D+ and D-. Therefore we don't need any
     * additional hardware initialization.
     */
    //    odDebugInit();
    //    DBG1(0x00, 0, 0);       /* debug output: main starts */

    usbInit();
    usbDeviceDisconnect();  /* enforce re-enumeration, do this while interrupts are disabled! */
    i = 0;
    while(--i){             /* fake USB disconnect for > 250 ms */
        wdt_reset();
        _delay_ms(1);
    }
    usbDeviceConnect();
    ioinit();

    //    sei();
    LED_PORT_DDR |= _BV(LED_BIT);   /* make the LED bit an output */

    uint16_t counter = 0;
    for(;;){                /* main event loop */
        wdt_reset();
        usbPoll();
        OCR2 = meter_value;
    }
}

/* ------------------------------------------------------------------------- */

