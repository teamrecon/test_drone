import roslib
roslib.load_manifest('test_drone')
import numpy
import rospy
import sys
import serial
from std_msgs.msg import String,Int32
from geometry_msgs.msg import Vector3,Twist
from rospy.numpy_msg import numpy_msg
from rospy_tutorials.msg import Floats

port = "dev/ttyS0"
baud = 115200

ser = serial.Serial()
ser.port = port
ser.baudrate = baud


def constrain(n, minn, maxn):
    return max(min(maxn, n), minn)


def main(args):

   pub=rospy.Publisher('rc_vals',Twist,queue_size = 10)
   rospy.init_node('ca_serial')
   r = rospy.Rate(0.2)
   while not rospy.is_shutdown():
      print "LOOP running"
 
      try:
         data_raw = ser.readline()
      except e:
         print e
      sd = data_raw.split(',')   
      if sd[0] == 777:
         CH1 = sd[1]
         CH2 = sd[2]
         CH3 = sd[3]
         CH4 = sd[4]
         CH5 = sd[5]
         pub.publish(Vector3(CH2,CH1,CH3),Vector3(777,CH5,CH4))




       else:
         print 'Invalid Data'
         pub.publish(Vector3(0,0,0),Vector3(0,0,0))




if __name__ == '__main__':
    import sys, getopt
    main(sys.argv)
    