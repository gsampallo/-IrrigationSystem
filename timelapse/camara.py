import cv2
from datetime import datetime
cap = cv2.VideoCapture('rtsp://192.168.1.10:554/user=admin&password=&channel=1&stream=0.sdp?')
ret, frame = cap.read()

now = datetime. now()
dt_string = now. strftime("%Y%m%d_%H%M%S.png")


cv2.imwrite(dt_string,frame)
cap.release()

