import cv2
import numpy as np

serverlink = "http://192.168.29.199/"

cap = cv2.VideoCapture(serverlink)
# cap.set(3, 840 + 400)
# cap.set(4, 680 + 400)
# cap.set(10, 10000000)

while (True):
    success, img = cap.read()
    cv2.imshow('Output', img)

    if cv2.waitKey(10) & 0xFF == 27:
        break

cap.release()
cv2.destroyAllWindows()