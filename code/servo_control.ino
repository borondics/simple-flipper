#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <CommandParser.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Instansiate command parser object
typedef CommandParser<> MyCommandParser;
MyCommandParser parser;

// Depending on your servo, the pulse width min and max may vary, you 
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
#define SERVOMIN  120 // This is the 'minimum' pulse length count (out of 4096) - CALIBRATED FOR LONRUNNER KY66 SERVOS
#define SERVOMAX  280 // This is the 'maximum' pulse length count (out of 4096) - CALIBRATED FOR LONRUNNER KY66 SERVOS
#define SERVOMID 200 // for initial position
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

// our servo # counter
uint8_t servonum = 0;
bool isActive = true;

void setup() {
  Serial.begin(9600);
  Serial.println("8 channel Servo test!");
  parser.registerCommand("POS", "su", &cmd_moves); // Positioning
  parser.registerCommand("GP", "u", &cmd_getpos); // Get Position

  pwm.begin();

  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates
  int servonum = 0;
  goPosO(servonum);
  delay(100);
  // pwm.wakeup();
  pwm.sleep();
  isActive = false;
  delay(1000);

}

void loop() {
  serialListener();
  delay(10);
}

void goPosO(uint8_t servo_num) {
  Serial.println("Jump to origin");
  if (isActive == false){turnOnOff();}
  pwm.setPWM(servo_num, 0, SERVOMID);
  delay(100);
  turnOnOff();
}

void goPosA(uint8_t servo_num) {
  // Feedback to serial
  // Serial.println("Sweep to Pos A");
  // Turn the controller ON if neccesarry
  if (isActive == false){turnOnOff();}
  delay(10);
  // Get the current pwm value "positon"
  int current_val = pwm.getPWM(servo_num,true);
  // Serial.print("Current value:"); Serial.println(current_val);
  // Change the pwm "position" value until arrive to Pos A (SERVOMAX)
  for (uint16_t pulselen = current_val; pulselen < SERVOMAX; pulselen++) {
    pwm.setPWM(servonum, 0, pulselen);
    delay(5); // change delay to change speed
  }
  // Turn it OFF after the movement
  turnOnOff();
}

void goPosB(uint8_t servo_num) {
  // Feedback to serial
  // Serial.println("Sweep to Pos B");
  // Turn the controller ON if neccesarry
  if (isActive == false){turnOnOff();}
  delay(10);
  // Get the current pwm value "positon"
  int current_val = pwm.getPWM(servo_num,true);
  Serial.print("Current value:"); Serial.println(current_val);
  // Change the pwm "position" value until arrive to Pos B (SERVOMIN)
  for (uint16_t pulselen = current_val; pulselen > SERVOMIN; pulselen--) {
    pwm.setPWM(servonum, 0, pulselen);
    delay(5);
  }
  // Turn it OFF after the movement
  turnOnOff();
}

// Turns ON or OFF 
void turnOnOff(){
  if (isActive == true){
    pwm.sleep();
    isActive = false;
    // Serial.print("isActive: ");Serial.print(isActive);Serial.println(" - Sleeping");
    }
  else if (isActive == false){
    pwm.wakeup();
    isActive = true;
    // Serial.print("isActive: ");Serial.print(isActive);Serial.println(" - Activated");
    }
}

// Command PoSA_motornumber
void cmd_moves(MyCommandParser::Argument *args, char *response) {
  // Serial.print("string: "); Serial.println(args[0].asString);
  // Serial.print("uint64: "); Serial.println((uint32_t)args[1].asUInt64); // NOTE: on older AVR-based boards, Serial doesn't support printing 64-bit values, so we'll cast it down to 32-bit
  // strlcpy(response, "success", MyCommandParser::MAX_RESPONSE_SIZE);

  String positionString = args[0].asString;
  char position = positionString[0];
  uint32_t motornum = (uint32_t)args[1].asUInt64;

  switch (position){
    case 'A':{
      goPosA(motornum);
      // Serial.println("A");
      break;
    }
    case 'B':{
      goPosB(motornum);
      // Serial.println("B");
      break;
    }
    case 'O':{
      goPosO(motornum);
      // Serial.println("O");
      break;
    }
  }
}

void cmd_getpos(MyCommandParser::Argument *args, char *response){
  // Serial.print("uint64: "); Serial.println((uint32_t)args[0].asUInt64); // NOTE: on older AVR-based boards, Serial doesn't support printing 64-bit values, so we'll cast it down to 32-bit
  // strlcpy(response, "success", MyCommandParser::MAX_RESPONSE_SIZE);

  uint8_t motornumber = (uint8_t)args[0].asUInt64;

  delay(10);
  int current_position = pwm.getPWM(motornumber,true);
  Serial.print("Position: ");Serial.println(current_position);

  if (current_position > SERVOMAX-20){
    Serial.println("pos:A");
  }
  else if (current_position < SERVOMIN+20){
    Serial.println("pos:B");
  }
  else if ((current_position >= SERVOMID-10) && (current_position <= SERVOMID+10)) {
    Serial.println("pos:O");
  }
}

void serialListener() {
  if (Serial.available()) {
    char line[128];
    size_t lineLength = Serial.readBytesUntil('\n', line, 127);
    line[lineLength] = '\0';

    char response[MyCommandParser::MAX_RESPONSE_SIZE];
    parser.processCommand(line, response);
    Serial.println(response);
  }
}