#include <TwoMsTimer.h>
#include <SoftwareSerial.h>
void setup(){
  lcd.begin( 20, 4);
  lcd.clear();
  for(i=0;i<40;i++){
    for(j=0;j<4;j++){
      for(k=0;k<20;k++){
        pnrdata[i][j][k]='\0';
      }
    }
  }
  
  
}
void loop(){
 
}



