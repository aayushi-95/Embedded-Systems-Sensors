
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <AsyncDelay.h>

AsyncDelay delay_3s;
char mssid[20] = "ahoy_net";
char mpassword[20] = "9810541423";
char default_auth[20]= "ahoyrl2";
char did[10];
String query;
boolean beagle_alert = false;
char master_auth[20];
boolean play_file = false;
char default_did[10]="808080";
char ssid[15];
char password[15];
char file_to_play[6];

String alert_code;
char *mp3_host = "192.168.10.209";
int mp3_port = 200;
char *wr600_host = "192.168.10.206";
int wr600_port = 8080;
boolean _connected;

char auth_table[5][10];

IPAddress ip(192, 168, 10, 202);  
IPAddress gateway(192, 168, 10, 1);  

IPAddress subnet(255, 255, 255, 0);  

ESP8266WebServer server(330);

int curr_num_auth = 0;
String send_string;
void clear_eeprom();
void load_master();
void load_auth();
void load_credentials();
void handle_root();
void handle_op();
void handle_add_auth();
void handle_rem_auth();
void handle_chg_master();
void handle_credentials();
void handle_did();

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
//clear_eeprom();
load_master();
load_auth();
load_credentials();
WiFi.mode(WIFI_STA);
_connected = false;
connect_wifi();
while (WiFi.status() != WL_CONNECTED ) 
    {
         delay(500);
        Serial.print(".");
      }
server.on("/",handle_root);
server.on("/op",handle_op);
server.on("/add_auth",handle_add_auth);
server.on("/rem_auth",handle_rem_auth);
server.on("/chg_master",handle_chg_master);
server.on("/credentials",handle_credentials);
server.on("/chg_did",handle_did);
server.begin();
delay_3s.start(3000, AsyncDelay::MILLIS);
}

void loop() {
  // put your main code here, to run repeatedly:
yield();
server.handleClient();
if(delay_3s.isExpired())
{
  if(WiFi.status()==WL_CONNECTED)
  Serial.println("@");
  delay_3s.repeat(); // Count from when the delay expired, not now
}
if(WiFi.status()!=WL_CONNECTED)
{
  connect_wifi();
}
if(play_file)
{
 // server.stop();
     HTTPClient http;
    query = "http://192.168.10.209:200/play?file="+String(file_to_play)+"&auth="+String(did);
    Serial.print(query);
    http.begin(send_string);
    int httpCode = http.GET();
    http.end();  
     play_file = false;
  }
 
if(beagle_alert)
{
  //server.stop();
  HTTPClient http;
  query = "http://192.168.10.206:8080/api/comm.php?code="+alert_code+"&did="+String(did);
  Serial.println(query);
  http.begin(send_string);
  int httpCode = http.GET();
  http.end();
    beagle_alert = false;
  }

  //server.begin();
}




void handle_root()
{
  String msg = "In Root";
  Serial.println(msg);
  server.send(200,"text/plain",msg);
}

void handle_add_auth()
{
 Serial.println("Entered Handle Add Auth");
 String msg;
 char auth[10];
 int len,i;
 server.arg("auth").toCharArray(auth,sizeof(auth));
 len = strlen(auth);
 Serial.println(auth);
 Serial.println(len);
 if(!(server.hasArg("auth") && server.hasArg("master_auth")))
   msg = "Incorrect Arguments";
 else if (server.arg("master_auth") != String(master_auth))
 {
   
   msg = "Master Authentication Failed";
 } 
 else if (len!=6)
 {
  msg = "Authentication code 6 chars only";
 }
 else if (curr_num_auth == 5)
  msg = "Authentication full";
 else if (!isValid(auth,6))
  msg = "Invalid Characters";
 else
 {
   for(i=0;i<curr_num_auth;i++)
   {
      if(strcmp(auth,auth_table[i])==0)
      {
          Serial.println("Duplicate");
          msg = String(auth) + "already exists";
          break;
      }
   }
   if(i==curr_num_auth)
   {
   strcpy(auth_table[curr_num_auth++],auth);
   save_auth_table();
   load_auth();
   msg = "Authentication Added";
   }
 }
  server.send(200,"text/plain",msg);
 }

void handle_op()
{
  Serial.println("Entered Handle Op");
  char relay[2],auth[10],state[2];
  int r_len, a_len, m_len,s_len;
  String msg;
  server.arg("relay").toCharArray(relay,sizeof(relay));
  server.arg("auth").toCharArray(auth,sizeof(auth));
  server.arg("mp3").toCharArray(file_to_play,sizeof(file_to_play));
  server.arg("state").toCharArray(state,sizeof(state));
  r_len = strlen(relay);
  a_len = strlen(auth);
  m_len = strlen(file_to_play);
  s_len = strlen(state);
  if(!(server.hasArg("relay") && server.hasArg("auth") && server.hasArg("state")))
  {
    msg = "Missing Arguments";
  }
  else if (r_len != 1|| a_len != 6 || r_len != 1 ||s_len != 1)
  {
    msg = "Invalid Length";
  }
  else if(!check_auth(auth))
  {
    msg = "authentication failed";
  }
  else if(!(relay[0]== '1' || relay[0] == '2'))
  {
    msg = "Invalid Relay";
  }
  else if (!(state[0] =='0' || state[0] == '1' ))
  {
    msg = "Invalid State";
  }
  else 
  {
    msg = perform_action(relay[0],state[0]);
    alert_code = msg;
    beagle_alert =  true;
  }
  if(server.hasArg("mp3")|| m_len == 4)
  {
    play_file= true;
  }
  Serial.println(msg);
  server.send(200,"text/plain",msg);
  Serial.println("Exit Handle Op");
}

void handle_rem_auth()
{
  String msg;
 char auth[10];
 int len,i,j;
 j= curr_num_auth -1;
 server.arg("auth").toCharArray(auth,sizeof(auth));
 len = strlen(auth);
 Serial.println(auth);
 Serial.println(len);
 if(!(server.hasArg("auth") && server.hasArg("master_auth")))
   msg = "Incorrect Arguments";
 else if (server.arg("master_auth") != String(master_auth))
 {
   
   msg = "Master Authentication Failed";
 } 
 else if(len != 6)
   msg = "Invalid";
 else if(!isValid(auth,6))
   msg = "Invalid";
 else
 {
  msg = "Not Found";
   for(i=0;i<=j;i++)
   {
    Serial.println(auth_table[i]);
    if(strcmp(auth,auth_table[i])==0)
    {
      strcpy(auth_table[i],auth_table[j]);
      Serial.println(auth_table[j]);
      memset(auth_table[j],0,sizeof(auth_table[j]));
      Serial.println(auth_table[j]);
      msg = "Removed " + String(auth);
      save_auth_table();
      load_auth();
      break;
    }
   }
 }
  Serial.println(msg);
  server.send(200,"text/plain",msg);
}

void handle_chg_master()
{
  Serial.println("Entered Change Master");
  String msg;
  int i;
  char tmp1[20], tmp2[20];
  boolean valid = true;
  server.arg("new").toCharArray(tmp1,sizeof(tmp1));
  server.arg("old").toCharArray(tmp2,sizeof(tmp2));
  int m_len = strlen(tmp1);
  if(!strcmp(master_auth,tmp2)==0)
    msg = "Authentication Failed";
  else if(m_len>15 || m_len<6)
    msg = "Length error";
  else 
  {
    for(i=0;i<m_len;i++)
      if(!isAlphaNumeric(tmp1[i]))
      {
        msg = "Auth is alphanumeric";
        break;
        valid = false;
      }
    if(valid)
    {
      strcpy(master_auth,tmp1);
      EEPROM.begin(512);
      EEPROM.put(200,master_auth);
      EEPROM.commit();
      EEPROM.end();
      load_master();
      msg = "Updated master to " + String(master_auth);
    }
      
  }
  Serial.println(msg);
  server.send(200,"text/plain",msg);
}

void load_master()
{
  Serial.println("Entered Load Master");
  
  EEPROM.begin(512);
  EEPROM.get(200,master_auth);
  int len = strlen(master_auth);
  int i;
  boolean valid = true;
  
  for(i=0;i<len;i++)
  {
    if(!isAlphaNumeric(master_auth[i]))
    {
      valid = false;
      break;
    }
  }
  if(!valid || len == 0)
     strcpy(master_auth,default_auth);
  Serial.println(master_auth);
  EEPROM.get(250,did);
  len = strlen(did);
  if(len != 6)
    strcpy(did,default_did);
  EEPROM.end();
}

void handle_credentials()
{
  Serial.println("Entered Handle Update Credentails");
  char temp_ssid[15],temp_password[15];
  int s_len,p_len,i;
  String msg;
  boolean valid = true;
  server.arg("ssid").toCharArray(temp_ssid,sizeof(temp_ssid));
  s_len = strlen(temp_ssid);
  server.arg("password").toCharArray(temp_password,sizeof(temp_password));
  p_len = strlen(temp_password);
  Serial.println(temp_ssid);
  Serial.println(s_len);
  Serial.println(temp_password);
  Serial.println(p_len);
  if(!(server.hasArg("ssid") && server.hasArg("password") && server.hasArg("master_auth")))
   msg = "Incorrect Arguments";
  else if (server.arg("master_auth") != String(master_auth))  
   msg = "Master Authentication Failed";
  else if(p_len < 8 || s_len < 8 || p_len >13 || s_len>13) 
   msg = "SSID and password should be 10 characters long";
 else
 {
  for(i=0;i<p_len;i++)
  {
    if(!isAlphaNumeric(temp_password[i]))
     {
      valid = false;
      break;
     }
  }
  for(i=0;i<p_len;i++)
  {
    if(!isAlphaNumeric(temp_password[i]))
     {
      valid = false;
      break;
     }
  }
  if(!valid)  
     msg = "SSID and password can only be alphanumeric";
  else
  {
    EEPROM.begin(512);
    EEPROM.put(0,temp_ssid);
    EEPROM.put(15,temp_password);
    EEPROM.commit();
    EEPROM.end();
    EEPROM.begin(512);
    EEPROM.get(0,temp_ssid);
    EEPROM.get(15,temp_password);
    EEPROM.end();
    Serial.println(temp_ssid);
    Serial.println(temp_password);
    msg = "Successfully updated wifi credentials";
  }
 }
 Serial.println(msg);
 server.send(200,"text/plain",msg);
}

void handle_did()
{
  Serial.println("Entered Handle Update DID");
 String msg;
 char key[10];
 int len,i;
 server.arg("did").toCharArray(key,sizeof(key));
 len = strlen(key);
 Serial.println(key);
 Serial.println(len);
 if(!(server.hasArg("did") && server.hasArg("master_auth")))
   msg = "Incorrect Arguments";
 else if (server.arg("master_auth") != String(master_auth))
 {
   
   msg = "Master Authentication Failed";
 } 
 else if (len!=6)
 {
  msg = "DID 6 chars only";
 }

 else if (!isValid(key,6))
  msg = "Invalid Characters";
 else
 {
  
    EEPROM.begin(512);
    EEPROM.put(250,key);
    EEPROM.commit();
    EEPROM.end();
    msg = "Updated DID";
    EEPROM.begin(512);
    EEPROM.get(250,did);
    EEPROM.end();
    Serial.println(did);
  
  }
 Serial.println(msg);
 server.send(200,"text/plain",msg);
}

boolean check_auth(char auth[])
{
  int i;
  for(i=0;i<curr_num_auth;i++)
  {
    if(strcmp(auth,auth_table[i])== 0)
       return true;
  }
  return 0;
  }

String perform_action(char relay, char state)
{
  String msg = "R" + String(relay);
  int trys = 0;
  if(state == '1')
    msg = msg + "_on";
  else 
    msg = msg + "_off";
  String cmd = "+" + String(relay) + String (state) + "\r";
  do
  {
     Serial.print(cmd);    
     trys++;
      delay(200);
  }while(!Serial.find("@OK") && trys < 10);
  if(trys == 10)
    msg = "Timeout";
  return msg;
}

void load_auth()
{
Serial.println("Entered Load Auth");
int len = 0;
EEPROM.begin(512);
Serial.println("Loading Authorization");
int i;
for(i=0;i<5;i++)
{
 EEPROM.get(30+10*i,auth_table[i]);
 Serial.println(auth_table[i]);
 len = strlen(auth_table[i]);

 if(len==0)
 {
    
    break;
 }
}
curr_num_auth = i;
EEPROM.end();
Serial.println(curr_num_auth);  
}

void load_credentials()
{
EEPROM.begin(512);
Serial.println("Loading WiFi Credentials");
EEPROM.get(0,ssid);
EEPROM.get(15,password);
Serial.println(ssid);
Serial.println(password);
EEPROM.end();
}

boolean isValid(char str[],int len)
{
  int i;
  char c;
  for (i = 0;i<len;i++)
  {
     c = str[i];
     if(!((c>='0' && c<='9')||(c>='A' && c<='D')))
       return 0;
  }
  return 1;
}

void save_auth_table()
{
  Serial.println("Entered save auth table");
  EEPROM.begin(512);
  int i;
  for(i=0;i<curr_num_auth;i++)
    EEPROM.put(30+10*i,auth_table[i]);
  EEPROM.commit();
  EEPROM.end();
}


void clear_eeprom()
{
 EEPROM.begin(512);
 long i;
 for(i=0;i<512;i++)
 {
  //if(EEPROM.read(i)==0xFF)
    EEPROM.write(i,0);
 }
 Serial.println("Cleared EEPROM");
 EEPROM.commit();
 EEPROM.end();
}

void connect_wifi()
{
  int trys = 0;
  Serial.println("Connecting to Master");
  WiFi.begin(mssid,mpassword);    
  WiFi.config(ip, gateway, subnet);
  while (WiFi.status() != WL_CONNECTED && trys<20) 
    {
        delay(500);
        Serial.print(".");
    trys++;
    }
  
  if(trys<20)
  {
    _connected = true;
    return;
  }
  Serial.println("Connecting to Secondary WiFi");
  WiFi.begin(ssid,password);    
  WiFi.config(ip, gateway, subnet);
  trys =0;
    while (WiFi.status() != WL_CONNECTED && trys<20) 
    {
        delay(500);
        Serial.print(".");
    trys++;
    }
  if(trys<20)
  {
    _connected = true;
  }
}


