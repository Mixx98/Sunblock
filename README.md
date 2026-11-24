# ☀️ Sunblock – 자외선 맞춤형 자동 선크림 디스펜서 (USG)

**실시간 자외선 지수**를 보여주고, 사용자의 **얼굴 크기에 맞는 양의 선크림**을 자동으로 짜 주는 IoT 디스펜서입니다.

## 동작 흐름

```
[스마트폰 앱] ──주소/얼굴 사진──▶ [Firebase (RTDB + Storage)] ◀──▶ [Python 서버 (USG.py)]
      │                                   │                         ├─ 기상청 API로 지역 UV 지수 조회
   Bluetooth(WiFi 정보)                   │                         └─ OpenCV로 얼굴 면적 계산
      ▼                                   ▼
[ESP32 #2 (USG2.ino)] ◀──UART──▶ [ESP32 #1 (USG.ino)]
 적외선 센서 + 펌프 모터              TFT LCD에 UV 단계 표시
```

1. 앱에서 주소를 입력하면 Python 서버가 **기상청 생활기상지수 API**로 해당 지역 UV 지수를 받아 Firebase에 저장합니다. (3시간마다 자동 갱신)
2. ESP32 #1이 UV 값을 받아 TFT LCD에 `LOW / NORMAL / HIGH / VERY HIGH / DANGER`로 표시합니다.
3. 앱에서 얼굴 사진을 올리면 서버가 **OpenCV Haar Cascade**로 얼굴 면적을 계산합니다.
4. 면적 값이 ESP32 #2로 전달되고, 손을 대면(적외선 센서) **면적에 비례한 시간만큼 펌프가 작동**해 선크림을 짭니다.

## Firebase 상태 값 (`Num`)

| 값 | 의미 |
| --- | --- |
| 0 | 변경 없음 |
| 1 | 위치 변경 → UV 다시 조회 |
| 2 | UV 값 변경 → LCD 갱신 |
| 3 | 얼굴 면적 변경 → 펌프 시간 갱신 |
| 4 | 새 사진 등록 → 면적 계산 |
| 5 | Python 서버 종료 |

## 파일 구조

```
├── USG.py     # Python 서버 – UV 조회, 얼굴 면적 계산, Firebase 동기화
├── USG.ino    # ESP32 #1 – WiFi/Firebase 연결, TFT(ST7735) 표시
├── USG2.ino   # ESP32 #2 – 블루투스 WiFi 설정, 적외선 센서, 펌프 모터
└── USG.aia    # MIT App Inventor 앱
```

## 사용 기술

`ESP32` `Firebase Realtime DB / Storage` `Python` `OpenCV` `pyrebase` `공공데이터포털 기상청 API` `ST7735 TFT` `MIT App Inventor`

## 실행 방법

1. `pip install pyrebase4 opencv-python pandas requests numpy`
2. `USG.py`의 Firebase 설정과 공공데이터포털 서비스 키를 본인 값으로 바꿉니다.
3. 같은 폴더에 지역 코드표 `code.csv`를 둡니다.
4. `python USG.py` 실행 후 두 ESP32에 각각 `.ino`를 업로드합니다.

---
📅 2023년 진행
