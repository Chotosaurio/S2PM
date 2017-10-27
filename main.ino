#include<Wire.h>
#include<LiquidCrystal_I2C.h>
//el 24 de octubre de 2017 subi la primera parte pero llevo avanzando lentamente con el programa desde fines de agosto


LiquidCrystal_I2C lcd(0x27,16,2);
int timerposta=0;
bool a=false,b=false,a2=false,b2=false;//variables de lectura de los cascos
bool repeON1, repeblue1=false,repeON2=false,repeblue2=false;//booleanos para guardas los estados;
short int globo=8;//el globo xd
short int ledazul2=7 , ledON2=3;//pines del segundo casco
short int ledON=4 , ledazul=5 , ledverde=6 , xd2;//pines del primer casco y una variable para hacer calculos
short int relayAlco=13 , relaygral=12;//los dos reles

void setup() {
timerposta;
Serial.begin(9600);
Wire.begin();
lcd.begin();
lcd.clear();
// put your setup code here, to run once:
pinMode(ledON2,INPUT_PULLUP);
pinMode(ledazul2,INPUT_PULLUP);
pinMode(ledON,INPUT_PULLUP);
pinMode(ledazul,INPUT_PULLUP);
pinMode(ledverde,INPUT_PULLUP);
pinMode(globo,INPUT_PULLUP);
pinMode(A0,INPUT);
pinMode(relayAlco,OUTPUT);
digitalWrite(relayAlco,LOW);
pinMode(relaygral,OUTPUT);
}

void loop() {
  timerposta++;
    digitalWrite(relayAlco,HIGH);
  delay(200);
  int lemia=0;
  a=digitalRead(4);//casco prendido
  b=digitalRead(5);//switch casco
  a2=digitalRead(3);//casco2
  b2=digitalRead(7);//switch casco 2
  if(!a/*and estasentado1*/){
    repeON1=false;
    repeblue1=false;
    lcd.setCursor(0,0);
    lcd.print("1 Casco=N            ");}
  if(a){
    if(repeON1==false){
    repeON1=true;
    lcd.clear();
    lcd.setCursor(0,0);}
    digitalWrite(relayAlco,HIGH);
    lcd.print("1 casco=S         ");
    if(!b and !repeblue1){
      repeblue1=true;
      lcd.setCursor(10,0);
      lcd.print("Puesto");
  }
  else if(!b and repeblue1){
    repeblue1=false;}
  }
  if(!a2/*and estasentado2*/){
    repeON2=false;
    repeblue2=false;
    lcd.setCursor(0,1);
    lcd.print("2 Casco=N            ");}
  if(a2 ){
    if(repeON2==false){
    repeON2=true;
    lcd.setCursor(0,1);}
    lcd.print("2 casco=S");
    if(b2 and !repeblue2){
      repeblue2=true;
      lcd.setCursor(10,1);
      lcd.print("Puesto=S");
  }
  else if(!b2 and repeblue2){
    repeblue2=false;}
  }
  
  if(a==true and !b==true/* and (bool)sentado1*/){
  digitalWrite(relayAlco,HIGH);
  int timer=0;
  for(int i=1;i<=20;i++){
    delay(1000);
    lcd.setCursor(0,1);
  }
  xd2=analogRead(A0);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("sople");
  delay(175);
  while(timer<10){
    bool xd=digitalRead(globo);
    if(dale(digitalRead(4),digitalRead(5)/*sentado1,sentado2*/)){
      timerposta=0;
      return;}
    while(!xd and timer<30){ 
      xd=digitalRead(globo);
      int aire=analogRead(A0);
      lcd.setCursor(0,1);
      lcd.print(aire);
      lcd.print("  ");
      delay(500);
      lemia=lemia+aire;
      timer++;
    }}
   lemia=lemia/10;
  }
  if(a and !b==true/*and sentado*/){
  bool enpedo=true;
  lcd.setCursor(0,0);
  if(lemia>=xd2+2){
    lcd.print("no puede conducir");
    enpedo=true;
    digitalWrite(relaygral,HIGH);
  }
  else{
    lcd.print("conduzca tranquilo");
    enpedo=false;
    digitalWrite(relaygral,LOW);
    for(unsigned i=1;i<3;i++){
    delay(400);
    lcd.scrollDisplayLeft();}
  }
  for(unsigned i;i<50000;i++){
  delay(1);
  timerposta++;
  if(Serial.read() or dale(digitalRead(4),digitalRead(5)/*sentado1,sentado2*/),timerposta>50000){
  timerposta=0;
  return;}}}
}
