/*************************************************
 * 
 *  To get service 'notify.mysensors' in Home assistant
 * 
 *  Use 'notify.mysensors' in Home assistant to send message to node
 *  sample notify with this code
 *  service: [Child description]
 * 
 *      notify.mysensors
 *      data:
 *        message: test notify
 *        target: "myS 99 V_TEXT" 
 *
 *  Note:
 *    Maximum message length 25 characters
 *    otherwise cut into several receptions of 25 characters
 * 
 *  Auteur : Eric H
 * 
 * ***********************************************/

#include <Arduino.h>

#define MY_RADIO_RF24

//Developpement
#define MY_RF24_CHANNEL 76

#define MY_RX_MESSAGE_BUFFER_FEATURE //for MY_RF24_IRQ_PIN
//Define this to use the IRQ pin of the RF24 module
#define MY_RF24_IRQ_PIN (2)

//NODE_ID
#define MY_NODE_ID 99

#include <MySensors.h>

#define CHILD_ID_TXT 0

#define SKETCH_NAME "Test V_TEXT"
#define SKETCH_VERSION "1.0"

// Wait times
#define LONG_WAIT 500
#define LONG_WAIT1 1000
#define LONG_WAIT2 2000
#define SHORT_WAIT 50

unsigned long previousTime = 0;
unsigned long Interval = 6000;

MyMessage msgTEXT(CHILD_ID_TXT, V_TEXT);

void before()
{
  // Optional method - for initialisations that needs to take place before MySensors transport has been setup (eg: SPI devices).
}

void presentation()  
{ 
  Serial.print("===> Envoyer présentation du noeud : "); Serial.println(MY_NODE_ID);
  sendSketchInfo(SKETCH_NAME, SKETCH_VERSION);
  wait(LONG_WAIT1);
  
  char sNoeud[] = STR(MY_NODE_ID);
  //  V_TEXT
  char sChild0[25];
  strcpy(sChild0, "myS ");
  strcat(sChild0, sNoeud);
  strcat(sChild0, " V_TEXT");
  Serial.print("S_INFO: "); Serial.print("'"); Serial.print(sChild0); Serial.println("'");
  present(CHILD_ID_TXT, S_INFO, sChild0); 
  wait(LONG_WAIT2);

  previousTime = millis();

}

void setup()
{
  // Called once at startup, usually used to initialize sensors.

  //ATTENTION 25 caractères maximum
  Serial.print("node sent first message : "); Serial.println("first message by node");
  send(msgTEXT.set("first message by node"));
  //wait(LONG_WAIT);
  
  Serial.println("node request first message --> ");
  request(CHILD_ID_TXT, V_TEXT);

}

void loop()
{
  if ( millis() > (previousTime + Interval) ) {
    previousTime= millis();

    //Pour Home assistant
    static bool next_message_sent = false;
    if (!next_message_sent) {
      Serial.println("======> Sending new message");
      send(msgTEXT.set( "new message by node" ));
      //wait(LONG_WAIT);

      request(CHILD_ID_TXT, V_TEXT);

      next_message_sent = true;
    }
  }

}

void receive(const MyMessage &message)
{

	Serial.print("Received a message type: ");
	if (message.type == V_TEXT) {
    Serial.println("V_TEXT");  
  }
  Serial.print("Received a message: ");
	Serial.println(message.getString());  

}