/*
 * prueba3.cpp
 *
 *  Created on: Jul 6, 2014
 *      Author: luiscarlosgonzalezsua
 */

#include <iostream>
#include <boost/circular_buffer.hpp>
#include "prueba3.hpp"
#include "Self.hpp"
#include "Position.hpp"
#include "Commands.hpp"
#include "Message.hpp"
#include "WorldModel.hpp"

namespace Prueba3 {

bool setup = false;
bool named = false;
bool arrived = false;
bool turned = false;
double pNeck = 1;
boost::circular_buffer<Position> posAgent(2);
Position positionToGo;

void onStart() {
	posAgent.push_back(Position(-40.0,   0.0));
	posAgent.push_back(Position( 40.0,   0.0));
}

void executeBeforeKickOff(WorldModel worldModel, std::vector<Message> messages, Commands* commands) {
	if(!setup){ //Setup
		commands->changeView("narrow");
		commands->move(-40.0, 0.0);
		positionToGo = posAgent.front();// AQUI SE DEFINE LA POSICION A ENVIAR
		setup = true;
	}else if(!turned){ // ESTO SOLO LO HACE GIRAR
		commands->turn(180.0);
		std::cout << "Turned" << std::endl;
		turned = true;
	}

	if(Self::HEAD_ANGLE == 0){ //Turn Neck
		if (pNeck > 0){
			pNeck = Self::HEAD_ANGLE;
			commands->turnNeck(-60.0);
		} else {
			pNeck = Self::HEAD_ANGLE;
			commands->turnNeck(60.0);
		}
	}else if(Self::HEAD_ANGLE > 0){
		if (pNeck > 0){
			pNeck = Self::HEAD_ANGLE;
			commands->turnNeck(-60.0);
		} else {
			pNeck = Self::HEAD_ANGLE;
			commands->turnNeck(60.0);
		}
	} else {
		if (pNeck > 0){
			pNeck = Self::HEAD_ANGLE;
			commands->turnNeck(-60.0);
		} else {
			pNeck = Self::HEAD_ANGLE;
			commands->turnNeck(60.0);
		}
	}
}

void executePlayOn(WorldModel worldModel, std::vector<Message> messages, Commands* commands) {
	const Position* p = Self::getPosition();
	const Position* oppPosition;
	bool lost = false;
	if(!Self::TEAM_NAME.compare("Fuzzy")){
		if (!named) { //Bautizo
			std::cout << "Soy Fuzzy" << std::endl;
			named = true;
		}

		//INSERTA AQUI LOS ALGORITMOS DE MOVIMIENTO PARA FUZZY





	}else if(!Self::TEAM_NAME.compare("Potential")){
		if (!named) { //Bautizo
			std::cout << "Soy Potential Fields " << std::endl;
			named = true;
		}

		//INSERTA AQUI LOS ALGORITMOS DE MOVIMIENTO PARA PFS






	} else {
		if (!named){ //Bautizo
			std::cout << "Soy el Dummy Agresivo" << std::endl;
			named = true;
		}
		lost = true;

		std::vector<Player*> opponents = worldModel.getPlayersOrderedByDistanceTo(*Self::getPosition());
		if (opponents.size() > 0){ //Encuentra al jugador
			Player *closestPlayer = opponents[0];
			oppPosition = closestPlayer->getPosition();
			positionToGo = *oppPosition;
			arrived = false;
		} else { //No encuentra al jugador y lo busca
			if (!Self::VIEW_MODE_WIDTH.compare("narrow")){ //Se cambia a narrow
				commands->changeView("wide");
			}
			std::cout << "No veo ni madres" << std::endl;
			//oppPosition = Self::getPosition();
			positionToGo = *oppPosition;
		}
	}
	//Aqui empieza a moverse.
	//std::cout << "X: " << positionToGo.x << " Y: " << positionToGo.y << std::endl;
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
			std::cout << "Llegue" <<std::endl;
			if((!Self::TEAM_NAME.compare("Fuzzy"))||(!Self::TEAM_NAME.compare("Potential"))){
				posAgent.rotate(posAgent.begin() + 1);
				positionToGo = posAgent.front();
				std::cout << "Me dirijo a X: " << positionToGo.x << " Y: " << positionToGo.y << std::endl;
			}
			arrived = true;
		}
	}
	//std::cout << "Neck: " << Self::HEAD_ANGLE << std::endl;
	//std::cout << "pNeck: " << pNeck << std::endl;
	double angNeck = 60.0;

	if(Self::HEAD_ANGLE == 0){ //Turn Neck
		if (pNeck > 0){
			pNeck = Self::HEAD_ANGLE;
			commands->turnNeck(-angNeck);
			//std::cout << "Neck Turn: -60.0" << std::endl;
		} else {
			pNeck = Self::HEAD_ANGLE;
			commands->turnNeck(angNeck);
			//std::cout << "Neck Turn: 60.0" << std::endl;
		}
	}else{
		pNeck = Self::HEAD_ANGLE;
		commands->turnNeck(-pNeck);
		//std::cout << "Neck Turn: " << -pNeck << std::endl;
	}
}

void onFinish() {

}

}

