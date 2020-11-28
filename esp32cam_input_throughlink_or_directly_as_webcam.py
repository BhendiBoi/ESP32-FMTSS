import cv2
import numpy as np

serverlink = "PUT THE SERVER'S LINK IN HERE"

cap = cv2.VideoCapture('http://127.0.0.1:8000')

while (True):
    frame = cap.read()
    cv2.imshow('Output', frame)
    k = cv2.waitKey(10) & 0xFF

    if k == 27:
        break

cap.release()
cv2.destroyAllWindows()
