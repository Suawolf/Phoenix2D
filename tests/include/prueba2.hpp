/*
 * prueba2.hpp
 *
 *  Created on: Jul 17, 2014
 *      Author: luiscarlosgonzalezsua
 */

#ifndef PRUEBA2_HPP_
#define PRUEBA2_HPP_

#include <vector>
#include "WorldModel.hpp"
#include "Messages.hpp"
#include "Commands.hpp"

using namespace Phoenix;

namespace Prueba2 {

void onStart();
void executeBeforeKickOff(WorldModel worldModel, std::vector<Message> messages, Commands* commands);
void executePlayOn(WorldModel worldModel, std::vector<Message> messages, Commands* commands);
void onFinish();
}




#endif /* PRUEBA2_HPP_ */
