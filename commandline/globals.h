#ifndef __GLOBALS_H_INCLUDED__
#define __GLOBALS_H_INCLUDED__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <usb.h>        /* this is libusb */
#include "opendevice.h" /* common code moved to separate module */

#include "../firmware/requests.h"   /* custom request numbers */
#include "../firmware/usbconfig.h"  /* device's VID/PID and names */

void test(void);
void open_handle(void);
void close_handle(void);

#endif __GLOBALS_H_INCLUDED__

#ifdef DEFINE_VARIABLES
#define EXTERN /* nothing */
#else
#define EXTERN extern
#endif /* DEFINE_VARIABLES */

EXTERN usb_dev_handle *handle;
