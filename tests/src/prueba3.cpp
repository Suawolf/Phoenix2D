/*
 * prueba3.cpp
 *
 *  Created on: Jul 6, 2014
 *      Author: luiscarlosgonzalezsua
 */

#include <iostream>
#include <boost/circular_buffer.hpp>
#include <cmath>
#include "Game.hpp"
#include "PFields.hpp"
#include "functions.hpp"
#include "geometry.hpp"
#include "prueba1.hpp"
#include "Self.hpp"
#include "Position.hpp"
#include "Commands.hpp"
#include "Message.hpp"
#include "WorldModel.hpp"

namespace Prueba3 {

bool setup = false;
bool named = false;
bool arrived = false;
bool expDone = false;
double pNeck = 1;
double dashPower = 100.0;
double thrDis = 1.0;
double xinit, yinit;

 //METRICS
double noCollisions;
double staminaInit = 0.0;
double totalStamina = 0.0;
int timeInit = 0;
int totalTime = 0;
int noExp = 0;


boost::circular_buffer<Position> posAgent(2);
Position positionToGo, potPosition, fuzzyPosition, goalPosition;

void onStart() {
	goalPosition = Position(40.0, 0.0);
}

void executeBeforeKickOff(WorldModel worldModel, std::vector<Message> messages, Commands* commands) {
	if(!setup){ //Setup
		commands->changeView("narrow");
		noCollisions = 0.0;
		// AQUI SE DEFINE LA POSICION A ENVIAR
		if ((!Self::TEAM_NAME.compare("Fuzzy"))||(!Self::TEAM_NAME.compare("Potential"))){
			xinit = -40.0;
			yinit =   0.0;
		} else {
			if (Self::UNIFORM_NUMBER == 1) {
				xinit =   0.0;
				yinit = -20.0;
			} else if (Self::UNIFORM_NUMBER == 2) {
				xinit = -40.0;
				yinit =  20.0;
			}
		}
		setup = true;
		expDone = false;
		timeInit = Game::GAME_TIME;
		//std::cout << "Starting at: " << timeInit << std::endl;
		commands->move(xinit, yinit);
	} else {
		staminaInit = Self::STAMINA_CAPACITY;
	}

	if (!Self::TEAM_NAME.compare("Dummy")) {
		//Implementar Busqueda
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
}

void executePlayOn(WorldModel worldModel, std::vector<Message> messages, Commands* commands) {
	const Position* p = Self::getPosition();
	const Position* oppPosition;
	bool lost = false;
	double angNeck = 60.0;
	setup = false;


	if(!Self::TEAM_NAME.compare("Fuzzy")){
		if (!named) { //Bautizo
			std::cout << "Soy Fuzzy" << std::endl;
			named = true;
		}


		//DETECCION DE COLISIONES
		if (Self::COLLISION.size() != 0){
			for (std::list<std::string>::iterator it = Self::COLLISION.begin();it != Self::COLLISION.end(); ++it){
				if (!(*it).compare("(player)")){
					noCollisions++;
					std::cout << "No choque, me chocaron " << noCollisions << std::endl;
				}
			}
		}

		//MOVER EL CUELLO
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

		//INSERTA AQUI LOS ALGORITMOS DE MOVIMIENTO PARA FUZZY
		positionToGo = goalPosition;
		dashPower = 100.0;



	}else if(!Self::TEAM_NAME.compare("Potential")){
		if (!named) { //Bautizo
			std::cout << "Soy Potential Fields " << std::endl;
			named = true;
		}

		//MOVER EL CUELLO
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

		//Aqu� detecta colisiones
		if (Self::COLLISION.size() != 0){
			for (std::list<std::string>::iterator it = Self::COLLISION.begin();it != Self::COLLISION.end(); ++it){
				if (!(*it).compare("(player)")){
					noCollisions++;
					std::cout << "No choque, me chocaron " << noCollisions << std::endl;
				}
			}
		}

		//Esto es para mientras se implementa el algoritmo
		dashPower = 100.0;

		//INSERTA AQUI LOS ALGORITMOS DE MOVIMIENTO PARA PFS
		std::vector<Player*> opponents = worldModel.getPlayersOrderedByDistanceTo(*Self::getPosition());
		if (opponents.size() > 0){ //Encuentra jugadores
			std::vector<Geometry::Point> obstacles;
			Geometry::Point theGoal = goalPosition.getPoint();
			Geometry::Point playerPos = p->getPoint();
			for (int i = 0; i < opponents.size(); i++){
				obstacles.push_back(opponents[i]->getPosition()->getPoint());
			}

			Math::Inverse equObstacles(15.0, -0.8, -1.5);
			Math::Linear equGoal(-0.0125, -2);
			Geometry::PFields<Math::Inverse, Math::Linear> potential(equObstacles, equGoal);

			Geometry::Vector2D potentialGo = potential.computePotential(playerPos, obstacles, theGoal);
			//potentialGo.scale(10.0); //Escalamiento
			potPosition = Position((double) (potentialGo.dx + p->x), (double) (potentialGo.dy + p->y));
			//std::cout << Game::GAME_TIME << ": Componente X: " << potentialGo.dx << " Componente Y: " << potentialGo.dy << std::endl;

			if (p->getDistanceTo(&potPosition) > p->getDistanceTo(&goalPosition)){
				//std::cout << Game::GAME_TIME << ": Esta cerca" << std::endl;
				positionToGo = goalPosition;
			} else if (p->getDistanceTo(&potPosition) >= thrDis){
				//std::cout << Game::GAME_TIME << ": Lo manda el potencial" << std::endl;
				positionToGo = potPosition;
			} else {
				//std::cout << Game::GAME_TIME << ": Potential es poco" << std::endl;
				positionToGo = goalPosition;
			}


			//Player *closestPlayer = opponents[0];
			//oppPosition = closestPlayer->getPosition();
			//positionToGo = *oppPosition;
		} else { //NO ve oponentes, va al objetivo directamente
			//std::cout << "NO hay moros en la costa" << std::endl;
			positionToGo = goalPosition;
		}

	} else {
		if (!named) { //Bautizo
			std::cout << "Soy Dummy Agresivo #: " << Self::UNIFORM_NUMBER << std::endl;
			named = true;
		}

		std::vector<Player*> opponents = worldModel.getPlayersOrderedByDistanceTo(*Self::getPosition());
		if (opponents.size() > 0){ //Encuentra al jugador
			for (int i = 0; i < opponents.size(); ++i){
				Player *aPlayer = opponents [i];
				if (!aPlayer->getTeam().compare("opp")){
					std::cout << Game::GAME_TIME << ": Soy #: " << Self::UNIFORM_NUMBER << " And I saw  my bitch: " << aPlayer->getTeam() << std::endl;
					oppPosition = aPlayer->getPosition();
					positionToGo = *oppPosition;
					pNeck = Self::HEAD_ANGLE;
					commands->turnNeck(-pNeck);
				}
			}

		} else { //No encuentra al jugador y lo busca

			//Implementar Busqueda
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


			std::cout << Game::GAME_TIME << ": Soy #: " << Self::UNIFORM_NUMBER << " No veo ni madres" << std::endl;
			positionToGo = *p;
		}
	}
	//Aqui empieza a moverse.
	//std::cout << "X: " << positionToGo.x << " Y: " << positionToGo.y << std::endl;
	double d = p->getDistanceTo(&positionToGo);
	if (d > 0.5) {
		arrived = false;
		double dir = p->getDirectionTo(&positionToGo);
		if (fabs(dir) > 5.0) { //Cambiar precisi�n
			commands->turn(dir);
		} else {
			commands->dash(100.0, 0.0);// Cambiar Power
		}
	} else{
		if (!arrived){
			if((!Self::TEAM_NAME.compare("Fuzzy"))||(!Self::TEAM_NAME.compare("Potential"))){
				if(ceil(p->getDistanceTo(&positionToGo)) == ceil(p->getDistanceTo(&goalPosition))){
					expDone = true;
				}
			}
			arrived = true;
		}

		//EVALUA TERMINA CICLO
		if(expDone){
			totalStamina = staminaInit - Self::STAMINA_CAPACITY;
			totalTime = Game::GAME_TIME - timeInit;
			std::cout << Game::GAME_TIME << ": SE ACABO: Sta: " << totalStamina << " Time:  " << totalTime <<  " Coll: " << noCollisions << std::endl;
			std::clog << ++noExp << ": Team: "<< Self::TEAM_NAME <<" Sta: " << totalStamina << " Time:  " << totalTime <<  " Coll: " << noCollisions << std::endl;
			commands->say("END");
		}

	}

}

void onFinish() {

}

}
