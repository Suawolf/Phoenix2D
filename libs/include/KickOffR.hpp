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

#ifndef KICKOFFR_H_
#define KICKOFFR_H_

#include "PlayMode.hpp"
#include "WorldModel.hpp"
#include <vector>
#include "Message.hpp"

/*! @addtogroup phoenix_main
 * @{
 */
namespace Phoenix {

class Commands;
/*! @addtogroup PlayModes
 * @{
 */
/*!
 * @brief <STRONG> KickOffR <BR> </STRONG>
 * The KickOffR lorem Ipsum
 */
class KickOffR : public PlayMode {
public:
	KickOffR(Commands *commands);
	~KickOffR();
	void onPlayerExecute(WorldModel world, std::vector<Message> messages);
	void onGoalieExecute(WorldModel world, std::vector<Message> messages);
	void onCoachExecute(WorldModel world, std::vector<Message> messages);
};
/*! @} */
} // End namespace Phoenix
/*! @} */

#endif /* KICKOFFR_H_ */