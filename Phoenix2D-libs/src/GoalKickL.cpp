/*
 * Phoenix2D (RoboCup Soccer Simulation 2D League)
 * Copyright (c) 2013 Ivan Gonzalez
 *
 * This file is part of Phoenix2D.
 *
 * Phoenix2D is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Phoenix2D is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Phoenix2D.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "GoalKickL.h"
#include "Commands.h"
namespace Phoenix
{
GoalKickL::GoalKickL(Commands *commands) : PlayMode(commands) {

}

GoalKickL::~GoalKickL() {

}

void GoalKickL::setup(WorldModel world) {

}

void GoalKickL::onPlayerExecute(WorldModel world) {

}

void GoalKickL::onGoalieExecute(WorldModel world) {

}

void GoalKickL::onCoachExecute(WorldModel world) {

}

void GoalKickL::onMessageReceived(std::string message, int sender) {

}
}