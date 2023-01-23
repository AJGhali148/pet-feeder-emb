#define SERVO_PIN PORTCbits.RC2
unsigned int TCntr = 0;
unsigned int chkctr;
char isclosed=1;
char opendoor1=0;
unsigned int weight=0;
char isfull=0;
unsigned int distance = 0;
void calcdist();
void BUZZER();
void opendoor();
void calcdisttank();
sbit LCD_RS at RB4_bit;
sbit LCD_EN at RB5_bit;
sbit LCD_D4 at RB0_bit;
sbit LCD_D5 at RB1_bit;
sbit LCD_D6 at RB2_bit;
sbit LCD_D7 at RB3_bit;

sbit LCD_RS_Direction at TRISB4_bit;
sbit LCD_EN_Direction at TRISB5_bit;
sbit LCD_D4_Direction at TRISB0_bit;
sbit LCD_D5_Direction at TRISB1_bit;
sbit LCD_D6_Direction at TRISB2_bit;
sbit LCD_D7_Direction at TRISB3_bit;

char txt1[] = "OPENING...";
char txt2[]= "CLOSING...";
char txt3[]= "WELCOME!!!";



void NoTDelay(unsigned int t){
        unsigned int count=0;
        unsigned int k=0;
        while(count<t){
                count++;
                for(k=0;k<2000;k++);
        }
}





void interrupt(){

if(INTCON & 0x04){
 TMR0=248;
 TCntr++;
 chkctr++;
  INTCON = INTCON & 0xFB;
}
 }


void closedoor(){
        unsigned int duty;
        unsigned char ctr=1;
        Lcd_Cmd(_LCD_CLEAR);
        Lcd_Out(1,4,txt2);
        while(ctr) { // timer for closing the door
            duty=(40*90)/100;
            PORTC = PORTC | 0x04; // turn on motor
            PORTC = PORTC | 0x01;
            //PORTC = PORTC & 0xFD;
            NoTDelay(100); // 5 secs delay
            PORTC = PORTC & 0xFA;
            isclosed=1;
            NoTDelay(20);
            ctr--;
        }
    Lcd_Cmd(_LCD_CLEAR);
}
void opendoor(){
        unsigned int duty;
        unsigned char ctr=1;
        Lcd_Cmd(_LCD_CLEAR);
        Lcd_Out(1,4,txt1);
        while(ctr) // timer for opening the door
        {
          duty=(40*90)/100;
          PORTC = PORTC | 0x08; // To open the door by using the motor bits
          PORTC = PORTC | 0x02;
          //PORTC = PORTC & 0xFE;
          NoTDelay(100); // 5 secs delay
          PORTC=PORTC & 0xF5; // to turn off the motor
          opendoor1=1;
          NoTDelay(20);
          ctr--;
        }
    isclosed=0;
    Lcd_Cmd(_LCD_CLEAR);
    closedoor();
}


 void calcdisttank(void)
{ unsigned int ultra=0;
 unsigned int i=0;
 while(i!=4000){i++;}
 TMR1H=0;
 TMR1L=0;
 PORTD = PORTD | 0x40;
 while(ultra!=20){ultra++;};
 PORTD = PORTD & 0xBF ;
 while(!(PORTD & 0x80));
 T1CON = T1CON | 0x01;
 while(PORTD & 0x80);
 T1CON = T1CON & 0xFE;
 distance=(TMR1L | (TMR1H<<8));
 distance =(distance/58);
     if(distance > 17 ){
      BUZZER();
    }
}



 void calcdist(){
     unsigned int ultra=0;
     unsigned int i=0;
     while(i!=4000){
        i++;
      }
     TMR1H=0;
     TMR1L=0;
     PORTC = PORTC | 0x40; //C6 Echo
     while(ultra!=20){ultra++;};
     PORTC = PORTC & 0xBF ; //turn of C6
     while(!(PORTC & 0x80));
     T1CON = T1CON | 0x01;
     while(PORTC & 0x80);
     T1CON = T1CON & 0xFE;
     distance=(TMR1L | (TMR1H<<8));
     distance =(distance/58);
     if(distance >= 15 ){
        opendoor();
      }
    }
void BUZZER(){
  unsigned int cntr;
  unsigned int i;
  for(cntr=0;cntr<10;cntr++){
      PORTC =PORTC | 0x20;
    }
  PORTC = PORTC & 0xDF;
 }



int main() {
 TRISD = 0x80; //set PORTB as output except B7
 TRISC = 0x80; //set PORTC as output except C7
 PORTD=0x00; //Initialize PORTB
 PORTC=0x00; //Initialize PORTC
 PIE1 = 0x01; //timer1 enable
 T1CON = 0x10;
 TRISA = 0x0F; //First 4 are Input
 PORTA = 0x00; // Initialize
 INTCON = 0x20; // T0IE on
 TMR0 = 248;
 chkctr = 0;
 Lcd_Init();// Initialize LCD
 Lcd_Cmd(_LCD_CLEAR);  // Clear display
 Lcd_Cmd(_LCD_CURSOR_OFF); // Cursor off
 Lcd_Out(1,5,txt3);

  while(1){
  calcdist();
  calcdisttank();
}


  return 0;
}
