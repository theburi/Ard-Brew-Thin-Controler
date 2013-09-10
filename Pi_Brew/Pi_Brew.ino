#include <Wire.h>

#include <U8glib.h>
#include <String.h>

#define SLAVE_ADDRESS 0x04

U8GLIB_ST7920_128X64_1X u8g(8, 9, 10);	// SPI Com: SCK = en = 18, MOSI = rw = 16, CS = di = 17

char command[10]; //command buffer
int state = 0;

char commandid;

char modeText[10] = "N/A";

void draw( ) {
  

  // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(u8g_font_unifont);
  //u8g.setFont(u8g_font_osb21);
  u8g.drawStr( 0, 22, "Mode");
  u8g.drawStr( 50, 22, modeText);
  
}


void setup() {
    pinMode(13, OUTPUT);
    Serial.begin(9600);         // start serial for output
    // initialize i2c as slave
    Wire.begin(SLAVE_ADDRESS);
 
    // define callbacks for i2c communication
    Wire.onReceive(receiveData);
    Wire.onRequest(sendData);
 
    Serial.println("Ready!");
}
 
void loop() {
    u8g.firstPage();  
    do {
      draw();
    } while( u8g.nextPage() );

    delay(100);
}
 
// callback for received data
void receiveData(int byteCount){

  int i=0;
  while(Wire.available()) 
  {
    if (i==0) commandid=Wire.read();
    else
    {
      char b = Wire.read();
      command[i-1] = b;
      Serial.print(b);

    }
      i++;
  }
  // we need to convert command    

  executeCommand(commandid, command);
  
}
 
// callback for sending data
void sendData(){
    Wire.write(command);
}

void executeCommand(char commandid, char* args)
{
  if (commandid== 0X04)
  {
    strcpy( modeText, args);


  }
}
