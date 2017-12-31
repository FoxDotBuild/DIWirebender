/* DIWire Bender
 * 3D Wire Bender by Pensa - www.PensaNYC.com
 * Written by Marco Perry. Email DIWire@PensaNYC.com for questions.
 * Drives on 3 Stepper Motors to bend wire in 3D space
 * 
 * Modified by Doug Kimber and Shane Snipe to use ASCII commands
 *
 * This file is part of the DIWire project. This is the Arduino firmware for 2D or 3D bends
 *
 *  DIWire is a free software & hardware device: you can redistribute it and/or modify it
 *  it's software under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, version 3 of the License.
 *   
 *  The hardware portion is licensed under the Creative Commons-Attributions-Share Alike License 3.0
 *  The CC BY SA license can be seen here: http://creativecommons.org/licenses/by-sa/3.0/us/
 * 
 *  DIWIre is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License and CC-BY-SA for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with DIWire.  If not, see <http://www.gnu.org/licenses/>. 
 *  and http://creativecommons.org/licenses/by-sa/3.0/us/legalcode
 *  
 *  No portion of this header can be removed from the code. 
 *  Now enjoy and start making something!
 */

/*This program is to be uploaded onto the DIWire arduino. 
 Bend angles and feed lengths are transmitted from a computer plugged into the arduino. 
 This program takes these lengths and angles and moves the motors and solenoid accordingly.
 */

/*
* Feed Motor 1 - drives wire
 * z Bend Motor - performs 3D bends by rotating the entire assembly
 * bend Motor - performs 2D bends by bending wire over
 * benderPin - solenoid
 */

 // Modified to use ASCII commands byy Doug Kimber and Shane Snipe
 // Commands have the following form:
 //
 // Fnnn  - Feed nnn millimeters
 // Rnnn  - Retract nnn millimeters
 // Bnnn  - Bend nnn degrees. nnn may be negative.
 // Mnnn  - Move the bender axis nnn degrees, but don't return like bend does
 // H     - Set this as the Home position
 // Znnn  - Rotate Z axis nnn degrees. nnn may be negative.
 // Sx    - Solenoid state. x=0 => Solenoid off, x=1 => Solenoid on
 //
 // All commands are ended by a carraige return and/or linefeed
 //

// pin assignments
// Motor pulse and solenoid pins
const int bendMotorPls = 9;
const int zMotorPls = 10;
const int feedMotorPls = 11;
const int benderPin = 12;

// AWO pins to allow motor shaft to free spin
// These are not needed with the motor controllers we used
const int bendMotorAWO = 3;
const int zMotorAWO = 4;
const int feedMotorAWO = 5;

// Direction pins to select drive direction
const int bendMotorDir = 6;
const int zMotorDir = 7;
const int feedMotorDir = 8;

//misc program constants
//const int pulseWidth = 20;
//const int pulseDelay = 700;
const int pulseWidth = 1000;
const int pulseDelay = 1000;

// Drive directions in english
boolean ccw = true; // counter-clockwise drive direction
boolean cw = false; // clockwise drive direction
boolean curDir = cw; // resets direction before next angle command 

int lastbend = 0;
int currentBend = 0;
int currentZ = 0;
int fieldindex=0;
bool home = false;
int values[300]; //creates array

// Buffer for an incoming line from the serial port
// Serial I/O is event (interrupt) driven
char lineBuf[ 256 ];
int lineIdx;


void setup() {
  Serial.begin (115200); //com port communication
  pinMode (bendMotorPls, OUTPUT); //Declaring motor pins as out
  pinMode (zMotorPls, OUTPUT); 
  pinMode (feedMotorPls, OUTPUT); 
  pinMode (bendMotorAWO, OUTPUT); 
  pinMode (zMotorAWO, OUTPUT); 
  pinMode (feedMotorAWO, OUTPUT); 
  pinMode (bendMotorDir, OUTPUT);
  pinMode (zMotorDir, OUTPUT); 
  pinMode (feedMotorDir, OUTPUT); 
  pinMode (benderPin, OUTPUT); 
  digitalWrite (bendMotorPls, LOW); //starts with everything off
  digitalWrite (zMotorPls, LOW); 
  digitalWrite (feedMotorPls, LOW); 
  digitalWrite (benderPin, LOW);
  digitalWrite (zMotorAWO, HIGH);
  digitalWrite (feedMotorAWO, HIGH);
  digitalWrite (bendMotorAWO, HIGH);

  lineIdx = 0;
  lastbend = 0;
  currentBend = 0;
  currentZ = 0;

}

void bend (float angle) {     //bender pin movement
  if (angle!=0){              //sets direction of bend based on + or - angle
    Serial.println("bending");
    Serial.println(angle);
    boolean dir=cw;
    boolean back=ccw;
    if (angle<0){
      dir = ccw;
      back = cw;
    }
    float rotations = 0;
    angle = abs(angle);
    if (angle <= 90){
      angle = -.0012*angle*angle+.5959*angle+.2452; //converts angle into calibrated motor steps 
      angle = 6000 * (angle/360)+220;
    }
    else if (91 <= angle <= 120){
      angle = .0044*angle*angle-.5481*angle+57.981; //converts angle into calibrated motor steps 
      angle = 5960 * (angle/360)+220;
    }
    else if (121<=angle<=180){
      angle = angle-63.26;  //converts angle into calibrated motor steps 
      angle = 5960 * (angle/360)+220;
    }   //calibration will differ depending on set up 
    rotations = angle;
    // Serial.println (angle);
    digitalWrite (bendMotorDir, dir);  
    for (int i=0; i <=rotations  ; i++){ //moves bender pin the desired angle
      digitalWrite(bendMotorPls, HIGH);
      delayMicroseconds (pulseWidth);
      digitalWrite(bendMotorPls, LOW);
      delayMicroseconds (pulseDelay);
    }
    delay (100);
    digitalWrite (bendMotorDir, back);   //moves bender pin back to home position ready for next feed
    for (int i=0; i <=rotations  ; i++){
      digitalWrite(bendMotorPls, HIGH);
      delayMicroseconds (pulseWidth);
      digitalWrite(bendMotorPls, LOW);
      delayMicroseconds (pulseDelay);
    }
  }
}

void rotatePin (boolean dir, float angle) { //moves bender pin during duck. direction specified from duck subroutine
  Serial.print( "Rotating Pin " );
  Serial.println( angle ); 
  float rotations = 0;
  angle = 6000 * (angle/360); //converts angle to steps
  rotations = angle;
  Serial.print( "Direction " );
  Serial.println (dir);
  if( dir == false )
    digitalWrite(bendMotorDir, HIGH );
   else
    digitalWrite( bendMotorDir, LOW );  
  for (int i=0; i <=rotations  ; i++){ //rotates bender motor appropriate number of steps
    digitalWrite(bendMotorPls, HIGH);
    delayMicroseconds (pulseWidth);
    digitalWrite(bendMotorPls, LOW);
    delayMicroseconds (pulseDelay);
  }
}

void zbend (float angle) { //rotates z motor
  if (angle!=0){
    Serial.print("Z bending ");
    Serial.println(angle);
    boolean dir=cw;
    if (angle<0){ //+ angle is clockwise - angle is counter clockwise
      dir = ccw;
    }
    float rotations = 0;
    angle = abs(angle);
    angle = (2000 * angle)/360; //converts angle to motor steps
    rotations = angle;
    digitalWrite (zMotorDir, dir);  //sets motor direction
    for (int i=0; i <=rotations  ; i++){ //rotates z motor appropriate number of steps
      digitalWrite(zMotorPls, HIGH);
      delayMicroseconds (pulseWidth);
      digitalWrite(zMotorPls, LOW);
      delayMicroseconds (2000);
    }
  }
}

void feed (float dist) { //feeds wire
  if (dist != 0){
    Serial.println("feeding");
    Serial.println(dist);
    float rotations = 0;
    float feedCalib = 25.4*.75*PI; //feed mm per revolution of motor
    dist = 2000 * dist/feedCalib;  //dist converted from mm to steps
    rotations = dist;
    digitalWrite (feedMotorDir, 0);  //feed motor only moves forward
    for (int i=0; i <=rotations  ; i++){ //rotate feed motor appropriate number of steps 
      digitalWrite(feedMotorPls, HIGH);
      delayMicroseconds (pulseWidth);
      digitalWrite(feedMotorPls, LOW);
      delayMicroseconds (pulseDelay);
    }
  }
}

void retract (float dist) { //feeds wire
  if (dist != 0){
    Serial.println("retracting");
    Serial.println(dist);
    float rotations = 0;
    float feedCalib = 25.4*.75*PI; //feed mm per revolution of motor
    dist = 2000 * dist/feedCalib;  //dist converted from mm to steps
    rotations = dist;
    digitalWrite (feedMotorDir, 1);  //feed motor only moves forward
    for (int i=0; i <=rotations  ; i++){ //rotate feed motor appropriate number of steps 
      digitalWrite(feedMotorPls, HIGH);
      delayMicroseconds (pulseWidth);
      digitalWrite(feedMotorPls, LOW);
      delayMicroseconds (pulseDelay);
    }
  }
}

void duck (){  //ducks bender pin under wire
  Serial.println("Ducking!");
  digitalWrite (benderPin, HIGH); 
  delay (100);
  rotatePin (curDir, 45);
  digitalWrite (benderPin, LOW);  //pin down move under wire
  curDir = !curDir;    //direction reversed for next duck
}

void processLine()
{
  int val;

  // Find the value portion of the command
  val = atoi( &( lineBuf[ 1 ]) );
  
  // process a single line of input
  switch( lineBuf[ 0 ] & 0x7f )
  {
    case 'h':
    case 'H':
      if( (lineBuf[ 1 ] == 'S') || (lineBuf[ 1 ] == 's') )
      {
        lastbend = 0;
        currentBend = 0;
        currentZ = 0;
        home = true;
      }
      else if( (lineBuf[ 1 ] == 'G') || (lineBuf[ 1 ] == 'g') )
      {
        // Go Home - are we already there?
        if( home == true )
          break;
        // figure out where we are and where we need to move.
        // First, pull the pin down
        Serial.println("Solenoid On!");
        digitalWrite (benderPin, HIGH);
        delay (10);        
        // Rotate Z first
        zbend( -currentZ );
        // Now rotate the bender head
       if( currentBend >= 0 )  
         rotatePin( 0, -currentBend);
       else
         rotatePin( 1, currentBend );                
      }
      break;
    case 'f':
    case 'F':
      // Feed a specific amount
      feed( val );
      delay( 10 );
      break;
    case 'r':
    case 'R':
      // Retract a specific amount
      retract( val );
      delay( 10 );
      break;
    case 'b':
    case 'B':
      // Bend the stated number of degrees
      //if incoming bend angle is opposite direction from previous angle duck pin
      if( val > 120 ) 
        val = 120;
      if( val < -120 )
        val = -120;  
      if( (val<0 && curDir==cw) || (val>0 && curDir ==ccw))
      {
        duck ();
        delay (10);
      }
      bend (val);
      currentBend = currentBend + val;
      lastbend = val;
      Serial.print( "Current Angle: " );
      Serial.println( currentBend );
      break;
    case 'm':
    case 'M':
      // Move the bender the stated number of degrees
      // Pull the pin down first
      digitalWrite (benderPin, HIGH);
      if( val > 120 ) 
        val = 120;
      if( val < -120 )
        val = -120;
      if( val >= 0 )  
        rotatePin( 1, val);
      else
        rotatePin( 0, -val );
      currentBend = currentBend + val;
      lastbend = val;
      Serial.print( "Current Angle: " );
      Serial.println( currentBend );
      break;
    case 'z':
    case 'Z':
      zbend( val );
      delay( 10 );
      currentZ = currentZ + val;
      Serial.print( "Current Z: " );
      Serial.println( currentZ );
      break;
    case 's':
    case 'S':
      if( val == 0 )
      { 
        Serial.println("Solenoid Off!");
        digitalWrite (benderPin, LOW);
        delay (10);
      }
      else
      {
        Serial.println("Solenoid On!");
        digitalWrite (benderPin, HIGH);
        delay (10);        
      }
      break;
    default:
      // ignore this line
      Serial.print( "Unknown Command: " );
      Serial.println( lineBuf[ 0 ] );
      break; 
  }

  lineIdx = 0;
}

// Handle an event on the serial port
void serialEvent()
{
  // In theory the event should be a received character
  char in = Serial.read();

  in = in & 0x7f;
  
  // Is this the end of a line?
  if( (in == '\n') || (in == '\r') )
  {
    // This is the end of an input line
    processLine();
    lineIdx = 0;
  }
  else
  {
    lineBuf[ lineIdx ] = in;
    lineIdx++;
    lineBuf[ lineIdx ] = '\0';  // Null terminate for safety
  }
}

int loopCount = 0;
bool Started = false;

void loop() {
  // Not much to do in the main loop - serial data is interrupt driven
  loopCount++;
  delay( 100 );

 if( Started != true )
 {
  feed( 10 );
  Started = true;
 }
  
#ifdef ORIGINAL
  int copies = 0;
  while (Serial.available ()){ //starts once serial entry made
    digitalWrite (bendMotorAWO, LOW);
    digitalWrite (zMotorAWO, LOW);
    digitalWrite (feedMotorAWO, LOW);

    int in = Serial.read();
    byte incoming = in+128; //converts bytes from processing
    if (incoming != 255){   //255 signifies end of incoming array
      Serial.println (Serial.read());
      values[fieldindex] = values[fieldindex]*10+incoming; //fills array fieldindex from incoming processing
      fieldindex++;
    }
    else{
      Serial.println("END");     //if array end marker inputs from processing end
      for (int i=0; i<=fieldindex;i++){
        Serial.println(values[i]-128);
      }
      copies=copies+1;
    }
  }
  if (copies==1){ //once bend is complete eject shape
    delay (1000);
    motorrun();
    feed(50); //eject 
    copies=copies+1;
  }
#endif
}

void motorrun(){
  int lastbend=0;
  for (int i=0; i<= fieldindex;i++){
    delay (100);
    if ((values[i]-128)==126){ //convert bytes from processing and look for feed motor marker
      feed (values[i+1]-128);  //if feed motor marker detected next value in array is a feed length
    }
    else if ((values[i]-128)==125){ //convert bytes from processing and look for bend motor marker
      int bendAng = (values[i+1]-128);  //if bend motor marker detected next value in array is a bend angle
      if ((bendAng<0&&curDir==cw) || (bendAng>0 && curDir ==ccw)){ //if incoming bend angle is opposite direction from previous angle duck pin
        duck ();
        delay (100);
      }
      bend (bendAng);
      lastbend = bendAng;
    }
    else if ((values[i]-128)==124){ //convert bytes from processing and look for z motor marker
      zbend (values[i+1]-128);  //if z motor marker detected next value in array is z bend angle
    }
  }
}



