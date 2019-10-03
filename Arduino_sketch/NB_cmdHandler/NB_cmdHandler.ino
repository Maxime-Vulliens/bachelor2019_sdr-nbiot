#include <MKRNB.h>

NB nb(1);  // 1 = debug mode - shows AT commands.

// modem verification object
NBModem modem;
NBModem modemTest;

// IMEI variable
String IMEI = "";
String ICCID = "";
String RESP = "";

void setup() {
  
  // initialize serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  // start modem test (reset and check response)
  Serial.print("Starting modem test...");
  if (modem.begin()) {
    Serial.println("modem.begin() succeeded");
  } else {
    Serial.println("ERROR, no modem answer.");
  }

  delay(1000);

}

void loop() {

    // get modem IMEI
  Serial.print("Checking IMEI...");
  atCmd("AT+CGSN");

  Serial.print("Resetting modem");
  if (modem.begin()) {
    Serial.println("Modem work fine.");
  } else {
    Serial.println("ERROR, restard modem fail.");
  }

  // AT command handler
  while (true){

    Serial.print("> ");
  
    Serial.setTimeout(-1);
    while (Serial.available() == 0);
    String uratInput = Serial.readStringUntil('\n');
    uratInput.trim();
    Serial.println(uratInput);
  
    atCmd(uratInput);

  }
}


void atCmd(String cmd)
{
  RESP.reserve(150);
  MODEM.send(cmd);
  MODEM.waitForResponse(18000, &RESP);

  if (RESP != NULL) {
    Serial.println(RESP);
  }
  
  RESP = "";
}

/*** Command AT Tested : 

AT+CGSN
AT+CCID
AT+COPS?      // current carrier 0 automatic, 1 manual, 2 deregister, 3 set only, 4 manual/automatic
AT+CSQ        // signal strength
AXT+IPR=%ld   // set baud rate
AT+CFUN=15    // Reset    0: minimum config / 1: full config
AT+CEREG?     // Access is alive ??
AT+CPIN?      //  is ther a pin code / =val for unlock sim
AT+UDCONF=1,1 // set hex mode  for read  AT+UDCONF=1
AT+CCLK?      // Get time
AT+CGATT=0    // Dettach Data
AT+CMGF=1     // Prefferef message format
AT+CGDCONT=1,\"IP\",\"%s\"", _apn   // Set apn
AT+CIMI       // request imsi
AT+COPS=?   
AT+UBANDMASK? // read current LTE band scanning

AT+USOCO=   // use to set socket,"host",port
AT+USOCR=17 // beegin UDP
AT+USOLI=%d,%d  // socket,port 

AT+CTZU=1   // set automatic time zone

Failed :
AT+COPS=?   // read networks
AT+CNUM=?   // Read MSISDN from sim


*** M-CENTER COMMAND - BASIC
AT
ATE0
ATE1
AT+CGMI
AT+CGMM
AT+CGMR
AT+CGSN
AT+CPIN?
AT+CLCK="SC",2    // ask status of sim card
AT+CPWROFF        // shut down module and save parameters in NVM
AT+CFUN=16        // 0 : minimum / 1 : full / 15 : restart modem / 16 : restar modem and sim
AT+CLAC           // available AT command
  AT+UPSV?  -> ERROR
AT+UPSV=0
AT+UPSV=1,##
AT&V      // Current configuration
AT&W
ATA   -> ERROR
ATH
AT+UEXTDCONF=0,1

*** NETWORK COMMAND
AT+CSQ
AT+CREG?
AT+COPS?
AT+COPS=0
AT+COPS=2
AT+COPS=5
AT+CGREG?
AT+CPOL?
AT+URAT?
AT+UBANDSEL?


AT+CGATT=0    // Detach GPRS
AT+UDCONF=1,1    // configure hex mode for some command
AT+CTZU=1      // configure auto time zone info after registration
AT+CGDCONT=1,"IP",""


*** State manage by MKNRB library

READY_STATE_SET_MINIMUM_FUNCTIONALITY_MODE,
READY_STATE_DETACH_DATA,
READY_STATE_CHECK_SIM,
READY_STATE_UNLOCK_SIM,(optional)
READY_STATE_SET_PREFERRED_MESSAGE_FORMAT,
READY_STATE_SET_HEX_MODE,
READY_STATE_SET_AUTOMATIC_TIME_ZONE,
READY_STATE_SET_APN,
READY_STATE_SET_FULL_FUNCTIONALITY_MODE,
READY_STATE_CHECK_REGISTRATION,
READY_STATE_DONE


ping : AT+UMQTTC=8,"192.168.253.1"



*/
