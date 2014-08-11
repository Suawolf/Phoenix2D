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
	v = new FVariable("effort", -1.0, 1.0);
	v->addMembershipFunction("tired",       new Math::RampDesc(0.766723633, 0.986450195));
	v->addMembershipFunction("fresh",       new Math::RampAsc(0.82421875, 0.897705078));
	fStage1.addVariable(v);

	v = new FVariable("stamina", 0.0, 8000.0);
	v->addMembershipFunction("critical",    new Math::RampDesc(1806.640625, 1808.59375));
	v->addMembershipFunction("normal",      new Math::Trapezoidal(1886.71875, 1888.671875, 4718.75, 4944.335938));
	v->addMembershipFunction("high",        new Math::RampAsc(5335.9375, 5387.695313));
	fStage1.addVariable(v);

	//STAGE 2
	v = new FVariable("distance", 0.0, 150.0);
	v->addMembershipFunction("vClose",  	new Math::RampDesc(0, 0.036621094));
	v->addMembershipFunction("close",       new Math::Trapezoidal(0, 0.036621094, 0.036621094, 0.091552734));
	v->addMembershipFunction("far",         new Math::RampAsc(0, 0.036621094));
	fStage2Slow.addVariable(v);
	v = new FVariable("distance", 0.0, 150.0);
	v->addMembershipFunction("vClose",  	new Math::RampDesc(0, 0.036621094));
	v->addMembershipFunction("close",       new Math::Trapezoidal(0, 0.036621094, 0.036621094, 0.091552734));
	v->addMembershipFunction("far",         new Math::RampAsc(0, 0.036621094));
	fStage2Jogging.addVariable(v);
	v = new FVariable("distance", 0.0, 150.0);
	v->addMembershipFunction("vClose",  	new Math::RampDesc(0, 0.036621094));
	v->addMembershipFunction("close",       new Math::Trapezoidal(0, 0.036621094, 0.036621094, 0.091552734));
	v->addMembershipFunction("far",         new Math::RampAsc(0, 0.036621094));
	fStage2Sprint.addVariable(v);

	v = new FVariable("relSpeed", -4.0, 4.0);
	v->addMembershipFunction("away",        new Math::RampDesc(0.05078125, 0.052734375));
	v->addMembershipFunction("slow",        new Math::Trapezoidal(-0.327148438, -0.142578125, 0.108398438, 0.213867188));
	v->addMembershipFunction("fast",        new Math::RampAsc(1.182617188, 1.666015625));
	fStage2Slow.addVariable(v);
	v = new FVariable("relSpeed", -4.0, 4.0);
	v->addMembershipFunction("away",        new Math::RampDesc(0.05078125, 0.052734375));
	v->addMembershipFunction("slow",        new Math::Trapezoidal(-0.327148438, -0.142578125, 0.108398438, 0.213867188));
	v->addMembershipFunction("fast",        new Math::RampAsc(1.182617188, 1.666015625));
	fStage2Jogging.addVariable(v);
	v = new FVariable("relSpeed", -4.0, 4.0);
	v->addMembershipFunction("away",        new Math::RampDesc(0.05078125, 0.052734375));
	v->addMembershipFunction("slow",        new Math::Trapezoidal(-0.327148438, -0.142578125, 0.108398438, 0.213867188));
	v->addMembershipFunction("fast",        new Math::RampAsc(1.182617188, 1.666015625));
	fStage2Sprint.addVariable(v);

	//STAGE 3
	v = new FVariable("direction", -90.0, 90.0);
	v->addMembershipFunction("farLeft",    	new Math::RampDesc(-53.26171875, -43.65966797));
	v->addMembershipFunction("left",        new Math::Trapezoidal(-64.24804688, -37.02392578, -33.70605469, -19.75341797));
	v->addMembershipFunction("center",      new Math::Trapezoidal(-31.37695313, -11.66748047, -11.66748047, 8.986816406));
	v->addMembershipFunction("right",       new Math::Trapezoidal(-0.87890625, 15.40283203, 15.40283203, 36.67236328));
	v->addMembershipFunction("farRight",  	new Math::RampAsc(10.63476563, 32.56347656));
	fStage3Stay.addVariable(v);
	v = new FVariable("direction", -90.0, 90.0);
	v->addMembershipFunction("farLeft",    	new Math::RampDesc(-53.26171875, -43.65966797));
	v->addMembershipFunction("left",        new Math::Trapezoidal(-64.24804688, -37.02392578, -33.70605469, -19.75341797));
	v->addMembershipFunction("center",      new Math::Trapezoidal(-31.37695313, -11.66748047, -11.66748047, 8.986816406));
	v->addMembershipFunction("right",       new Math::Trapezoidal(-0.87890625, 15.40283203, 15.40283203, 36.67236328));
	v->addMembershipFunction("farRight",  	new Math::RampAsc(10.63476563, 32.56347656));
	fStage3Turn.addVariable(v);
	v = new FVariable("direction", -90.0, 90.0);
	v->addMembershipFunction("farLeft",    	new Math::RampDesc(-53.26171875, -43.65966797));
	v->addMembershipFunction("left",        new Math::Trapezoidal(-64.24804688, -37.02392578, -33.70605469, -19.75341797));
	v->addMembershipFunction("center",      new Math::Trapezoidal(-31.37695313, -11.66748047, -11.66748047, 8.986816406));
	v->addMembershipFunction("right",       new Math::Trapezoidal(-0.87890625, 15.40283203, 15.40283203, 36.67236328));
	v->addMembershipFunction("farRight",  	new Math::RampAsc(10.63476563, 32.56347656));
	fStage3Strafe.addVariable(v);

	v = new FVariable("angSpeed", -180.0, 180.0);
	v->addMembershipFunction("toLeft",     	new Math::RampDesc(-15.73242188, -15.64453125));
	v->addMembershipFunction("static",      new Math::Trapezoidal(-11.11816406, -7.514648438, -7.514648438, -7.3828125));
	v->addMembershipFunction("toRight",    	new Math::RampAsc(-17.35839844, -11.77734375));
	fStage3Stay.addVariable(v);
	v = new FVariable("angSpeed", -180.0, 180.0);
	v->addMembershipFunction("toLeft",     	new Math::RampDesc(-15.73242188, -15.64453125));
	v->addMembershipFunction("static",      new Math::Trapezoidal(-11.11816406, -7.514648438, -7.514648438, -7.3828125));
	v->addMembershipFunction("toRight",    	new Math::RampAsc(-17.35839844, -11.77734375));
	fStage3Turn.addVariable(v);
	v = new FVariable("angSpeed", -180.0, 180.0);
	v->addMembershipFunction("toLeft",     	new Math::RampDesc(-15.73242188, -15.64453125));
	v->addMembershipFunction("static",      new Math::Trapezoidal(-11.11816406, -7.514648438, -7.514648438, -7.3828125));
	v->addMembershipFunction("toRight",    	new Math::RampAsc(-17.35839844, -11.77734375));
	fStage3Strafe.addVariable(v);

	//OUTPUTS
	v = new FVariable("dash", 0.0, 110.0);
	v->addMembershipFunction("walk",        new Math::RampDesc(9.790039063, 19.42138672));
	v->addMembershipFunction("jogging",     new Math::Trapezoidal(0, 10.08300781, 10.08300781, 22.43652344));
	v->addMembershipFunction("sprint",      new Math::RampAsc(83.203125, 94.921875));
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
	v->addMembershipFunction("hLeft",       new Math::RampDesc(-89.82421875, -52.36083984));
	v->addMembershipFunction("left",        new Math::Trapezoidal(-74.53125, -42.29736328, -42.29736328, -23.33496094));
	v->addMembershipFunction("straight",    new Math::Trapezoidal(-8.217773438, -7.580566406, -7.580566406, -2.570800781));
	v->addMembershipFunction("right",       new Math::Trapezoidal(-1.691894531, 9.624023438, 12.74414063, 58.66699219));
	v->addMembershipFunction("hRight",  	new Math::RampAsc(33.70605469, 82.63916016));
	fStage3Stay.addVariable(v);
	v = new FVariable("turn", -90.0, 90.0);
	v->addMembershipFunction("hLeft",       new Math::RampDesc(-89.82421875, -52.36083984));
	v->addMembershipFunction("left",        new Math::Trapezoidal(-74.53125, -42.29736328, -42.29736328, -23.33496094));
	v->addMembershipFunction("straight",    new Math::Trapezoidal(-8.217773438, -7.580566406, -7.580566406, -2.570800781));
	v->addMembershipFunction("right",       new Math::Trapezoidal(-1.691894531, 9.624023438, 12.74414063, 58.66699219));
	v->addMembershipFunction("hRight",  	new Math::RampAsc(33.70605469, 82.63916016));
	fStage3Turn.addVariable(v);
	v = new FVariable("turn", -90.0, 90.0);
	v->addMembershipFunction("hLeft",       new Math::RampDesc(-89.82421875, -52.36083984));
	v->addMembershipFunction("left",        new Math::Trapezoidal(-74.53125, -42.29736328, -42.29736328, -23.33496094));
	v->addMembershipFunction("straight",    new Math::Trapezoidal(-8.217773438, -7.580566406, -7.580566406, -2.570800781));
	v->addMembershipFunction("right",       new Math::Trapezoidal(-1.691894531, 9.624023438, 12.74414063, 58.66699219));
	v->addMembershipFunction("hRight",  	new Math::RampAsc(33.70605469, 82.63916016));
	fStage3Strafe.addVariable(v);

	v = new FVariable("strafe", -90.0, 90.0);
	v->addMembershipFunction("aLeft",  	 	new Math::RampDesc(-90.0, -52.44873047));
	v->addMembershipFunction("dLeft",  		new Math::Trapezoidal(-90.0, -59.98535156, -59.98535156, -15.77636719));
	v->addMembershipFunction("face",        new Math::Trapezoidal(-47.02148438, -9.184570313, -7.536621094, 21.04980469));
	v->addMembershipFunction("dRight", 		new Math::Trapezoidal(-3.8671875, 26.30126953, 30.03662109, 67.71972656));
	v->addMembershipFunction("aRight", 		new Math::RampAsc(28.56445313, 63.65478516));
	fStage3Stay.addVariable(v);
	v = new FVariable("strafe", -90.0, 90.0);
	v->addMembershipFunction("aLeft",  	 	new Math::RampDesc(-90.0, -52.44873047));
	v->addMembershipFunction("dLeft",  		new Math::Trapezoidal(-90.0, -59.98535156, -59.98535156, -15.77636719));
	v->addMembershipFunction("face",        new Math::Trapezoidal(-47.02148438, -9.184570313, -7.536621094, 21.04980469));
	v->addMembershipFunction("dRight", 		new Math::Trapezoidal(-3.8671875, 26.30126953, 30.03662109, 67.71972656));
	v->addMembershipFunction("aRight", 		new Math::RampAsc(28.56445313, 63.65478516));
	fStage3Turn.addVariable(v);
	v = new FVariable("strafe", -90.0, 90.0);
	v->addMembershipFunction("aLeft",  	 	new Math::RampDesc(-90.0, -52.44873047));
	v->addMembershipFunction("dLeft",  		new Math::Trapezoidal(-90.0, -59.98535156, -59.98535156, -15.77636719));
	v->addMembershipFunction("face",        new Math::Trapezoidal(-47.02148438, -9.184570313, -7.536621094, 21.04980469));
	v->addMembershipFunction("dRight", 		new Math::Trapezoidal(-3.8671875, 26.30126953, 30.03662109, 67.71972656));
	v->addMembershipFunction("aRight", 		new Math::RampAsc(28.56445313, 63.65478516));
	fStage3Strafe.addVariable(v);

	//DA RULES
	//STAGE 1
	fStage1.addRule(FRule("effort = tired   	  & stamina = critical -> dash = walk"));
	fStage1.addRule(FRule("effort = tired   	  & stamina = normal   -> dash = sprint"));
	fStage1.addRule(FRule("effort = tired   	  & stamina = high     -> dash = sprint"));
	fStage1.addRule(FRule("effort = fresh   	  & stamina = critical -> dash = walk"));
	fStage1.addRule(FRule("effort = fresh   	  & stamina = normal   -> dash = walk"));
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
	fStage3Turn.addRule(FRule("direction = left		& angSpeed = toRight	-> turn = hRight"));
	fStage3Turn.addRule(FRule("direction = center	& angSpeed = toLeft		-> turn = right"));
	if(rightFlank){// Go Right Preference
		fStage3Turn.addRule(FRule("direction = center	& angSpeed = static		-> turn = right"));
	} else { // Go Left Preference
		fStage3Turn.addRule(FRule("direction = center	& angSpeed = static		-> turn = left"));
	}
	fStage3Turn.addRule(FRule("direction = center	& angSpeed = toRight	-> turn = left"));
	fStage3Turn.addRule(FRule("direction = right	& angSpeed = toLeft		-> turn = hLeft"));
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
	fStage3Strafe.addRule(FRule("direction = left		& angSpeed = toRight	-> strafe = aRight"));
	fStage3Strafe.addRule(FRule("direction = center		& angSpeed = toLeft		-> strafe = dRight"));
	if(rightFlank){// Go Right Preference
		fStage3Strafe.addRule(FRule("direction = center	& angSpeed = static		-> strafe = dRight"));
	} else { // Go Left Preference
		fStage3Strafe.addRule(FRule("direction = center	& angSpeed = static		-> strafe = dLeft"));
	}
	fStage3Strafe.addRule(FRule("direction = center	& angSpeed = toRight	-> strafe = dLeft"));
	fStage3Strafe.addRule(FRule("direction = right	& angSpeed = toLeft		-> strafe = aLeft"));
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
	string theWinner1 = "sprint";
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
	if (!theWinner1.compare("walk")){ //Stage1 = slow
		fStage2 = &fStage2Slow;
	} else if (!theWinner1.compare("jogging")){ //Stage1 = jogging
		fStage2 = &fStage2Jogging;
	} else if (!theWinner1.compare("sprint")){	//Stage1 = sprint
		fStage2 = &fStage2Sprint;
	}
	//AQUI COLOCA EL PROCESO DEL STAGE 2
	double winner2 = 0.0;
	string theWinner2 = "turn";
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
	std::map<std::string, double> outputs3 = fStage3->evaluate(inputs3);
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
 * Stage Variable Name, a, b,c, d
 * fStage1, fStage2Slow, fStage2Jogging, fStage2Sprint, fStage3Stay, fStage3Turn, fStage3Strafe;
 */
void changeMF(int noStage, string variable, string nameMF, double a, double b, double c, double d){
	switch (noStage){
	case 1:
		fStage1.getVariable(variable)->addMembershipFunction(nameMF, new Math::Trapezoidal(a, b, c, d));
		break;
	case 2:
		fStage2Slow.getVariable(variable)->addMembershipFunction(nameMF, new Math::Trapezoidal(a, b, c, d));
		fStage2Jogging.getVariable(variable)->addMembershipFunction(nameMF, new Math::Trapezoidal(a, b, c, d));
		fStage2Sprint.getVariable(variable)->addMembershipFunction(nameMF, new Math::Trapezoidal(a, b, c, d));
		break;
	case 3:
		fStage3Stay.getVariable(variable)->addMembershipFunction(nameMF, new Math::Trapezoidal(a, b, c, d));
		fStage3Turn.getVariable(variable)->addMembershipFunction(nameMF, new Math::Trapezoidal(a, b, c, d));
		fStage3Strafe.getVariable(variable)->addMembershipFunction(nameMF, new Math::Trapezoidal(a, b, c, d));
		break;
	default:
		break;

	}

	//al engine le pongo=> engine.getVariable(string variable)->addMembershipFunction(string nameMF,new Math::Trapezoidal(double a, double b, double c, double d));

}

/*!
 * Changes fuzzy Variables
 * Stage Variable Name, a, b,c, d, RampType True for Asc
 * fStage1, fStage2Slow, fStage2Jogging, fStage2Sprint, fStage3Stay, fStage3Turn, fStage3Strafe;
 */
void changeMF(int noStage, string variable, string nameMF, double a, double b, bool rampType){
	if (rampType){
		switch (noStage){
		case 1:
			fStage1.getVariable(variable)->addMembershipFunction(nameMF, new Math::RampAsc(a, b));
			break;
		case 2:
			fStage2Slow.getVariable(variable)->addMembershipFunction(nameMF, new Math::RampAsc(a, b));
			fStage2Jogging.getVariable(variable)->addMembershipFunction(nameMF, new Math::RampAsc(a, b));
			fStage2Sprint.getVariable(variable)->addMembershipFunction(nameMF, new Math::RampAsc(a, b));
			break;
		case 3:
			fStage3Stay.getVariable(variable)->addMembershipFunction(nameMF, new Math::RampAsc(a, b));
			fStage3Turn.getVariable(variable)->addMembershipFunction(nameMF, new Math::RampAsc(a, b));
			fStage3Strafe.getVariable(variable)->addMembershipFunction(nameMF, new Math::RampAsc(a, b));
			break;
		default:
			break;
			//al engine le pongo=> engine.getVariable(string variable)->addMembershipFunction(string nameMF,new Math::RampDesc(double a, double b));
		}
	} else {
		switch (noStage){
		case 1:
			fStage1.getVariable(variable)->addMembershipFunction(nameMF, new Math::RampDesc(a, b));
			break;
		case 2:
			fStage2Slow.getVariable(variable)->addMembershipFunction(nameMF, new Math::RampDesc(a, b));
			fStage2Jogging.getVariable(variable)->addMembershipFunction(nameMF, new Math::RampDesc(a, b));
			fStage2Sprint.getVariable(variable)->addMembershipFunction(nameMF, new Math::RampDesc(a, b));
			break;
		case 3:
			fStage3Stay.getVariable(variable)->addMembershipFunction(nameMF, new Math::RampDesc(a, b));
			fStage3Turn.getVariable(variable)->addMembershipFunction(nameMF, new Math::RampDesc(a, b));
			fStage3Strafe.getVariable(variable)->addMembershipFunction(nameMF, new Math::RampDesc(a, b));
			break;
		default:
			break;
			//al engine le pongo=> engine.getVariable(string variable)->addMembershipFunction(string nameMF,new Math::RampDesc(double a, double b));
		}
	}
}
}

