/*
 * prueba1.cpp
 *
 *  Created on: Jul 17, 2014
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

namespace Prueba1 {

bool setup = false;
bool named = false;
bool arrived = false;
bool expDone = false;
bool lapDone =  false;
double pNeck = 1;
double dashPower = 100.0;
double thrDis = 1.0;
double xinit, yinit;
int noLaps;

 //METRICS
double noCollisions;
double staminaInit = 0.0;
double totalStamina = 0.0;
int timeInit = 0;
int totalTime = 0;
int noExp = 0;

boost::circular_buffer<Position> posAgent(6);
boost::circular_buffer<Position> dummyPath1(2);
boost::circular_buffer<Position> dummyPath2(2);
boost::circular_buffer<Position> dummyPath3(2);
Position positionToGo, potPosition, fuzzyPosition;


/*
 * f(x) = a/(b-exp((-x-c)/d))
 */
struct Exponential {
	double a;
	double b;
	double c;
	double d;
	Exponential() : a(1.0), b(1.0), c(0.0), d(1.0) {};
	Exponential(double a, double b, double c, double d) : a(a), b(b), c(c), d(d) {};
	double evaluate(double x) {
		return a/(b-exp((-x-c)/d));
	}
};

void onStart() {
	posAgent.push_back(Position(-20.0,  20.0));
	posAgent.push_back(Position( 20.0,  20.0));
	posAgent.push_back(Position( 40.0,   0.0));
	posAgent.push_back(Position( 20.0, -20.0));
	posAgent.push_back(Position(-20.0, -20.0));
	posAgent.push_back(Position(-40.0,   0.0));

	dummyPath1.push_back(Position(-10.0, -25.0));
	dummyPath1.push_back(Position(-10.0, -15.0));

	dummyPath2.push_back(Position(  0.0, -10.0));
	dummyPath2.push_back(Position(  0.0, -30.0));

	dummyPath3.push_back(Position( 10.0, -28.0));
	dummyPath3.push_back(Position( 10.0, -11.0));
}

void executeBeforeKickOff(WorldModel worldModel, std::vector<Message> messages, Commands* commands) {
	if(!setup){ //Setup
		commands->changeView("narrow");
		noCollisions = 0.0;
		// AQUI SE DEFINE LA POSICION A ENVIAR
		if ((!Self::TEAM_NAME.compare("Fuzzy"))||(!Self::TEAM_NAME.compare("Potential"))){
			xinit = -20.0;
			yinit =  20.0;
		} else {
			if (Self::UNIFORM_NUMBER == 1) {
				xinit = dummyPath1.front().x;
				yinit = dummyPath1.front().y;
			} else if (Self::UNIFORM_NUMBER == 2) {
				xinit = dummyPath2.front().x;
				yinit = dummyPath2.front().y;
			} else if (Self::UNIFORM_NUMBER == 3) {
				xinit = dummyPath3.front().x;
				yinit = dummyPath3.front().y;
			} else if (Self::UNIFORM_NUMBER == 4) {
				xinit = -10.0;
				yinit =  20.0;
			} else if (Self::UNIFORM_NUMBER == 5) {
				xinit =  0.0;
				yinit =  20.0;
			} else if (Self::UNIFORM_NUMBER == 6) {
				xinit =  10.0;
				yinit =  20.0;
			}
		}
		setup = true;
		expDone = false;
		lapDone = false;
		timeInit = Game::GAME_TIME;
		if (timeInit < 100){
			noLaps = 3; //Vueltas + 1
		} else {
			noLaps = 2; //Vueltas
		}
		std::cout << "Starting at: " << timeInit << std::endl;
		commands->move(xinit, yinit);
	} else {
		staminaInit = Self::STAMINA_CAPACITY;
	}
}

void executePlayOn(WorldModel worldModel, std::vector<Message> messages, Commands* commands) {
	const Position* p = Self::getPosition();
	double angNeck = 60.0;

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
		positionToGo = posAgent.front();
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

		//Aqu’ detecta colisiones
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
			Geometry::Point theGoal = posAgent.front().getPoint();
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

			if (p->getDistanceTo(&potPosition) > p->getDistanceTo(&posAgent.front())){
				//std::cout << Game::GAME_TIME << ": Esta cerca" << std::endl;
				positionToGo = posAgent.front();
			} else if (p->getDistanceTo(&potPosition) >= thrDis){
				//std::cout << Game::GAME_TIME << ": Lo manda el potencial" << std::endl;
				positionToGo = potPosition;
			} else {
				//std::cout << Game::GAME_TIME << ": Potential es poco" << std::endl;
				positionToGo = posAgent.front();
			}


			//Player *closestPlayer = opponents[0];
			//oppPosition = closestPlayer->getPosition();
			//positionToGo = *oppPosition;
		} else { //NO ve oponentes, va al objetivo directamente
			//std::cout << "NO hay moros en la costa" << std::endl;
			positionToGo = posAgent.front();
		}

		//std::cout << Game::GAME_TIME << ": Estoy en X: " << p->x << " Y: " << p->y << std::endl;
		//std::cout << Game::GAME_TIME <<  ": Debo ir a X: " << positionToGo.x << " Y: " << positionToGo.y << std::endl;
		//std::cout << Game::GAME_TIME << ": Waypoint X: " << posAgent.front().x << " Y: " << posAgent.front().y << std::endl;

	}else {
		if (!named) { //Bautizo
					std::cout << "Soy Dummy #: " << Self::UNIFORM_NUMBER << std::endl;
					named = true;
				}

				if (Self::UNIFORM_NUMBER == 1) {
					positionToGo = dummyPath1.front();
					dashPower = 55.0;
				} else if (Self::UNIFORM_NUMBER == 2) {
					positionToGo = dummyPath2.front();
					dashPower = 55.0;
				} else if (Self::UNIFORM_NUMBER == 3) {
					positionToGo = dummyPath3.front();
					dashPower = 100.0;
				} else if (Self::UNIFORM_NUMBER == 4) {
					positionToGo = Position(-10.0, 20.0);
				}else if (Self::UNIFORM_NUMBER == 5) {
					positionToGo = Position(0.0, 20.0);
				}else if (Self::UNIFORM_NUMBER == 6) {
					positionToGo = Position(10.0, 20.0);
				}
	}

	double d = p->getDistanceTo(&positionToGo);
		if (d > thrDis) {
			arrived = false;
			double dir = p->getDirectionTo(&positionToGo);
			if (fabs(dir) > 10.0) { //Cambiar precisi—n
				commands->turn(dir);
				//DEBUGGING QUITAR LUEGO
				//if((!Self::TEAM_NAME.compare("Fuzzy"))||(!Self::TEAM_NAME.compare("Potential"))){
				//	std::cout << "Turning" << std::endl;
				//}
			} else {
				commands->dash(dashPower, 0.0);
				//DEBUGGING QUITAR LUEGO
				//if((!Self::TEAM_NAME.compare("Fuzzy"))||(!Self::TEAM_NAME.compare("Potential"))){
				//	std::cout << "Dashing" << std::endl;
				//}
			}
		} else{
			if (!arrived){
				//std::cout << Game::GAME_TIME << ": Llegue a X: " << positionToGo.x << " Y: " << positionToGo.y << std::endl;
				if((!Self::TEAM_NAME.compare("Fuzzy"))||(!Self::TEAM_NAME.compare("Potential"))){
					if ((posAgent.front().x == xinit) && (yinit == posAgent.front().y)) {
						if (!lapDone){
							noLaps--;
							lapDone =  true;
						}

						if (noLaps == 0){
							expDone = true;
						}
					} else {
						lapDone = false;
					}

					if(ceil(p->getDistanceTo(&positionToGo)) == ceil(p->getDistanceTo(&posAgent.front()))){
						posAgent.rotate(posAgent.begin() + 1);
						positionToGo = posAgent.front();
					}

					//Determina que dio la vuelta

					//std::cout << "Me dirijo a X: " << positionToGo.x << " Y: " << positionToGo.y << std::endl;
				} else {
					if (Self::UNIFORM_NUMBER == 1){
						dummyPath1.rotate(dummyPath1.begin() + 1);
						positionToGo = dummyPath1.front();
					} else if (Self::UNIFORM_NUMBER == 2){
						dummyPath2.rotate(dummyPath2.begin() + 1);
						positionToGo = dummyPath2.front();
					} else if (Self::UNIFORM_NUMBER == 3){
						dummyPath3.rotate(dummyPath3.begin() + 1);
						positionToGo = dummyPath3.front();
					}
					//std::cout << "Me dirijo a X: " << positionToGo.x << " Y: " << positionToGo.y << std::endl;
				}
				arrived = true;
			}

			//EVALUA TERMINA CICLO
			if(expDone){
				totalStamina = staminaInit - Self::STAMINA_CAPACITY;
				totalTime = Game::GAME_TIME - timeInit;
				std::cout << Game::GAME_TIME << ": SE ACABO: Sta: " << totalStamina << " Time:  " << totalTime <<  " Coll: " << noCollisions << std::endl;
				std::clog << ++noExp << ": Team: "<< Self::TEAM_NAME <<" Sta: " << totalStamina << " Time:  " << totalTime <<  " Coll: " << noCollisions << std::endl;
				setup = false;
				commands->say("END");
			}

		}



}

void onFinish() {

}
}


