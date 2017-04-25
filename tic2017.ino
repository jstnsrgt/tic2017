#include <connection4g.h>
#include <telstraiot.h>
#include <iotshield.h>
#include <shieldinterface.h>
#include <Adafruit_PN532.h>
#include <Wire.h>

//Adafruit Shield
#define PN532_IRQ   (2)
#define PN532_RESET (3)

Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);



//Telstra Shield
char signalQuality[10];

ShieldInterface shieldif;
IoTShield shield(&shieldif);
Connection4G conn(true,&shieldif);

TelstraIoT iotPlatform(&conn,&shield);

const char host[] = "tic2017team016.iot.telstra.com";
int port = 443;

//const char host[] = "http://home.rushyservers.com/win7/connection.php";
//int port = 80;


char id[] = "10407";
char tenant[] = "tic2017team016";
char username[] = "device";
char password[] = "arduinoboard2017";

void setup() {
  Serial.begin(115200);
  delay(5000);
  //Adafruit

  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();
  if( !versiondata )
  {
    Serial.print("Didn't find PN53x board");
    while (1)
    {
      delay(100000);
    }// haltb
  }

// Got ok data, print it out!
  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
  nfc.SAMConfig();
  Serial.println("Waiting for a Card ...");

  
  //Telstra Shield
  

  Serial.println(F("[START] PHP Test"));
  
  if(!shield.isShieldReady())
  {
     Serial.println("waiting for shield ...");
    shield.waitUntilShieldIsReady();
  } else {
    Serial.println("Shield ready!");
  }
  //while(Serial.available()==0); // Wait for serial character before starting

  Serial.println("Reading credentials...");
  shield.readCredentials(id,tenant,username,password);
  Serial.print("ID: ");
  Serial.println(id);
  Serial.print("Tenant: ");
  Serial.println(tenant); 
  Serial.print("Username: ");
  Serial.println(username);
  Serial.print("Password: ");
  Serial.println(password);
  
  Serial.println("Setting credentials...");
  iotPlatform.setCredentials(id,tenant,username,password,"");
  
  Serial.println("Setting host...");
  iotPlatform.setHost(host,port);
  
  conn.openTCP(host,port);  

}

void loop() {
  uint8_t success;
  uint8_t uid[] = {0,0,0,0,0,0,0};
  uint8_t uidLength;
  delay(1000);
  char lightString[15];


  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  if(success)
  {
    // Display some basic information about the card
    Serial.println("Found a card");
    Serial.print("  UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
    Serial.print("  UID Value: ");
    nfc.PrintHex(uid, uidLength);
    Serial.println("");
    
    if (uidLength == 4)
    {
      
      // We probably have an NTAG2xx card (though it could be Ultralight as well)
      Serial.println("Seems to have correct UID length");    
      
    }
    delay(5000);
  }
  
  Serial.println("############################ Preparing to read MEASUREMENTS #############################");
  //Read Light measurement from device
  shield.getLightLevel(lightString);
  Serial.print(F("[    ] Light: "));
  Serial.println(lightString);

  Serial.println("############################ Preparing to send MEASUREMENTS #############################");  
  iotPlatform.sendMeasurement("LightMeasurement", "LightMeasurement", "Light level (lux)", lightString, "lux");

  
  /*
  //Read temperature measurement from device
  shield.getTemperature(tempString);
  Serial.print(F("[    ] Temp: "));
  Serial.println(tempString);
  */
  /*
  shield.getTemperature(tempString);
  iotPlatform.sendMeasurement("TemperatureMeasurement", "TemperatureMeasurement", "Temperature (degrees Celsius)", tempString, "degrees Celsius");
  */
}
