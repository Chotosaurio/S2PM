#include<Wire.h>
#include<LiquidCrystal_I2C.h>

struct duo{
  int pinOn, pinPuesto, contador=10, pinSentado;
  bool On, puesto, sentado;
} p1, p2;
struct  bandera{
  int counteron=0, countersentado=0, counterpuesto=0, countertodo=0;
  bool On, Ya, sentado, puesto, todo;
}flaguno,flagdos;
struct sensor{
  int counter=0, relay, soplo, aire;
  bool flag, no;
}alcol;
LiquidCrystal_I2C lcd(0x27, 16, 2);
int globo = 8, asientoat = 10, ledverde = 6 , xd2 , relaygral = 11;
bool a = false, b = false, a2 = false, b2 = false, yaseprobo=false,HDP;

void setup() {
  HDP=false;
  p1.pinOn = 4;
  p1.pinPuesto = 5;
  p1.pinSentado = 9;
  alcol.relay = 12;
  alcol.no=false;
  p2.pinOn = 3;
  p2.pinPuesto = 7;
  p2.pinSentado=10;
  Serial.begin(9600);
  Wire.begin();
  lcd.begin();
  lcd.clear();
  lcd.print("probando lalala");
  pinMode(p2.pinOn, INPUT_PULLUP);
  pinMode(p2.pinPuesto, INPUT_PULLUP);
  pinMode(p1.pinOn, INPUT_PULLUP);
  pinMode(p1.pinPuesto, INPUT_PULLUP);
  pinMode(globo, INPUT_PULLUP);
  pinMode(A0, INPUT);
  pinMode(alcol.relay, OUTPUT);
  digitalWrite(alcol.relay, LOW);
  pinMode(relaygral, OUTPUT);
  digitalWrite(relaygral, HIGH);
  pinMode(p1.pinSentado, INPUT);
  pinMode(p2.pinSentado, INPUT_PULLUP);
  lcd.clear();
}
void relay(sensor &alcohol){
  if(alcohol.no==true and yaseprobo and HDP==false){//si alcohol no hay
    digitalWrite(relaygral,LOW);//pasamos energia a la moto
    lcd.setCursor(0,1);
    lcd.print("Conduzca tranqui");
    //futuro=cambiar 
  }
}
void leerp1(duo &uno){
  uno.sentado=digitalRead(uno.pinSentado);
  uno.On=digitalRead(uno.pinOn);
  uno.puesto=digitalRead(uno.pinPuesto);
}
void resetflags(bandera &flag1){
  HDP=false;
  flag1.On=false;
  flag1.puesto=false;
  flag1.sentado=false;
  flag1.todo=false;
  flag1.countertodo=0;
  flag1.counterpuesto=0;
  flag1.countersentado=0;
  flag1.counteron=0;
  lcd.setCursor(0,0);
  lcd.print("                   ");
  lcd.setCursor(0,1);
  lcd.print("                   ");
}

void procesosit(bandera &flag1){
    flag1.countersentado++;
    flag1.countertodo=0;
    flag1.counterpuesto=0;
    flag1.counteron=0;
    if (flag1.countersentado>2){
      if(flag1.sentado==false){
        flag1.sentado=true;
        flag1.On=false;
        flag1.Ya=false;
        flag1.puesto=false;
        flag1.todo=false;
        }
      lcd.setCursor(0,0);
      lcd.print("sientese        ");
    if (flag1.countersentado>=6){
     HDP=true;
    lcd.setCursor(0,1);
    digitalWrite(relaygral,HIGH);
    lcd.print("!!                 "); 
    }
    }
}
void procesotodo(bandera &flag1){
  flag1.countersentado=0;
  flag1.countertodo++;
  flag1.counterpuesto=0;
  flag1.counteron=0;
  if (flag1.countertodo>3){ 
    if (flag1.todo==false){
      flag1.sentado=false;
      flag1.On=false;
      flag1.puesto=false;
      flag1.Ya=false;
      flag1.todo=true;
      }
    lcd.setCursor(0,0);
    lcd.print("haga todo bien  ");
   if(flag1.countertodo>8 ){
    HDP=true;
    lcd.setCursor(0,1);
    digitalWrite(relaygral,HIGH);
    lcd.print("!!                 ");
  }
  } 
}
void procesocascopuesto(bandera &flag1){
  flag1.countersentado=0;
  flag1.countertodo=0;
  flag1.counterpuesto++;
  flag1.counteron=0;
  if (flag1.counterpuesto>3){
    if(flag1.puesto==false){
      flag1.sentado=false;
      flag1.On=false;
      flag1.Ya=false;
      flag1.puesto=true;
      flag1.todo=false;
      }
    lcd.setCursor(0,0);
    lcd.print("pongase el casco");
    if(flag1.counterpuesto>8 ){
      lcd.setCursor(0,1);
      HDP=true;
      digitalWrite(relaygral,HIGH);
      lcd.print("!!                ");
    }
  }
}
void procesocascoon(bandera &flag1){
  flag1.countersentado=0;
  flag1.countertodo=0;
  flag1.counterpuesto=0;
  flag1.counteron++;
  if (flag1.counteron>5){
    if(flag1.On==false){
      flag1.sentado=false;
      flag1.Ya=false;
      flag1.On=true;
      flag1.puesto=false;
      flag1.todo=false;
      }
      lcd.setCursor(0,0);
      lcd.print("prenda el casco ");
       if(flag1.counteron>8){
    lcd.setCursor(0,1);
    HDP=true;
    digitalWrite(relaygral,HIGH);
    lcd.print("!!                 ");
  }
  }
}
void sensordealcohol(bandera &flag1, sensor &alcohol){
  if(yaseprobo==false){
  if(flag1.Ya==true){
    alcohol.flag=true;
    digitalWrite(alcohol.relay,HIGH);
    alcohol.counter++;
    if (alcohol.counter<10){ 
      lcd.setCursor(7,1);
      lcd.print(alcohol.counter);
      lcd.print("/10      ");
    }
    if (alcohol.counter>=15){
      lcd.setCursor(5,1);
      lcd.print("     sople   ");
      bool a=digitalRead(globo);
      lcd.setCursor(0,1);
      lcd.print(a);
      alcohol.soplo+=a;
      if( a and alcohol.soplo<3){
        alcohol.aire+=digitalRead(A0);
        return;
      }else if (alcohol.soplo>=3){
        if (alcohol.aire<3){//si aire es menor a 3 osea, si las 3 veces que detecto alcohol eran falsas
          alcohol.no=true;//alcohol no hay
          lcd.setCursor(0,1);
          lcd.print("alcohol 0           ");
          yaseprobo=true;
        }else{
          alcohol.no=false;
          lcd.setCursor(0,1);
          lcd.print("no conduzca           ");
        }yaseprobo=true;
      }
    }
  return;
  }else{
    alcohol.no=false;
    alcohol.flag=false;
    alcohol.counter=false;
    alcohol.aire=0;
  }
}
}
void comprobacionp1(duo &uno , bandera &flag1){//poner aca algo como void comprobacion(struct persona) y en vez de poner uno.on pongo persona.on y funca para las 2 y ahorro memoria
   if(uno.On==false and uno.puesto==true and uno.sentado==0){
    procesotodo(flag1);
    return;
  }if(uno.On== true and uno.puesto==false and uno.sentado==true){
    resetflags(flag1);//no se si tengo q hacer algo mas
    flag1.Ya=true;
    return;
  }
  if(uno.puesto==true and uno.On==true){
    procesocascopuesto(flag1);
    return;
  }if(uno.sentado==0 ){
    procesosit(flag1);
    return;
  }  if (uno.On==false){
    procesocascoon(flag1);
    return;
  }
  resetflags(flag1);
  flag1.Ya=false;
}
void personauno(){
  leerp1(p1);//puedo usar todos estos para el persona2 
  comprobacionp1(p1,flaguno);//puedo usar todos estos para el persona2 
  sensordealcohol(flaguno, alcol);
//void personados(){
//leerp1(dos);
//comprobacionp1(dos)
}

void debug(sensor &alcohol){
  Serial.print(p1.On);
  Serial.print(p1.puesto);
  Serial.print(p1.sentado);
  Serial.println(alcohol.aire);
}
void loop(){
personauno();
//personados();
relay(alcol);
debug(alcol);
delay(950);
}//se repite
