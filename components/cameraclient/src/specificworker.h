/*
 *    Copyright (C)2019 by YOUR NAME HERE
 *
 *    This file is part of RoboComp
 *
 *    RoboComp is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    RoboComp is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with RoboComp.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
       \brief
       @author authorname
*/



#ifndef SPECIFICWORKER_H
#define SPECIFICWORKER_H

#include <genericworker.h>
#include <innermodel/innermodel.h>
#include <ipcamreader.h>

//#define URL "http://192.168.0.100:88/cgi-bin/CGIStream.cgi?cmd=GetMJStream&usr=guest&pwd=smpt00"
#define URL "http://10.253.247.24:88/cgi-bin/CGIStream.cgi?cmd=GetMJStream&usr=guest&pwd=smpt00"

using SKELETON_CONNECTIONS = std::vector<std::tuple<std::string, std::string>>;
using HUMAN_JOINT_HEIGHTS = std::map<std::string, double>;

class SpecificWorker : public GenericWorker
{
Q_OBJECT
public:
	SpecificWorker(TuplePrx tprx);
	~SpecificWorker();
	bool setParams(RoboCompCommonBehavior::ParameterList params);

	void CameraSimple_getImage(RoboCompCameraSimple::TImage &im);

public slots:
	void compute();
	void initialize(int period);

private:
	shared_ptr<InnerModel> innermodel;
	SKELETON_CONNECTIONS skeleton;
	HUMAN_JOINT_HEIGHTS joint_heights;
	float scale;
	IPCamReader cam;
	cv::VideoCapture camcv;
	cv::Mat fgMaskMOG2; //fg mask fg mask generated by MOG2 method
	cv::Ptr<cv::BackgroundSubtractorMOG2> pMOG2; //MOG2 Background subtractor
	cv::Mat kernel, erode, dilate;
	void drawBody(cv::Mat frame, const RoboCompPeopleServer::People &people);
	QVec getFloorCoordinates(const RoboCompPeopleServer::Person &person);
	std::tuple<bool, QVec> inverseRay(const RoboCompPeopleServer::Person &p, const std::string &joint);
	QMat K, Ki;
};

#endif
