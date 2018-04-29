import sys
import traceback
import numpy as np
import time as t
import smbus
import cv2
import struct

bus = smbus.SMBus(1)
addr = 0x43

BALL_DATA=0
PICKUP=1
START=2

def serializeMsg(msg):
    return [b for num in msg for b in struct.pack('>h', num)]

def sendBallData(size, offsetY):
    try:
        msg = serializeMsg([size, offsetY])
        bus.write_block_data(addr, BALL_DATA, msg ) 
    except IOError:
        print ("can't communicate with arduino")

def sendPickup():
    try:
        msg = serializeMsg([0])
        bus.write_block_data(addr, PICKUP, msg ) 
    except IOError:
        print ("can't communicate with arduino")

def sendStart():
    try:
        msg = serializeMsg([0])
        bus.write_block_data(addr, START, msg ) 
    except IOError:
        print ("can't communicate with arduino")

############################MAIN##################################

'''
while True:
    for i in range(-50,50):
        sendBallData(0, i)
        print(i)
        t.sleep(.1)
    for i in range(50,-50,-1):
        sendBallData(0, i)
        print(i)
        t.sleep(.1)

'''
cap = cv2.VideoCapture(0)

lower_ball = np.array([25,50,150])
upper_ball = np.array([50,255,255])

kernel = np.ones((5,5), np.uint8)

SCREEN_CENTER = [int(i/2) for i in cap.read()[1].shape[:2]][::-1]

sendStart()

while (True):
    try:
        _, frame = cap.read()
        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        ball_mask = cv2.inRange(hsv, lower_ball, upper_ball)

        #noise filtering
        ball_mask = cv2.erode(ball_mask, kernel, iterations=1)
        ball_mask = cv2.dilate(ball_mask, kernel, iterations=1)

        #contour finding
        _, contours, _ = cv2.findContours(ball_mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
        if contours:
            max_contour = contours[np.argmax(np.array([cv2.contourArea(cnt) for cnt in contours]), axis=0)]
            #process max contour
            M = cv2.moments(max_contour)
            if (M['m00']) != 0:
                cx, cy = int(M['m10']/M['m00']), int(M['m01']/M['m00'])
                center = (cx, cy)
                cv2.circle(frame, center, 5, [0,0,255], -1)
                area = int(cv2.contourArea(max_contour))
                offset = (center[0] - SCREEN_CENTER[0], center[1] - SCREEN_CENTER[1])
                #we can see the ball
                if area > 100:
                    #ball needs to be picked up
                    if offset[0] < -230:
                        sendPickup()
                        print ("PICKUP")
                    else:
                        sendBallData(int(area/1000.0), offset[1])
                        print (offset)
        #cv2.imshow('frame',frame)
        #cv2.imshow('mask', ball_mask)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    except KeyboardInterrupt:
        sys.exit(0)
    except:
        traceback.print_exc()
        pass
