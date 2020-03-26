/*
 *    Copyright (C)2020 by YOUR NAME HERE
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
#include "specificworker.h"

/**
* \brief Default constructor
*/
SpecificWorker::SpecificWorker(TuplePrx tprx) : GenericWorker(tprx)
{

}

/**
* \brief Default destructor
*/
SpecificWorker::~SpecificWorker()
{
	std::cout << "Destroying SpecificWorker" << std::endl;
	while (!model_people.empty())
  	{
    	SpecificWorker::ModelPerson p = model_people.back();
		model_people.pop_back();
		delete p.human;
  	}
}

bool SpecificWorker::setParams(RoboCompCommonBehavior::ParameterList params)
{
	try
	{
		RoboCompCommonBehavior::Parameter par = params.at("InnerModelPath");
		std::string innermodel_path = par.value;
		innerModel = std::make_shared<InnerModel>(innermodel_path);
	}
	catch(const std::exception &e) { qFatal("Error reading config params"); }
	this->params = params;

	return true;
}

void SpecificWorker::initialize(int period)
{
	std::cout << "Initialize worker" << std::endl;

	resize(QDesktopWidget().availableGeometry(this).size() * 0.6);
	scene.setSceneRect(dimensions.HMIN, dimensions.VMIN, dimensions.WIDTH, dimensions.HEIGHT);
	graphicsView->setViewport(new QGLWidget);
	graphicsView->scale(1, -1);
	graphicsView->setScene(&scene);
	graphicsView->fitInView(scene.sceneRect(), Qt::KeepAspectRatio);

	//Load World
	initializeWorld();

	this->Period = period;
	timer.start(Period);

}

void SpecificWorker::compute()
{
	ModelPeople new_people;	
	//add one per camera
	for(auto &cam : cameraList)
	{
		if( const auto &[success, observed_people] = cam.pop(); success == true )
		{
//qDebug()<<"PROCESS"<<observed_people.cameraId<<observed_people.peoplelist.size();
			//transformar a coordenadas del mundo y calculo pose
			auto observed_model_people = transformToWorld(observed_people);
			//update people
//qDebug()<<"LIST"<<observed_model_people.size();			
			for(const auto &op : observed_model_people)
			{
				std::vector<ModelPerson>::iterator itmin;
				int minimum = 999999;
				//get minimum
				for (std::vector<ModelPerson>::iterator it = model_people.begin(); it != model_people.end(); it++)
				{
//					if (not it->matched)
					{
						int distance = computeDistance(*it, op);
	//qDebug()<<"Distance"<<distance;					
						if (distance < minimum)
						{
							minimum = distance;
							itmin = it;
						}
					}
				}
				// check if it is the same person
				if (minimum < MINDISTANCE)
				{
					update_person(&(*itmin), op);
				}
				else
				{
					new_people.push_back(op);
				}
			}
			//add people
			if(new_people.size() > 0)
				add_people(new_people);
			clearMatchedPeople();
		}
	}
	removePeople();
	joinPeople();
}

//Unfiy person if distance between then is lower tahn minimum 
void SpecificWorker::joinPeople()
{
	for (std::vector<ModelPerson>::iterator it = model_people.begin(); it != model_people.end();)
	{
		for (std::vector<ModelPerson>::iterator it2 = it +1; it2 != model_people.end(); )
		{
			int distance = computeDistance(*it, *it2);
			if(distance < MINDISTANCE) //join
			{
				//use median position
				it->x = (it->x + it2->x)/2;
				it->z = (it->z + it2->z)/2;
				it->angle = (it->angle + it2->angle)/2;
				qDebug()<<"JOINT PERSON"<<it->id<<it2->id;
				delete it2->human;
				model_people.erase(it2); 
			}
			else
			{
				++it2;
			}
			
		}
		++it;
	}
}

void SpecificWorker::clearMatchedPeople()
{
	for (std::vector<ModelPerson>::iterator it = model_people.begin(); it != model_people.end();it++)
		it->matched = false;
}

void SpecificWorker::removePeople()
{
	//Check if some people must be removed
	auto now = std::chrono::system_clock::now();
	for (std::vector<ModelPerson>::iterator it = model_people.begin(); it != model_people.end();)
	{
		auto last_view = std::chrono::duration_cast<std::chrono::milliseconds>(now - it->tiempo_no_visible).count();
		
//qDebug()<<"last view time"<<last_view;
		if(last_view > MAXTIME)
		{
			qDebug()<<"REMOVE PERSON"<<it->id <<"("<<it->x<<","<<it->z<<")";
			delete it->human;
			model_people.erase(it);  
		}
		else //reset matched 
		{
			++it;
		}
	}

}

int SpecificWorker::computeDistance(const ModelPerson &p_old, const ModelPerson &p_new)
{
	return sqrt( pow(p_old.x - p_new.x, 2) + pow(p_old.z - p_new.z, 2) );
}

void SpecificWorker::add_people(SpecificWorker::ModelPeople plist)
{
	qDebug()<<"ADD PEOPLE"<<model_people.size();
	for (std::vector<ModelPerson>::iterator it = plist.begin(); it != plist.end(); it++)
	{
		it->id = newId;
		newId++;
		
		model_people.push_back(*it);
		qDebug()<<"add person"<<it->id <<"("<<it->x<<","<<it->z<<")";
	}
}

void SpecificWorker::update_person(ModelPerson *p_old, ModelPerson p_new)
{
	p_old->matched = true;
	p_old->x = p_new.x;
	p_old->z = p_new.z;
	p_old->angle = p_new.angle;
	p_old->tiempo_no_visible = p_new.tiempo_no_visible;
	p_old->viewedTimes++;
	if(p_old->human == nullptr)
	{ 
		if(p_old->viewedTimes > MINFRAMES){
			qDebug()<<"MAKE VISIBLE";
			p_old->human = new Human(p_old->id, 3, QRectF(0, 0, 200, 200), QPointF(p_old->x, p_old->z), p_old->angle, &scene);
		}
	}
	else
	{
		p_old->human->update(p_new.cameraId, p_old->x, p_old->z, degreesToRadians(p_old->angle));
	}
}

float SpecificWorker::degreesToRadians(const float angle_)
{	
	if (angle_ == 99999)
		return 99999;
	float angle = angle_ * 2*M_PI / 360;
	if(angle > M_PI)
   		return angle - M_PI*2;
	else if(angle < -M_PI)
   		return angle + M_PI*2;
	else return angle;
}

std::tuple<bool, float, float> SpecificWorker::getPosition(std::vector<float> &acum_x, std::vector<float> &acum_z)
{
	//QVec acum = QVec::zeros(3);
	// compute position as the median of projected joints coordinates on the floor
	if(acum_x.size() == 0 or acum_z.size() == 0)
			return std::make_tuple(false, 0.0, 0.0);

	size_t n = acum_x.size() / 2;
    std::nth_element(acum_x.begin(), acum_x.begin() + n, acum_x.end());
	std::nth_element(acum_z.begin(), acum_z.begin() + n, acum_z.end());
    float median_x = acum_x[n];	
	float median_z = acum_z[n];	
	//acum = acum/(T)obs_person.joints.size();
	return std::make_tuple(true, median_x, median_z);
}

std::tuple<bool, float> SpecificWorker::getOrientation(const ModelPerson &ob_p)
{
	std::pair<std::string, std::string> sh_pair{"left_shoulder", "right_shoulder"};
	std::pair<std::string, std::string> hip_pair{"left_hip", "right_hip"};
	std::vector<std::pair<std::string, std::string>> pair_list{sh_pair, hip_pair};
	for(const auto &pair : pair_list)
	{
		if(ob_p.joints.count(pair.first)>0 and ob_p.joints.count(pair.second)>0)
		{
			auto first = QPointF(ob_p.joints.at(pair.first).x, ob_p.joints.at(pair.first).z);
			auto second = QPointF(ob_p.joints.at(pair.second).x, ob_p.joints.at(pair.second).z);
			auto line =QLineF(first, second).normalVector();	
			return std::make_tuple(true, 90-line.angle());
		}
	}
	return std::make_tuple(false, 99999);
}

SpecificWorker::ModelPeople SpecificWorker::transformToWorld(const RoboCompHumanCameraBody::PeopleData &observed_people)
{
	ModelPeople res;
	for(const auto &obs_person : observed_people.peoplelist)
	{
		std::vector<float> acum_x, acum_z;
		QVec wj;
		ModelPerson person;
		for(const auto &[name, key] : obs_person.joints)
		{
			//qDebug() << "Trans [" << key.x << key.y << key.z << "]";
			wj = innerModel->transform("world", QVec::vec3(key.x, key.y, key.z), "world_camera_" + QString::number(observed_people.cameraId));
			acum_x.push_back(wj.x());
			acum_z.push_back(wj.z());
			person.joints[name] = { wj.x(), wj.y(), wj.z(), key.i, key.j, key.score};
		}
		
		if(person.joints.size() > 0)   
		{
			// compute orientation
			auto [success_r, angle_degrees] = getOrientation(person);
			if (not success_r) //using last computed angle
			{
				angle_degrees = last_computed_angle[observed_people.cameraId];
			}
			else{
				last_computed_angle[observed_people.cameraId] = angle_degrees;
			}
			// compute position
			auto [success_p, median_x, median_z] = getPosition(acum_x, acum_z);

			person.id = obs_person.id;
			person.x = median_x;
			person.y = 0; 
			person.z = median_z;
			person.angle = angle_degrees; 
			person.matched = false; 
			person.human = nullptr; 
			person.to_delete = false;
			person.cameraId = observed_people.cameraId;
			person.tiempo_no_visible = std::chrono::system_clock::now();
			person.gtruth_x = obs_person.x;
			person.gtruth_y = obs_person.y;
			person.gtruth_z = obs_person.z;
			person.gtruth_angle = obs_person.rz;
			res.push_back( person ); 
		}
	}
	return res;
} 

//load world model from file
void SpecificWorker::initializeWorld()
{
	QString val;
	QFile file(QString::fromStdString(params["World"].value));
	if (not file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "Error reading world file, check config params:" << QString::fromStdString(params["World"].value);
		exit(-1);
	}
	val = file.readAll();
	file.close();
	QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
	QJsonObject jObject = doc.object();
	QVariantMap mainMap = jObject.toVariantMap();
	//load dimensions
	QVariantMap dim = mainMap[QString("dimensions")].toMap();
	dimensions = Dimensions{dim["LEFT"].toFloat(), dim["BOTTOM"].toFloat(), dim["WIDTH"].toFloat(), dim["HEIGHT"].toFloat()};
	//int x_offset = -3200;
	//int y_offset = 1850;
	int x_offset = 0;
	int y_offset = 0;
	//load roundtables
	QVariantMap rtables = mainMap[QString("roundTables")].toMap();
	for (auto &t : rtables)
	{
		QVariantList object = t.toList();
		auto box = scene.addEllipse(QRectF(-object[2].toFloat() / 2, -object[3].toFloat() / 2, object[2].toFloat(), object[3].toFloat()), QPen(QColor("Khaki")), QBrush(QColor("Khaki")));
		box->setPos(object[4].toFloat(), object[5].toFloat());
		boxes.push_back(box);
	}
	//load tables
	QVariantMap tables = mainMap[QString("tables")].toMap();
	for (auto &t : tables)
	{
		QVariantList object = t.toList();
		auto box = scene.addRect(QRectF(-object[2].toFloat() / 2, -object[3].toFloat() / 2, object[2].toFloat(), object[3].toFloat()), QPen(QColor("SandyBrown")), QBrush(QColor("SandyBrown")));
		box->setPos(object[4].toFloat()+x_offset, object[5].toFloat()+y_offset);
		box->setRotation(object[6].toFloat());
		boxes.push_back(box);
	}
	//load walls
	QVariantMap walls = mainMap[QString("walls")].toMap();
	for (auto &t : walls)   
	{
		QVariantList object = t.toList();
		auto box = scene.addRect(QRectF(-object[2].toFloat() / 2, -object[3].toFloat() / 2, object[2].toFloat(), object[3].toFloat()), QPen(QColor("Brown")), QBrush(QColor("Brown")));
		box->setPos(object[4].toFloat()+x_offset, object[5].toFloat()+y_offset);
		box->setRotation(object[6].toFloat());
		boxes.push_back(box);
	}

	//load points
	QVariantMap points = mainMap[QString("points")].toMap();
	for (auto &t : points)
	{
		QVariantList object = t.toList();
		auto box = scene.addRect(QRectF(-object[2].toFloat() / 2, -object[3].toFloat() / 2, object[2].toFloat(), object[3].toFloat()), QPen(QColor("Brown")), QBrush(QColor("Brown")));
		box->setPos(object[4].toFloat()+x_offset, object[5].toFloat()+y_offset);
		boxes.push_back(box);
	}
	//load boxes
	QVariantMap cajas = mainMap[QString("boxes")].toMap();
	for (auto &t : cajas)
	{
		QVariantList object = t.toList();
		auto box = scene.addRect(QRectF(-object[2].toFloat() / 2, -object[3].toFloat() / 2, object[2].toFloat(), object[3].toFloat()), QPen(QColor("Brown")), QBrush(QColor("Orange")));
		box->setPos(object[4].toFloat()+x_offset, object[5].toFloat()+y_offset);
		//box->setPos(object[4].toFloat(), object[5].toFloat());
		//box->setRotation(object[6].toFloat()*180/M_PI2);
		box->setFlag(QGraphicsItem::ItemIsMovable);
		boxes.push_back(box);
	}
	QTransform t;
  	//t.translate(3200, -1850);
  	t.rotate(-90);
 	//t.translate(-3200, 1850);
	for(auto &item : boxes)
	{	
    	item->setPos(t.map(item->pos()));
    	item->setRotation(item->rotation() + 90);
		item->setPos(item->pos() + QPointF(1850,3200));
	}
	/////////////
	//AXIS
	scene.addLine(0,0,200,0,QPen(QBrush(QColor("red")),20));
	scene.addLine(0,0,0,200,QPen(QBrush(QColor("blue")),20));
	
}




void SpecificWorker::HumanCameraBody_newPeopleData(PeopleData people)
{
//qDebug()<<"newPeople"<<people.cameraId;
	if(people.cameraId +1 > (int)cameraList.size())
		cameraList.resize(people.cameraId + 1);
	cameraList[people.cameraId].push(people);
}


