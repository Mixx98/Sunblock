#include <HardwareSerial.h>
#include "BluetoothSerial.h"

#define ir 15  // 적외선 센서 4번 핀에
#define AA 13  // 모터 드라이버 A 33번 핀에
#define AB 12  // 모터 드라이버 B 32번 핀에
#define TX 17
#define RX 16


HardwareSerial Sender(2);
HardwareSerial Receiver(2);
BluetoothSerial SerialBT;
int area = 3000;
int num = 0;
int val;


/* 저장값
0 : 변경사항 없음
1 : 현재 위치 변경
2 : 자외선 값 변경
3 : 얼굴 면적 값 변경
4 : 새로운 사진 등록
 */

/********************************************************************************************/


void setup() 
{
  Serial.begin(115200);
  Sender.begin(115200, SERIAL_8N1, TX, RX);
  Receiver.begin(115200, SERIAL_8N1, TX, RX);
  
  SerialBT.begin("USG"); //Bluetooth device name
  pinMode(ir, INPUT);    // 적외선 센서 핀번호 선언
  pinMode(AA, OUTPUT);   // 모터 드라이버 A 핀번호 선언 
  pinMode(AB, OUTPUT);   // 모터 드라이버 B 핀번호 선언
  
}

 
void loop() 
{
  val = digitalRead(ir);     // 센서값 읽어옴

  // 적외선 센서에 감지되면 모터 작동
  if (val != HIGH) {         
    Serial.println("ok");
    digitalWrite(AA, HIGH);  // 바깥쪽 단자가 Vcc
    digitalWrite(AB, LOW);
    Serial.println("모터 동작");
    delay(area);             // area 만큼 대기
    digitalWrite(AA, LOW);
    digitalWrite(AB, LOW);
    delay(3000);             // 3초 대기
  }

  // 블루투스 신호가 들어오면 동작하는 함수(wifi정보)
  if (SerialBT.available()){ 
    String wifi = SerialBT.readStringUntil('\n');
    Serial.print(wifi);
    Sender.print(wifi);       // wifi정보 전송
  }

  // area값 들어오면 동작하는 함수
  if(Receiver.available()){
    num = Receiver.parseInt();
    if(num>10){
      area = num;
      Serial.println(area);
    }
  }
}
