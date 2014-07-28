/*
 * prueba1.hpp
 *
 *  Created on: Jul 17, 2014
 *      Author: luiscarlosgonzalezsua
 */

#ifndef PRUEBA1_HPP_
#define PRUEBA1_HPP_

#include <vector>
#include "WorldModel.hpp"
#include "Messages.hpp"
#include "Commands.hpp"

using namespace Phoenix;

namespace Prueba1 {

void onStart();
void executeBeforeKickOff(WorldModel worldModel, std::vector<Message> messages, Commands* commands);
void executePlayOn(WorldModel worldModel, std::vector<Message> messages, Commands* commands);
void onFinish();
}



#endif /* PRUEBA1_HPP_ */
