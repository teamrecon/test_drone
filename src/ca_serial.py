#!/usr/bin/env python

import roslib
roslib.load_manifest('test_drone')
import numpy
import rospy
import sys
import serial
from std_msgs.msg import String,Int32
from geometry_msgs.msg import Vector3
from rospy.numpy_msg import numpy_msg
from rospy_tutorials.msg import Floats

port = "dev/ttyS0"
baud = 115200

ser = serial.Serial()
ser.port = port
ser.baudrate = baud



#################################################################################################

def constrain(n, minn, maxn):
    return max(min(maxn, n), minn)


def main(args):

   Kp=3
   setpoint = 120
   pub=rospy.Publisher('sonar_vals',Vector3,queue_size = 10)
   rospy.init_node('ca_serial')
   r = rospy.Rate(45454)
   while not rospy.is_shutdown():
      print "LOOP running"
 
      try:
         data_raw = ser.readline()
      except e:
         print e
      sd = data_raw.split(',')   
      if sd[0] == 777:
         sonar1 = sd[1]
         sonar2 = sd[2]
         sonar3 = sd[3]
         sonar4 = sd[4]
         sonar5 = sd[5]
         sonar6 = sd[6]
         sonar7 = sd[7]
         sonar8 = sd[8]

         sonar1 = constrain(soanr1,0,200)
         sonar2 = constrain(soanr2,0,200)
         sonar3 = constrain(soanr3,0,200)
         sonar4 = constrain(soanr4,0,200)
         sonar5 = constrain(soanr5,0,200)
         sonar6 = constrain(soanr6,0,200)
         sonar7 = constrain(soanr7,0,200)
         sonar8 = constrain(soanr8,0,200)

         e1 = setpoint - sonar1
         e2 = setpoint - sonar2
         e3 = setpoint - sonar3
         e4 = setpoint - sonar4
         e5 = setpoint - sonar5
         e6 = setpoint - sonar6
         e7 = setpoint - sonar7
         e8 = setpoint - sonar8
         
         eLeft = (e1+e2)/2
         eFront = (e3+e4)/2
         eRight = (e5+e6)/2
         eBack = (e7+e8)/2
         eLeft = eLeft * Kp
         eRight = eRight * Kp
         eFront = eFront * Kp
         eBack = eBack * Kp
         roll = eLeft - eRight
         pitch = eBack - eFront
         pub.publish(Vector3(roll,pitch,117))




       else:
         print 'Invalid Data'
         pub.publish(Vector3(0,0,0))




if __name__ == '__main__':
    import sys, getopt
    main(sys.argv)
    
    

