#
# Copyright (C) 2019 by YOUR NAME HERE
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

import sys, os, Ice

ROBOCOMP = ''
try:
	ROBOCOMP = os.environ['ROBOCOMP']
except:
	print '$ROBOCOMP environment variable not set, using the default value /opt/robocomp'
	ROBOCOMP = '/opt/robocomp'
if len(ROBOCOMP)<1:
	print 'ROBOCOMP environment variable not set! Exiting.'
	sys.exit()

additionalPathStr = ''
icePaths = []
try:
	icePaths.append('/opt/robocomp/interfaces')
	SLICE_PATH = os.environ['SLICE_PATH'].split(':')
	for p in SLICE_PATH:
		icePaths.append(p)
		additionalPathStr += ' -I' + p + ' '
except:
	print 'SLICE_PATH environment variable was not exported. Using only the default paths'
	pass

ice_AprilTagsServer = False
for p in icePaths:
	print 'Trying', p, 'to load AprilTagsServer.ice'
	if os.path.isfile(p+'/AprilTagsServer.ice'):
		print 'Using', p, 'to load AprilTagsServer.ice'
		preStr = "-I/opt/robocomp/interfaces/ -I"+ROBOCOMP+"/interfaces/ " + additionalPathStr + " --all "+p+'/'
		wholeStr = preStr+"AprilTagsServer.ice"
		Ice.loadSlice(wholeStr)
		ice_AprilTagsServer = True
		break
if not ice_AprilTagsServer:
	print 'Couldn\'t load AprilTagsServer'
	sys.exit(-1)
from RoboCompAprilTagsServer import *
ice_CameraSimple = False
for p in icePaths:
	print 'Trying', p, 'to load CameraSimple.ice'
	if os.path.isfile(p+'/CameraSimple.ice'):
		print 'Using', p, 'to load CameraSimple.ice'
		preStr = "-I/opt/robocomp/interfaces/ -I"+ROBOCOMP+"/interfaces/ " + additionalPathStr + " --all "+p+'/'
		wholeStr = preStr+"CameraSimple.ice"
		Ice.loadSlice(wholeStr)
		ice_CameraSimple = True
		break
if not ice_CameraSimple:
	print 'Couldn\'t load CameraSimple'
	sys.exit(-1)
from RoboCompCameraSimple import *
ice_PeopleServer = False
for p in icePaths:
	print 'Trying', p, 'to load PeopleServer.ice'
	if os.path.isfile(p+'/PeopleServer.ice'):
		print 'Using', p, 'to load PeopleServer.ice'
		preStr = "-I/opt/robocomp/interfaces/ -I"+ROBOCOMP+"/interfaces/ " + additionalPathStr + " --all "+p+'/'
		wholeStr = preStr+"PeopleServer.ice"
		Ice.loadSlice(wholeStr)
		ice_PeopleServer = True
		break
if not ice_PeopleServer:
	print 'Couldn\'t load PeopleServer'
	sys.exit(-1)
from RoboCompPeopleServer import *

class CameraSimpleI(CameraSimple):
	def __init__(self, worker):
		self.worker = worker

	def getImage(self, c):
		return self.worker.getImage()
