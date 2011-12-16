#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <usb.h>        /* this is libusb */
#include "opendevice.h" /* common code moved to separate module */

#include "globals.h"
#include "led.h"
#include "meter.h"
#include "lcd.h"

static void usage(char *name)
{
    fprintf(stderr, "usage:\n");
    fprintf(stderr, "  %s on ......................... turn on LED\n", name);
    fprintf(stderr, "  %s off ........................ turn off LED\n", name);
    fprintf(stderr, "  %s status ..................... ask current status of LED\n", name);
    fprintf(stderr, "  %s test ....................... run driver reliability test\n", name);
    fprintf(stderr, "  %s get_meter .................. get the meter value (0-255)\n", name);
    fprintf(stderr, "  %s set_meter <val> ............ set the meter value\n", name);
    fprintf(stderr, "  %s clear_lcd .................. clear lcd\n", name);
    fprintf(stderr, "  %s lcd_goto <x_val> <y_val> ... goto position\n", name);
    fprintf(stderr, "  %s lcd_command <command> ...... exec lcd command\n", name);
    fprintf(stderr, "  %s lcd_puts <string> .......... put string to lcd\n", name);    
}



int main(int argc, char **argv)
{
  int isOn;
  char* endptr;
  
  if(argc < 2){   /* we need at least one argument */
    usage(argv[0]);
    exit(1);
  }
  open_handle();

  /* ok, actually do stuff */
  if(strcasecmp(argv[1], "status") == 0){
    print_status();
  }else if((isOn = (strcasecmp(argv[1], "on") == 0)) || strcasecmp(argv[1], "off") == 0){
    toggle_led(isOn);
  }else if(strcasecmp(argv[1], "test") == 0){
    test();
  }
  else if(strcasecmp(argv[1], "get_meter") == 0){    
    fprintf(stderr, "meter: %i\n", get_meter_value());
  }else if(strcasecmp(argv[1], "set_meter") == 0){
    unsigned int val = strtoul(argv[2], &endptr, 0);
    set_meter_value(val);
  }else if(strcasecmp(argv[1], "clear_lcd") == 0){
    clear_lcd();
  }else if(strcasecmp(argv[1], "lcd_goto") == 0){
    int x = strtoul(argv[2], &endptr, 0);
    int y = strtoul(argv[3], &endptr, 0);
    lcd_goto(x, y);
  }else if(strcasecmp(argv[1], "lcd_command") == 0){
    int command_val = strtoul(argv[2], &endptr, 0);  
    lcd_command(command_val);
  }else if(strcasecmp(argv[1], "lcd_puts") == 0){
    lcd_puts(argv[2]);
  }else{
    usage(argv[0]);
    exit(1);
  }
  close_handle();
  return 0;
}

