////
//
// General code from http://www.pushingbox.com for Arduino GSM shield (official) v1.0
// adapted from the original code for the WiFi shield
// created 21 October 2014
// by Simon Schvartzman
//
////

#include <GSM.h>
// PIN Number
#define PINNUMBER ""

// APN data (you may need to modify it according to your data provider, in most cases there is no need to replace)
#define GPRS_APN       "GPRS_APN" // replace your GPRS APN
#define GPRS_LOGIN     "login"    // replace with your GPRS login
#define GPRS_PASSWORD  "password" // replace with your GPRS password

// initialize the library instance
GSMClient client;
GPRS gprs;
GSM gsmAccess; 
 

char DEVID1[] = "Your_DevID_Here";        //Scenario : "The mailbox is open"

//Numeric Pin where you connect your switch
uint8_t pinDevid1 = 6; // Example : the mailbox switch is connect to the Pin 6.
                       // Please take into account you can't use Pin 3 (as in the Wifi example)
                       // because it is used by the GSM module

// Debug mode
boolean DEBUG = true;
  //////////////
 //   End    //
//////////////


char serverName[] = "api.pushingbox.com";
int port = 80; // port 80 is the default for HTTP

boolean pinDevid1State = false;                // Save the last state of the Pin for DEVID1
boolean lastConnected = false;  


void setup() 
{
  // initialize serial:
  Serial.begin(9600);
  pinMode(pinDevid1, INPUT);
 
  Serial.println("Connecting to GSM network... ");
  // connection state
  boolean notConnected = true;

  // After starting the modem with GSM.begin()
  // attach the shield to the GPRS network with the APN, login and password
  while(notConnected)
  {
    if((gsmAccess.begin(PINNUMBER)==GSM_READY) &
      (gprs.attachGPRS(GPRS_APN, GPRS_LOGIN, GPRS_PASSWORD)==GPRS_READY))
    {
      notConnected = false;
      Serial.println ("Connected");
    }
    else
    {
      Serial.println("Not connected");
      delay(1000);
    }
  }
 }


void loop() {
   ////
   // Listening for the pinDevid1 state
   ////
  if (digitalRead(pinDevid1) == HIGH && pinDevid1State == false) // switch on pinDevid1 is ON
  {
    if(DEBUG){Serial.println("pinDevid1 is HIGH");}
    pinDevid1State = true;
    //Sending request to PushingBox when the pin is HIGHT
    sendToPushingBox(DEVID1);
  }
   if (digitalRead(pinDevid1) == LOW && pinDevid1State == true) // switch on pinDevid1 is OFF
  {
    if(DEBUG){Serial.println("pinDevid1 is LOW");}
    pinDevid1State = false;
    //Sending request to PushingBox when the pin is LOW
    //sendToPushingBox(DEVID1);    //Here you can run an other scenario by creating a DEVID2 variable
  }
 
  //DEBUG part
  // this write the respons from PushingBox Server.
  // You should see a "200 OK"
  if (client.available()) {
    char c = client.read();
    if(DEBUG){Serial.print(c);}
  }
 
  // if there's no net connection, but there was one last time
  // through the loop, then stop the client:
  if (!client.connected() && lastConnected) {
    if(DEBUG){Serial.println();}
    if(DEBUG){Serial.println("disconnecting.");}
    client.stop();
  }
  lastConnected = client.connected();
  
}


//Function for sending the request to PushingBox
void sendToPushingBox(char devid[]){
  client.stop(); if(DEBUG){Serial.println("connecting to PushingBox...");}
  if(client.connect(serverName, 80)) { 
    if(DEBUG){Serial.println("connected");}
    if(DEBUG){Serial.println("sendind request");}
    client.print("GET /pushingbox?devid=");
    client.print(devid);
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(serverName);
    client.println("User-Agent: Arduino");
    client.println();
  } 
  else { 
    if(DEBUG){Serial.println("connection failed");} 
  } 
}


