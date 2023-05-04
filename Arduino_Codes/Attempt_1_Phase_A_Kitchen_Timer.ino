#define BUTTON_1  2
#define BUTTON_2  3
#define GREEN_LED 4
#define RED_LED   5

#define DATA      9   //74HC595  pin 8 DS
#define LATCH     8   //74HC595  pin 9 STCP
#define CLOCK     7  //74HC595  pin 10 SHCP

#define DIGIT_4   10
#define DIGIT_3   11
#define DIGIT_2   12
#define DIGIT_1   13

unsigned char table[]=
{0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c
,0x39,0x5e,0x79,0x71,0x00};

unsigned char table2[]=
{0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c
,0x39,0x5e,0x79,0x71,0x00};

char count = 15;
int count1;
int count2;
int count3;
int count4;
unsigned int buttonState = 0;  // variable for reading the pushbutton status
int temp = 0;
volatile unsigned char isr_1_flag = 0;
volatile unsigned char isr_2_flag = 0;
volatile unsigned char isr_3_flag = 0;
volatile bool change; 
bool go = false;
void setup() {
  
  // LEDs Pins
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  // Button Pins
  pinMode(BUTTON_1, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_1), Button_1_ISR, CHANGE);
  pinMode(BUTTON_2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_2), Button_2_ISR, CHANGE);

  // 7-Seg Display
  pinMode(DIGIT_1, OUTPUT);
  pinMode(DIGIT_2, OUTPUT);
  pinMode(DIGIT_3, OUTPUT);
  pinMode(DIGIT_4, OUTPUT);

  // Shift Register Pins
  pinMode(LATCH, OUTPUT);
  pinMode(CLOCK, OUTPUT);
  pinMode(DATA, OUTPUT);

  disp_off();
  

  // initialize timer1 
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  OCR1A = 62500;            // compare match register 16MHz/256
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt

  interrupts();             // enable all interrupts
}

void Display(unsigned char num)
{
  digitalWrite(LATCH, LOW);
  shiftOut(DATA, CLOCK, MSBFIRST, table[num]);
  //shiftOut(DATA, CLOCK, MSBFIRST, 0x3F);
  digitalWrite(LATCH, HIGH);
}


void disp_off(){
   digitalWrite(DIGIT_1, HIGH);
   digitalWrite(DIGIT_2, HIGH);
   digitalWrite(DIGIT_3, HIGH);
   digitalWrite(DIGIT_4, HIGH);
}

void disp_on(int num){
  if(num == 1)
  {
    disp_off();
    digitalWrite(DIGIT_1, LOW);
  }else if (num==2){
    disp_off();
     digitalWrite(DIGIT_2, LOW);
  }else if (num==3){
    disp_off();
     digitalWrite(DIGIT_3, LOW);
  }else if (num==4){
    disp_off();
     digitalWrite(DIGIT_4, LOW);
  }
}
void Display_Numbers()
{
  //Display(count);


  if(count>= 50){
    temp = count -50;
    count4= temp;
    count3 = count - temp;
    count2= 0;
    count1 = 0;
  } else if (count < 50 && count >= 40){
    temp = count -40;
    count4= temp;
    count3 = count - temp;
    count2=0;
    count1 = 0;
  } else if(count < 40 && count >= 30){
    temp = count -30;
    count4= temp;
    count3 = count - temp;
    count2=0;
    count1 = 0;    
  }else if(count <30 && count >= 20){
    temp = count -20;
    count4= temp;
    count3 = count - temp;
    count2=0;
    count1 = 0;
  }else if(count <20 && count >= 10){
    temp = count -10;
    count4= temp;
    count3 = count - temp;
    count2=0;
    count1 = 0;
  } else if(count < 10){
    count4 = count;
    count1 = 0;
    count2 =0;
    count3=0;
  }
  disp_off();
  disp_on(1);
    Display(count1);
  
  disp_on(2);
    Display(count2);
  
  disp_on(3);
    Display(count3);
  
  disp_on(4);
    Display(count4);
  
}


void Button_1_ISR()
{
  // Set ISR Flag
  isr_1_flag = 1;
}

void Button_2_ISR()
{ 
  // Set ISR Flag
  isr_2_flag = 1;
}

ISR(TIMER1_COMPA_vect) // timer compare interrupt service routine
{
  // Set ISR Flag
  isr_3_flag = 1;
}

void loop() 
{ 
  // Combination of Pooling and Interrupt

  // Attend Button 1
  if(isr_1_flag == 1)
  {
    // Reset ISR Flag
    isr_1_flag = 0;

    // Code
    buttonState = digitalRead(BUTTON_1);
    digitalWrite(RED_LED, buttonState);
    if(buttonState){
      count ++;
    }
  }

  // Attend Button 2
  if(isr_2_flag == 1)
  {
    
    // Reset ISR Flag
    isr_2_flag = 0;

    // Code
    buttonState = digitalRead(BUTTON_2);

     change = !change; 
  
    if(change){
      go = !go;
    }
    digitalWrite(GREEN_LED, buttonState);
    
  }

  // Attend 7-Seg Display
  if(isr_3_flag == 1)
  {
    // Reset ISR Flag
    isr_3_flag = 0;

    // Code
    
    Display_Numbers();
    
    if(go){
    
      if(count > 0){
        count--;
      }
    }
      
  }
}