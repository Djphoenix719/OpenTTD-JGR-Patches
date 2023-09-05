/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <http://www.gnu.org/licenses/>.
 */

#include <memory>
#include "stdafx.h"
#include "command_type.h"
#include <table/strings.h>
#include "blueprint/internal/blueprint_item.hpp"
#include "track_func.h"
#include "settings_type.h"
#include "blueprint/internal/blueprint_position.hpp"
#include "blueprint_item_track.hpp"

namespace blueprint {
    std::unique_ptr<CommandContainer> BlueprintItemTrack::MakeCommand(TileIndex tile_index) {
        auto track = TrackdirToTrack(this->direction);
        return std::make_unique<CommandContainer>(NewCommandContainerBasic(
            AddTileIndexDiffCWrap(tile_index, PositionToDiff(this->offset)), // tile
            this->type, // p1
            track | (_settings_client.gui.auto_remove_signals << 3), // p2
            (uint32) (CMD_BUILD_SINGLE_RAIL | CMD_MSG(STR_ERROR_CAN_T_BUILD_RAILROAD_TRACK)), // cmd
            nullptr // callback
        ));
    }
}