/*
 * theFuzzy.hpp
 *
 *  Created on: Aug 1, 2014
 *      Author: luiscarlosgonzalezsua
 */

#ifndef THEFUZZY_HPP_
#define THEFUZZY_HPP_


using namespace std;

namespace fuzzySpace {
map<string,double> obtainOut(double effort, double stamina, double distance, double relSpeed, double direction, double angSpeed);
void changeMF(string variable, string nameMF, double a, double b, double c, double d);
void changeMF(string variable, string nameMF, double a, double b);
}



#endif /* THEFUZZY_HPP_ */
