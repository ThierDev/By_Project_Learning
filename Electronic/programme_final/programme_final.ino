#include <Servo.h>

#define LED 34

/// Define pins for multiplexor ///
#define pin0 11
#define pin1 12
#define pin2 13
const int d = 0; // for delaying the pins

/// Variable for alarm
float sinVal;
int toneVal;


/// Variable for the




/// Defining the pins for the IRsensor

int RIRsensorPin = 24;    // Pin for the right IR Sensor
float RIRsensorValue = 0;  // variable to store the value coming from the sensor
int RIRdistance = 0; // distance for the right sensor

int LIRsensorPin = 25;    // Pin for the right IR Sensor
float LIRsensorValue = 0;  // variable to store the value coming from the sensor
int LIRdistance = 0; // distance for the right sensor

////

Servo myvolet;

void setup() {

  // Configuration des pins pour le multiplexeurs
  pinMode(pin0, OUTPUT);
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  Serial.begin(9600);
  Serial1.begin(9600);

  myvolet.attach(PF_3); // pin 39 ceci est pour le volet.
}

void loop()
{
  //readIRsensor();
  sendTrame(readIRsensor(0),'01', '1');	//VAL,NUM,TYP
}

void turnLedOn(int outputPin)
// turns on output at pin 'outputPin'
{
  switch (outputPin)
  {
    case 0:
      digitalWrite(pin0, LOW);
      digitalWrite(pin1, LOW);
      digitalWrite(pin2, LOW);
      break;
    case 1:
      digitalWrite(pin0, HIGH);
      digitalWrite(pin1, LOW);
      digitalWrite(pin2, LOW);
      break;
    case 2:
      digitalWrite(pin0, LOW);
      digitalWrite(pin1, HIGH);
      digitalWrite(pin2, LOW);
      break;
    case 3:
      digitalWrite(pin0, HIGH);
      digitalWrite(pin1, HIGH);
      digitalWrite(pin2, LOW);
      break;
    case 4:
      digitalWrite(pin0, LOW);
      digitalWrite(pin1, LOW);
      digitalWrite(pin2, HIGH);
      break;
    case 5:
      digitalWrite(pin0, HIGH);
      digitalWrite(pin1, LOW);
      digitalWrite(pin2, HIGH);
      break;
    case 6:
      digitalWrite(pin0, LOW);
      digitalWrite(pin1, HIGH);
      digitalWrite(pin2, HIGH);
      break;
    case 7:
      digitalWrite(pin0, HIGH);
      digitalWrite(pin1, HIGH);
      digitalWrite(pin2, HIGH);
      break;
  }
}


/*
   Emulation de lumière d'alarme, elles clignotent à la suite

*/
void emulateAlarm(int dd)
{
  for (int i = 0; i < 8; i++)
  {
    turnLedOn(i);
    delay(dd);
  }
  for (int i = 7; i >= 0; --i)
  {
    turnLedOn(i);
    delay(dd);
  }

}

void allLedOn(int dd)
// scans all outputs on to emulate all being on at once for 'dd' cycles
{
  for (int j = 0; j < dd; j++)
  {
    for (int i = 0; i < 8; i++)
    {
      turnLedOn(i);
    }
  }

}

/*
   Read the value for the IR sensor, and store it in the corresponding variables
   0 == left sensor, 1== right sensor
*/
float readIRsensor(bool i) {

  if (i == 1) {
    RIRsensorValue = analogRead(RIRsensorPin) * 0.0008056640625;
    RIRdistance = 13 * pow(RIRsensorValue, -1);

    if (RIRdistance < 7) {
      alarm();
    }
    else {
      analogWrite(PB_4, 0);
    }
    return RIRdistance;

  }

  else if(i == 0) {

    LIRsensorValue = analogRead(LIRsensorPin) * 0.0008056640625;
    LIRdistance = 13 * pow(LIRsensorValue, -1);
    Serial.println(LIRdistance);

    return LIRdistance;
  }
}


void alarm() {

  emulateAlarm(25);

  allLedOn(5000);
  for (int x = 0; x < 180; x++) {
    // convert degrees to radians then obtain sin value
    sinVal = (sin(x * (3.1412 / 180)));
    // generate a frequency from the sin value
    toneVal = 2000 + (int(sinVal * 1000));
    analogWrite(PB_4, toneVal);
  }
}

char toText(char text) {
  if (text < 10) {
    text += 0x30;
  }
  else {
    text += 55;
  }
  return text;
}

void scoutTrame(){
	char trame[18];
	
	for(int i=0; i<19; i++) {
		trame[i] = Serial1.read();		// Lecture de la trame
	}
	Serial.print(trame);		//Copie dans la console
	
	if(trame[5]=='2'){		//Si requête en lecture (l'objet reçoit l'info)
		char TYP = trame[6];
		char NUM = trame[7] + trame[8];
		char VAL = trame[9] + trame[10] + trame[11] + trame[12];
		action(TYP, NUM, VAL);
	}
}

void action(char TYP, char NUM, char VAL) {
	if(VAL=='0000') {
		//LOW
	}
	else if(VAL=='0001') {
		//HIGH
	}
}

void sendTrame(int VAL, char NUM, char TYP) {
	char trame[18];
	
	// Ecriture de la trame (sans la checksum)
	trame[0] = '1';		//TRA
	trame[1] = '0';		//OBJ
	trame[2] = '0'; 	//OBJ
	trame[3] = '9'; 	//OBJ
	trame[4] = 'D';		//OBJ
	trame[5] = '1';		//REQ
	trame[6] = TYP;		//TYP
	trame[7] = toText((NUM >> 4) & 0x0f);	//NUM
	trame[8] = toText(NUM & 0x0f);	//NUM
	
	trame[9] = toText((VAL >> 12) & 0x0f);	
  trame[10] = toText((VAL >> 8) & 0x0f);
  trame[11] = toText((VAL >> 4) & 0x0f);
  trame[12] = toText(VAL & 0x0f);
	trame[13] = '0';	//TIM
	trame[14] = 'E';	//TIM
	trame[15] = 'C';	//TIM
	trame[16] = 'R';	//TIM


	// Calcul de la checksum avec les valeurs présentes 
/*	int CHK = 00;
	CHK = checksum(trame);*/
	
	/* Insertion checksum dans la trame */
	trame[17] = '0';	//CHK
	trame[18] = 'B';	//CHK
	
	/* Système d'envoie de la trame */
	int pot = analogRead(28);
	if(pot>1000) {
		for(int i=0; i<19; i++) {
			Serial1.print(trame[i]);	//Envoie à la passerelle
			Serial.print(trame[i]);		//Copie à la console
			Serial.println();
		}
	}
	else {
		Serial.println("Switch(pot) is OFF!");
	}
}

/*
int checksum(char trame) {
	int res = 0x00;
	for(int i=0; i<17; i++) {
		switch(trame[i]) {
			case '0':
				res += 0x30;
				break;
			case '1':
				res += 0x31;
				break;
			case '2':
				res += 0x32;
				break;
			case '3':
				res += 0x33;
				break;
			case '4':
				res += 0x34;
				break;
			case '5':
				res += 0x35;
				break;
			case '6':
				res += 0x36;
				break;
			case '7':
				res += 0x37;
				break;
			case '8':
				res += 0x38;
				break;
			case '9':
				res += 0x39;
				break;
			case 'A':
				res += 0x40;
				break;
			case 'B':
				res += 0x41;
				break;
			case 'C':
				res += 0x42;
				break;
			case 'D':
				res += 0x43;
				break;
			case 'E':
				res += 0x44;
				break;
			case 'F':
				res += 0x45;
				break;
		}
	}
	return res;
}
*/
