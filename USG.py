from time import sleep
import pyrebase
import os
import numpy
from datetime import datetime

config = {
    "apiKey": "AIzaSyD70eG1rAxbiRoPyX6OJPp1G8mFo4_uA-A",
    "authDomain": "usg-project-5aa12.firebaseapp.com",
    "databaseURL": "https://usg-project-5aa12-default-rtdb.firebaseio.com",
    "projectId": "usg-project-5aa12",
    "storageBucket": "usg-project-5aa12.appspot.com",
    "messagingSenderId": "459469217132",
    "appId": "1:459469217132:web:1097d1d90504e597612749",
    "measurementId": "G-REKZTP18HW"
}

firebase = pyrebase.initialize_app(config)
storage = firebase.storage()
database = firebase.database()
now = datetime.now()
    

def get(locate): #파이어베이스 값을 가져오는 함수
    return database.child(locate).get().val()


def update(locate, value): #파이어베이스 값을 수정하는 함수
    database.update({locate:value})


def UV(): #실시간 UV지수를 구하는 함수
    import requests
    import pandas as pd
    import json

    code = "" #지역코드가 담길 변수
    address = get('address') #주소를 가져와서
    list = address.split(" ") #리스트화 시킨다

    table = pd.read_csv('./code.csv',encoding='euc-kr',header=None) #코드표를 불러와서
    for i in range(0,len(table)):
        if(table.iat[i,2]==list[2]): #주소가 일치하는 코드를 찾는다
            code=table.iat[i,0]
            break

    url = 'http://apis.data.go.kr/1360000/LivingWthrIdxServiceV3/getUVIdxV3'
    queryParams = '?' + 'serviceKey=' + 'd8gYBgdr1DKbClLtN6BSU3vJO9Vj3uYNT%2F9LxMo1Rx3gBLUvrX1Q7og%2BZ9pvIAop36WDe7dHhDLzjX5aVtEBxA%3D%3D' \
            + '&areaNo=' + code \
            + '&time=' + now.strftime('%Y%m%d%H') \
            + '&dataType=' + 'json'
    url = url + queryParams
    result = requests.get(url) # 지역코드와 현재시간을 기준으로 실시간 UV값을 가져옴
    jdata=json.loads(result.text)
    val=jdata['response']['body']['items']['item'][0]["h0"] #원하는 데이터만 뽑아냄

    update("UV",val) #파이어베이스에 수정
    update("Num",'2')


def area(): #새로운 사진이 들어오면 면적값을 계산하는 함수
    
    path_file = "Face.png"
    storage.child(path_file).download("",path_file) #Download
    
    update("area",face()) #파이어베이스에 수정
    update("Num",'3')

    #사진 삭제(컴퓨터, Storage)
    '''
    if os.path.exists(path_file):
        os.remove(path_file)
    storage.child(path_file).delete(path_file,"")
    '''
    

def face(): #얼굴 면적 계산
    import cv2

    area = 10000
    # haarcascade 불러오기
    face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')

    file = cv2.imread('Face.png')
    for val in numpy.arange(1,0.1,-0.1):
        img = cv2.resize(file,(0,0), fx=val, fy=val)
        gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

        faces = face_cascade.detectMultiScale(gray, scaleFactor= 1.1, minNeighbors=7, minSize=(500,500))
        if (len(faces)):
            print("얼굴 측정 성공")
            print(val)
            for (x, y, w, h) in faces:
                cv2.rectangle(img, (x, y), (x + w, y + h), (255, 0, 0), 10)
                area = ((h*w)//(val**2))/100
                print(area)
            
            img = cv2.resize(img,(int(img.shape[1] * 0.2), int(img.shape[0] * 0.2)))
            cv2.imshow('image', img)
            key = cv2.waitKey(0)
            cv2.destroyAllWindows()
            
            break

    return area

'''
저장값 [num, "space", value_uv, value_area]
0 : 변경사항 없음
1 : 현재 위치 변경
2 : 자외선 값 변경
3 : 얼굴 면적 값 변경
4 : 새로운 사진 등록
5 : 파이썬 종료
'''
upd = True

while(1):
    sleep(1)
    print(".")
    Num=get("Num") #Num값을 갱신

    if(Num=='1'): #주소값이 변경되면 UV값도 수정
        UV()
        print("UV값 변경 완료")
    elif(Num=='4'): #새로운 사진이 올라오면 면적값 수정
        area()
        print("면적값 변경 완료")
    elif(Num=='5'): #종료
        print("종료하겠습니다.")
        update("Num",'0')
        break

    if(now.hour%3==0): #3시간 간격으로 UV데이터를 갱신
        if(upd):
            UV()
            print("UV값 Update 완료")
            upd=False
    else:
        upd=True
    
