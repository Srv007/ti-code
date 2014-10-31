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

char pnrdata[40][4][20]; //storage for pnr numbers history
int current_pnr_no=0;
char ttedata[40][20]; //storage for pnr numbers history
int current_tte_no=0;
char data[4][30];
//post request 
String tteurl_statement="Enter TTE CODE:";
String pnrurl_statement="Enter PNR CODE:";
String ticketanalysisurl_statement="ENTER STATION CODE:";
String tteurl="/srv.php?ttecode=";
String posturl="/atcad_handler.php";
String host="www.ti-atcad.com";

/gobel variables
int lcd_backlight_value;                       // lcd_backlight_value value of light sensor
int backlight_value;  
boolean backlight_status;     // backlight status
boolean temp_backlight_status;     // backlight status
boolean vibration_status;     // backlight status
String id_card="4E00707BD590";//id card number of tte for unlock device
String password=("1234");
char device_number[]="SLVE_9856"; //This is device number//9856 is password
char tte_code[12];
//globle variables for Device Uptime
unsigned long time;
unsigned long sec;
unsigned long mint;
unsigned long hrs;
//Globle Variables for real time dislay
int real_sec;
int real_mint;
int real_hrs;
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




void setup(){
  backlight_status=true;
  pinMode( button1, INPUT_PULLUP); 
  pinMode( button2, INPUT_PULLUP); 
  pinMode( lock_button, INPUT_PULLUP); 
  pinMode( button3, INPUT_PULLDOWN); 
  pinMode( vibraton_pin, OUTPUT);
  digitalWrite( vibraton_pin, LOW);
  TwoMsTimer::set(200, automatic_bacllight);
  TwoMsTimer::start();
  Serial1.begin(9600);   // initialize UART communication @ 9600 bps serial port 1 is used pin P3.3 and P3.4 are used 
  rfid.begin(9600); 
  delay(1000);
  lcd.clear();
  lcd.print("       ATCAD");
  // initialize UART communication with RF-ID @ 9600 bps software-serial is used
  for (x=0; x<4; x++)  {
    pinMode(column[x], OUTPUT);
  }
  for (y=0; y<4; y++)  {
    pinMode(row[y], INPUT_PULLUP);
  } 
 // connect();
  //
//  main_lock();
  lcd.clear();
  get_tte_number();
  tte_code[12]='\0';
  lcd.setCursor(0,3);
  lcd.print("Press to continue...");
  key();
  lcd.createChar(2, black);
  lcd.createChar(3, arrow);
  backlight_status==true;
  
  
}
void loop(){
 
}



