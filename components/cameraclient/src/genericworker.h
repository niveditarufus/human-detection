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
#ifndef GENERICWORKER_H
#define GENERICWORKER_H

#include "config.h"
#include <stdint.h>
#include <qlog/qlog.h>

#if Qt5_FOUND
	#include <QtWidgets>
#else
	#include <QtGui>
#endif
#include <ui_mainUI.h>

#include <CommonBehavior.h>

#include <CameraSimple.h>
#include <GetAprilTags.h>
#include <PeopleServer.h>

#define CHECK_PERIOD 5000
#define BASIC_PERIOD 100

using namespace std;
using namespace RoboCompCameraSimple;
using namespace RoboCompGetAprilTags;
using namespace RoboCompPeopleServer;

using TuplePrx = std::tuple<RoboCompGetAprilTags::GetAprilTagsPrxPtr,RoboCompPeopleServer::PeopleServerPrxPtr>;


class GenericWorker :
#ifdef USE_QTGUI
	public QWidget, public Ui_guiDlg
#else
	public QObject
 #endif
{
Q_OBJECT
public:
	GenericWorker(TuplePrx tprx);
	virtual ~GenericWorker();
	virtual void killYourSelf();
	virtual void setPeriod(int p);

	virtual bool setParams(RoboCompCommonBehavior::ParameterList params) = 0;
	QMutex *mutex;


	GetAprilTagsPrxPtr getapriltags_proxy;
	PeopleServerPrxPtr peopleserver_proxy;

	virtual void CameraSimple_getImage(RoboCompCameraSimple::TImage &im) = 0;

protected:
	QTimer timer;
	int Period;

private:


public slots:
	virtual void compute() = 0;
	virtual void initialize(int period) = 0;
signals:
	void kill();
};

#endif
