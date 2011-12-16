#include "globals.h"

// code
void print_status(void)
{
  char                buffer[4];
  int cnt = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN, CUSTOM_RQ_GET_STATUS, 0, 0, buffer, sizeof(buffer), 5000);
  if(cnt < 1){
    if(cnt < 0){
      fprintf(stderr, "USB error: %s\n", usb_strerror());
    }else{
      fprintf(stderr, "only %d bytes received.\n", cnt);
    }
  }else{
    printf("LED is %s\n", buffer[0] ? "on" : "off");
  }
}

void toggle_led(int state)
{
    char                buffer[4];
    int cnt = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_OUT, CUSTOM_RQ_SET_STATUS, state, 0, buffer, 0, 5000);
    if(cnt < 0){
        fprintf(stderr, "USB error: %s\n", usb_strerror());
    }  
}
