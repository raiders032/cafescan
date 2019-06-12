#include <Wire.h>
#include <ESP8266WiFi.h>
#define DIST_S 100*58.2

const char* server2 = "3.16.161.61";
//String apiKey ="XLTL6DFFAB7QER25";
//const char* MY_SSID = "SK_WiFiGIGAEDBC";
const char* MY_SSID = "myWifi";
//const char* MY_PWD = "1701012482";
const char* MY_PWD = "myPassword";


int vib =D0;

int echoPin = D1;
int trigPin = D2;

int echoPin2 = D3;
int trigPin2 = D4;

int echoPin3 = D5;
int trigPin3 = D6;

int echoPin4 = D7;
int trigPin4 = D8;

String C_id="001";            //cafe id
String T_id="001";              //table id
String S_id="001";    //seat id

int presence_flag=0;     //presence of the table
int idx;

String len_payload[4];
String vib_payload[4];
#define IDX_SIZE 60

// 실행시 가장 먼저 호출되는 함수이며, 최초 1회만 실행됩니다.
// 변수를 선언하거나 초기화를 위한 코드를 포함합니다.
void setup() {
  // 적외선 장애물 감지 센서 핀을 INPUT으로 설정합니다.
    idx=0;
    for(int i=0;i<4;i++){
      len_payload[i]="";
      vib_payload[i]="";
    }
    connectWifi();//connecting WIFI
    
      pinMode(vib, INPUT);
      pinMode(trigPin, OUTPUT);
      pinMode(echoPin, INPUT);
      pinMode(trigPin2, OUTPUT);
      pinMode(echoPin2, INPUT);
      pinMode(trigPin3, OUTPUT);
      pinMode(echoPin3, INPUT);
      pinMode(trigPin4, OUTPUT);
      pinMode(echoPin4, INPUT);
  

  // 적외선 장애물 감지 센서의 상태를 확인하기 위하여 시리얼 통신의 속도를 설정합니다.
  Serial.begin(9600);
}

// setup() 함수가 호출된 이후, loop() 함수가 호출되며,
// 블록 안의 코드를 무한히 반복 실행됩니다.
void loop() {
  presence_flag=0;

  double distance[4];
  
  long measurement =TP_init();
  
  // 초음파를 보낸다. 다 보내면 echo가 HIGH 상태로 대기하게 된다.
    distance[0]=get_dis(echoPin,trigPin);
    distance[1]=get_dis(echoPin2,trigPin2);
    distance[2]=get_dis(echoPin3,trigPin3);
    distance[3]=get_dis(echoPin4,trigPin4);

 for(int i=0;i<4;i++){

  Serial.print(distance[i]);
  Serial.println("cm");
  
  Serial.print("measurment = ");
  Serial.println(measurement);
 }
 
   if(idx != IDX_SIZE-1){
    for(int i=0;i<4;i++){ 
      len_payload[i]+=String(distance[i])+",";
      vib_payload[i]+=String(measurement)+",";
    }
  }
  else{
    for(int i=0;i<4;i++){
      len_payload[i]+=String(distance[i]);
      vib_payload[i]+=String(measurement);
    }
  }
   //서버 전송
  if(idx==IDX_SIZE-1){
    //장애물 감지 센서
    for(int i=0;i<4;i++){
       sendSV(C_id, T_id, S_id, len_payload[i],vib_payload[i], presence_flag);
       len_payload[i]="";
       vib_payload[i]="";
    }
    idx=0;
  }
  else
    idx++;
  delay(100);
  
}

float get_dis(int echo ,int trig){
  float duration;
  
  digitalWrite(trig, HIGH);
  delay(10);
  digitalWrite(trig, LOW);
   
  duration = pulseIn(echo, HIGH, DIST_S); 
  return ((float)(340 * duration) / 10000) / 2;
}

long TP_init(){
  delay(10);
  long measurement=pulseIn (vib, HIGH);
  return measurement;
}

void sendSV(String C_id, String T_id, String S_id, String len_payload, String vib_payload, int presence_flag)
{  
   WiFiClient client;
     if (client.connect(server2, 3000)) { 
   Serial.println("SV WiFi Client connected ");
   
   String getheader = "GET /data?C_id="+ String(C_id)+ "&T_id="+ String(T_id)+ "&S_id="+ String(S_id)
   +"&len="+ String(len_payload)+"&vib="+ String(vib_payload) +"&state="+ String(presence_flag) +" HTTP/1.1";
   client.println(getheader);
   client.println("User-Agent: ESP8266 ");  
   client.print("Host: 3.16.161.61\n");
   client.print("Connection: close\n");
   client.print("\n\n");

   delay(1000);
  }
  client.stop();
}

void connectWifi(){
  Serial.println();
  Serial.print("connecting to WiFi: ");
  Serial.println(MY_SSID);
  WiFi.begin(MY_SSID,MY_PWD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\n Got WiFi, IP address: ");
  Serial.println(WiFi.localIP());   
}
