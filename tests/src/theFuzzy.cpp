/*
 * theFuzzy.cpp
 *
 *  Created on: Aug 1, 2014
 *      Author: luiscarlosgonzalezsua
 */

#include "theFuzzy.hpp"
#include "fuzzy.hpp"
#include <iostream>

using namespace Fuzzy;

namespace fuzzySpace{

bool rightFlank = true; //Default Right
double dashPower = 0.0;
double turnAng = 0.0;
double strafeAng = 0.0;
FVariable* v;
FEngine fStage1, fStage2Slow, fStage2Jogging, fStage2Sprint, fStage3Stay, fStage3Turn, fStage3Strafe;
FEngine *fStage2, *fStage3;


/*!
 * Creates the Fuzzy Thing
 */
void createFuzzy(){

		//INPUTS
		//STAGE 1
		v = new FVariable("effort", 0.0, 1.0);
		v->addMembershipFunction("tired",       new Math::RampDesc(0.99, 1.0));
		v->addMembershipFunction("fresh",       new Math::RampAsc(0.99, 1.0));
		fStage1.addVariable(v);

		v = new FVariable("stamina", 0.0, 8000.0);
		v->addMembershipFunction("critical",    new Math::RampDesc(2700.0, 3000.0));
		v->addMembershipFunction("normal",      new Math::Trapezoidal(2950.0, 4850.0, 6000.0, 7000.0));
		v->addMembershipFunction("high",        new Math::RampAsc(6000.0, 7000.0));
		fStage1.addVariable(v);

		//STAGE 2
		v = new FVariable("distance", 0.0, 150.0);
		v->addMembershipFunction("vClose",  	new Math::RampDesc(2.0, 5.0));
		v->addMembershipFunction("close",       new Math::Trapezoidal(2.0, 5.0, 10.0, 13.0));
		v->addMembershipFunction("far",         new Math::RampAsc(10.0, 13.0));
		fStage2Slow.addVariable(v);
		v = new FVariable("distance", 0.0, 150.0);
		v->addMembershipFunction("vClose",  	new Math::RampDesc(2.0, 5.0));
		v->addMembershipFunction("close",       new Math::Trapezoidal(2.0, 5.0, 10.0, 13.0));
		v->addMembershipFunction("far",         new Math::RampAsc(10.0, 13.0));
		fStage2Jogging.addVariable(v);
		v = new FVariable("distance", 0.0, 150.0);
		v->addMembershipFunction("vClose",  	new Math::RampDesc(2.0, 5.0));
		v->addMembershipFunction("close",       new Math::Trapezoidal(2.0, 5.0, 10.0, 13.0));
		v->addMembershipFunction("far",         new Math::RampAsc(10.0, 13.0));
		fStage2Sprint.addVariable(v);

		v = new FVariable("relSpeed", -4.0, 4.0);
		v->addMembershipFunction("away",        new Math::RampDesc(-0.1, 0.0));
		v->addMembershipFunction("slow",        new Math::Trapezoidal(-0.1, 0.0, 0.5, 1.0));
		v->addMembershipFunction("fast",        new Math::RampAsc(1.0, 2.0));
		fStage2Slow.addVariable(v);
		v = new FVariable("relSpeed", -4.0, 4.0);
		v->addMembershipFunction("away",        new Math::RampDesc(-0.1, 0.0));
		v->addMembershipFunction("slow",        new Math::Trapezoidal(-0.1, 0.0, 0.5, 1.0));
		v->addMembershipFunction("fast",        new Math::RampAsc(1.0, 2.0));
		fStage2Jogging.addVariable(v);
		v = new FVariable("relSpeed", -4.0, 4.0);
		v->addMembershipFunction("away",        new Math::RampDesc(-0.1, 0.0));
		v->addMembershipFunction("slow",        new Math::Trapezoidal(-0.1, 0.0, 0.5, 1.0));
		v->addMembershipFunction("fast",        new Math::RampAsc(1.0, 2.0));
		fStage2Sprint.addVariable(v);

		//STAGE 3
		v = new FVariable("direction", -90.0, 90.0);
		v->addMembershipFunction("farLeft",    	new Math::RampDesc(-50.0, -25.0));
		v->addMembershipFunction("left",        new Math::Trapezoidal(-50.0, -25.0, -25.0, 0.0));
		v->addMembershipFunction("center",      new Math::Trapezoidal(-25.0, 0.0, 0.0, 25.0));
		v->addMembershipFunction("right",       new Math::Trapezoidal(0.0, 25.0, 25.0, 50.0));
		v->addMembershipFunction("farRight",  	new Math::RampAsc(25.0, 50.0));
		fStage3Stay.addVariable(v);
		v = new FVariable("direction", -90.0, 90.0);
		v->addMembershipFunction("farLeft",    	new Math::RampDesc(-50.0, -25.0));
		v->addMembershipFunction("left",        new Math::Trapezoidal(-50.0, -25.0, -25.0, 0.0));
		v->addMembershipFunction("center",      new Math::Trapezoidal(-25.0, 0.0, 0.0, 25.0));
		v->addMembershipFunction("right",       new Math::Trapezoidal(0.0, 25.0, 25.0, 50.0));
		v->addMembershipFunction("farRight",  	new Math::RampAsc(25.0, 50.0));
		fStage3Turn.addVariable(v);
		v = new FVariable("direction", -90.0, 90.0);
		v->addMembershipFunction("farLeft",    	new Math::RampDesc(-50.0, -25.0));
		v->addMembershipFunction("left",        new Math::Trapezoidal(-50.0, -25.0, -25.0, 0.0));
		v->addMembershipFunction("center",      new Math::Trapezoidal(-25.0, 0.0, 0.0, 25.0));
		v->addMembershipFunction("right",       new Math::Trapezoidal(0.0, 25.0, 25.0, 50.0));
		v->addMembershipFunction("farRight",  	new Math::RampAsc(25.0, 50.0));
		fStage3Strafe.addVariable(v);

		v = new FVariable("angSpeed", -180.0, 180.0);
		v->addMembershipFunction("toLeft",     	new Math::RampDesc(-2.0, 0.0));
		v->addMembershipFunction("static",      new Math::Trapezoidal(-2.0, 0.0, 0.0, 2.0));
		v->addMembershipFunction("toRight",    	new Math::RampAsc(0.0, 2.0));
		fStage3Stay.addVariable(v);
		v = new FVariable("angSpeed", -180.0, 180.0);
		v->addMembershipFunction("toLeft",     	new Math::RampDesc(-2.0, 0.0));
		v->addMembershipFunction("static",      new Math::Trapezoidal(-2.0, 0.0, 0.0, 2.0));
		v->addMembershipFunction("toRight",    	new Math::RampAsc(0.0, 2.0));
		fStage3Turn.addVariable(v);
		v = new FVariable("angSpeed", -180.0, 180.0);
		v->addMembershipFunction("toLeft",     	new Math::RampDesc(-2.0, 0.0));
		v->addMembershipFunction("static",      new Math::Trapezoidal(-2.0, 0.0, 0.0, 2.0));
		v->addMembershipFunction("toRight",    	new Math::RampAsc(0.0, 2.0));
		fStage3Strafe.addVariable(v);

		//OUTPUTS
		v = new FVariable("dash", 0.0, 200.0);
		v->addMembershipFunction("walk",        new Math::RampDesc(10.0, 15.0));
		v->addMembershipFunction("jogging",     new Math::Trapezoidal(10.0, 15.0, 15.0, 20.0));
		v->addMembershipFunction("sprint",      new Math::RampAsc(90.0, 100.0));
		fStage1.addVariable(v);

		v = new FVariable("action", -1.0, 3.0); //NO INCLUIR CON AG
		v->addMembershipFunction("stay",        new Math::Triangular(-1.0, 0.0, 1.0));
		v->addMembershipFunction("turn",        new Math::Triangular(0.0, 1.0, 2.0));
		v->addMembershipFunction("dodge",      	new Math::Triangular(1.0, 2.0, 3.0));
		fStage2Slow.addVariable(v);
		v = new FVariable("action", -1.0, 3.0); //NO INCLUIR CON AG
		v->addMembershipFunction("stay",        new Math::Triangular(-1.0, 0.0, 1.0));
		v->addMembershipFunction("turn",        new Math::Triangular(0.0, 1.0, 2.0));
		v->addMembershipFunction("dodge",      	new Math::Triangular(1.0, 2.0, 3.0));
		fStage2Jogging.addVariable(v);
		v = new FVariable("action", -1.0, 3.0); //NO INCLUIR CON AG
		v->addMembershipFunction("stay",        new Math::Triangular(-1.0, 0.0, 1.0));
		v->addMembershipFunction("turn",        new Math::Triangular(0.0, 1.0, 2.0));
		v->addMembershipFunction("dodge",      	new Math::Triangular(1.0, 2.0, 3.0));
		fStage2Sprint.addVariable(v);

		v = new FVariable("turn", -90.0, 90.0);
		v->addMembershipFunction("hLeft",       new Math::RampDesc(-90.0, -30.0));
		v->addMembershipFunction("left",        new Math::Trapezoidal(-60.0, -10.0, -10.0, 0.0));
		v->addMembershipFunction("straight",    new Math::Trapezoidal(-10.0, 0.0, 0.0, 10.0));
		v->addMembershipFunction("right",       new Math::Trapezoidal(0.0, 10.0, 10.0, 60.0));
		v->addMembershipFunction("hRight",  	new Math::RampAsc(30.0, 90.0));
		fStage3Stay.addVariable(v);
		v = new FVariable("turn", -90.0, 90.0);
		v->addMembershipFunction("hLeft",       new Math::RampDesc(-90.0, -30.0));
		v->addMembershipFunction("left",        new Math::Trapezoidal(-60.0, -10.0, -10.0, 0.0));
		v->addMembershipFunction("straight",    new Math::Trapezoidal(-10.0, 0.0, 0.0, 10.0));
		v->addMembershipFunction("right",       new Math::Trapezoidal(0.0, 10.0, 10.0, 60.0));
		v->addMembershipFunction("hRight",  	new Math::RampAsc(30.0, 90.0));
		fStage3Turn.addVariable(v);
		v = new FVariable("turn", -90.0, 90.0);
		v->addMembershipFunction("hLeft",       new Math::RampDesc(-90.0, -30.0));
		v->addMembershipFunction("left",        new Math::Trapezoidal(-60.0, -10.0, -10.0, 0.0));
		v->addMembershipFunction("straight",    new Math::Trapezoidal(-10.0, 0.0, 0.0, 10.0));
		v->addMembershipFunction("right",       new Math::Trapezoidal(0.0, 10.0, 10.0, 60.0));
		v->addMembershipFunction("hRight",  	new Math::RampAsc(30.0, 90.0));
		fStage3Strafe.addVariable(v);

		v = new FVariable("strafe", -90.0, 90.0);
		v->addMembershipFunction("aLeft",  	 	new Math::RampDesc(-90.0, -45.0));
		v->addMembershipFunction("dLeft",  		new Math::Trapezoidal(-90.0, -45.0, -45.0, 0.0));
		v->addMembershipFunction("face",        new Math::Trapezoidal(-45.0, 0.0, 0.0, 45.0));
		v->addMembershipFunction("dRight", 		new Math::Trapezoidal(0.0, 45.0, 45.0, 90.0));
		v->addMembershipFunction("aRight", 		new Math::RampAsc(45.0, 90.0));
		fStage3Stay.addVariable(v);
		v = new FVariable("strafe", -90.0, 90.0);
		v->addMembershipFunction("aLeft",  	 	new Math::RampDesc(-90.0, -45.0));
		v->addMembershipFunction("dLeft",  		new Math::Trapezoidal(-90.0, -45.0, -45.0, 0.0));
		v->addMembershipFunction("face",        new Math::Trapezoidal(-45.0, 0.0, 0.0, 45.0));
		v->addMembershipFunction("dRight", 		new Math::Trapezoidal(0.0, 45.0, 45.0, 90.0));
		v->addMembershipFunction("aRight", 		new Math::RampAsc(45.0, 90.0));
		fStage3Turn.addVariable(v);
		v = new FVariable("strafe", -90.0, 90.0);
		v->addMembershipFunction("aLeft",  	 	new Math::RampDesc(-90.0, -45.0));
		v->addMembershipFunction("dLeft",  		new Math::Trapezoidal(-90.0, -45.0, -45.0, 0.0));
		v->addMembershipFunction("face",        new Math::Trapezoidal(-45.0, 0.0, 0.0, 45.0));
		v->addMembershipFunction("dRight", 		new Math::Trapezoidal(0.0, 45.0, 45.0, 90.0));
		v->addMembershipFunction("aRight", 		new Math::RampAsc(45.0, 90.0));
		fStage3Strafe.addVariable(v);

		//DA RULES
		//STAGE 1
		fStage1.addRule(FRule("effort = tired   	  & stamina = critical -> dash = walk"));
		fStage1.addRule(FRule("effort = tired   	  & stamina = normal   -> dash = walk"));
		fStage1.addRule(FRule("effort = tired   	  & stamina = high     -> dash = walk"));
		fStage1.addRule(FRule("effort = fresh   	  & stamina = critical -> dash = jogging"));
		fStage1.addRule(FRule("effort = fresh   	  & stamina = normal   -> dash = sprint"));
		fStage1.addRule(FRule("effort = fresh   	  & stamina = high     -> dash = sprint"));

		//STAGE 2
		//Stage 1 = slow
		fStage2Slow.addRule(FRule("distance = vClose	& relSpeed = away    	-> action =  stay"));
		fStage2Slow.addRule(FRule("distance = vClose   	& relSpeed = slow    	-> action =  dodge"));
		fStage2Slow.addRule(FRule("distance = vClose 	& relSpeed = fast    	-> action =  dodge"));
		fStage2Slow.addRule(FRule("distance = close  	& relSpeed = away    	-> action =  stay"));
		fStage2Slow.addRule(FRule("distance = close  	& relSpeed = slow    	-> action =  turn"));
		fStage2Slow.addRule(FRule("distance = close  	& relSpeed = fast    	-> action =  turn"));
		fStage2Slow.addRule(FRule("distance = far   	& relSpeed = away    	-> action =  stay"));
		fStage2Slow.addRule(FRule("distance = far   	& relSpeed = slow    	-> action =  stay"));
		fStage2Slow.addRule(FRule("distance = far   	& relSpeed = fast    	-> action =  stay"));

		//Stage 1 = Jogging
		fStage2Jogging.addRule(FRule("distance = vClose  	& relSpeed = away    	-> action =  stay"));
		fStage2Jogging.addRule(FRule("distance = vClose   	& relSpeed = slow    	-> action =  dodge"));
		fStage2Jogging.addRule(FRule("distance = vClose   	& relSpeed = fast    	-> action =  dodge"));
		fStage2Jogging.addRule(FRule("distance = close   	& relSpeed = away   	-> action =  stay"));
		fStage2Jogging.addRule(FRule("distance = close   	& relSpeed = slow   	-> action =  turn"));
		fStage2Jogging.addRule(FRule("distance = close   	& relSpeed = fast   	-> action =  turn"));
		fStage2Jogging.addRule(FRule("distance = far   		& relSpeed = away   	-> action =  stay"));
		fStage2Jogging.addRule(FRule("distance = far  	 	& relSpeed = slow   	-> action =  stay"));
		fStage2Jogging.addRule(FRule("distance = far   		& relSpeed = fast   	-> action =  stay"));

		//Stage 1 = Sprint
		fStage2Sprint.addRule(FRule("distance = vClose   	& relSpeed = away		-> action =  stay"));
		fStage2Sprint.addRule(FRule("distance = vClose   	& relSpeed = slow   	-> action =  dodge"));
		fStage2Sprint.addRule(FRule("distance = vClose   	& relSpeed = fast   	-> action =  dodge"));
		fStage2Sprint.addRule(FRule("distance = close   	& relSpeed = away   	-> action =  stay"));
		fStage2Sprint.addRule(FRule("distance = close   	& relSpeed = slow   	-> action =  turn"));
		fStage2Sprint.addRule(FRule("distance = close   	& relSpeed = fast   	-> action =  turn"));
		fStage2Sprint.addRule(FRule("distance = far   		& relSpeed = away   	-> action =  stay"));
		fStage2Sprint.addRule(FRule("distance = far   		& relSpeed = slow   	-> action =  stay"));
		fStage2Sprint.addRule(FRule("distance = far   		& relSpeed = fast   	-> action =  stay"));

		//Stage 2 = Stay
		fStage3Stay.addRule(FRule("direction = farLeft	& angSpeed = toLeft		-> turn = straight"));
		fStage3Stay.addRule(FRule("direction = farLeft	& angSpeed = static		-> turn = straight"));
		fStage3Stay.addRule(FRule("direction = farLeft	& angSpeed = toRight	-> turn = straight"));
		fStage3Stay.addRule(FRule("direction = left		& angSpeed = toLeft		-> turn = straight"));
		fStage3Stay.addRule(FRule("direction = left		& angSpeed = static		-> turn = straight"));
		fStage3Stay.addRule(FRule("direction = left		& angSpeed = toRight	-> turn = straight"));
		fStage3Stay.addRule(FRule("direction = center	& angSpeed = toLeft		-> turn = straight"));
		fStage3Stay.addRule(FRule("direction = center	& angSpeed = static		-> turn = straight"));
		fStage3Stay.addRule(FRule("direction = center	& angSpeed = toRight	-> turn = straight"));
		fStage3Stay.addRule(FRule("direction = right	& angSpeed = toLeft		-> turn = straight"));
		fStage3Stay.addRule(FRule("direction = right	& angSpeed = static		-> turn = straight"));
		fStage3Stay.addRule(FRule("direction = right	& angSpeed = toRight	-> turn = straight"));
		fStage3Stay.addRule(FRule("direction = farRight	& angSpeed = toLeft		-> turn = straight"));
		fStage3Stay.addRule(FRule("direction = farRight	& angSpeed = static		-> turn = straight"));
		fStage3Stay.addRule(FRule("direction = farRight	& angSpeed = toRight	-> turn = straight"));

		fStage3Stay.addRule(FRule("direction = farLeft	& angSpeed = toLeft		-> strafe = face"));
		fStage3Stay.addRule(FRule("direction = farLeft	& angSpeed = static		-> strafe = face"));
		fStage3Stay.addRule(FRule("direction = farLeft	& angSpeed = toRight	-> strafe = face"));
		fStage3Stay.addRule(FRule("direction = left		& angSpeed = toLeft		-> strafe = face"));
		fStage3Stay.addRule(FRule("direction = left		& angSpeed = static		-> strafe = face"));
		fStage3Stay.addRule(FRule("direction = left		& angSpeed = toRight	-> strafe = face"));
		fStage3Stay.addRule(FRule("direction = center	& angSpeed = toLeft		-> strafe = face"));
		fStage3Stay.addRule(FRule("direction = center	& angSpeed = static		-> strafe = face"));
		fStage3Stay.addRule(FRule("direction = center	& angSpeed = toRight	-> strafe = face"));
		fStage3Stay.addRule(FRule("direction = right	& angSpeed = toLeft		-> strafe = face"));
		fStage3Stay.addRule(FRule("direction = right	& angSpeed = static		-> strafe = face"));
		fStage3Stay.addRule(FRule("direction = right	& angSpeed = toRight	-> strafe = face"));
		fStage3Stay.addRule(FRule("direction = farRight	& angSpeed = toLeft		-> strafe = face"));
		fStage3Stay.addRule(FRule("direction = farRight	& angSpeed = static		-> strafe = face"));
		fStage3Stay.addRule(FRule("direction = farRight	& angSpeed = toRight	-> strafe = face"));

		//Stage 2 = Turn
		fStage3Turn.addRule(FRule("direction = farLeft	& angSpeed = toLeft		-> turn = straight"));
		fStage3Turn.addRule(FRule("direction = farLeft	& angSpeed = static		-> turn = straight"));
		fStage3Turn.addRule(FRule("direction = farLeft	& angSpeed = toRight	-> turn = straight"));
		fStage3Turn.addRule(FRule("direction = left		& angSpeed = toLeft		-> turn = straight"));
		fStage3Turn.addRule(FRule("direction = left		& angSpeed = static		-> turn = right"));
		fStage3Turn.addRule(FRule("direction = left		& angSpeed = toRight	-> turn = hLeft"));
		fStage3Turn.addRule(FRule("direction = center	& angSpeed = toLeft		-> turn = right"));
		if(rightFlank){// Go Right Preference
			fStage3Turn.addRule(FRule("direction = center	& angSpeed = static		-> turn = right"));
		} else { // Go Left Preference
			fStage3Turn.addRule(FRule("direction = center	& angSpeed = static		-> turn = left"));
		}
		fStage3Turn.addRule(FRule("direction = center	& angSpeed = toRight	-> turn = left"));
		fStage3Turn.addRule(FRule("direction = right	& angSpeed = toLeft		-> turn = hRight"));
		fStage3Turn.addRule(FRule("direction = right	& angSpeed = static		-> turn = left"));
		fStage3Turn.addRule(FRule("direction = right	& angSpeed = toRight	-> turn = straight"));
		fStage3Turn.addRule(FRule("direction = farRight	& angSpeed = toLeft		-> turn = straight"));
		fStage3Turn.addRule(FRule("direction = farRight	& angSpeed = static		-> turn = straight"));
		fStage3Turn.addRule(FRule("direction = farRight	& angSpeed = toRight	-> turn = straight"));

		fStage3Turn.addRule(FRule("direction = farLeft	& angSpeed = toLeft		-> strafe = face"));
		fStage3Turn.addRule(FRule("direction = farLeft	& angSpeed = static		-> strafe = face"));
		fStage3Turn.addRule(FRule("direction = farLeft	& angSpeed = toRight	-> strafe = face"));
		fStage3Turn.addRule(FRule("direction = left		& angSpeed = toLeft		-> strafe = face"));
		fStage3Turn.addRule(FRule("direction = left		& angSpeed = static		-> strafe = face"));
		fStage3Turn.addRule(FRule("direction = left		& angSpeed = toRight	-> strafe = face"));
		fStage3Turn.addRule(FRule("direction = center	& angSpeed = toLeft		-> strafe = face"));
		fStage3Turn.addRule(FRule("direction = center	& angSpeed = static		-> strafe = face"));
		fStage3Turn.addRule(FRule("direction = center	& angSpeed = toRight	-> strafe = face"));
		fStage3Turn.addRule(FRule("direction = right	& angSpeed = toLeft		-> strafe = face"));
		fStage3Turn.addRule(FRule("direction = right	& angSpeed = static		-> strafe = face"));
		fStage3Turn.addRule(FRule("direction = right	& angSpeed = toRight	-> strafe = face"));
		fStage3Turn.addRule(FRule("direction = farRight	& angSpeed = toLeft		-> strafe = face"));
		fStage3Turn.addRule(FRule("direction = farRight	& angSpeed = static		-> strafe = face"));
		fStage3Turn.addRule(FRule("direction = farRight	& angSpeed = toRight	-> strafe = face"));

		//Stage 2 = Strafe
		fStage3Strafe.addRule(FRule("direction = farLeft	& angSpeed = toLeft		-> turn = straight"));
		fStage3Strafe.addRule(FRule("direction = farLeft	& angSpeed = static		-> turn = straight"));
		fStage3Strafe.addRule(FRule("direction = farLeft	& angSpeed = toRight	-> turn = straight"));
		fStage3Strafe.addRule(FRule("direction = left		& angSpeed = toLeft		-> turn = straight"));
		fStage3Strafe.addRule(FRule("direction = left		& angSpeed = static		-> turn = straight"));
		fStage3Strafe.addRule(FRule("direction = left		& angSpeed = toRight	-> turn = straight"));
		fStage3Strafe.addRule(FRule("direction = center		& angSpeed = toLeft		-> turn = straight"));
		fStage3Strafe.addRule(FRule("direction = center		& angSpeed = static		-> turn = straight"));
		fStage3Strafe.addRule(FRule("direction = center		& angSpeed = toRight	-> turn = straight"));
		fStage3Strafe.addRule(FRule("direction = right		& angSpeed = toLeft		-> turn = straight"));
		fStage3Strafe.addRule(FRule("direction = right		& angSpeed = static		-> turn = straight"));
		fStage3Strafe.addRule(FRule("direction = right		& angSpeed = toRight	-> turn = straight"));
		fStage3Strafe.addRule(FRule("direction = farRight	& angSpeed = toLeft		-> turn = straight"));
		fStage3Strafe.addRule(FRule("direction = farRight	& angSpeed = static		-> turn = straight"));
		fStage3Strafe.addRule(FRule("direction = farRight	& angSpeed = toRight	-> turn = straight"));

		fStage3Strafe.addRule(FRule("direction = farLeft	& angSpeed = toLeft		-> strafe = face"));
		fStage3Strafe.addRule(FRule("direction = farLeft	& angSpeed = static		-> strafe = face"));
		fStage3Strafe.addRule(FRule("direction = farLeft	& angSpeed = toRight	-> strafe = face"));
		fStage3Strafe.addRule(FRule("direction = left		& angSpeed = toLeft		-> strafe = face"));
		fStage3Strafe.addRule(FRule("direction = left		& angSpeed = static		-> strafe = dRight"));
		fStage3Strafe.addRule(FRule("direction = left		& angSpeed = toRight	-> strafe = aLeft"));
		fStage3Strafe.addRule(FRule("direction = center		& angSpeed = toLeft		-> strafe = dRight"));
		if(rightFlank){// Go Right Preference
			fStage3Strafe.addRule(FRule("direction = center	& angSpeed = static		-> strafe = dRight"));
		} else { // Go Left Preference
			fStage3Strafe.addRule(FRule("direction = center	& angSpeed = static		-> strafe = dLeft"));
		}
		fStage3Strafe.addRule(FRule("direction = center	& angSpeed = toRight	-> strafe = dLeft"));
		fStage3Strafe.addRule(FRule("direction = right	& angSpeed = toLeft		-> strafe = aRight"));
		fStage3Strafe.addRule(FRule("direction = right	& angSpeed = static		-> strafe = dLeft"));
		fStage3Strafe.addRule(FRule("direction = right	& angSpeed = toRight	-> strafe = face"));
		fStage3Strafe.addRule(FRule("direction = farRight	& angSpeed = toLeft		-> strafe = face"));
		fStage3Strafe.addRule(FRule("direction = farRight	& angSpeed = static		-> strafe = face"));
		fStage3Strafe.addRule(FRule("direction = farRight	& angSpeed = toRight	-> strafe = face"));
}

/*! Obtains the output of the fuzzy
 * obtainOut(Y Location, Effort, Stamina, Distance, Relative Speed, Direction, Angular Speed)
 *
 */
std::vector<double> obtainOut(double yLocation, double effort, double stamina, double distance, double relSpeed, double direction, double angSpeed){
	//Flank Calculation
	double threshold = 15;
	if (std::abs(yLocation) > threshold){
		rightFlank = !rightFlank;
	}



	//AQUI COLOCA PROCESO DE SALIDA DE STAGE 1
	double winner1 = 0.0;
	string theWinner1 = "";
	for (std::vector<FRule>::iterator it = fStage1.begin(); it != fStage1.end(); ++it) {
		it->evaluate(effort, stamina);
		//std::cout << it->getOutputName() << " is " << it->getOutputSet() << " with " << it->getOutput() << std::endl;
		if (it->getOutput() >= winner1){
			winner1 =  it->getOutput();
			theWinner1 = it->getOutputSet();
		}
	}

	std::map<std::string, double> inputs;
	inputs["effort"] = effort;
	inputs["stamina"] = stamina;
	std::map<std::string, double> outputs = fStage1.evaluate(inputs);
	dashPower = outputs["dash"];


	//STAGE 2
	if (!theWinner1.compare("slow")){ //Stage1 = slow
		fStage2 = &fStage2Slow;
	} else if (!theWinner1.compare("jogging")){ //Stage1 = jogging
		fStage2 = &fStage2Jogging;
	} else if (!theWinner1.compare("sprint")){	//Stage1 = sprint
		fStage2 = &fStage2Sprint;
	}
	//AQUI COLOCA EL PROCESO DEL STAGE 2
	double winner2 = 0.0;
	string theWinner2 = "";
	for (std::vector<FRule>::iterator it = fStage2->begin(); it != fStage2->end(); ++it) {
		it->evaluate(distance, relSpeed);
		//std::cout << it->getOutputName() << " is " << it->getOutputSet() << " with " << it->getOutput() << std::endl;
		if (it->getOutput() >= winner1){
			winner2 =  it->getOutput();
			theWinner2 = it->getOutputSet();
		}
	}


	//STAGE 3
	if (!theWinner2.compare("stay")){ //Stage 2 = stay
		fStage3 = &fStage3Stay;
	}else if (!theWinner2.compare("turn")){ //Stage 2 = Turn;
		fStage3 = &fStage3Turn;
	} else if (!theWinner2.compare("dodge")){ //Stage 2 = strafe
		fStage3 = &fStage3Strafe;
	}
	//AQUI COLOCA PROCESO DE SALIDA DE STAGE 3
	std::map<std::string, double> inputs3;
	inputs3["direction"] = direction;
	inputs3["angSpeed"] = angSpeed;
	std::map<std::string, double> outputs3 = fStage3->evaluate(inputs);
	turnAng = outputs3["turn"];
	strafeAng = outputs3["strafe"];



	//Se crea el vector de salida
	std::vector<double> fuzzyOutput;
	fuzzyOutput.push_back(dashPower);
	fuzzyOutput.push_back(turnAng);
	fuzzyOutput.push_back(strafeAng);

	return fuzzyOutput;
}
/*!
 * Changes fuzzy Variables
 * Variable Name, a, b,c, d
 */
void changeMF(string variable, string nameMF, double a, double b, double c, double d){

}
void changeMF(string variable, string nameMF, double a, double b){

}
}

