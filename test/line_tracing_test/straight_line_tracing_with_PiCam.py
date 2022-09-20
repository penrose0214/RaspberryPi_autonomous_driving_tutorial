#With live Picamera
import time
import picamera
import numpy as np
import cv2


#camera setting
cap = cv2.VideoCapture(0)


while True:
    
    #capture from live Picamera, can scale to gray-scale
    ret, frame = cap.read()
    
    #Gaussian blur to simplify calculation
    kernel_size = 7
    frame = cv2.GaussianBlur(frame,(kernel_size,kernel_size),0)
    
    #exception handling
    if frame is None:
        print("ERROR: frame is NoneType")
        break
    else:
        #convert frame file to gray scale & dsiplay through window
        '''
        가끔 docker problem때문에 raspberry pi에 SSH 원격으로 조종할때, cv2.imshow() 함수가 window를 띄우지 못하고 아래와 같이
        Can't initialize GTK backend in function 'cvInitSystem'라는 errortype이 뜨는데, 이를 해결하기 위해서는 
        export DISPLAY=:0
        xhost +
        sudo docker run -it --rm --runtime nvidia --network host
        라고 해당 cmd 셸에 명령어를 쓰자
        (아직 정확한 원리는 파악되지 않았으나, docker 해제 문제?로 인해 발생되는 것으로 추측됨)
        정확한 guideline은 아래의 주소에 있는 글을 참조바란다:
        https://forums.developer.nvidia.com/t/unable-to-display-images-using-open-cv-when-using-docker-container/198559
        '''
        src_gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        cv2.imshow('grayscale', src_gray)
        
        #Canny function: cv2.Canny(image, threshold1(lower bound), threshold2(upper bound), edges=None, apetureSize=None(default:3, solbel kernel size), L2gradient=None (L1, L2gradient option))
        canny = cv2.Canny(src_gray, 50, 150)
        if canny is None:   #exception handling for canny == None
            print("Error: canny is NoneType")
            break
        else:
            cv2.imshow('canny', canny)

        #HoughLines Transformation
        lines = cv2.HoughLines(canny,1,np.pi/180,120,None,0,0)
        '''PC에서는 문제가 없었는데, raspberrypi에서 len(lines)에서 'TypeError: object of type 'NoneType' has no len()'이라는 error가 뜬다.
        이 error는 
        >>>my_list = None
        >>>print(len(my_list))
        와 같이 작성할때 발견되는 error인데, None 변수의 length를 강제로 찾게할때 발생한다

        NoneType error 해결:
        검출되는 선이 아예 없을때, "Error: line is not detected" 구문 출력
        검출되면, 실시간 영상에 직선 긋기
        '''
        if lines is None:   #exception handling for lines == None
            print("Error: line is not detected")
            continue
        else:
            print("Success: line(s) are detected")
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
    
#destroy all windows
cv2.destroyAllWindows()