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
#include "theFuzzy.hpp"


namespace Prueba3 {

bool setup = false;
bool named = false;
bool arrived = false;
bool expDone = false;
bool turned = false;
double pNeck = 1;
double dashPower = 100.0;
double thrDis = 1.0;
double xinit, yinit;

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


Position positionToGo, potPosition, fuzzyPosition, goalPosition;
Position lastPos = Position(30.0, 0.0);

void onStart() {
	goalPosition = Position(40.0, 0.0);
	if(!Self::TEAM_NAME.compare("Fuzzy")){
		if (!start) { //Crear Fuzzy
			fuzzySpace::createFuzzy();
			start = true;
		}
	}
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
				yinit =   0.0;
			} else if (Self::UNIFORM_NUMBER == 2) {
				xinit = -30.0;
				yinit =   0.0;
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
	bool lost = true;
	double angNeck = 60.0;
	setup = false;
	turned = false;


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


				positionToGo = goalPosition;

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
				positionToGo = goalPosition;
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
			Geometry::Point theGoal = goalPosition.getPoint();
			Geometry::Point playerPos = p->getPoint();
			for (int i = 0; i < opponents.size(); i++){
				obstacles.push_back(opponents[i]->getPosition()->getPoint());
			}

			Math::Inverse equObstacles(12.5, -0.8, -1.5);
			Math::Inverse equGoal(-20.0, -0.9, -0.5);
			Geometry::PFields<Math::Inverse, Math::Inverse> potential(equObstacles, equGoal);

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
		if (opponents.size() > 0){ //Ve algo
			for (int i = 0; i < opponents.size(); ++i){
				Player *aPlayer = opponents [i];
				if (!aPlayer->getTeam().compare("opp")){ //Encuentra al jugador
					//std::cout << Game::GAME_TIME << ": Soy #: " << Self::UNIFORM_NUMBER << " And I saw  my bitch: " << aPlayer->getTeam() << std::endl;
					oppPosition = aPlayer->getPosition();
					positionToGo = *oppPosition;
					lost = false;
				}
			}
			if (!lost){
				pNeck = Self::HEAD_ANGLE;
				commands->turnNeck(-pNeck);
			} else {
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
				positionToGo = lastPos;
			}


		} else { //No encuentra al jugador y lo busca
			lost = true;

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


			//std::cout << Game::GAME_TIME << ": Soy #: " << Self::UNIFORM_NUMBER << " No veo ni madres" << std::endl;
			positionToGo = lastPos;
		}
	}
	//Aqui empieza a moverse.
	double d = p->getDistanceTo(&positionToGo);
	if (d > 0.5) {
		arrived = false;
		double dir = p->getDirectionTo(&positionToGo);
		if ((fabs(dir + turnAngle)) * turnTime > 10.0) { //Cambiar precisi—n
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
			if((!Self::TEAM_NAME.compare("Fuzzy"))||(!Self::TEAM_NAME.compare("Potential"))){
				if(ceil(p->getDistanceTo(&positionToGo)) == ceil(p->getDistanceTo(&goalPosition))){
					expDone = true;
				}
			}
			arrived = true;
		}



	}
	//EVALUA TERMINA CICLO
	if(expDone){
		totalStamina = staminaInit - Self::STAMINA_CAPACITY;
		totalTime = Game::GAME_TIME - timeInit;
		std::cout << Game::GAME_TIME << ": SE ACABO: Sta: " << totalStamina << " Time:  " << totalTime <<  " Coll: " << noCollisions << std::endl;
		std::clog <<"P3-," << ++noExp << ",: Team: ,"<< Self::TEAM_NAME << ", Sta: ," << totalStamina << ", Time:  ," << totalTime <<  ", Coll: ," << noCollisions << std::endl;
		commands->say("END");
	}

}

void onFinish() {

}

}

