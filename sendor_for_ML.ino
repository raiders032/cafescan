#include <Wire.h>
#include <ESP8266WiFi.h>

//장애물 감지 센서
int sensorpin = D4; // out for D4(장애물 감지 센서값을 읽을 단자 설정)
int detec_val; // 센서값

//초음파 거리 센서
#define PIN_D1 5 // TRIGGER for HC-SR04(초음파 거리 센서)
#define PIN_D2 4 // ECHO for HC-SR04(초음파 거리 센서)

//진동 감지 센서
int vib =D8;     // DO for SW-420(진동감지)

// Information to connect My Private Server EC2
const char* private_server = "54.180.101.233"; 
const int serverPort       = 3000;

// Information to connect private Wifi
const char* ssid     = "2363";
const char* password = "47552363";

//global variable
String C_id="001";            //cafe id
String T_id="001";              //table id
String S_id="010";              //seat id 
int presence_flag=0;     //presence of the table
int idx;
String len_payload;
String vib_payload;
#define IDX_SIZE 60
 
void setup(){
  idx=0;
  len_payload="";
  vib_payload="";
  connectWifi();//connecting WIFI
  pinMode(vib, INPUT); //진동센서핀 입력
  pinMode(PIN_D1, OUTPUT); // 초음파 송신부 D1을 output으로
  pinMode(PIN_D2, INPUT);  // 초음파 수신부 D2를 input으로
  pinMode (sensorpin, INPUT); // 센서값을 인풋으로 설정 장애물 감지 센서
  Serial.begin(9600); //시리얼통신 설정 9600
  Serial.println("----------------------start------------------------");
}

void loop(){
  presence_flag=0;
  
  //초음파 거리 센서
  double distance=get_length();
  Serial.print("distance");
  Serial.print(String(idx)+": ");
  Serial.print(distance);
  Serial.println("cm");

  delay(10);
  
  //진동센서
  long measurement =TP_init();
  Serial.print("measurment");
  Serial.print(String(idx)+": ");
  Serial.println(measurement);
  delay(10);
  if(idx != IDX_SIZE-1){ 
    len_payload+=String(distance)+",";
    vib_payload+=String(measurement)+",";
  }
  else{
    len_payload+=String(distance);
    vib_payload+=String(measurement);
  }
  
  //서버 전송
  if(idx==IDX_SIZE-1){
    //장애물 감지 센서
    detec_val = digitalRead (sensorpin); // 센서값을 읽어옴
    if (detec_val == LOW) { // 장애물 감지 안됨
      presence_flag=1;
    }
    Serial.print("table id: ");
    Serial.println(T_id);
    Serial.print("seat id: ");
    Serial.println(S_id);
    Serial.println("len payload: ");
    Serial.println(len_payload);
    Serial.println("vib payload: ");
    Serial.println(vib_payload);
    Serial.print("presence_flag: ");
    Serial.println(presence_flag);
    sendSV(C_id, T_id, S_id, len_payload,vib_payload, presence_flag);
    len_payload="";
    vib_payload="";
    idx=0;
  }
  else
    idx++;
  delay(100);
}

void connectWifi(){
  Serial.println();
  Serial.print("connecting to WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\n Got WiFi, IP address: ");
  Serial.println(WiFi.localIP());   
}

//초음파 거리 센서 거리 측정
double get_length(){
  double duration;
  double distance;
  digitalWrite(PIN_D1, LOW);  
  delayMicroseconds(2); 
  digitalWrite(PIN_D1, HIGH);
  delayMicroseconds(10); 
  digitalWrite(PIN_D1, LOW);
  duration = pulseIn(PIN_D2, HIGH); // 왕복한 시간
  distance = (duration/2) / 29.0; // 거리 = 시간 x 속도, 소리의 속도 29um/cm
  return distance; 
}

long TP_init(){
  //delay(10);
  long measurement=pulseIn (vib, HIGH);
  if(measurement>1000)
    measurement=1000;
  return measurement;
}

void sendSV(String C_id, String T_id, String S_id, String len_payload, String vib_payload, int presence_flag){  
   WiFiClient client;
   if (client.connect(private_server, serverPort)) { 
       Serial.println("SV WiFi Client connected ");
       String getheader = "GET /data?C_id="+ C_id + "&T_id="+ T_id + "&S_id="+ S_id
       +"&len="+ len_payload +"&vib="+ vib_payload +"&state="+ String(presence_flag) +" HTTP/1.1";
       client.println(getheader);
       client.println("User-Agent: ESP8266 ");  
       client.print("Host: 3.16.161.61\n");
       client.print("Connection: close\n");
       client.print("\n\n");
  }
  client.stop();
}
