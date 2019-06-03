#include <Adafruit_MLX90614.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
//RFID
#include <SPI.h>
#include <MFRC522.h>
#define PIN_A0 A0 // 아날로그 핀(A0)
#define SS_PIN 2 //D4 rfid
#define RST_PIN 0 //D3 rfid

// Information to connect My Private Server EC2
const char* private_server = "3.16.161.61"; 
const int serverPort       = 3000;

// Information to connect private Wifi
const char* ssid     = "3627";
const char* password = "hyeongchan";

//global variable
int ID=1; //table id
Adafruit_MLX90614 mlx = Adafruit_MLX90614();  //적외선 온도 센서
int sensor_sr501 = D8; //  D8 : 적외선 인체 감지센서
bool detection_flag[30];
int detec_idx;
int num_of_detec;      //the number of detection during 30 secs
int presence_flag;     //presence of the table

void setup() {
  Serial.begin(9600);
  pinMode(D8, INPUT); //인체감지센서 dout <-> D8
  mlx.begin();      //적외선 온도 센서
  connectWifi();  //와이파이 커넥션
  detec_idx=0;    //감지횟수 초기화
  for(int i=0;i<30;i++){
    detection_flag[i]=0;
  }
}
 
void loop() {
  presence_flag=0;
  //detecting motion for 10sec
  for(int i=0;i<10;i++){
    long state = digitalRead(sensor_sr501);            
    if(state == HIGH){
     detection_flag[detec_idx]=true;
     Serial.print(detec_idx);
     Serial.println("Motion detected!");
    } 
    else {
      detection_flag[detec_idx]=false;
      Serial.print(detec_idx);
      Serial.println("Motion absent!");
    }
    detec_idx_up();
    if(i != 9){
      delay(1000);
    }
  }

  //20초 전 정보 측정
  int pre_volt2 = getVolt();                                //적외선 거리 센서 볼트 측정
  float pre_ambi_temp2 = mlx.readAmbientTempC();            //적외선 온도 감지 센서 주변 온도 측정
  float pre_obj_temp2 = mlx.readObjectTempC();              //타켓 온도 측정
  float pre_gap_temp2 = pre_ambi_temp2-pre_obj_temp2;       //온도 차
  
  //detecting motion for 10sec
  for(int i=0;i<10;i++){
    long state = digitalRead(sensor_sr501);            
    if(state == HIGH){
     detection_flag[detec_idx]=true;
     Serial.print(detec_idx);
     Serial.println("Motion detected!");
    } 
    else {
      detection_flag[detec_idx]=false;
      Serial.print(detec_idx);
      Serial.println("Motion absent!");
    }
    detec_idx_up();
    if(i != 9){
      delay(1000);
    }
  }

  //10초 전 정보 측정
  int pre_volt1 = getVolt();   //10 초전 볼트 읽기      
  float pre_ambi_temp1 = mlx.readAmbientTempC();      //10초 전 주변온도 측정    
  float pre_obj_temp1 = mlx.readObjectTempC();        //10초 전 타켓 온도 측정
  float pre_gap_temp1 = pre_ambi_temp1-pre_obj_temp1; //10초 전 온도 차
  
  //detecting motion for 10sec
  for(int i=0;i<10;i++){
    long state = digitalRead(sensor_sr501);            
    if(state == HIGH){
     detection_flag[detec_idx]=true;
     Serial.print(detec_idx);
     Serial.println("Motion detected!");
    } 
    else {
      detection_flag[detec_idx]=false;
      Serial.print(detec_idx);
      Serial.println("Motion absent!");
    }
    detec_idx_up();
    if(i != 9){
      delay(1000);
    }
  }

  //현재 정보 측정
  int cur_volt = getVolt();                          
  float cur_ambi_temp = mlx.readAmbientTempC();      //현재 주변 온도 측정
  float cur_obj_temp = mlx.readObjectTempC();        //현재 타켓 온도 측정
  float cur_gap_temp = cur_ambi_temp-cur_obj_temp;   //현재 온도 차
  
  /////////////////////////////////////RFID/////////////////////////////////////////
  MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
  int statuss = 0;
  int out = 0;
  SPI.begin();                        // Initiate  SPI bus
  mfrc522.PCD_Init();                 // Initiate MFRC522
  if(mfrc522.PICC_IsNewCardPresent() &&mfrc522.PICC_ReadCardSerial()){
    //Show UID on serial monitor
    Serial.println();
    Serial.print(" UID tag :");
    String content= "";
    byte letter;
    for (byte i = 0; i < mfrc522.uid.size; i++){
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    content.toUpperCase();
    Serial.println();
    
  if (content.substring(1) == "69 04 06 2B") //change UID of the card that you want to give access
  {
    Serial.println(" Access Granted ");
    Serial.println(" Welcome Mr.Circuit ");
    Serial.println(" Have FUN ");
    Serial.println();
    presence_flag = 1;
  }
  else   {
    Serial.println(" Access Denied ");
  }
    
  }
  int num=count_detec();
  
  //send data to server
  sendSV(ID,num,  cur_volt, pre_volt1, pre_volt2 , cur_ambi_temp, pre_ambi_temp1, pre_ambi_temp2, cur_obj_temp, pre_obj_temp1, pre_obj_temp2, cur_gap_temp, pre_gap_temp1, pre_gap_temp2);

  //print ouput
    //볼트 출력
    Serial.print("volt : ");
    Serial.print(pre_volt2);
    Serial.print(", ");
    Serial.print(pre_volt1);
    Serial.print(", ");
    Serial.println(cur_volt);
    //주변 온도 출력
    Serial.print("Ambient = "); 
    Serial.print(pre_ambi_temp2);
    Serial.print(", ");
    Serial.print(pre_ambi_temp1);
    Serial.print(", ");
    Serial.println(cur_ambi_temp);
    //타켓 온도
    Serial.print("Object = "); 
    Serial.print(pre_obj_temp2);
    Serial.print(", ");
    Serial.print(pre_obj_temp1);
    Serial.print(", ");
    Serial.println(cur_obj_temp);
    //온도 차 출력
    Serial.print("gap2 = "); 
    Serial.print(pre_gap_temp2);
    Serial.print(", ");
    Serial.print(pre_gap_temp1);
    Serial.print(", ");
    Serial.println(cur_gap_temp);
    //num 0f detec 30초간 감지 횟수 출력
    Serial.print("num 0f detec for 30 sec = "); 
    
    Serial.println(num);
    Serial.print("presence:");
    Serial.println(presence_flag);
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

int getVolt(){
    float volt;  // 전압(V)
    double dist; // 거리(cm)
    volt = analogRead(PIN_A0);// 센서로부터의 출력 전압
    return volt;
}

void sendSV(int id,int num, int cur_volt,int pre_volt1,int pre_volt2 ,float cur_ambi_temp,float pre_ambi_temp1,float pre_ambi_temp2,float cur_obj_temp,float pre_obj_temp1,float pre_obj_temp2,float cur_gap_temp,float pre_gap_temp1,float pre_gap_temp2)
{  
   WiFiClient client;
   Serial.print("\nconnecting to Host: ");
   Serial.println(private_server);

    //Connect to Private Server
    if(client.connect(private_server,serverPort)){
    //http call to server by using GET Method.
    String getheader = "GET /data?id="+ String(id) +"&num="+ String(num)
    +"&cur_volt="+String(cur_volt)+"&pre_volt1="+String(pre_volt1)+"&pre_volt2="+String(pre_volt2)
    +"&cur_ambi_temp="+String(cur_ambi_temp)+"&pre_ambi_temp1="+String(pre_ambi_temp1)+"&pre_ambi_temp2="+String(pre_ambi_temp2)
    +"&cur_obj_temp="+String(cur_obj_temp)+"&pre_obj_temp1="+String(pre_obj_temp1)+"&pre_obj_temp2="+String(pre_obj_temp2)
    +"&cur_gap_temp="+String(cur_gap_temp)+"&pre_gap_temp1="+String(pre_gap_temp1)+"&pre_gap_temp2="+String(pre_gap_temp2)
    +" HTTP/1.1";
    client.println(getheader);
    client.println("User-Agent: ESP8266");  
    client.println("Host: " + String(private_server));  
    client.println("Connection: close");  
    client.println();
  
    Serial.println(getheader);//To Check
    
  }
  else{
    Serial.print("connection failed to ");
    Serial.println(private_server);
  }
  
  Serial.println("Done cycle.");
   delay(1000);
  client.stop();
}

void detec_idx_up(){
  if(detec_idx==29){
    detec_idx=0;  
  }
  else{
    detec_idx++;
  }
}

int count_detec(){
  int count=0;
  for(int i=0;i<30 ;i++){
    if(detection_flag[i]==true){
      count++;
    }
  }
  return count;
}
