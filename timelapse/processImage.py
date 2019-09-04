import os
import cv2
import numpy as np

ruta = "C:/Users/Guillermo/Desktop/img_riego"
output = "C:/Users/Guillermo/Desktop/output/"
nroDia = 0
nombre = ''
for file in os.listdir(ruta):
    if file.endswith(".png"):
        print(file)

        img = cv2.imread(ruta+"/"+file)

        cv2.rectangle(img,(850,54),(1261,84),(0,0,0),-1)

        t = file.split("_")

        if(nombre != str(t[0])):
            nombre = str(t[0])
            nroDia = nroDia + 1

        hora = t[1].split(".")[0]
        texto = "Dia "+str(nroDia)+" "+str(hora)

        font = cv2.FONT_HERSHEY_SIMPLEX
        cv2.putText(img,texto,(850,82), font, 1,(255,255,255),2,cv2.LINE_AA)

        cv2.imwrite(output+file,img)    
