#include <TFT.h>  // 아두이노 LCD 라이브러리 포함
#include <SPI.h>  // 아두이노 SPI 라이브러리 포함
 
#define dc  12  // DC or A0를 12번 핀에
#define rst 13  // RESET은 13번 핀에
#define cs  14  // CS를 14번 핀에
#define ir  25  // 적외선 센서 25번 핀에
#define mA  33  // 모터 드라이버 A 33번 핀에
#define mB  32  // 모터 드라이버 B 32번 핀에

TFT TFTscreen = TFT(cs, dc, rst);    // TFT 클래스 생성
char sensorPrintout[4];              // 센서 값을 저장할 변수

void setup() 
{
  TFTscreen.begin();                           // TFT 클래스 시작
  TFTscreen.background(0, 0, 0);               // 검정색으로 화면 초기화
  TFTscreen.stroke(255, 255, 255);             // 폰트 색깔 흰색으로 설정
  TFTscreen.setTextSize(2);                    // 폰트 크기 2로 설정
  TFTscreen.text("Sensor Value :\n ", 0, 0);   // LCD에 글씨 표시 
  TFTscreen.setTextSize(5);                    // 폰트 크기 5로 설정
  pinMode(ir, INPUT);                          // 적외선 센서 핀번호 선언
  pinMode(mA, OUTPUT);                         // 모터 드라이버 A 핀번호 선언 
  pinMode(mB, OUTPUT);                         // 모터 드라이버 B 핀번호 선언 
}

 
void loop() 
{
   lcd();

   int val = analogRead(ir) ; // 적외선 센서값
   if(val>0){                 // 적외선 센서에 감지되면 모터 작동
    digitalWrite(mA, HIGH);
    digitalWrite(mB, LOW);
    delay(1000); 
    digitalWrite(mA, LOW);
    digitalWrite(mB, LOW);
   }
}


void lcd(){
  String sensorVal = String(analogRead(A0));  // A0의 값을 받아와서 문자열 변수에 저장
 
  sensorVal.toCharArray(sensorPrintout, 4);  
    // 읽어온 센서 문자열 변수를 가지고 각각의 캐릭터 변수에 저장
 
  TFTscreen.stroke(150, 0, 150);            // 폰트 색깔 설정
  TFTscreen.text(sensorPrintout, 0, 20);    // LCD X:0, Y:20 위치에 글씨 표시
 
  delay(250);                               // 250ms 기다림
  
  TFTscreen.stroke(0, 0, 0);                // 글씨체 검정색으로
  TFTscreen.text(sensorPrintout, 0, 20);    // 화면에서 안보이게 하기 위해 검정색으로 표시함
}
