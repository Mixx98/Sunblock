import pyrebase
import os

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
"zzzz"


'''
저장값 [num, "space", value_uv, value_area]
0 : 변경사항 없음
1 : 현재 위치 변경
2 : 자외선 값 변경
3 : 얼굴 면적 값 변경
4 : 새로운 사진 등록
5 : Wifi 정보 수정
'''




def UV():
    import requests

    url = 'http://apis.data.go.kr/1360000/LivingWthrIdxServiceV3/getUVIdxV3'
    queryParams = '?' + 'serviceKey=' + 'd8gYBgdr1DKbClLtN6BSU3vJO9Vj3uYNT%2F9LxMo1Rx3gBLUvrX1Q7og%2BZ9pvIAop36WDe7dHhDLzjX5aVtEBxA%3D%3D' \
            + '&areaNo=' + '1111051500' \
            + '&time=' + '2022112418' \
            + '&dataType=' + 'json'
    url = url + queryParams
    result = requests.get(url)
    
    return result.content



def area():
    firebase = pyrebase.initialize_app(config)
    storage = firebase.storage()
    database = firebase.database()

    path_file = "Face.jpg"
    '''if os.path.exists(path_file):
    os.remove(path_file)'''

    #storage.child(path_on_cloud).put(path_local)
    #storage.child(path_on_cloud).delete(path_on_cloud,"") #Delete
    #storage.child(path_on_cloud).download("",path_on_cloud) #Download
    return

print(UV())

