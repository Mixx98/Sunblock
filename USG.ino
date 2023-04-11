#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>
#include <WiFi.h>
#include "FirebaseESP32.h"
#include <HardwareSerial.h>



#define FIREBASE_HOST "https://usg-project-5aa12-default-rtdb.firebaseio.com" //Change to your Firebase RTDB project ID e.g. Your_Project_ID.firebaseio.com
#define FIREBASE_AUTH "ZltPODHHk66Dj00yIF3gFUAX0FegLbAKwUb22d03" //Change to your Firebase RTDB secret password



#define TFT_CS     33
#define TFT_RST    14  
#define TFT_DC     26
//#define SCK      18
//#define SDA/MOSI 23
#define TX 17
#define RX 16


Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);
FirebaseData firebaseData;
HardwareSerial Sender(2);
HardwareSerial Receiver(2);


int uv = 0;
int area = 3000;
byte num = 0;
bool con = true;


/* 저장값(num)
0 : 변경사항 없음
1 : 현재 위치 변경
2 : 자외선 값 변경
3 : 얼굴 면적 값 변경
4 : 새로운 사진 등록
 */

/********************************************************************************************/


void setup() 
{
  //시리얼 통신
  Serial.begin(115200);
  Sender.begin(115200, SERIAL_8N1, TX, RX);
  Receiver.begin(115200, SERIAL_8N1, TX, RX);


  //LCD 선언
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);   
}

 
void loop() 
{
  
  //WiFi가 연결되어 있을 경우
  if(WiFi.status() == WL_CONNECTED){
    
    Firebase.getInt(firebaseData, "/Num", &num);  // num값을 가져옴
    Serial.println(num);
    if(!con){ //wifi 연결시 uv값을 한번 가져옴
      Firebase.getInt(firebaseData, "/UV", &uv);
      level(uv);
      con=true;
    }
    
    if(num=='2'){ // num이 2일 경우 
      Firebase.getInt(firebaseData, "/UV", &uv);
      Firebase.setInt(firebaseData, "/Num", 0);
      level(uv);
    }    
    else if(num=='3'){ // num이 3일 경우
      Firebase.getInt(firebaseData, "/area", &area);
      Firebase.setInt(firebaseData, "/Num", 0);
      Sender.print(area);
      Serial.println(area);
    }
    
    
  }else{ //WiFi가 연결 안된 경우
    if(con){
      wifi_not();  
      Serial.println("연결 안됨");
      con=false;
    }
  }
}

// wifi 로그인 하는 함수
void login(char id[], char pw[]){
  // 로그인이 시도되면 30초동안 시도하고 종료
  WiFi.begin(id, pw);
  for(int i=0; i<100; i++){
    if(WiFi.status() != WL_CONNECTED){
      delay(300);
      Serial.println(".");
    }
    else{
      Serial.println("연결 성공");
      // 파이어베이스 연결
      Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
      Firebase.reconnectWiFi(true);
      break;
    }
  }
}



// 블루투스로 로그인하는 함수
void bl(){
  
  String str = Receiver.readStringUntil('\n');
  Serial.println(str);
  int id_2 = str.indexOf('"',2);
  int pw_1 = str.indexOf('"',(id_2)+1);
  int pw_2 = str.indexOf('"',(pw_1)+1);

  String id_s = str.substring(2,id_2);
  String pw_s = str.substring((pw_1)+1,pw_2);
  char id[32] = {0};
  char pw[32] = {0};
  (id_s).toCharArray(id,((id_s).length()+1));
  (pw_s).toCharArray(pw,((pw_s).length()+1));
  Serial.println(id);
  Serial.println(pw);
  
  login(id, pw);  
  
}

// uv값을 입력하면 해당하는 등급의 화면을 표시하는 함수
void level(int uv){
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(50, 110);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(1.5);
  tft.print("Level = ");
  tft.print(uv);
  
  tft.setTextSize(4);
  if(uv<3){
      low();
    }
    else if(uv<6){
      normal();
    }
    else if(uv<8){
      high();
    }
    else if(uv<11){
      veryHigh();
    }
    else{
      danger();
    }
}

// wifi가 연결 안됬을때 LCD 화면
void wifi_not(){
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(30, 50);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);
  tft.print("WiFi not");
  tft.setCursor(25, 70);
  tft.print("connected");
}

// uv <3
void low() {
  tft.setCursor(47, 45);
  tft.setTextColor(ST77XX_BLUE);
  tft.print("LOW");
}

// 3<= uv <6
void normal(){
  tft.setCursor(12, 50);
  tft.setTextColor(ST77XX_GREEN);
  tft.print("NORMAL");
}

// 6<= uv <8
void high(){
  tft.setCursor(35, 50);
  tft.setTextColor(ST77XX_YELLOW);
  tft.print("HIGH");
}

// 8<= uv <11
void veryHigh(){
  tft.setCursor(35, 30);
  tft.setTextColor(ST77XX_MAGENTA);
  tft.print("VERY");
  tft.setCursor(35, 70);
  tft.print("HIGH");
}

// 11<= uv
void danger(){
  tft.setCursor(10, 50);
  tft.setTextColor(ST77XX_RED);
  tft.print("DANGER");
}
