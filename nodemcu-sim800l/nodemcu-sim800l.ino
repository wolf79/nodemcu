/*
  Sends SMS Using NODEMCU and SIM800L C oroboard

*/

//int onModulePin = 13;
int8_t answer;
//char aux_string[30];
//char phone_number[] = "04168262667"; // ********* is the number to call
//char sms_text[] = "Test-Arduino-Hello World";

void setup() {

  //pinMode(onModulePin, OUTPUT);
  // initialize serial communications at 115200 bps:
  Serial.begin(115200);
  Serial.println("Starting...");
  power_on();
  delay(3000);

  Serial.println("Connecting to the network..."); // AT+CFUN=1\r\n","OK\r\n

  //while (sendATcommand("AT+CFUN=1\r\n", "OK\r\n", 2000) == 0 );
  while ( (sendATcommand("AT+CREG?", "+CREG: 0,1", 5000) ||
           sendATcommand("AT+CREG?", "+CREG: 0,5", 5000)) == 0 );

   sendSMS("04168262667", "hola mundo");         

//  Serial.print("Setting SMS mode...");
//  sendATcommand("AT+CMGF=1", "OK", 5000);    // sets the SMS mode to text
//  Serial.println("Sending SMS");
//
//  sprintf(aux_string, "AT+CMGS=\"%s\"", phone_number);
//  answer = sendATcommand(aux_string, ">", 20000);    // send the SMS number
//  if (answer == 1)
//  {
//    Serial.println(sms_text);
//    Serial.write(0x1A);
//    answer = sendATcommand("", "OK", 20000);
//    if (answer == 1)
//    {
//      Serial.print("Sent ");
//    }
//    else
//    {
//      Serial.print("error ");
//    }
//  }
//  else
//  {
//    Serial.print("error ");
//    Serial.println(answer, DEC);
//  }

}


void loop() {
}
///////////////////////////////////////////////////////
void power_on() {

  uint8_t answer = 0;

  // checks if the module is started
  answer = sendATcommand("AT", "OK", 5000);
  if (answer == 0)
  {
    // waits for an answer from the module
    while (answer == 0) {   // Send AT every two seconds and wait for the answer
      answer = sendATcommand("AT", "OK", 2000);
    }
  }

}
/////////////////////////////////////////////////////////////////
int8_t sendATcommand(char* ATcommand, char* expected_answer, unsigned int timeout) {

  uint8_t x = 0,  answer = 0;
  char response[100];
  unsigned long previous;

  memset(response, '\0', 100);    // Initialize the string

  delay(100);

  //while ( Serial.available() > 0) Serial.read();   // Clean the input buffer

  while (Serial.available()) { //Cleans the input buffer
    Serial.read();
  }

  Serial.println(ATcommand);    // Send the AT command


  x = 0;
  previous = millis();

  // this loop waits for the answer
  do {
    if (Serial.available() != 0) {
      // if there are data in the UART input buffer, reads it and checks for the asnwer
      response[x] = Serial.read();
      x++;
      // check if the desired answer  is in the response of the module
      if (strstr(response, expected_answer) != NULL)
      {
        answer = 1;
      }
    }
    // Waits for the asnwer with time out
  } while ((answer == 0) && ((millis() - previous) < timeout));

  return answer;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
int sendSMS(char *phone_number, char *sms_text) {

  char aux_string[30];
  //char phone_number[] = "04168262667"; // ********* is the number to call
  //char sms_text[] = "Test-Arduino-Hello World";
  Serial.print("Setting SMS mode...");
  sendATcommand("AT+CMGF=1", "OK", 5000);    // sets the SMS mode to text
  Serial.println("Sending SMS");

  sprintf(aux_string, "AT+CMGS=\"%s\"", phone_number);
  answer = sendATcommand(aux_string, ">", 20000);    // send the SMS number
  if (answer == 1)
  {
    Serial.println(sms_text);
    Serial.write(0x1A);
    answer = sendATcommand("", "OK", 20000);
    if (answer == 1)
    {
      Serial.print("Sent ");
    }
    else
    {
      Serial.print("error ");
    }
  }
  else
  {
    Serial.print("error ");
    Serial.println(answer, DEC);
  }
  return answer;
}

