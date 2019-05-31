# 적외선 거리 센서
- 모델명 : GP2Y0A21YK0F


## 스펙
- Distance measuring range : 10 to 80cm
- Analog output type
- Pakage size : 29.5x13x13.5mm
- Consumption current : Typ. 30mA 
- Supply voltage : 4.5 to 5.5V

출처 : [LINK](https://www.sharp-world.com/products/device/lineup/data/pdf/datasheet/gp2y0a21yk_e.pdf)

## PINOUT

![pinout](./images/gp2y0a21yk0f_pinout.png)

출처 : [LINK](https://www.sharp-world.com/products/device/lineup/data/pdf/datasheet/gp2y0a21yk_e.pdf)


## 연결(to NodeMCU)

| GP2Y0A21YK0F |  | NodeMCU |
|--|--|--|
| VCC(Red) | <-> | 5V(VIN or VU) |
| GND(Black) | <-> | GND |
| V0(Yellow) | <-> | A0 |

![to nodemcu](./images/gp2y0a21yk0f_to_nodemcu.png)


## 소스코드
 
	#define PIN A0; // 아날로그 데이터 A0
	float Vcc = 5.0;
	float dig_v1;
	float dist1;
	
	void setup() {
		Serial.begin(9600); // 시리얼 통신 초기화
	}
 
	void loop() {
		dig_v1 = Vcc * analogRead(PIN) / 1023;
		dist1 = 3.6473 * (1/pow(dig_v1,3)) 
				 - 9.6037 * (1/pow(dig_v1,2))
				 + 36.39 * (1/dig_v1)
				 - 4.1143;
		delay(100);
	}

   출처 : [LINK](https://ssaji.tistory.com/321)

