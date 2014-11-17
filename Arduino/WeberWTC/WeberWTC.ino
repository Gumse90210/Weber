#include <FastIO.h>
#include <I2CIO.h>
#include <LCD.h>
#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>
#include <LiquidCrystal_SR.h>
#include <LiquidCrystal_SR2W.h>
#include <LiquidCrystal_SR3W.h>

#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SdFat.h>
#include <PWM.h>
#include <avr/pgmspace.h>

//use wpa2 or wep
//#define _WPA2
#define _WEP

//use DS18b20 or not
//#define _DALLAS

//demo build will generate measurement automatically
//also it will skip all user interactions like yes - no
#define _DEMO


#ifdef _DALLAS
  #include <DallasTemperature.h>
  #include <OneWire.h>
  
  #define ONE_WIRE_BUS 8
#endif


enum {
  NO_PROBES,    //0
  ET732_PROBES, //1
  //2
};

enum {
  BTN_DOWN = 6,
  BTN_UP = 5,
  BTN_RIGHT = 2,
  BTN_LEFT = 3,

  BTN_NO = BTN_LEFT,
  BTN_YES = BTN_RIGHT,
};

enum {
  LCD_INIT_CONNECTED,
  LCD_SAMPLING,
  LCD_SENDING_DATA_START,
  LCD_SENDING_DATA_END,
};

#define TEMP_MIN 0
#define TEMP_MAX 499


/*-----( Declare objects )-----*/
// set the LCD address to 0x27 for a 20 chars 4 line display
// Set the pins on the I2C chip used for LCD connections:
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

//wifi stuff
char ssid[25];  // your network SSID (name) 
char key[15];   // your network key
char probe_type = ET732_PROBES;

char wifi_enabled = false;
char sampling_enabled = false;

const int keyIndex = 0;               // your network key Index number
int status = WL_IDLE_STATUS;    // the Wifi radio's status

WiFiClient client;

//ip stuff
const IPAddress server(195, 128, 174, 119);

// the IP address for the shield:
const IPAddress ip(192, 168, 1, 120);
const IPAddress dns(192, 162, 153, 164);
const IPAddress gateway(192, 168, 1, 1);
const IPAddress subnet(255, 255, 255, 0);

#ifdef _DALLAS
  // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
  OneWire oneWire(ONE_WIRE_BUS);

  // Pass our oneWire reference to Dallas Temperature. 
  DallasTemperature sensors(&oneWire);
  float amb_t;
#endif


//values read from hw
int chn0, chn1, chn2, chn3;
int chn0_t, chn1_t, chn2_t, chn3_t;

int client_close_cnt = 0;
String server_resp = "110110050 ";  //default setpoint
int Setpoint = 110;
int pwm_duty = 0;
float pid_err = 0;

int Kp = 110;
int Ki = 50;

const char MaxOut = 100;
const char MinOut = 15;

SdFat sd;

enum {
  NO_LCD_CMD = 0,
  CLEAR_LCD_CMD,
  SHOW_READINGS_CMD,
  SHOW_SETTINGS_CMD
};

enum {
  TXT_ERROR_READING = 0,
  TXT_BBQ_MASTER,
  TXT_READING_CONF,
  TXT_SSID,
  TXT_KEY,
  TXT_PROBES,
  TXT_ENABLE_WIFI,
  TXT_CONNECTING,
  TXT_WAIT,
  TXT_CONNECTED,
  TXT_IP,
  TXT_RSSI,
  TXT_START_SAMPLING,
  TXT_DATA_INDICATOR_ON,
  TXT_DATA_INDICATOR_OFF,
  //TXT_READINGS,
  TXT_SETTINGS,
  TXT_TEMP_HEADINGS,
  TXT_YES_NO,
  TXT_DATA_INDICATOR_ERR,

  TXT_NO_TXT,

};

//                              "                    ";
prog_char  string_0[] PROGMEM = "Error Reading ...   ";
prog_char  string_1[] PROGMEM = "  BBQ Master 2014   ";
prog_char  string_2[] PROGMEM = "Reading Config ...  ";
prog_char  string_3[] PROGMEM = "SSID   : ";
prog_char  string_4[] PROGMEM = "Key    : ";
prog_char  string_5[] PROGMEM = "Probes : ";
prog_char  string_6[] PROGMEM = "Enable WiFi ?";
prog_char  string_7[] PROGMEM = "Connecting to WiFi  ";
prog_char  string_8[] PROGMEM = "Wait ...";
prog_char  string_9[] PROGMEM = "Connected to WiFi   ";
prog_char string_10[] PROGMEM = "IP   : ";
prog_char string_11[] PROGMEM = "RSSI : ";
prog_char string_12[] PROGMEM = "Start Sampling ?    ";
prog_char string_13[] PROGMEM = "+";
prog_char string_14[] PROGMEM = " ";
#ifdef _DALLAS
  prog_char string_15[] PROGMEM = "Ambient :";
#else
  //prog_char string_15[] PROGMEM = "Readings :";
  prog_char string_15[] PROGMEM = " SP  FAN    Kp   Ki ";
#endif
prog_char string_16[] PROGMEM =   " T0   T1    T2   T3 ";
prog_char string_17[] PROGMEM = "   No         Yes   ";
prog_char string_18[] PROGMEM = "-";
prog_char string_19[] PROGMEM = ""; //not used


PROGMEM const char *string_table[] = 	   // change "string_table" name to suit
{   
  string_0,
  string_1,
  string_2,
  string_3,
  string_4,
  string_5,
  string_6,
  string_7,
  string_8,
  string_9,
  string_10,
  string_11,
  string_12,
  string_13,
  string_14,
  string_15,
  string_16,
  string_17,
  string_18,
  string_19,
};


void setup() {
  
  //Serial.begin(9600);

  lcd.begin(20,4);         // initialize the lcd for 20 chars 4 lines, turn on backlight

  nLCDUpdate(CLEAR_LCD_CMD, 0, 0, (char)TXT_BBQ_MASTER, NULL);
  delay(2000);
  nLCDUpdate(NO_LCD_CMD, 0, 0, (char)TXT_READING_CONF, NULL);

#ifdef _DALLAS
  sensors.begin();
#endif
  pinMode(9, OUTPUT);  //pwmpin
  InitTimersSafe();
  SetPinFrequencySafe(9, 25000);
  /////
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin 
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output 
  // or the SD library functions will not work. 
  pinMode(10, OUTPUT);
  if (!sd.begin(4, SPI_HALF_SPEED)) 
    while(1);

  ReadFromFile( (char *)(("wifissid.txt")), ssid);
  ReadFromFile( (char *)(("wifipass.txt")), key);
  ReadFromFile( (char *)(("probetyp.txt")), &probe_type);
  probe_type -= 48;  //convert to value

  nLCDUpdate(NO_LCD_CMD, 0, 1, TXT_SSID, NULL);
  lcd.print(ssid);

  nLCDUpdate(NO_LCD_CMD, 0, 2, TXT_KEY, NULL);
  lcd.print(key);

  nLCDUpdate(NO_LCD_CMD, 0, 3, TXT_PROBES, NULL);
  if (probe_type == ET732_PROBES)
    lcd.print((F("ET732")));

  delay(5000);
 
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    // don't continue:
    while(true);
  }

  nLCDUpdate(CLEAR_LCD_CMD, 0, 1, TXT_ENABLE_WIFI, NULL);
#ifdef _DEMO
  if (1)
#else
  if (LCDPromptForYesNo(BTN_YES))
#endif
  {

#ifdef _WPA2
#else
    WiFi.config(ip, dns, gateway, subnet);
#endif

    // attempt to connect to Wifi network:
    nLCDUpdate(CLEAR_LCD_CMD, 0, 0, TXT_CONNECTING, NULL);
    nLCDUpdate(NO_LCD_CMD, 0, 1, TXT_WAIT, NULL);
    
#ifdef _WPA2
    while(WL_CONNECTED != WiFi.begin(ssid, key))
#else
    while(WL_CONNECTED != WiFi.begin(ssid, keyIndex, key))
#endif
    {
      // wait 10 seconds for connection:
      delay(10000);
    }
    wifi_enabled = true;

    // once you are connected :
    nLCDUpdate(CLEAR_LCD_CMD, 0, 0, TXT_CONNECTED, NULL);

    nLCDUpdate(NO_LCD_CMD, 0, 2, TXT_IP, NULL);
    lcd.print(WiFi.localIP());

    nLCDUpdate(NO_LCD_CMD, 0, 3, TXT_RSSI, NULL);
    lcd.print(WiFi.RSSI());

    delay(4000);
  }

  nLCDUpdate(CLEAR_LCD_CMD, 0, 1, TXT_START_SAMPLING, NULL);
#ifdef _DEMO
#else
  while( 0 == LCDPromptForYesNo(BTN_YES) );
#endif
  //sampling screen
  //nLCDUpdate(CLEAR_LCD_CMD, 0, 0, TXT_READINGS, NULL);
  nLCDUpdate(CLEAR_LCD_CMD, 0, 0, TXT_SETTINGS, NULL);
  nLCDUpdate(NO_LCD_CMD, 0, 2, TXT_TEMP_HEADINGS, NULL);
  nLCDUpdate(NO_LCD_CMD, 0, 3, TXT_NO_TXT, &chn0);

  sampling_enabled = true;
}


void loop() {

  static int loop_cnt = 1;

  chn0 = analogRead(0);
  chn1 = analogRead(1);
  chn2 = analogRead(2);
  chn3 = analogRead(3);
  
#ifdef _DEMO
  chn0 = 750 /*+ random(0, 58)*/;
  chn1 = 300 + random(0, 10);
  chn2 = 400 + random(0, 5);
  chn3 = 600 + random (0,2);
#endif

  chn0 = MovingAverage(chn0, 0);
  chn0_t = Thermister(chn0);

  chn1 = MovingAverage(chn1, 1);
  chn1_t = Thermister(chn1);

  chn2 = MovingAverage(chn2, 2);
  chn2_t = Thermister(chn2);

  chn3 = MovingAverage(chn3, 3);
  chn3_t = Thermister(chn3);
  
  if ((loop_cnt % 20) == 0)
  {
    
#ifdef _DALLAS
    nLCDUpdate(NO_LCD_CMD, 11, 0, TXT_NO_TXT, NULL);
    sensors.requestTemperatures(); // Send the command to get temperatures
    amb_t = sensors.getTempCByIndex(0);
    lcd.print(amb_t); 
    lcd.print((char)223);
#endif

    RunPid();
    pwmWrite(9, (pwm_duty * 255) / 100);
  
    nLCDUpdate(SHOW_SETTINGS_CMD, 0, 1, TXT_NO_TXT, &chn0);  //chn0 = dummy ptr
    nLCDUpdate(SHOW_READINGS_CMD, 0, 3, TXT_NO_TXT, &chn0);  //chno = dummy ptr
  }
  
  if ((loop_cnt % 100) == 0)
  {
    if (wifi_enabled)
    {
      nLCDUpdate(NO_LCD_CMD, 19, 0, TXT_DATA_INDICATOR_ON, NULL);
      SendDataToWeb();
      
      //hopefully we now got a setpoint
      int NewSetpoint = (int)server_resp.substring(0,3).toInt();
      if (NewSetpoint != 0)
      {
        Setpoint = NewSetpoint;
        Kp = server_resp.substring(3,6).toInt();  //take the chance with these two
        Ki = server_resp.substring(6,9).toInt();  // -
      }
      
      //run the pid regulator
      //RunPid();
      //Fan will probably stop below 20%
      //pwmWrite(9, (pwm_duty * 255) / 100);
      
      //nLCDUpdate(SHOW_SETTINGS_CMD, 0, 1, TXT_NO_TXT, &chn0);  //chn0 = dummy ptr
    }
  }

  if (client_close_cnt > 0)
    if (--client_close_cnt == 0)
    {
      client.stop();
      nLCDUpdate(NO_LCD_CMD, 19, 0, TXT_DATA_INDICATOR_OFF, NULL);
    }

  loop_cnt++;

  delay(100); 
}



void SendDataToWeb(void)
{
  char sp_string_begin = false;
  char buffer[25];
  char url[] = "http://www.dreamfabric.dk/arduino/arduino_update_data.php?";

  status = client.connect(server, 80);
  if (status)
  {
    client.print(("GET "));
    client.print(url);

    sprintf(buffer, "CHN0=%d&CHN1=%d&CHN2=%d&CHN3=%d&SP=%d&FAN=%d", chn0_t, chn1_t, chn2_t, chn3_t, Setpoint, pwm_duty);
    client.print(buffer);

    client.println((" HTTP/1.1"));
    client.println(("Host: www.dreamfabric.dk"));

    client.println(("Connection: close"));
    client.println();
    /////////////
    while(client.connected() && !client.available())
      delay(1); //waits for data
    
    while (client.connected() || client.available())
    { //connected or data available
      char c = client.read();
      if (c == '}')
      {
        sp_string_begin = false;
        //Serial.println(sp_string);
      }
      
      if (sp_string_begin)
      {
        server_resp += c;
      }
      if (c == '{')
      {
        sp_string_begin = true;
        server_resp = "";
      }
    }  
    ///////////
    client_close_cnt = 30;  //close after 30 x 100 ms = 3 sec
  }
  else
  {
    nLCDUpdate(NO_LCD_CMD, 19, 0, TXT_DATA_INDICATOR_ERR, NULL);
    client.stop();
  }
}

//void printWifiData()
//{
//  byte mac[6];  
//  WiFi.macAddress(mac);
//}

//void printCurrentNet()
//{
//  byte bssid[6];
//  WiFi.BSSID(bssid);    
//
//  long rssi = WiFi.RSSI();
//  byte encryption = WiFi.encryptionType();
//
//  IPAddress ip = WiFi.localIP();
//}


//et-732 4Mohm at 0 deg, 1 Mohm at 25 deg, 45000 at 100 deg
//4.332593352e-4
//2.070738117e-4
//2.272580700e-8
//22kOhm pullup

int Thermister(int aval)
{
  double R, T;

  if (probe_type == ET732_PROBES)
  {

    // These were calculated from the thermister data sheet
    //
    // This is the value of the other half of the voltage divider
    //	Rknown = 22200;

    // Do the log once so as not to do it 4 times in the equation
    //	R = log(((1023/(double)aval)-1)*(double)22200);
    R = log((1 / ((1023 / (double) aval) - 1)) * (double) 21800);

    // Compute degrees C
    T = (1 / ((4.332593352e-4) + (2.070738117e-4) * R + (2.272580700e-8) * R * R * R)) - 273.15;

  }

  //limit range
  if (T < TEMP_MIN) T = TEMP_MIN;
  if (T > TEMP_MAX) T = TEMP_MAX;

  return ((int)T);
}


int MovingAverage(int sample, const unsigned char no)
{
  static int sample_array[4][10];
  int sample_sum = 0;
  unsigned char i;

  for (i = 0; i < 9; i++)
  {
    sample_array[no][i] = sample_array[no][i + 1];
    sample_sum += sample_array[no][i];
  }

  sample_array[no][9] = sample;

  sample_sum += sample;
  sample_sum /= 10;

  return sample_sum;
}


int LCDPromptForYesNo(int button)
{
  int return_value = 0;
  int btn_no, btn_yes;


  nLCDUpdate(NO_LCD_CMD, 0, 3, TXT_YES_NO, NULL);

  pinMode(BTN_YES, INPUT);
  digitalWrite(BTN_YES, LOW);       // turn off pullup resistors

  pinMode(BTN_NO, INPUT);
  digitalWrite(BTN_NO, LOW);       // turn off pullup resistors

  do
  {
    btn_no  = digitalRead(BTN_NO);
    btn_yes  = digitalRead(BTN_YES);
    
    delay(100);
  }
  while ((btn_no != 1) && (btn_yes != 1));

  if (btn_no == 1)
  {
    if (button == BTN_NO)
      return_value = 1;
  }
  else
    if (btn_yes == 1)
    {
      if (button == BTN_YES)
        return_value = 1;
    }

  return return_value;
}



void ReadFromFile(char *filename, char *dest)
{
  SdFile myFile;

  // open the file for reading:
  char index = 0;
  char a;

  if (myFile.open(filename, O_READ))
  {
    // read from the file until we hit a '#'
    do
    {
      a = myFile.read();

      if (a != '#')
        dest[index++] = a;

    } 
    while (a != '#');

    // close the file:
    myFile.close();
  }
  else
  {
    // if the file didn't open, print an error:
    nLCDUpdate(CLEAR_LCD_CMD, 0, 0, TXT_ERROR_READING, NULL);
    nLCDUpdate(NO_LCD_CMD, 0, 1, TXT_NO_TXT, NULL);
    lcd.print(filename);

    while(true);
  }
}


void nLCDUpdate(char lcd_cmd, char lcd_pos, char lcd_line, char lcd_txt, void *data)
{
  char lcd_buffer[20+1];

  if (lcd_cmd == CLEAR_LCD_CMD)
    lcd.clear();

  if ((lcd_pos != 0xFF) && (lcd_line != 0xFF))
    lcd.setCursor(lcd_pos, lcd_line);

  if (data == NULL)
  {
    if (lcd_txt != TXT_NO_TXT)
    {
      strcpy_P(lcd_buffer, (char*)pgm_read_word(&(string_table[lcd_txt])));
      lcd.print(lcd_buffer);
    }
  }
  if (lcd_cmd == SHOW_READINGS_CMD)
  {
    sprintf(lcd_buffer, "%03d  %03d   %03d  %03d ", chn0_t, chn1_t, chn2_t, chn3_t);

    lcd_buffer[3] = (char)223;  //adding degree character
    lcd_buffer[8] = (char)223;
    lcd_buffer[14] = (char)223;
    lcd_buffer[19] = (char)223;

    lcd.print(lcd_buffer);
  }
  if (lcd_cmd == SHOW_SETTINGS_CMD)
  {
    sprintf(lcd_buffer, "%03d  %03d   %03d  %03d ", Setpoint, pwm_duty, Kp, Ki);

    lcd_buffer[3] = (char)223;  //adding degree char
    lcd_buffer[8] = (char)37;   //adding percentage char

    lcd.print(lcd_buffer);
  }

}

void RunPid(void)
{
  static float I_err = 0;
  
  float P_err;
  
  pid_err = Setpoint - chn0_t;
  
  I_err += ((float)Ki * pid_err)/100;
  
  if (I_err > MaxOut)
    I_err = MaxOut;
  if (I_err < MinOut)
    I_err = MinOut;
    
  P_err = ((float)Kp * pid_err)/100.0f + I_err;
  
  if (P_err > MaxOut)
    P_err = MaxOut;
  if (P_err < MinOut)
    P_err = MinOut;
  
  pwm_duty = (int)P_err;
  //return (int)(P_err);
}

