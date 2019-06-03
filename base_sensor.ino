#include <Adafruit_MLX90614.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#define PIN_A0 A0 // 아날로그 핀(A0)
/*  참고
static const uint8_t D0   = 16;
static const uint8_t D1   = 5;
static const uint8_t D2   = 4;
static const uint8_t D3   = 0;
static const uint8_t D4   = 2;
static const uint8_t D5   = 14;
static const uint8_t D6   = 12;
static const uint8_t D7   = 13;
static const uint8_t D8   = 15;
static const uint8_t D9   = 3;
static const uint8_t D10  = 1;
*/

Adafruit_MLX90614 mlx = Adafruit_MLX90614();  //적외선 온도 센서
int sensor_sr501 = D3; //  D3 : 적외선 인체 감지센서

const char* server2 = "3.16.161.61";
//String apiKey ="XLTL6DFFAB7QER25";
const char* MY_SSID = "SK_WiFiGIGAEDBC";
//const char* MY_SSID = "3627";
const char* MY_PWD = "1701012482";
//const char* MY_PWD = "hyeongchan";


int id=1;
void setup() {
  // 시리얼 통신 초기화
  Serial.begin(9600);
  pinMode(D3, INPUT);
  mlx.begin();      //적외선 온도 센서
  //connectWifi();  //와이파이 커넥션
}
 
void loop() {
 int volt = getVolt();                //적외선 거리 센서 거리 읽기
 long state = digitalRead(sensor_sr501);      //인체감지센서 상태 읽기
 float ambi_temp=mlx.readAmbientTempC();      //적외선 온도 감지 센서 주변 온도 읽기
 float obj_temp=mlx.readObjectTempC();        //타켓 온도 읽기
 float gap_temp=ambi_temp-obj_temp;
 
 if(state == HIGH){
    //모션 감지 센서
    Serial.println("Motion detected!");
    //sendSV(id,state,distance,ambit,objt);
  } 
  else {
    Serial.println("Motion absent!");
    //sendSV(id,state,distance,ambit,objt);
  }
  //적외선 거리 센서
  Serial.print("volt : ");
  Serial.println(volt);
  //적외선 온도 센서
  Serial.print("Ambient = "); 
  Serial.println(ambi_temp); 
  Serial.print("Object = "); 
  Serial.println(obj_temp);
  Serial.print("gap = "); 
  Serial.println(gap_temp);  
  delay(3000);
}
 void connectWifi(){
  Serial.print("Connecting to "+*MY_SSID);
  WiFi.begin(MY_SSID, MY_PWD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Connected");
  Serial.println("");  
}//end connect

int getVolt(){
    float volt;  // 전압(V)
    double dist; // 거리(cm)
    volt = analogRead(PIN_A0);// 센서로부터의 출력 전압
    return volt;
}


void sendSV(int id,long state, int distance ,float ambit,float objt )
{  
   WiFiClient client;
     if (client.connect(server2, 3000)) { 
   Serial.println("SV WiFi Client connected ");
   
   //client.print("GET /data HTTP/1.1\n");
   
    String getheader = "GET /data?id="+ String(id)+"&state="+ String(state)+"&distance="+ String(distance) +"&ambit="+ String(ambit) +"&objt="+ String(objt) +" HTTP/1.1";
    client.println(getheader);
    client.println("User-Agent: ESP8266 ");  
   client.print("Host: 3.16.161.61\n");
   client.print("Connection: close\n");
   client.print("\n\n");

   delay(1000);
   //delay(15000);

  }
  client.stop();
}
