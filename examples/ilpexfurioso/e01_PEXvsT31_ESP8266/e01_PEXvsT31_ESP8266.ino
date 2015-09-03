/**************************************************************************
    Souliss - Hello World for Expressif ESP8266
    
    This is the basic example, create a software push-button on Android
    using SoulissApp (get it from Play Store).  
    
    Load this code on ESP8266 board using the porting of the Arduino core
    for this platform.
        
***************************************************************************/

// Configure the framework
#include "bconf/MCU_ESP8266.h"              // Load the code directly on the ESP8266
#include "conf/Gateway.h"                   // The main node is the Gateway, we have just one node
#include "conf/IPBroadcast.h"

// **** Define the WiFi name and password ****
#define WIFICONF_INSKETCH
#define WiFi_SSID               "mywifi"
#define WiFi_Password           "mypassword"    

// Include framework code and libraries
#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include "Souliss.h"

// This identify the number of the LED logic
#define MYLEDLOGIC          0               

// **** Define here the right pin for your ESP module **** 
#define	OUTPUTPIN			5
#define ANALOGDAQ               0           // This is the memory slot used for the execution of the logic in network_address1
#define DEADBAND                0.05        // Deadband value 5%  

uint8_t ip_address[4]  = {192, 168, 1, 77};
uint8_t subnet_mask[4] = {255, 255, 255, 0};
uint8_t ip_gateway[4]  = {192, 168, 1, 1};
#define myvNet_address  ip_address[3]

float analogvalue = 12.12;

void setup()
{   
    Initialize();

    // Connect to the WiFi network and get an address from DHCP
    SetIPAddress(ip_address, subnet_mask, ip_gateway);
    SetAsGateway(myvNet_address);  
	
    Set_SimpleLight(MYLEDLOGIC);        // Define a simple LED light logic
	Set_Thermostat(ANALOGDAQ);          // Set a logic for the thermostat
 
}

void loop()
{ 
    // Here we start to play
    EXECUTEFAST() {                     
        UPDATEFAST();   
        
        FAST_50ms() {   // We process the logic and relevant input and output every 50 milliseconds
            Logic_SimpleLight(MYLEDLOGIC);
        } 

        // Execute the code every 11 time_base_fast       
        FAST_91110ms() {  

			analogvalue+=0.5;
            ImportAnalog(ANALOGDAQ+1,analogvalue);

            // The logic is executed faster than the data acquisition just to have a 
            // prompt answer to the user commands, but the real rate is defined by
            // the temperature measure rate.
        
            // Compare the setpoint from the user interface with the measured temperature
            Logic_Thermostat(ANALOGDAQ);
            
        }   
              
        // Here we handle here the communication with Android
        FAST_GatewayComms();                                        
    }
} 