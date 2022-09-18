#With live Picamera
import time
from picamera import PiCamera
from time import sleep
import numpy as np
import cv2


while True:
    #camera setting
    camera = picamera.PiCamera()
    camera.resolution = (320, 240)
    camera.framerate = 24   #24 FPS
    time.sleep(2)   #necessary to wait 2 secs at least
    
    #capture from live Picamera, can scale to gray-scale
    camera.capture(capt, 'bgr')
    
    #Gaussian blur to simplify calculation
    kernel_size = 5
    frame = cv2.GaussianBlur(frame,(kernel_size,kernel_size),0)
    
    #exception handling
    if ret == True:
        
        #convert frame file to gray scale & dsiplay through window
        src_gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        cv2.imshow("VideoFrame_grayscale", src_gray)
        
        #Canny function: cv2.Canny(image, threshold1(lower bound), threshold2(upper bound), edges=None, apetureSize=None(default:3, solbel kernel size), L2gradient=None (L1, L2gradient option))
        canny = cv2.Canny(src_gray, 50, 150)
        cv2.imshow('canny_result', canny)
        
        #HoughLines Transformation
        lines = cv2.HoughLines(canny,1,np.pi/180,170,None,0,0)
        for i in range(0, len(lines)):
            rho = lines[i][0][0]
            theta = lines[i][0][1]
            a = np.cos(theta)
            b = np.sin(theta)
            x0 = a*rho
            y0 = b*rho
            x1=int(x0+1000*(-b))
            y1=int(y0+1000*(a))
            x2=int(x0-1000*(-b))
            y2=int(y0-1000*(a))
            
            cv2.line(frame,(x1,y1), (x2,y2), (0,0,255), 1)
            cv2.imshow('line_traced', frame)
        #quit
        if cv2.waitKey(33) == ord('q'):
            break
    else:
        break
    
    
#close mp4 file
capture.release()
#destroy all windows
cv2.destroyAllWindows()
