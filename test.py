"""import crypto
import sys
sys.modules['Crypto'] = crypto"""
import pyrebase

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

path_on_cloud = "test.jpg"
path_local = "test.jpg"
storage.child(path_on_cloud).put(path_local)
