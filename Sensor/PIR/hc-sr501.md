# 모션 감지 센서(PIR sensor)
- 모델명 : HC-SR501


## 스펙
- Voltage: 5V – 20V
- Power Consumption: 65mA 
- TTL output: 3.3V, 0V
- Delay time: Adjustable (.3->5min) 
- Lock time: 0.2 sec
- Trigger methods: L – disable repeat trigger, H enable repeat trigger 
- Sensing range: less than 120 degree, within 7 meters 
- Temperature: – 15 ~ +70 
- Dimension: 32*24 mm, distance between screw 28mm, M2, Lens dimension in diameter: 23mm

출처 : [LINK](https://www.mpja.com/download/31227sc.pdf)

## PINOUT

![pinout](./images/hc-sr501_pinout.jpg)
출처 : [LINK](https://www.theengineeringprojects.com/2019/01/introduction-to-hc-sr501.html)


## 연결(to NodeMCU)

| HC-SR501 |  | NodeMCU |
|--|--|--|
| VCC | <-> | 5V(VIN or VU) |
| OUT | <-> | D4(Data) |
| GND | <-> | GND |

![to nodemcu](./images/hc-sr501_to_nodemcu.png)


## 소스코드
 
	#define D4 02; // 데이터 포트 D4
	
	void setup() {
		Serial.begin(9600); // 시리얼 통신 초기화
		pinMode(D4, INPUT);
	}
 
	void loop() {
		long state = digitalRead(D4);
 
		if (state == HIGH) {
		    Serial.println("Detected");
		    delay(2000);
		} 
		else {
		    Serial.println("Non-detected");
		    delay(2000);
		}
	}

    
