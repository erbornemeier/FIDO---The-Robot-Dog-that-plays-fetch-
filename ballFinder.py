import numpy as np
import cv2

cap = cv2.VideoCapture(1)

lower_ball = np.array([25,50,150])
upper_ball = np.array([50,255,255])

kernel = np.ones((5,5), np.uint8)

SCREEN_CENTER = [int(i/2) for i in self.cap.read()[1].shape[:2]][::-1]

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
            M = cv2.moments(max_contour)
            if (M['m00']) != 0:
                cx, cy = int(M['m10']/M['m00']), int(M['m01']/M['m00'])
                center = (cx, cy)
                cv2.circle(frame, center, 5, [0,0,255], -1)


        cv2.imshow('frame',frame)
        cv2.imshow('mask', ball_mask)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break


    except:
        pass
