#
# Copyright (C) 2017 by YOUR NAME HERE
#
#    This file is part of RoboComp
#
#    RoboComp is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    RoboComp is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with RoboComp.  If not, see <http://www.gnu.org/licenses/>.
#

import sys, os, traceback, time
import numpy as np
import cv2
from PySide import QtGui, QtCore
from genericworker import *
from pprint import pprint

# If RoboComp was compiled with Python bindings you can use InnerModel in Python
# sys.path.append('/opt/robocomp/lib')
# import librobocomp_qmat
# import librobocomp_osgviewer
# import librobocomp_innermodel

class SpecificWorker(GenericWorker):
    def __init__(self, proxy_map):
        super(SpecificWorker, self).__init__(proxy_map)
        self.timer.timeout.connect(self.compute)
        self.Period = 100
        self.timer.start(self.Period)

    def setParams(self, params):
        self.capL = cv2.VideoCapture(0)
        return True

    @QtCore.Slot()
    def compute(self):
        # print 'SpecificWorker.compute...'
        retL, self.frameL = self.capL.read()
        if retL:
            rows, cols, depth = self.frameL.shape
            cv2.imshow("frameL", self.frameL)
            if cv2.waitKey(1) & 0xFF == ord('q'):  # wait for ESC key to exit
                exit()
            if self.apriltagsserver_proxy is not None:
                print(self.apriltagsserver_proxy.getAprilTags(self.getImage(), 159, 830, 830))
            else:
                print "No apriltag server"
            # if self.peopleserver_proxy is not None:
            #     people = self.peopleserver_proxy.processImage(self.getImage2(), 0.3)
            #     print people
        else:
            print "No frame could be read"

        return True


    def getImage(self):
        im = Image()
        im.timeStamp = time.time()
        im.data = self.frameL.data
        im.frmt.height, im.frmt.width, _ = self.frameL.shape
        return im

    def getImage2(self):
        im = TImage()
        im.image = self.frameL.data
        im.width, im.height, im.depth = self.frameL.shape
        return im
