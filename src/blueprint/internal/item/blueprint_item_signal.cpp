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
#include "../blueprint_item.hpp"
#include "track_func.h"
#include "settings_type.h"
#include "../blueprint_position.hpp"
#include "blueprint_item_signal.hpp"
#include "rail_map.h"
#include "table/sprites.h"
#include "viewport_func.h"
#include "rail.h"
#include "tilehighlight_type.h"
#include "table/autorail.h"
#include "spritecache.h"
#include "command_func.h"

namespace blueprint {
    std::unique_ptr<CommandContainer> blueprint::BlueprintSignalItem::MakeCommand(TileIndex tile_index) {
        uint32 signal_data = 0;
        SB(signal_data, 0, 3, this->track);
        SB(signal_data, 4, 1, this->variant);
        SB(signal_data, 5, 3, this->type);
        SB(signal_data, 15, 2, this->backward);

        return std::make_unique<CommandContainer>(NewCommandContainerBasic(
            AddTileIndexDiffCWrap(tile_index, PositionToDiff(-this->offset)),
            signal_data,
            0,
            CMD_BUILD_SIGNALS | CMD_MSG(STR_ERROR_CAN_T_BUILD_SIGNALS_HERE),
            CcPlaySound_CONSTRUCTION_RAIL
        ));
    }

    void BlueprintSignalItem::Draw(const TileInfo *tile_info) {
        // no-op
    }

    void BlueprintSignalItem::MarkDirty(TileIndex next_origin) {
        // no-op
    }
}
