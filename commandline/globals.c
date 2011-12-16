#define DEFINE_VARIABLES
#include "globals.h"
#include "opendevice.h"

void test(void)
{
  int i;
  char                buffer[4];
  srandom(time(NULL));
  for(i = 0; i < 50000; i++){
    int value = random() & 0xffff, index = random() & 0xffff;
    int rxValue, rxIndex;
    if((i+1) % 100 == 0){
      fprintf(stderr, "\r%05d", i+1);
      fflush(stderr);
    }
    int cnt = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN, CUSTOM_RQ_ECHO, value, index, buffer, sizeof(buffer), 5000);
    if(cnt < 0){
      fprintf(stderr, "\nUSB error in iteration %d: %s\n", i, usb_strerror());
      break;
    }else if(cnt != 4){
      fprintf(stderr, "\nerror in iteration %d: %d bytes received instead of 4\n", i, cnt);
      break;
    }
    rxValue = ((int)buffer[0] & 0xff) | (((int)buffer[1] & 0xff) << 8);
    rxIndex = ((int)buffer[2] & 0xff) | (((int)buffer[3] & 0xff) << 8);
    if(rxValue != value || rxIndex != index){
      fprintf(stderr, "\ndata error in iteration %d:\n", i);
      fprintf(stderr, "rxValue = 0x%04x value = 0x%04x\n", rxValue, value);
      fprintf(stderr, "rxIndex = 0x%04x index = 0x%04x\n", rxIndex, index);
    }
  }
  fprintf(stderr, "\nTest completed.\n");
}

void open_handle(void)
{
    const unsigned char rawVid[2] = {USB_CFG_VENDOR_ID}, rawPid[2] = {USB_CFG_DEVICE_ID};
    char                vendor[] = {USB_CFG_VENDOR_NAME, 0}, product[] = {USB_CFG_DEVICE_NAME, 0};
    int                 vid, pid;
    usb_init();
    /* compute VID/PID from usbconfig.h so that there is a central source of information */
    vid = rawVid[1] * 256 + rawVid[0];
    pid = rawPid[1] * 256 + rawPid[0];
    /* The following function is in opendevice.c: */
    if(usbOpenDevice(&handle, vid, vendor, pid, product, NULL, NULL, NULL) != 0){
        fprintf(stderr, "Could not find USB device \"%s\" with vid=0x%x pid=0x%x\n", product, vid, pid);
        exit(1);
    }
}

void close_handle(void)
{
  usb_close(handle);
}
