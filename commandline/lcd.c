#include "globals.h"

void clear_lcd( void )
{
    char                buffer[4];
    int cnt = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_OUT, CUSTOM_RQ_LCD_CLEAR, 0, 0, buffer, 0, 5000);
    if(cnt < 0){
        fprintf(stderr, "USB error: %s\n", usb_strerror());
    }
}

void lcd_goto(int x, int y)
{
  
  if (x < 0 || x > 15) {
      fprintf(stdout, "X value out of range (0-16) %i\n", x);
      exit(1);
  }

  if (y < 0 || y > 1) {
      fprintf(stdout, "Y value out of range (0-1) %i\n", y);
      exit(1);
  }
    
  char                buffer[6];
  int cnt = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN, CUSTOM_RQ_LCD_GOTO, 0, (y<<4)|x, buffer, sizeof(buffer), 5000);
  
  if(cnt < 0){
    fprintf(stderr, "\nUSB error. in iteration %s\n", usb_strerror());
    exit(1);
  }
}

void lcd_command(int command_code)
{
  char                buffer[6];
  int cnt = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN, CUSTOM_RQ_LCD_COMMAND, 0, command_code, buffer, sizeof(buffer), 5000);
  
  if(cnt < 0){
    fprintf(stderr, "\nUSB error. in iteration %s\n", usb_strerror());
    exit(1);
  }
}

void lcd_puts(char* string)
{
    int cnt = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_OUT, CUSTOM_RQ_LCD_PUTS, 0, (unsigned int)strlen(string), string, strlen(string), 5000);

    if(cnt < 0){
        fprintf(stderr, "\nUSB error. in iteration %s\n", usb_strerror());
        exit(1);
    }
}