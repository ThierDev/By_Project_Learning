void sendTrame(char VAL, char NUM, char TYP) {
	char trame[18];
	
	//Définition de la trame
	trame[0] = '1';		//TRA
	trame[1] = '0';		//OBJ
	trame[2] = '0';		//OBJ
	trame[3] = '9';		//OBJ
	trame[4] = 'D';		//OBJ
	trame[5] = '1';		//REQ
	trame[6] = TYP;		//TYP
	trame[7] = NUM[0];	//NUM
	trame[8] = NUM[1];	//NUM
	trame[9] = VAL[0];	//VAL
	trame[10] = VAL[1];	//VAL
	trame[11] = VAL[2];	//VAL
	trame[12] = VAL[3];	//VAL
	trame[13] = '0';	//TIM
	trame[14] = 'E';	//TIM
	trame[15] = 'C';	//TIM
	trame[16] = 'R';	//TIM
	trame[17] = '4';	//CHK
	trame[18] = '4';	//CHK
	
	// A mettre dans le loop ensuite pour gérer activation de l'écoute ou de l'écriture de la carte
	int pot = analogRead(28);	//Lecture de la valeur du potentiomètre(28)
	
	if(pot>1000) {
		for(int i=0; i<19; i++) {
			Serial1.print(trame[i]);
			Serial.print(trame[i]); 	// Not sure if we really need this line
			Serial.println();
		}
	}
}