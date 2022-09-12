import numpy as np
import cv2


capture = cv2.VideoCapture("example.MP4")


while True:
    ret, frame = capture.read()
    #exception handling
    if frame is None:
        print("capture load failed")
        sys.exit()    
    
    #if functioning normally
    if ret == True:
        
        #convert frame file to gray scale & dsiplay through window
        src_gray = cv2.imread('frame', cv2.IMREAD_GRAYSCALE)
        cv2.imshow("VideoFrame_grayscale", src_gray)
        
        #Canny function: cv2.Canny(image, threshold1(lower bound), threshold2(upper bound), edges=None, apetureSize=None(default:3, solbel kernel size), L2gradient=None (L1, L2gradient option))
        canny = cv2.Canny(src_gray, 50, 150)
        cv2.imshow('line_traced', canny)
        
        #quit
        if cv2.waitKey(33) == ord('q'):
            break
    else:
        break
    
    
capture.release()
cv2.destroyAllWindows()