/*
 * prueba3.hpp
 *
 *  Created on: Jul 6, 2014
 *      Author: luiscarlosgonzalezsua
 */

#ifndef PRUEBA3_HPP_
#define PRUEBA3_HPP_

#include <vector>
#include "WorldModel.hpp"
#include "Messages.hpp"
#include "Commands.hpp"

using namespace Phoenix;

namespace Prueba3 {

void onStart();
void executeBeforeKickOff(WorldModel worldModel, std::vector<Message> messages, Commands* commands);
void executePlayOn(WorldModel worldModel, std::vector<Message> messages, Commands* commands);
void onFinish();


}



#endif /* PRUEBA3_HPP_ */
