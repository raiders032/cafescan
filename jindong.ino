
int vib =D7;
 
void setup(){
  pinMode(vib, INPUT); //센서핀 입력
  Serial.begin(9600); //시리얼통신 설정 9600
  Serial.println("----------------------vibration demo------------------------");
}
void loop(){
  long measurement =TP_init();
  delay(50);
  Serial.print("measurment = ");
  Serial.println(measurement);
}
 
long TP_init(){
  delay(10);
  long measurement=pulseIn (vib, HIGH);
  return measurement;
}
