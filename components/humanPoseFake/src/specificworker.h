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
#include <QFileDialog>
#include <QMessageBox>
#include <genericworker.h>
#include <innermodel/innermodel.h>



struct FakePoses
{
	RoboCompHumanPose::humansDetected data;
	QString text;
};
Q_DECLARE_METATYPE(FakePoses)

class SpecificWorker : public GenericWorker
{
Q_OBJECT
public:
	SpecificWorker(MapPrx& mprx);
	~SpecificWorker();
	bool setParams(RoboCompCommonBehavior::ParameterList params);
	FakePoses ui_to_human_struct();
	void publish_humans(RoboCompHumanPose::humansDetected publish_humans);
	void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent *event);


public slots:
	void compute();
	void initialize(int period);
	void publish_clicked();
	void publish_next();
    void load_file();
    void save_file();
	void save_file(QString filename, QString text);
    void add_frame();
	void clear_list();
	void frame_clicked(QListWidgetItem *item);

private:
	InnerModel *innerModel;
	QTimer *publish_timer;
	int current_frame_index;
	bool controlKeyPressed;

};

#endif
