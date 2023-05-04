/* author: Noe Sheridan
created: 05-03-2023
updated:
how updated:
worked with: Reagan Hardy 
*/

#define LED 13 //assigns value LED to pin 13 

volatile unsigned char gISRFlag2 =0;
unsigned int reloadtimer1 = 100; //0.4 ms 


int buffer_size = 20;
char array[4];
bool start; 
char inChar;
char gCommsMsgBuff[buffer_size];
int ibuffer = 0;
byte packageflag = 0;
byte processdataflag = 0; 


char compareArrays(char a[], char b[], int size){//function compares arrays
  int i;
  char result = 1;  // default: the arrays are equal
  
  for(i = 0; i<size; i++)
  {
    if(a[i]!=b[i])
    {
      result = 0;
      break;
    }
  }
  return result;
}


void setup() {
// put your setup code here, to run once:
pinMode(LED, OUTPUT);

Serial.begin(9600);

// Initialize Timer1 for Serial Comms
// Speed of Timer1 = 16MHz/64 = 250 KHz
TCCR1A = 0;
TCCR1B = 0;
OCR1A = reloadtimer1;            //  65535
TCCR1A |= (1<<WGM11);
TCCR1B = (1<<CS11) | (1<<CS10);   // 64 prescaler
TIMSK1 |= (1<<OCIE1A);
interrupts();
}

void loop() {


  char altgCommsMsgBuff[buffer_size];
  int altcount =0;
  unsigned char altdigit = '0';

  if(gISRFlag2 == 1){
    gISRFlag2 = 0;
    inChar = (Serial.read());

    if(packageflag == 1){
      gCommsMsgBuff[ibuffer] = inChar; //reccords the character as part of the message
      ibuffer++;

      if(ibuffer == buffer_size){
        packageflag =0;
        processdataflag =1;
      }
    }
  }

  if(inChar == '$'){ //start of package
    packageflag =1;

    for(int i=0;i< buffer_size; i++){ //clears previous package
      gCommsMsgBuff[i]=0;
    }

    ibuffer = 0;
  }

  if((inChar == '\n')&(packageflag ==1 )){//end of package
    packageflag =0;
    processdataflag = 1;
  }


  if(processdataflag ==1){//proccess serial commands 

    processdataflag = 0;

    if(compareArrays(gCommsMsgBuff, "STR", 3) ==1 ){
      //turn on LED 
      //start command
      digitalWrite(LED, HIGH);
    }
    if(compareArrays(gCommsMsgBuff, "STP", 3) == 1){
      //stop command 
      digitalWrite(LED, LOW);
    }
    if(compareArrays(gCommsMsgBuff, "GET", 3) == 1){
      //send clock 
      Serial.print("$00:01\n");
    }


  }
  
}

/**
 * @brief Timer 1 ISR
 * @param TIMER1_COMPA_vect
 * @return
 */
ISR(TIMER1_COMPA_vect)  // Timer1 interrupt service routine (ISR)
{
  if(Serial.available()>0)
  {
    gISRFlag2 = 1;
  }
}

