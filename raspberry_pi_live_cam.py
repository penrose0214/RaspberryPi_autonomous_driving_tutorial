from picamera import PiCamera
from time import sleep


def capture_cam():
    camera = PiCamera()
    ret, cam = camera.read()