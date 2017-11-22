#include<Wire.h>
#include<LiquidCrystal_I2C.h>
struct duo{
  int pinOn;
  int pinPuesto;
  int contador;
  int pinSentado;
  bool On;
  bool puesto;
  bool sentado;
} uno, dos;
struct  bandera{
  int counteron;
  int countersentado; 
  int counterpuesto;
  int countertodo;
  bool On;
  bool Ya;
  bool sentado;
  bool puesto;
  bool todo;
}flag1,flag2;
struct sensor{
  int counter;
  int relay;
  int soplo;
  int aire;
  bool flag;
  bool no;
}alcohol;
LiquidCrystal_I2C lcd(0x27, 16, 2);
int globo = 8, asientoat = 10, ledverde = 6 , xd2 , relaygral = 12;
bool a = false, b = false, a2 = false, b2 = false, alcol=false;

void setup() {
  uno.pinOn = 4;
  uno.pinPuesto = 5;
  uno.pinSentado = 9;
  alcohol.relay = 13;
  dos.pinOn = 3;
  dos.pinPuesto = 7;
  Serial.begin(9600);
  Wire.begin();
  lcd.begin();
  lcd.clear();
  pinMode(dos.pinOn, INPUT_PULLUP);
  pinMode(dos.pinPuesto, INPUT_PULLUP);
  pinMode(uno.pinOn, INPUT_PULLUP);
  pinMode(uno.pinPuesto, INPUT_PULLUP);
  pinMode(ledverde, INPUT_PULLUP);
  pinMode(globo, INPUT_PULLUP);
  pinMode(A0, INPUT_PULLUP);
  pinMode(alcohol.relay, OUTPUT);
  digitalWrite(alcohol.relay, LOW);
  pinMode(relaygral, OUTPUT);
  digitalWrite(relaygral, HIGH);
  pinMode(uno.pinSentado, INPUT_PULLUP);
  pinMode(asientoat, INPUT_PULLUP);
}
void relay(){
  if(alcohol.no==true){//si alcohol no hay
    digitalWrite(relaygral,LOW);//pasamos energia a la moto
    //futuro=cambiar 
  }
}
void leerp1(){
  uno.sentado=digitalRead(uno.pinSentado);
  uno.On=digitalRead(uno.pinOn);
  uno.puesto=digitalRead(uno.pinPuesto);
}
void resetflags(){
  flag1.Ya=true;
  flag1.On=false;
  flag1.puesto=false;
  flag1.sentado=false;
  flag1.todo=false;
  flag1.countertodo=0;
  flag1.counterpuesto=0;
  flag1.countersentado=0;
  flag1.counteron=0;
}

void procesosit(){
    flag1.countersentado++;
    flag1.countertodo=0;
    flag1.counterpuesto=0;
    flag1.counteron=0;
    if (flag1.countersentado>6){
      if(flag1.sentado==false){
      flag1.sentado=true;
        flag1.On=false;
        flag1.Ya=false;
        flag1.puesto=false;
        flag1.todo=false;}
      lcd.setCursor(0,0);
      lcd.print("sientese bien   ");
    }
}
void procesotodo(){
  flag1.countersentado=0;
  flag1.countertodo++;
  flag1.counterpuesto=0;
  flag1.counteron=0;
  if (flag1.countertodo>6){ 
    if (flag1.todo==false){
      flag1.sentado=false;
      flag1.On=false;
      flag1.puesto=false;
      flag1.Ya=false;
      flag1.todo=true;}
    lcd.setCursor(0,0);
    lcd.print("haga todo bien  ");
  } 
}
void procesocascopuesto(){
  flag1.countersentado=0;
  flag1.countertodo=0;
  flag1.counterpuesto++;
  flag1.counteron=0;
  if (flag1.counterpuesto>6){
    if(flag1.puesto){
      flag1.sentado=false;
      flag1.On=false;
      flag1.Ya=false;
      flag1.puesto=true;
      flag1.todo=false;}
    lcd.setCursor(0,0);
    lcd.print("pongase el casco");
  }
}
void procesocascoon(){
  flag1.countersentado=0;
  flag1.countertodo=0;
  flag1.counterpuesto=0;
  flag1.counteron++;
  if (flag1.countersentado>8){
    if(flag1.On==false){
      flag1.sentado=false;
      flag1.Ya=false;
      flag1.On=true;
      flag1.puesto=false;
      flag1.todo=false;}
    lcd.setCursor(0,0);
    lcd.print("prenda el casco ");
  }
}
void sensordealcohol(){
  if(flag1.Ya==false){
    alcohol.flag=true;
    digitalWrite(alcohol.relay,HIGH);
    alcohol.counter++;
    if (alcohol.counter<15){ 
      lcd.setCursor(12,1);
      lcd.print(alcohol.counter);
      lcd.print("/15");
    }
    if (alcohol.counter>=15){
      lcd.setCursor(10,1);
      lcd.print("sople   ");
      bool a=digitalRead(globo);
      if( a and alcohol.soplo<3){
        alcohol.aire+=digitalRead(A0);
        return;
      }else if (alcohol.soplo>=3){
        if (alcohol.aire<3){//si aire es menor a 3 osea, si las 3 veces que detecto alcohol eran falsas
          alcohol.no=true;//alcohol no hay
        //ACA ALGO CREO
        }
      }
    }
  return;
  } else {
    alcohol.no=false;
    alcohol.flag=false;
    alcohol.counter=false;
    alcohol.aire=0;
  }
}
void comprobacionp1(){//poner aca algo como void comprobacion(struct persona) y en vez de poner uno.on pongo persona.on y funca para las 2 y ahorro memoria
  if(uno.On== true and uno.puesto==false and uno.sentado==false){
    resetflags();//no se si tengo q hacer algo mas
    return;
  }
  if(uno.sentado==true and uno.puesto==false and uno.On==true){
    procesosit();
    return;
  }if(uno.On==false and uno.puesto==true and uno.sentado==true){
    procesotodo();
    return;
  }if(uno.puesto==true and uno.On==true){
    procesocascopuesto();
    return;
  }if (uno.On==false){
    procesocascoon();
    return;
  }
}
void personauno(){
  leerp1();//puedo usar todos estos para el persona2 
  comprobacionp1();//puedo usar todos estos para el persona2 
  sensordealcohol();
  leerp1();//puedo usar todos estos para el persona2 
}//solo necesito pasarles el struct que van a usar y listo, el tema es q todavia no se si funcionan
void loop(){
personauno();
//personados();
relay();
delay(900);
}
