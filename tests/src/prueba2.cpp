/*
 * prueba2.cpp
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
#include "theFuzzy.hpp"

namespace Prueba2 {

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

//FUZZY
double lowSta = 2600.0;
double highSta = 6000.0;
double prevDistance = -1.0;
double prevAngle = 1000.0;
double prevStamina = 8000.0;
double timePAng = 0.0;
double timePDist = 0.0;
double strafePower = 0.0;
double turnAngle = 0.0;
bool turnTime = true;
bool start = false;

 //METRICS
double noCollisions;
double staminaInit = 0.0;
double totalStamina = 0.0;
int timeInit = 0;
int totalTime = 0;
int noExp = 0;


boost::circular_buffer<Position> posAgent(2);
boost::circular_buffer<Position> dummyPath1(2);
boost::circular_buffer<Position> dummyPath2(2);
boost::circular_buffer<Position>::iterator dP1origin;
boost::circular_buffer<Position>::iterator dP2origin;
Position positionToGo, potPosition, fuzzyPosition;

void onStart() {
	posAgent.push_back(Position( 40.0,   0.0));
	posAgent.push_back(Position(-36.0,   0.0));

	dummyPath1.push_back(Position(-30.0,  10.0));
	dummyPath1.push_back(Position(-10.0, -10.0));
	dP1origin = dummyPath1.begin();

	dummyPath2.push_back(Position( 30.0,  10.0));
	dummyPath2.push_back(Position( 10.0, -10.0));
	dP2origin = dummyPath2.begin();

	if(!Self::TEAM_NAME.compare("Fuzzy")){
		if (!start) { //Crear Fuzzy
			fuzzySpace::createFuzzy();
			start = true;
		}
	}
}

void executeBeforeKickOff(WorldModel worldModel, std::vector<Message> messages, Commands* commands) {
	if(!setup){ //Setup
		named = false;
		arrived = false;
		commands->changeView("narrow");
		noCollisions = 0.0;
		// AQUI SE DEFINE LA POSICION A ENVIAR
		if ((!Self::TEAM_NAME.compare("Fuzzy"))||(!Self::TEAM_NAME.compare("Potential"))){
			xinit = -36.0;
			yinit =   0.0;
		} else {
			if (Self::UNIFORM_NUMBER == 1) {
				dummyPath1.rotate(dP1origin);
				xinit = dummyPath1.front().x;
				yinit = dummyPath1.front().y;
			} else if (Self::UNIFORM_NUMBER == 2) {
				dummyPath2.rotate(dP2origin);
				xinit = dummyPath2.front().x;
				yinit = dummyPath2.front().y;
			}
		}
		setup = true;
		expDone = false;
		lapDone = false;
		timeInit = Game::GAME_TIME;
		noLaps = 1; //Vueltas + 1
		//std::cout << "Starting at: " << timeInit << std::endl;
		commands->move(xinit, yinit);
	} else {
		staminaInit = Self::STAMINA_CAPACITY;
	}
}

void executePlayOn(WorldModel worldModel, std::vector<Message> messages, Commands* commands) {
	const Position* p = Self::getPosition();
	double angNeck = 60.0;
	setup = false;

	if(!Self::TEAM_NAME.compare("Fuzzy")){
		turnTime = (Game::GAME_TIME % 2);
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
		std::vector<double> fuzzyOut;
		std::vector<Player*> opponents = worldModel.getPlayersOrderedByDistanceTo(*Self::getPosition());
		if (opponents.size() > 0){
			Position *oppPos = opponents[0]->getPosition();

			double yLoc = p->y;
			double staDen = fabs(Self::STAMINA - prevStamina);
			if(!staDen){
				staDen = -1.0;
			}
			double effort = ((Self::STAMINA - prevStamina) / staDen)*Self::EFFORT;
			//double effort = Self::EFFORT;
			double stamina = Self::STAMINA;


			double distance =  p->getDistanceTo(oppPos);
			double relSpeed;
			if (prevDistance == -1.0){
				relSpeed = 0.0;
			} else {
				relSpeed = (prevDistance - distance)/(Game::GAME_TIME - timePDist);
			}

			prevDistance = distance;


			double direction = p->getDirectionTo(oppPos);
			double angSpeed;
			if (prevAngle == 1000.0){
				angSpeed = 0.0;
			} else {
				angSpeed = (direction -  prevAngle)/(Game::GAME_TIME - timePAng);
			}

			prevAngle = direction;
			timePAng = timePDist = Game::GAME_TIME;


			//std::cout << Game::GAME_TIME << ": yLoc: " << yLoc  << " effort: " << effort << " stamina: " << stamina << "prevStamina: " << prevStamina << std::endl;
			//std::cout << Game::GAME_TIME << ": distance: " << distance  << " Relative Speed: " << relSpeed << std::endl;
			//std::cout << Game::GAME_TIME << ": direction: " << direction  << " Angular Speed: " << angSpeed << std::endl;
			if ((stamina <= lowSta)||(stamina >= highSta)) {
				prevStamina = stamina;
			} else if (distance < 15.0){
				prevStamina = 8000.0;
			}

			fuzzyOut = fuzzySpace::obtainOut(yLoc, effort, stamina, distance, relSpeed, direction, angSpeed);
			if (fuzzyOut[0] > 100.0){
				dashPower = 100.0;
			} else {
				dashPower = round(fuzzyOut[0]);
			}


			turnAngle = round(fuzzyOut[1]);

			strafePower = round(fuzzyOut[2]);

			//turnAngle = turnAngle + p->getDirectionTo(&posAgent.front());
			//turnAngle = p->getDirectionTo(&posAgent.front());

			//Geometry::Vector2D fuzzyGo(p->getDistanceTo(&posAgent.front()), Geometry::toRadians(turnAngle), true);
			//fuzzyPosition = Position((double) (fuzzyGo.dx + p->x), (double) (fuzzyGo.dy + p->y));

			//std::cout << Game::GAME_TIME << ": FDash: " << fuzzyOut[0] << " Turn: " << fuzzyOut[1] << " Straf: " << fuzzyOut[2] << " turnTime: "<< turnTime << std::endl;
			//std::cout << Game::GAME_TIME << ": DistoGoal: " << p->getDistanceTo(&posAgent.front()) << " DirtoGoal: " << p->getDirectionTo(&posAgent.front()) << std::endl;
			//std::cout << Game::GAME_TIME << ": Fuzzy X: " << fuzzyGo.dx << " Y: " << fuzzyGo.dy << std::endl;


			positionToGo = posAgent.front();

		} else { //NO ve oponentes, va al objetivo directamente
			//std::cout << "NO hay moros en la costa" << std::endl;
			double yLoc = p->y;
			double staDen = fabs(Self::STAMINA - prevStamina);
			if(!staDen){
				staDen = -1.0;
			}
			double effort = ((Self::STAMINA - prevStamina) / staDen)*Self::EFFORT;
			//double effort = Self::EFFORT;
			double stamina = Self::STAMINA;

			if ((stamina <= lowSta)||(stamina >= highSta)) {
				prevStamina = stamina;
			}

			fuzzyOut = fuzzySpace::obtainOut(yLoc, effort, stamina, 150.0, -4.0, 0.0, 0.0);
			if (fuzzyOut[0] > 100.0){
				dashPower = 100.0;
			} else {
				dashPower = round(fuzzyOut[0]);
			}
			strafePower = round(fuzzyOut[2]);
			turnAngle = 0.0;
			turnTime = true;
			positionToGo = posAgent.front();
			//std::cout << Game::GAME_TIME << "  es difuso" << std::endl;
			//std::cout << Game::GAME_TIME << ": yLoc: " << yLoc  << " effort: " << effort << " stamina: " << stamina << std::endl;

		}

		//std::cout << Game::GAME_TIME << ": FDash: " << fuzzyOut[0] << " Turn: " << fuzzyOut[1] << " Straf: " << fuzzyOut[2] << " turnTime: "<< turnTime << std::endl;
		//std::cout << Game::GAME_TIME << ": Dash: " << dashPower  << " Turn: " << turnAngle << " Strafe: " << strafePower << std::endl;
		//std::cout << Game::GAME_TIME << ": Estoy en X: " << p->x << " Y: " << p->y << std::endl;
		//std::cout << Game::GAME_TIME <<  ": Debo ir a X: " << positionToGo.x << " Y: " << positionToGo.y << " Dir: " << p->getDirectionTo(&positionToGo) << std::endl;
		//std::cout << Game::GAME_TIME << ": Waypoint X: " << posAgent.front().x << " Y: " << posAgent.front().y << " Dir: " << p->getDirectionTo(&posAgent.front()) << std::endl << std::endl;
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
			Geometry::Point theGoal = posAgent.front().getPoint();
			Geometry::Point playerPos = p->getPoint();
			for (int i = 0; i < opponents.size(); i++){
				obstacles.push_back(opponents[i]->getPosition()->getPoint());
			}

			//Math::Inverse equObstacles(15.0, -0.8, -1.5);
			//Math::Linear equGoal(-0.0125, -2);
			//Geometry::PFields<Math::Inverse, Math::Linear> potential(equObstacles, equGoal);

			Math::Inverse equObstacles(12.5, -0.8, -1.5);
			Math::Inverse equGoal(-20.0, -0.9, -0.5);
			Geometry::PFields<Math::Inverse, Math::Inverse> potential(equObstacles, equGoal);

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
			dashPower = 100.0;
		} else if (Self::UNIFORM_NUMBER == 2) {
			positionToGo = dummyPath2.front();
			dashPower = 100.0;
		}
		//std::cout << "Soy Dummy #: " << Self::UNIFORM_NUMBER << ": Debo ir a X: " << positionToGo.x << " Y: " << positionToGo.y << std::endl;
	}

	double d = p->getDistanceTo(&positionToGo);
			if (d > thrDis) {
				arrived = false;
				double dir = p->getDirectionTo(&positionToGo);
				if ((fabs(dir + turnAngle)) * turnTime > 10.0) { //Cambiar precisi�n
					commands->turn((dir + turnAngle) * turnTime);
					//DEBUGGING QUITAR LUEGO
					//if((!Self::TEAM_NAME.compare("Fuzzy"))||(!Self::TEAM_NAME.compare("Potential"))){
					//	std::cout << "Turning " << (dir + turnAngle) * turnTime << " dir: " << dir << std::endl;
					//}
				} else {
					commands->dash(dashPower, strafePower);
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
						}
						//std::cout << "Me dirijo a X: " << positionToGo.x << " Y: " << positionToGo.y << std::endl;
					}
					arrived = true;
				}

			}
			//EVALUA TERMINA CICLO
			//ACOMODAR QUE SOLO FUZZY LO HAGA
			if(expDone){
				totalStamina = staminaInit - Self::STAMINA_CAPACITY;
				totalTime = Game::GAME_TIME - timeInit;
				std::cout << Game::GAME_TIME << ": SE ACABO: Sta: " << totalStamina << " Time:  " << totalTime <<  " Coll: " << noCollisions << std::endl;
				std::clog << "P2-," << ++noExp << ",: Team: ,"<< Self::TEAM_NAME <<", Sta: ," << totalStamina << ", Time:  ," << totalTime <<  ", Coll: ," << noCollisions << std::endl;
				//double eval = 2000 + (800 - totalTime) + 0.01 * (25000 - totalStamina) - (100 * noCollisions);
				//if ((eval < 1.0) || (totalTime >= 1200.0)){
				//	eval = 1.0;
				//}
				//std::cout << Game::GAME_TIME << " : Eval: " << eval << std::endl;
				//std::clog << "P1-," << noExp << ",: Eval: ," << eval << std::endl;

				commands->say("END");
			}
}

void onFinish() {

}
}


