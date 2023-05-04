/* author: Noe Sheridan
created: 04-25-2023
updated:
how updated:
worked with: Reagan Hardy 

*/

#define LED 13 //assigns value LED to pin 13 

int iscommand;
char array[4];
bool start; 

void setup() {
  // put your setup code here, to run once:


  Serial.begin(9600); //starts the serial 

}

void loop() {
  // put your main code here, to run repeatedly:
  
  if(Serial.available()){
    //Serial.print(" I received ");
    Serial.write("Received command"); //writes what the serial reads 
    //Serial.write(Serial.read());
    char data = (Serial.read());
    if(data == '$'){
      iscommand = 1;
      
    }
    if(data == '\n'){
      //safety measure
    }

    if(iscommand){
      iscommand = 0;
      int i = 0;
      array[i]= data;
      i++;
      
    }

    if(array == 'STR'){
      start = true;
    } else if(array == 'STP'){
      start = false;
    }

    blink(start);
    for(int i= 0; i<4; i++){
        array[i]=0; //clear array
    }
  }
  
}

void blink(bool){
  if(start){
    digitalWrite(LED, HIGH);
  }else{
    digitalWrite(LED, LOW);
  }
}
