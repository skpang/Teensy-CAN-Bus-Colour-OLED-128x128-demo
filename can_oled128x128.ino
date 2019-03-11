/* Teensy CAN-Bus with OLED 128x128 demo *

www.skpang.co.uk
V1.0 March 2017
For use with Teensy CAN-Bus with Colour OLED 128x128 and SD card holder:
http://skpang.co.uk/catalog/teensy-canbus-with-colour-oled-128x128-and-sd-card-holder-p-1523.html

Also requres new Teensy SSD1351 libarary
https://github.com/kirberich/teensy_ssd1351

*/

#include <Arduino.h>
#include <SPI.h>
#include <ssd1351.h>
#include <FlexCAN.h>


static CAN_message_t msg,rxmsg;
volatile uint32_t count = 0;
IntervalTimer TX_timer;
String CANStr(""); 
volatile uint32_t can_msg_count = 0;

// use this to do Color c = RGB(...) instead of `RGB c = RGB(...)` or ssd1351::LowColor c = RGB(...)
// because it's slightly faster and guarantees you won't be sending wrong colours to the display.

// Choose color depth - IndexedColor, LowColor and HighColor currently supported
// typedef ssd1351::IndexedColor Color;
// typedef ssd1351::LowColor Color;
typedef ssd1351::HighColor Color;

// Choose display buffering - NoBuffer or SingleBuffer currently supported
// auto display = ssd1351::SSD1351<Color, ssd1351::NoBuffer, 128, 96>();
auto display = ssd1351::SSD1351<Color, ssd1351::SingleBuffer, 128, 128>();


void setup(void) {
  Serial.begin(9600);
  Serial.print("CAN-Bus demo");
  
  display.begin();
  display.fillScreen(ssd1351::RGB());

  display.setFont(FreeSans9pt7b);
//  display.setTextSize(3);

  display.setTextColor(ssd1351::RGB(255, 255, 255));
  
  display.setCursor(0,20);
  display.println("128x128 OLED");
  display.println("CAN-Bus demo ");
  display.println("SK Pang");
  display.updateScreen();


  Can0.begin(500000); //set speed here. 
  msg.buf[0] = 1;
  msg.buf[1] = 2;
  msg.buf[2] = 0;
  msg.buf[3] = 0;
  msg.buf[4] = 0;
  msg.buf[5] = 0;
  msg.buf[6] = 0;
  msg.len = 8;
  msg.id = 0x222;

  TX_timer.begin(tx_CAN, 1000000);    /* Start interrutp timer */

    
}
/* From Timer Interrupt */
void tx_CAN(void)
{
  msg.buf[4] = can_msg_count >> 24;
  msg.buf[5] = can_msg_count >> 16;
  msg.buf[6] = can_msg_count >> 8;
  msg.buf[7] = can_msg_count;
  Can0.write(msg);
  can_msg_count++;
}


void loop() 
{
 int i;
  
  while(Can0.read(rxmsg))
  { 
     String CANStr(""); 
     for (int i=0; i < 8; i++) {     

         CANStr += String(rxmsg.buf[i],HEX);
         CANStr += (" ") ;
     }
     Serial.print(rxmsg.id,HEX); 
     Serial.print(' '); 
     Serial.print(rxmsg.len,HEX); 
     Serial.print(' ');
     Serial.println(CANStr);  
     
   //  tft.fillRect(0,15,128, 30,BLACK);
   //  tft.setCursor(0,15);
     
   //  tft.println(rxmsg.id,HEX); 
   //  tft.println(rxmsg.len,HEX); 
   //  tft.println(CANStr);
   //  tft.display();

     
  }




}
