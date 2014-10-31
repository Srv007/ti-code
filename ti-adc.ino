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
char menu[][20]={
  "1-PNR QUERY",
  "2-CURR. ASSIGNMENT",
  "3-SETTINGS",
  "4-TTE LOGOUT",
  "5-MENU_5",
  "6-MENU_6"
};
byte black[8] = {
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
}; 
byte arrow[8] = {
  0b00011,
  0b00110,
  0b01100,
  0b11111,
  0b11111,
  0b01100,
  0b00110,
  0b00011 
};

void automatic_bacllight(void){
  if(automatic_lock_status==true){
    lock_time=millis();
    lock_sec=lock_time/1000;
    if(lock_sec>lock_time_sec) device_lock();//saurav
  }

  if(backlight_status==true){
    lcd_backlight_value=analogRead(A3);
    lcd_backlight_value=lcd_backlight_value/12;
    lcd_backlight_value=255-lcd_backlight_value;
    if(lcd_backlight_value<0) lcd_backlight_value=0;
    if(backlight_value<lcd_backlight_value) analogWrite(P1_4,backlight_value++);
    if(backlight_value>lcd_backlight_value) analogWrite(P1_4,backlight_value--);
  }
  //check alarm status and turn on alarm if time is over
  if(alarm_status==true){
    time=millis();// put your main code here, to run repeatedly:
    time=time/1000;
    if(time>alarm_time){
      TwoMsTimer::stop();
      lcd.clear();
      lcd.print("ALARM is on");
      key();
      alarm_status=false;
      TwoMsTimer::start();
    }
  }
} //end of automatic
//return button press on keypad in integer
int keypress_int() {
  int keypress;
  for (int x=0; x<4; x++)  {
    digitalWrite(column[x], HIGH);
  }
  keypress='z';
  while (1)  {
    for (int x=0; x<4; x++)  {
      digitalWrite(column[x], LOW);
      for (int y=0; y<4; y++) {
        if (!(digitalRead(row[y]))) {
          digitalWrite(vibraton_pin,LOW);
          keypress =Keypad_int[x] [y];
          while(!(digitalRead(row[y])));
          if(keypress!='z') goto out;
        }
      }
      digitalWrite(column[x], HIGH);
      if(keypress!='z') break;
    }
out:;
    if(keypress!='z') break;
  }
  delay(10);
  digitalWrite(vibraton_pin,LOW);
  return keypress;
}
char keypress() {
  char keypress;
  for (x=0; x<4; x++)  {
    digitalWrite(column[x], HIGH);
  }
  keypress='z';
  while (1)  {
    for (x=0; x<4; x++)  {
      digitalWrite(column[x], LOW);
      for (y=0; y<4; y++) {
        if (!(digitalRead(row[y]))) {
          digitalWrite(vibraton_pin,LOW);
          keypress =Keypad[x] [y];
          if(keypress!='z') goto out;
        }
      }
      digitalWrite(column[x], HIGH);
      if(keypress!='z') break;
    }
out:;
    if(keypress!='z') break;
  }//end while(1)
  delay(10);
  digitalWrite(vibraton_pin,LOW);
  return keypress;
}//  end of keypress_int()
char key(){
  char keypress;
  for (x=0; x<4; x++)  {
    digitalWrite(column[x], HIGH);
  }
  keypress='z';
  while (1)  {
    if (digitalRead(button1)==LOW) function1();
    if (digitalRead(button2)==LOW) function2();
    if (digitalRead(button3)==HIGH) function3();
    if (digitalRead(lock_button)==LOW) {
      device_lock();
      break;
    }
    for (x=0; x<4; x++)  {
      digitalWrite(column[x], LOW);
      for (y=0; y<4; y++) {
        if (!(digitalRead(row[y]))) {
          digitalWrite(vibraton_pin,LOW);
          keypress =Keypad[x] [y];
          while(!(digitalRead(row[y])));
          if(keypress!='z') goto out;
        }
      }
      digitalWrite(column[x], HIGH);
      if(keypress!='z') break;
    }
out:;
    if(keypress!='z') break;
  }
  digitalWrite(vibraton_pin,LOW);
  return keypress;
}
char keylock() {
  char keypress;
  for (x=0; x<4; x++)  {
    digitalWrite(column[x], HIGH);
  }
  keypress='z';
  while (1)  {
    if(digitalRead(lock_button)==LOW) {
      device_lock();
      break;
    }
    for (x=0; x<4; x++)  {
      digitalWrite(column[x], LOW);
      for (y=0; y<4; y++) {
        if (!(digitalRead(row[y]))) {
          digitalWrite(vibraton_pin,LOW);
          keypress =Keypad[x] [y];
          while(!(digitalRead(row[y])));
          if(keypress!='z') goto out;
        }
      }
      digitalWrite(column[x], HIGH);
      if(keypress!='z') break;
    }
out:;
    if(keypress!='z') break;
  }
  digitalWrite(vibraton_pin,LOW);
  return keypress;
}
//function to get smart card number
String smart_card(void){
  String smart_card_number;
  rfid.flush();
  i=0;
  while(i<12){
    if (digitalRead(button3)==HIGH) loop();
    if(rfid.available()){
      a=rfid.read();
      smart_card_number.concat(a);
      i++;
    }
  }
  rfid.println(smart_card_number);
  return smart_card_number;
}
//main lock
int main_lock(void){
  String user_enter_password;
  String code=String(12);
  print_menu("        LOCK    ","1-Manual","2-ID CARD","");
  a=keylock();
  switch(a){
  case '1':
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("ENTER PASSWORD:");
    lcd.setCursor(0, 1);
    lcd.print("       ");
    lcd.setCursor(0, 1);
    lcd.blink();
    for  (i=0;i<4;i++){
      a = keylock();
      lcd.print("*");
      user_enter_password.concat(a);	
    }
    lcd.noBlink();
    lcd.setCursor(0, 1);
    digitalWrite(vibraton_pin,LOW);
    if ((password.compareTo(user_enter_password))==0){
      lcd.print("TRUE ");
    }
    else{
      lcd.print("WORNG");
      delay(1000);
      digitalWrite(vibraton_pin,LOW);
      main_lock();
    }
    break;

  case '2':
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Swipe Your Card:");
    code=smart_card();
    lcd.setCursor(0,1);
    lcd.print(code);
    lcd.setCursor(0,1);
    lcd.print("********");
    if ((id_card.compareTo(code))==0){
      lcd.print("TRUE ");
    }
    else{
      lcd.print("WORNG");
      delay(1000);
      digitalWrite(vibraton_pin,LOW);
      main_lock();
    }
    break;

  default:
    main_lock();
    break;
  }
  lcd.clear();
  digitalWrite(vibraton_pin,LOW);
  lcd.noBlink();
  return 0;
}

/lock function to call any where
int device_lock(void){
  if(automatic_lock_status==true) get_lock_time();
  String user_enter_password;
  String code=String(12);
  lcd.clear();
  lcd.print("        LOCK");
  delay(500);
  temp_backlight_status=backlight_status;
  backlight_status=false;
  delay(500);
  lcd.clear();
  analogWrite(P1_4,0);
  while(digitalRead(lock_button)==HIGH);
  lcd.clear();
  backlight_status=temp_backlight_status;
  analogWrite(P1_4,244);
  print_menu("        LOCK    ","1-Manual","2-ID CARD","");
  delay(500);
  a=keylock();
  switch(a){
  case '1':
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("ENTER PASSWORD:");
    lcd.setCursor(0, 1);
    lcd.print("       ");
    lcd.setCursor(0, 1);
    lcd.blink();
    for  (i=0;i<4;i++){
      a = keylock();
      lcd.print("*");
      user_enter_password.concat(a);	
    }
    lcd.noBlink();
    lcd.setCursor(0, 1);
    digitalWrite(vibraton_pin,LOW);
    if ((password.compareTo(user_enter_password))==0){
      lcd.print("TRUE ");
      TwoMsTimer::start();
    }
    else{
      lcd.print("WORNG");
      delay(1000);
      digitalWrite(vibraton_pin,LOW);
      device_lock();
      break;
    }
    break;

  case '2':
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Swipe Your Card:");
    code=smart_card();
    lcd.setCursor(0,1);
    lcd.print(code);
    lcd.setCursor(0,1);
    lcd.print("********");
    if ((id_card.compareTo(code))==0){
      lcd.print("TRUE ");
    }
    else{
      lcd.print("WORNG");
      delay(1000);
      digitalWrite(vibraton_pin,LOW);
      device_lock();
      break;
    }
    break;

  default:
    device_lock();
    break;
  }
  lcd.clear();
  digitalWrite(vibraton_pin,LOW);
  lcd.noBlink();
  loop();
  return 0;
}
void send_cmd(String cmd,char *response,int wait){
  delay(100);
  Serial1.flush();
  Serial1.println(" ");
  delay(10);
  Serial1.println(cmd);
  Serial1.println(" ");
  Serial1.setTimeout(wait); 
  while(1){
    if(Serial1.available())  break;
  }
  if(Serial1.find(response)) lcd.print("*");
}
  Serial1.setTimeout(wait); 
  while(1){
    if(Serial1.available())  break;
  }
  if(Serial1.find(response)) lcd.print("*");
}

void CIICR(void){
  delay(100);
  Serial1.setTimeout(5000);
  Serial1.println("AT+CIFSR");
  if(Serial1.find("ERROR")){
    send_cmd("AT+CIICR","OK",1000); 
    CIICR();
  }
}
void CCR(void){
  delay(100);
  Serial1.setTimeout(5000);
  Serial1.println("AT+CIFSR");
  if(Serial1.find("ERROR")){
    send_cmd("AT+CIICR","OK",1000); 
    CIICR();
  }
}
void query_get(String query_url, String query_statement)
{
  Serial1.flush();
  lcd.clear();
  t=0;
  String parameter(10);
  lcd.setCursor(0,0);
  lcd.print(query_statement);
  lcd.setCursor(0,1);
  for(i=0;i<10;i++){
    a = keylock();
    lcd.print(a);
    parameter.concat(a);	
  }
  CIPSTART(host);
  Serial1.println("AT+CIPSEND");
  delay(500);
  Serial1.print("GET ");
  Serial1.print(query_url);
  Serial1.print(parameter);
  Serial1.println(" HTTP/1.1");
  Serial1.print("HOST:");
  Serial1.println(host);
  Serial1.println("User-Agent: ATCAD");
  Serial1.write(10);
  Serial1.write(26);
  delay(4000);
}

void CIPSTART(String host)
{
  Serial1.flush();
  Serial1.print("AT+CIPSTART=\"TCP\",\"");
  Serial1.print(host);
  Serial1.println("\",\"80\"");
  Serial1.find("OK");
  Serial1.setTimeout(8000);
  while(1){
    if(Serial1.available()) break;
  }
  if((Serial1.find("CONNECT OK"))) lcd.print("Connected");
  else{
    if(t>3){
      send_cmd("AT+CIPCLOSE","CLOSE OK",5000);
      loop();
    }
    t++;
    lcd.clear();
    lcd.print("NOT CONNECTED");
    CIPSTART(host);
  }
}//

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



