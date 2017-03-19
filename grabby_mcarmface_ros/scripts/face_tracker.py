#!/usr/bin/env python

# created: Mar 19
# author: Noni
# nwHacks 2017 @UBC

import rospy
import cv2
import numpy as np
#from cv_bridge import CvBridge, CvBridgeError
#from sensor_msgs.msg import Image
from std_msgs.msg import UInt8, UInt8MultiArray

class faceTracker:
    def __init__(self):
        #self.bridge = cv_bridge.CvBridge()
        #self.img_sub = rospy.Subscriber('/camera', Image, self.img_cb)
        self.web_cap = cv2.VideoCapture(0)
        self.face_pub = rospy.Publisher('/face_found', UInt8, queue_size = 5)
        self.mv_pub = rospy.Publisher('/joint_state', UInt8MultiArray, queue_size = 5)

    def main(self):
        face_cascade = cv2.CascadeClassifier('/home/ka/412_ws/src/Grabby-McArmface/grabby_mcarmface_ros/data/haarcascade_frontalface_default.xml')
        while(True):
            ret, frame = self.web_cap.read()
            if ret:
                gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
                faces = face_cascade.detectMultiScale(gray, 1.3, 7)
                if len(faces) > 0:
                    (x, y, w, h) = faces[0]
                    msg = UInt8MultiArray()
                    if x + w/2 < 640 / 3:
                        print("move left")
                    elif 640 / 3 < x + w/2 < 640 / 3 * 2:
                        if y + h/2 < 480 / 2:
                            print("move down")
                        else:
                            print("move up")
                    else:
                        print("move right")
                    if w * h > 100 * 100:
                        cv2.rectangle(frame,(x,y),(x+w,y+h),(255,0,0),2)

            cv2.imshow('webcam', frame)

            if cv2.waitKey(1) & 0xFF == ord('q'):
                break

        self.web_cap.release()
        cv2.destroyAllWindows()


if __name__ == "__main__":
    rospy.init_node("tracker")

    tracker = faceTracker()
    tracker.main()
    #rospy.spin()
