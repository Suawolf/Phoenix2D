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
#include "theFuzzy.hpp"
#include "GAlgorithm.hpp"

namespace Prueba1 {

//GA
bool onGA = true; //Hacer False para anular GA
Genetics::GAlgorithm ga(0.8, 0.2);
int popSize = 30;
int countInd = -1; //contador de individuos
int countPop = 0; //contador de generaci—n
std::vector<Genetics::Individual>::iterator it; //Puntero al individuo actual que se esta evaluando

bool setup = false;
bool named = false;
bool arrived = false;
bool expDone = false;
bool lapDone =  false;
bool start = false;
double pNeck = 1;
double dashPower = 0.0;
double strafePower = 0.0;
double turnAngle = 0.0;
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
bool turnTime = true;

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
boost::circular_buffer<Position>::iterator dP1origin;
boost::circular_buffer<Position>::iterator dP2origin;
boost::circular_buffer<Position>::iterator dP3origin;
Position positionToGo, potPosition, fuzzyPosition;


/*
 * f(x) = a/(b-exp((-x-c)/d))

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
};*/

void onStart() {
	posAgent.push_back(Position(-20.0,  20.0));
	posAgent.push_back(Position( 20.0,  20.0));
	posAgent.push_back(Position( 40.0,   0.0));
	posAgent.push_back(Position( 20.0, -20.0));
	posAgent.push_back(Position(-20.0, -20.0));
	posAgent.push_back(Position(-40.0,   0.0));

	dummyPath1.push_back(Position(-10.0, -25.0));
	dummyPath1.push_back(Position(-10.0, -15.0));
	dP1origin = dummyPath1.begin();

	dummyPath2.push_back(Position(  0.0, -10.0));
	dummyPath2.push_back(Position(  0.0, -30.0));
	dP2origin = dummyPath2.begin();

	dummyPath3.push_back(Position( 10.0, -28.0));
	dummyPath3.push_back(Position( 10.0, -11.0));
	dP3origin = dummyPath3.begin();

	if(!Self::TEAM_NAME.compare("Fuzzy")){
		if (!start) { //Crear Fuzzy
			fuzzySpace::createFuzzy();
			start = true;
			if (onGA){
				for (int i = 0; i < 92; ++i) {
					ga.addVariable(13);
				}
				//ga.generatePopulation(popSize);
				ga.loadPopulation("data.txt");
				it = ga.begin(); //inicializado al primer individuo de la primera poblacion
			}
		}
	}
}

void executeBeforeKickOff(WorldModel worldModel, std::vector<Message> messages, Commands* commands) {
	if(!setup){ //Setup
		arrived = false;
		commands->changeView("narrow");
		noCollisions = 0.0;
		if((!Self::TEAM_NAME.compare("Fuzzy")) && onGA){
			//std::cout << "En BKO" << std::endl;
			if (countInd > -1){
				it++;
			}
			countInd++;
			if (countInd == popSize){
				// Aqui tienes que accesar a los fits y elegir el mejor
				double best_fit = -1.0;
				std::vector<Genetics::Individual>::iterator it_best;
				for (std::vector<Genetics::Individual>::iterator itind = ga.begin(); itind != ga.end(); ++itind) {
					if (itind->fit > best_fit) {
						it_best = itind;
						best_fit = itind->fit;
					}
				}
				for (std::vector<int>::iterator itvar = it_best->variables.begin(); itvar != it_best->variables.end(); ++itvar) {
					//std::clog << *itvar << ",";
				}
				ga.runGeneration(true);
				countPop++;
				countInd = 0;
				it = ga.begin();
			}
			/* Esto loggeae el individuo que se esta evaluando
			 for (std::vector<int>::iterator itvar = it->variables.begin(); itvar != it->variables.end(); ++itvar) {
				std::clog << *it << ",";
			}*/
			//std::clog << std::endl;
			std::vector<int> variable = it->variables; //De 0 a 91
			std::clog << "P1-," << ++noExp << "," ;

			for (int i = 0; i < 92; i++){
				std::clog << variable[i] << ", ";
			}
			std::clog << std::endl;
			//Define los valores de 0 a 91 en el orden, hay que escalarlos segun los rangos m‡ximos de cada variable
			//INPUTS
			//STAGE 1
			//Effort Rango de 0 a 1  => / 8192.0
			fuzzySpace::changeMF(1,"effort", "tired", (double) (variable[0]) / 8192.0,
					(double) (variable[0] + variable[1] + 1) / 8192.0, false);
			fuzzySpace::changeMF(1,"effort", "fresh", (double) (variable[2]) / 8192.0,
					(double) (variable[2] + variable[3] + 1) / 8192.0, true);

			//Stamina Rango de 0.0 a 8000.0 => * (8000.0 / 8192.0)
			fuzzySpace::changeMF(1,"stamina", "critical", (double) (variable[4]) * (8000.0 / 8192.0),
					(double) (variable[4] + variable[5] + 1) * (8000.0 / 8192.0), false);
			fuzzySpace::changeMF(1,"stamina", "normal", (double) (variable[6]) * (8000.0 / 8192.0),
					(double) (variable[6] + variable[7] + 1) * (8000.0 / 8192.0),
					(double) (variable[6] + variable[7] + variable[8] + 1) * (8000.0 / 8192.0),
					(double) (variable[6] + variable[7] + variable[8] + variable[9] + 2) * (8000.0 / 8192.0));
			fuzzySpace::changeMF(1,"stamina", "high", (double) (variable[10]) * (8000.0 / 8192.0),
					(double) (variable[10] + variable[11] + 1) * (8000.0 / 8192.0), true);

			lowSta = (variable[4] + variable[5] + 1) * (8000.0 / 8192.0);
			highSta = (variable[10] + variable[11] + 1) * (8000.0 / 8192.0);

			//STAGE 2
			//Distance Rango de 0.0 a 150.0 => * (150.0 / 8192.0)
			fuzzySpace::changeMF(2,"distance", "vClose", (double) (variable[12]) * (150.0 / 8192.0),
					(double) (variable[12] + variable[13] + 1) * (150.0 / 8192.0), false);
			fuzzySpace::changeMF(2,"distance", "close", (double) (variable[14]) * (150.0 / 8192.0),
					(double) (variable[14] + variable[15] + 1) * (150.0 / 8192.0),
					(double) (variable[14] + variable[15] + variable[16] + 1) * (150.0 / 8192.0),
					(double) (variable[14] + variable[15] + variable[16] + variable[17] + 2) * (150.0 / 8192.0));
			fuzzySpace::changeMF(2,"distance", "far", (double) (variable[18]) * (150.0 / 8192.0),
					(double) (variable[18] + variable[19] + 1) * (150.0 / 8192.0), true);

			//relSpeed Rango de -4.0 a 4.0 => * (8.0 / 8192.0) - 4.0
			fuzzySpace::changeMF(2,"relSpeed", "away", (double) (variable[20]) * (8.0 / 8192.0) - 4.0,
					(double) (variable[20] + variable[21] + 1) * (8.0 / 8192.0) - 4.0, false);
			fuzzySpace::changeMF(2,"relSpeed", "slow", (double) (variable[22]) * (8.0 / 8192.0) - 4.0,
					(double) (variable[22] + variable[23] + 1) * (8.0 / 8192.0) - 4.0,
					(double) (variable[22] + variable[23] + variable[24] + 1) * (8.0 / 8192.0) - 4.0,
					(double) (variable[22] + variable[23] + variable[24] + variable[25] + 2) * (8.0 / 8192.0) - 4.0);
			fuzzySpace::changeMF(2,"relSpeed", "fast", (double) (variable[26]) * (8.0 / 8192.0) - 4.0,
					(double) (variable[26] + variable[27] + 1) * (8.0 / 8192.0) - 4.0, true);

			//STAGE 3
			//Direction -90.0 a 90.0 => * (180.0 / 8192.0) - 90.0
			fuzzySpace::changeMF(3,"direction", "farLeft", (double) (variable[28]) * (180.0 / 8192.0) - 90.0,
					(double) (variable[28] + variable[29] + 1) * (180.0 / 8192.0) - 90.0, false);
			fuzzySpace::changeMF(3,"direction", "left", (double) (variable[30]) * (180.0 / 8192.0) - 90.0,
					(double) (variable[30] + variable[31] + 1) * (180.0 / 8192.0) - 90.0,
					(double) (variable[30] + variable[31] + variable[32] + 1) * (180.0 / 8192.0) - 90.0,
					(double) (variable[30] + variable[31] + variable[32] + variable[33] + 2) * (180.0 / 8192.0) - 90.0);
			fuzzySpace::changeMF(3,"direction", "center", (double) (variable[34]) * (180.0 / 8192.0) - 90.0,
					(double) (variable[34] + variable[35] + 1) * (180.0 / 8192.0) - 90.0,
					(double) (variable[34] + variable[35] + variable[36] + 1) * (180.0 / 8192.0) - 90.0,
					(double) (variable[34] + variable[35] + variable[36] + variable[37] + 2) * (180.0 / 8192.0) - 90.0);
			fuzzySpace::changeMF(3,"direction", "right", (double) (variable[38]) * (180.0 / 8192.0) - 90.0,
					(double) (variable[38] + variable[39] + 1) * (180.0 / 8192.0) - 90.0,
					(double) (variable[38] + variable[39] + variable[40] + 1) * (180.0 / 8192.0) - 90.0,
					(double) (variable[38] + variable[39] + variable[40] + variable[41] + 2) * (180.0 / 8192.0) - 90.0);
			fuzzySpace::changeMF(3,"direction", "farRight", (double) (variable[42]) * (180.0 / 8192.0) - 90.0,
					(double) (variable[42] + variable[43] + 1) * (180.0 / 8192.0) - 90.0, true);

			//AngSpeed -180.0 a 180 => * (360.0 / 8192.0) - 180.0
			fuzzySpace::changeMF(3,"angSpeed", "toLeft", (double) (variable[44]) * (360.0 / 8192.0) - 180.0,
					(double) (variable[44] + variable[45] + 1) * (360.0 / 8192.0) - 180.0, false);
			fuzzySpace::changeMF(3,"angSpeed", "static", (double) (variable[46]) * (360.0 / 8192.0) - 180.0,
					(double) (variable[46] + variable[47] + 1) * (360.0 / 8192.0) - 180.0,
					(double) (variable[46] + variable[47] + variable[48] + 1) * (360.0 / 8192.0) - 180.0,
					(double) (variable[46] + variable[47] + variable[48] + variable[49] + 2) * (360.0 / 8192.0) - 180.0);
			fuzzySpace::changeMF(3,"angSpeed", "toRight", (double) (variable[50]) * (360.0 / 8192.0) - 180.0,
					(double) (variable[50] + variable[51] + 1) * (360.0 / 8192.0) - 180.0, true);

			//OUTPUTS
			//dash 0.0 a 100.0 => * (100.0 / 8192.0)
			fuzzySpace::changeMF(1,"dash", "walk", (double) (variable[52]) * (100.0 / 8192.0),
					(double) (variable[52] + variable[53] + 1) * (100.0 / 8192.0), false);
			fuzzySpace::changeMF(1,"dash", "jogging", (double) (variable[54]) * (100.0 / 8192.0),
					(double) (variable[54] + variable[55] + 1) * (100.0 / 8192.0),
					(double) (variable[54] + variable[55] + variable[56] + 1) * (100.0 / 8192.0),
					(double) (variable[54] + variable[55] + variable[56] + variable[57] + 2) * (100.0 / 8192.0));
			fuzzySpace::changeMF(1,"dash", "sprint", (double) (variable[58]) * (100.0 / 8192.0),
					(double) (variable[58] + variable[59] + 1) * (100.0 / 8192.0), true);

			//turn -90.0 a 90.0 => * (180.0 / 8192.0) - 90.0
			fuzzySpace::changeMF(3,"turn", "hLeft", (double) (variable[60]) * (180.0 / 8192.0) - 90.0,
					(double) (variable[60] + variable[61] + 1) * (180.0 / 8192.0) - 90.0, false);
			fuzzySpace::changeMF(3,"turn", "left", (double) (variable[62]) * (180.0 / 8192.0) - 90.0,
					(double) (variable[62] + variable[63] + 1) * (180.0 / 8192.0) - 90.0,
					(double) (variable[62] + variable[63] + variable[64] + 1) * (180.0 / 8192.0) - 90.0,
					(double) (variable[62] + variable[63] + variable[64] + variable[65] + 2) * (180.0 / 8192.0) - 90.0);
			fuzzySpace::changeMF(3,"turn", "straight", (double) (variable[66]) * (180.0 / 8192.0) - 90.0,
					(double) (variable[66] + variable[67] + 1) * (180.0 / 8192.0) - 90.0,
					(double) (variable[66] + variable[67] + variable[68] + 1) * (180.0 / 8192.0) - 90.0,
					(double) (variable[66] + variable[67] + variable[68] + variable[69] + 2) * (180.0 / 8192.0) - 90.0);
			fuzzySpace::changeMF(3,"turn", "right", (double) (variable[70]) * (180.0 / 8192.0) - 90.0,
					(double) (variable[70] + variable[71] + 1) * (180.0 / 8192.0) - 90.0,
					(double) (variable[70] + variable[71] + variable[72] + 1) * (180.0 / 8192.0) - 90.0,
					(double) (variable[70] + variable[71] + variable[72] + variable[73] + 2) * (180.0 / 8192.0) - 90.0);
			fuzzySpace::changeMF(3,"turn", "hRight", (double) (variable[74]) * (180.0 / 8192.0) - 90.0,
					(double) (variable[74] + variable[75] + 1) * (180.0 / 8192.0) - 90.0, true);

			//strafe -90.0 a 90.0 => * (180.0 / 8192.0) - 90.0
			fuzzySpace::changeMF(3,"strafe", "aLeft", (double) (variable[76]) * (180.0 / 8192.0) - 90.0,
					(double) (variable[76] + variable[77] + 1) * (180.0 / 8192.0) - 90.0, false);
			fuzzySpace::changeMF(3,"strafe", "dLeft", (double) (variable[78]) * (180.0 / 8192.0) - 90.0,
					(double) (variable[78] + variable[79] + 1) * (180.0 / 8192.0) - 90.0,
					(double) (variable[78] + variable[79] + variable[80] + 1) * (180.0 / 8192.0) - 90.0,
					(double) (variable[78] + variable[79] + variable[80] + variable[81] + 2) * (180.0 / 8192.0) - 90.0);
			fuzzySpace::changeMF(3,"strafe", "face", (double) (variable[82]) * (180.0 / 8192.0) - 90.0,
					(double) (variable[82] + variable[83] + 1) * (180.0 / 8192.0) - 90.0,
					(double) (variable[82] + variable[83] + variable[84] + 1) * (180.0 / 8192.0) - 90.0,
					(double) (variable[82] + variable[83] + variable[84] + variable[85] + 2) * (180.0 / 8192.0) - 90.0);
			fuzzySpace::changeMF(3,"strafe", "dRight", (double) (variable[86]) * (180.0 / 8192.0) - 90.0,
					(double) (variable[86] + variable[87] + 1) * (180.0 / 8192.0) - 90.0,
					(double) (variable[86] + variable[87] + variable[88] + 1) * (180.0 / 8192.0) - 90.0,
					(double) (variable[86] + variable[87] + variable[88] + variable[89] + 2) * (180.0 / 8192.0) - 90.0);
			fuzzySpace::changeMF(3,"strafe", "aRight", (double) (variable[90]) * (180.0 / 8192.0) - 90.0,
					(double) (variable[90] + variable[91] + 1) * (180.0 / 8192.0) - 90.0, true);
		}

		// AQUI SE DEFINE LA POSICION A ENVIAR
		if ((!Self::TEAM_NAME.compare("Fuzzy"))||(!Self::TEAM_NAME.compare("Potential"))){
			xinit = -20.0;
			yinit =  20.0;
		} else {
			if (Self::UNIFORM_NUMBER == 1) {
				dummyPath1.rotate(dP1origin);
				xinit = dummyPath1.front().x;
				yinit = dummyPath1.front().y;
			} else if (Self::UNIFORM_NUMBER == 2) {
				dummyPath2.rotate(dP2origin);
				xinit = dummyPath2.front().x;
				yinit = dummyPath2.front().y;
			} else if (Self::UNIFORM_NUMBER == 3) {
				dummyPath3.rotate(dP3origin);
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

		//Aqu’ detecta colisiones
		if (Self::COLLISION.size() != 0){
			for (std::list<std::string>::iterator it = Self::COLLISION.begin();it != Self::COLLISION.end(); ++it){
				if (!(*it).compare("(player)")){
					noCollisions++;
					std::cout << "No choque, me chocaron " << noCollisions << std::endl;
				}
			}
		}

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
			dashPower = 100.0;
			strafePower = 0.0;
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

		}
		//EVALUA TERMINA CICLO
		//ACOMODAR QUE SOLO FUZZY LO HAGA
		if((!Self::TEAM_NAME.compare("Fuzzy")) && onGA){
			if ((Game::GAME_TIME - timeInit) > 1200){
				expDone = true;
			}
		}
		if(expDone){
			totalStamina = staminaInit - Self::STAMINA_CAPACITY;
			totalTime = Game::GAME_TIME - timeInit;
			std::cout << Game::GAME_TIME << ": SE ACABO: Sta: " << totalStamina << " Time:  " << totalTime <<  " Coll: " << noCollisions << std::endl;
			std::clog << "P1-," << noExp << ",: Team: ,"<< Self::TEAM_NAME <<", Sta: ," << totalStamina << ", Time:  ," << totalTime <<  ", Coll: ," << noCollisions << std::endl;
			double eval = 2000 + (800 - totalTime) + 0.01 * (25000 - totalStamina) - (100 * noCollisions);
			if ((eval < 1.0) || (totalTime >= 1200.0)){
				eval = 1.0;
			}
			std::cout << Game::GAME_TIME << " : Eval: " << eval << std::endl;
			std::clog << "P1-," << noExp << ",: Eval: ," << eval << std::endl;


			if((!Self::TEAM_NAME.compare("Fuzzy")) && onGA){
				it->fit = eval;
			}
			commands->say("END");
		}
}

void onFinish() {

}
}


