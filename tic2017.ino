#include <connection4g.h>
#include <telstraiot.h>
#include <iotshield.h>
#include <shieldinterface.h>

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
    delay(1000);
    conn.getSignalQuality(signalQuality);
    Serial.println(signalQuality);


    
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
