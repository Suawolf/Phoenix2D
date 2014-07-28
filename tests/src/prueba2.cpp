/*
 * prueba2.cpp
 *
 *  Created on: Jul 17, 2014
 *      Author: luiscarlosgonzalezsua
 */

#include <iostream>
#include <boost/circular_buffer.hpp>
#include "prueba2.hpp"
#include "Self.hpp"
#include "Position.hpp"
#include "Commands.hpp"
#include "Message.hpp"
#include "WorldModel.hpp"

namespace Prueba2 {

bool setup = false;
bool named = false;
bool arrived = false;
bool turned = false;
double pNeck = 1;
boost::circular_buffer<Position> posAgent(2);
boost::circular_buffer<Position> dummyPath1(2);
boost::circular_buffer<Position> dummyPath2(2);
Position positionToGo;

void onStart() {
	posAgent.push_back(Position( 40.0,   0.0));
	posAgent.push_back(Position(-40.0,   0.0));

	dummyPath1.push_back(Position(-30.0,  10.0));
	dummyPath1.push_back(Position(-10.0, -10.0));

	dummyPath2.push_back(Position( 30.0,  10.0));
	dummyPath2.push_back(Position( 10.0, -10.0));
}

void executeBeforeKickOff(WorldModel worldModel, std::vector<Message> messages, Commands* commands) {
	if(!setup){ //Setup
		commands->changeView("narrow");
		double xinit, yinit;
		Position setupPos;
		// AQUI SE DEFINE LA POSICION A ENVIAR
		if ((!Self::TEAM_NAME.compare("Fuzzy"))||(!Self::TEAM_NAME.compare("Potential"))){
			xinit = -36.0;
			yinit =   0.0;
		} else {
			if (Self::UNIFORM_NUMBER == 1) {
				xinit = dummyPath1.front().x;
				yinit = dummyPath1.front().y;
			} else if (Self::UNIFORM_NUMBER == 2) {
				xinit = dummyPath2.front().x;
				yinit = dummyPath2.front().y;
			} else if (Self::UNIFORM_NUMBER == 3) {
				xinit = 0.0;
				yinit = 0.0;
			}
		}
		commands->move(xinit, yinit);
		setup = true;
	}
}

void executePlayOn(WorldModel worldModel, std::vector<Message> messages, Commands* commands) {
	const Position* p = Self::getPosition();
	const Position* oppPosition;
	if(!Self::TEAM_NAME.compare("Fuzzy")){
		if (!named) { //Bautizo
			std::cout << "Soy Fuzzy" << std::endl;
			named = true;
		}

		//INSERTA AQUI LOS ALGORITMOS DE MOVIMIENTO PARA FUZZY

		positionToGo = posAgent.front();

	}else if(!Self::TEAM_NAME.compare("Potential")){
		if (!named) { //Bautizo
			std::cout << "Soy Potential Fields " << std::endl;
			named = true;
		}

		//INSERTA AQUI LOS ALGORITMOS DE MOVIMIENTO PARA PFS

		positionToGo = posAgent.front();



	}else {
		if (!named) { //Bautizo
			std::cout << "Soy Dummy #: " << Self::UNIFORM_NUMBER << std::endl;
			named = true;
		}

		if (Self::UNIFORM_NUMBER == 1) {
			positionToGo = dummyPath1.front();
		} else if (Self::UNIFORM_NUMBER == 2) {
			positionToGo = dummyPath2.front();
		} else if (Self::UNIFORM_NUMBER == 3) {
			positionToGo = Position(0.0, 0.0);
		}

	}

	double d = p->getDistanceTo(&positionToGo);
	if (d > 0.5) {
		arrived = false;
		double dir = p->getDirectionTo(&positionToGo);
		if (fabs(dir) > 5.0) { //Cambiar precisi—n
			commands->turn(dir);
		} else {
			commands->dash(100.0, 0.0);// Cambiar Power
		}
	} else{
		if (!arrived){
			std::cout << "Llegue" << std::endl;
			if((!Self::TEAM_NAME.compare("Fuzzy"))||(!Self::TEAM_NAME.compare("Potential"))){
				posAgent.rotate(posAgent.begin() + 1);
				positionToGo = posAgent.front();
				std::cout << "Me dirijo a X: " << positionToGo.x << " Y: " << positionToGo.y << std::endl;
			} else {
				if (Self::UNIFORM_NUMBER == 1){
					dummyPath1.rotate(dummyPath1.begin() + 1);
					positionToGo = dummyPath1.front();
				} else if (Self::UNIFORM_NUMBER == 2){
					dummyPath2.rotate(dummyPath2.begin() + 1);
					positionToGo = dummyPath2.front();
				}
				//std::cout << "Me dirijo a X: " << positionToGo.x << " Y: " << positionToGo.y << std::endl;
			}
			arrived = true;

		}

	}
}

void onFinish() {

}
}


