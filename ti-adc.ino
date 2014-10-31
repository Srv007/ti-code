
#include <TwoMsTimer.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#define rfid_RX P1_5
#define rfid_TX P6_0

//Side button Configuration//
#define button1 P4_3
#define button2 P4_0
#define button3 P8_2
#define lock_button P4_0

//vibration pin Configuration
#define vibraton_pin P7_0

//uart configuration for rf-id
SoftwareSerial rfid(rfid_RX,rfid_TX);

//Keypad pin interface pins:
const int column[] = {
  P4_1,P4_2,P2_7,P3_2};
const int row[] = {
  P6_6, P1_6, P1_2, P1_3};

//keypad number control:
char Keypad[4] [4] = {  
  'D' , 'C' , 'B' , 'A',
  '#' , '9' , '6' , '3',
  '0' , '8' , '5' ,' 2',
  '*' , '7' , '4' , '1'   };  

int Keypad_int[4] [4] = {  
  0 , 0 , 0 ,  0,
  0 , 9 , 6 ,  3,
  0 , 8 , 5 ,  2,
  0 , 7 , 4 ,  1    };    

LiquidCrystal lcd(P2_2, P3_0, P3_1, P2_6, P2_3, P8_1); //LCD pin Configuration


//gobel variables
int lcd_backlight_value;                       // lcd_backlight_value value of light sensor

//
unsigned long alarm_time;//store alarm sec
unsigned long alarm_sec; //
unsigned long alarm_mint;
unsigned long alarm_hrs;
boolean alarm_status=false;
boolean automatic_lock_status=false;
unsigned long lock_time;
unsigned long lock_sec;
int lock_time_sec;//saurav
int locktime;
char a;
int i,j,k,t,w,x,y;
float temp;
void loop(){
  i=0;
main_menu:
  Serial1.flush();
  if(i<0) i=5;
  if(i<4){
    w=i;
    x=i+1;
    y=i+2;
  }
  if(i==4){
    w=i;
    x=i+1;
    y=5-5;
  }
  if(i==5){
    w=i;
    x=5-5;
    y=5-4;
  }
  if(i==6){ 
    i=0;
    w=i;
    x=i+1;
    y=i+2;
  }

  print_menu(menu[w],menu[x],menu[y],"");
  lcd.setCursor(0,3); 
  lcd.write(2); 
  lcd.write(2);  
  lcd.write(2); 
  lcd.print("OK"); 
  lcd.write(2); 
  lcd.write(2);  
  lcd.write(2); 
  lcd.print(" "); 
  lcd.write(2); 
  lcd.write(2);  
  lcd.print("CANCEL"); 
  lcd.write(2); 
  lcd.write(2);
  lcd.write(2);
  lcd.setCursor(19,0);
  lcd.write(3); 
  if(automatic_lock_status==true) get_lock_time();
  a = key();
  switch(a){

  case 'C':
    switch(i+1){
    case 1:
      pnr_query();
      pnr_post_result();
      break;

    case 2:
      device_verification(posturl,"CURRENT ASSIGNMENT:","assignment","75");
      post_result();
      break;

    case 3:
      settings();
      break;

    case 4:
      //tte_login_logout();
      break;

    case 5:
      break;

    case 6:
      break;
    }
    break;

  case 'A':
    i=i-1;  
    goto main_menu;
    break;

  case 'B':
    i=i+1;
    goto main_menu;
    break;


  default:
    goto main_menu;
  }
}



