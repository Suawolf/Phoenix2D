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

#include "FreeKickR.hpp"
#include "Commands.hpp"

namespace Phoenix {

FreeKickR::FreeKickR(Commands *commands) : PlayMode(commands) {

}

FreeKickR::~FreeKickR() {

}

void FreeKickR::setup(WorldModel world) {

}

void FreeKickR::onPlayerExecute(WorldModel world) {

}

void FreeKickR::onGoalieExecute(WorldModel world) {

}

void FreeKickR::onCoachExecute(WorldModel world) {

}

void FreeKickR::onMessageReceived(std::string message, int sender) {

}

}