/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef OPENTTD_BLUEPRINT_STATE_HPP
#define OPENTTD_BLUEPRINT_STATE_HPP

#include <memory>
#include <vector>
#include "stdafx.h"
#include "blueprint_class.hpp"

namespace blueprint {
    std::vector<std::shared_ptr<Blueprint>> _previous_blueprints;
    std::shared_ptr<Blueprint> GetLastBlueprint();
}

#endif //OPENTTD_BLUEPRINT_STATE_HPP
