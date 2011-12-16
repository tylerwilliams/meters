#include "globals.h"

int get_meter_value(void)
{
  char buffer[6];
  int cnt = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN, CUSTOM_RQ_GET_METER, 0, 0, buffer, sizeof(buffer), 5000);
  if(cnt < 0){
    fprintf(stderr, "\nUSB error. in iteration %s\n", usb_strerror());
    exit(1);
  }else if(cnt != 1){
    fprintf(stderr, "\nerror %d bytes received instead of 6\n", cnt);
    exit(1);
  }
  int meter_value = (int)buffer[0] & 0xff;
  return meter_value;
}

void set_meter_value(int val)
{
  if (val < 0 || val > 255) {
    fprintf(stderr, "desired meter val must be between 0 and 255. was: %d\n", val);
    exit(1);
  }

  char                buffer[6];
  int cnt = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN, CUSTOM_RQ_SET_METER, 0, val, buffer, sizeof(buffer), 5000);
  
  if(cnt < 0){
    fprintf(stderr, "\nUSB error. in iteration %s\n", usb_strerror());
    exit(1);
  }
}
