#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

int lcd_key     = 0;
int adc_key_in  = 0;
int a = 0;//direccion giro 0 = nada, 1 derecha, 2 izquierda
int b = 0;//velocidad 0 = baja, 1 = alta empieza en alta por defecto
int c = 0;//0 = dirrecion giro, 1= velocidad    para selection saber donde esta empieza en direccion 
int distancia=0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5


const int TriggerPin = 0; // Trigger ultrasonidos (el echo es definido en el setup por estar a traves de los pines analogicos

//Configuracion MOTOR
int ENB = 3;   // ENB  PIN  
int IN3 = 2;   // IN3  PIN  
int IN4 = 1;   // IN4  PIN  
int IN1 = 11;   // IN1  PIN   
int IN2 = 12;   // IN2  PIN  
int ENA = 13;   // ENA  PIN   


  

void setup() {
      pinMode(A1, INPUT);//echo pin A1 analogico 1
      lcd.begin(16, 2);              // Inicializar el LCD
      lcd.setCursor(0,0);
      lcd.print("Direccion giro:");     
      Serial.begin(9600);
      pinMode(TriggerPin, OUTPUT);
      pinMode(A1, INPUT);
      pinMode (ENA, OUTPUT);    // ENB conectado al PIN 1
      pinMode (ENB, OUTPUT);    // ENA conectado al PIN 13
      pinMode (IN1, OUTPUT);    // Input1 conectado al PIN 12
      pinMode (IN2, OUTPUT);    // Input2 conectado al PIN 11
      pinMode (IN3, OUTPUT);    // Input3 conectado al PIN 2
      pinMode (IN4, OUTPUT);    // Input4 conectado al PIN 3

}

void loop()
   {  
 
     
 
      lcd_key = read_LCD_buttons();
             lcd_key = read_LCD_buttons();
      if( lcd_key == btnRIGHT){
              lcd.setCursor(0,0);
              lcd.print("Direccion giro:");    
              a = 1; //giro a derechas
              lcd.setCursor(0,1);              // Cursor a linea 2, posicion 1
              lcd.print("derecha  ");
              c=0;
              lcd.setCursor(15,1);
              lcd.print(c);
      }
      else if ( lcd_key == btnLEFT ){
              lcd.setCursor(0,0);
              lcd.print("Direccion giro:");    
              a = 2; //izquierda
              lcd.setCursor(0,1);              // Cursor a linea 2, posicion 1
              lcd.print("izquierda");
              c=0;
              lcd.setCursor(15,1);
              lcd.print(c);
      }
      else if ( lcd_key == btnUP){
              lcd.setCursor(0,0);
              lcd.print("Velocidad:       ");    
              b = 1; //
              lcd.setCursor(0,1);              // Cursor a linea 2, posicion 1
              lcd.print("alta     ");
              c=0;
              lcd.setCursor(15,1);
              lcd.print(c);
      }
      else if ( lcd_key == btnDOWN){
              
              lcd.setCursor(0,0);
              lcd.print("Velocidad:            ");    
              b = 0; //
              lcd.setCursor(0,1);              // Cursor a linea 2, posicion 1
              lcd.print("baja     ");
              c=0;
              lcd.setCursor(15,1);
              lcd.print(c);
      }
      else if ( lcd_key == btnSELECT){
        if(c==1){
           c=0;
           lcd.setCursor(15,1);
           lcd.print(c); 
      }
       if(c==0){
         c=1;
         lcd.setCursor(15,1);
        lcd.print(c);
      } 
      }
              
      /*else if ( lcd_key == btnNONE)
              lcd.print("NONE  ");*/


         
         int cm = ping(TriggerPin, A1);
         Serial.print("Distancia: ");
         Serial.println(cm);
         delay(1000);
         lcd.setCursor(11,1);
         lcd.print(cm);
         if(cm<10){
          lcd.setCursor(12,1);
          lcd.print("   ");
         }
         if(cm>=10 && cm<100){
          lcd.setCursor(13,1);
          lcd.print("  ");
         }
         if(cm>=100 && cm<1000){
          lcd.setCursor(14,1);
          lcd.print(" ");
         }

       
         
        if(c==1){ 
           if(cm<=50){
            digitalWrite (IN1, HIGH);
            digitalWrite (IN2, HIGH);
            digitalWrite (IN3, HIGH);
            digitalWrite (IN4, HIGH);
           }
           else if(cm<80){
              if(a==1){//giros a derechas
                  Serial.println("giro Derecha");
                  giroDerecha();
              }
              else if (a==2){//giros a izquierdas
                  Serial.println("giroIzquierda");
                  giroIzquierda();
              }
              else 
                  continua(b);
           }
           else  
              continua(b);
        }
         
   }

int read_LCD_buttons()  
  { adc_key_in = analogRead(0);      // Leemos A0
    // Mis botones dan:  0, 145, 329,507,743
    // Y ahora los comparamos con un margen comodo
    if (adc_key_in > 900) return btnNONE;     // Ningun boton pulsado 
    if (adc_key_in < 50)   return btnRIGHT; 
    if (adc_key_in < 250)  return btnUP;
    if (adc_key_in < 450)  return btnDOWN;
    if (adc_key_in < 650)  return btnLEFT;
    if (adc_key_in < 850)  return btnSELECT; 

    return btnNONE;  // Por si todo falla
  }

int ping(int TriggerPin, int EchoPin) {
 long duration, distanceCm;
 
 digitalWrite(TriggerPin, LOW);  //para generar un pulso limpio ponemos a LOW 4us
 delayMicroseconds(4);
 digitalWrite(TriggerPin, HIGH);  //generamos Trigger (disparo) de 10us
 delayMicroseconds(10);
 digitalWrite(TriggerPin, LOW);
 
 duration = pulseIn(EchoPin, HIGH);  //medimos el tiempo entre pulsos, en microsegundos
 
 distanceCm = duration * 10 / 292/ 2;   //convertimos a distancia, en cm
 return distanceCm;
}

void continua(int velocidad){//si no hay obstaculos mas adelante
        
          
          //rueda 2 OUT B
          Serial.println("continua B");
          digitalWrite (IN3, LOW);
          digitalWrite (IN4, HIGH);
          if(velocidad==0){
            Serial.println("continua rueda B");
            analogWrite(ENB,150);//velocidad baja
          }else 
            analogWrite(ENB,200);//velocidad alta

            //rueda 1 OUT A
          Serial.println("continua rueda A");
          digitalWrite (IN1, LOW);
          digitalWrite (IN2, HIGH);
          if(velocidad==0)
             analogWrite(ENA,150);//velocidad baja
          else 
            analogWrite(ENA,200);//velocidad alta
          //delay(2000);
}

void giroDerecha(){
  //Lado OUT B
          digitalWrite(IN3,HIGH);
          analogWrite(ENB,0);
          delay(2000);
          digitalWrite(IN3,LOW);
          if(b==0)
            analogWrite(ENB,150);
          else
            analogWrite(ENB,250);
}

void giroIzquierda(){
  //Lado OUT A
          digitalWrite (IN1, HIGH);
          digitalWrite (IN2, HIGH);

          delay(2000);
          digitalWrite(IN1,LOW);
          digitalWrite(IN2,HIGH);
          if(b==0)
            analogWrite(ENA,150);
          else
            analogWrite(ENA,250);
}

