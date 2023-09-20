/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <http://www.gnu.org/licenses/>.
 */
#include "blueprint_item_tunnel.hpp"
#include <memory>
#include "stdafx.h"
#include "command_type.h"
#include <table/strings.h>
#include "blueprint/internal/blueprint_item.hpp"
#include "track_func.h"
#include "settings_type.h"
#include "blueprint/internal/blueprint_position.hpp"
#include "rail_map.h"
#include "table/sprites.h"
#include "viewport_func.h"
#include "rail.h"
#include "tilehighlight_type.h"
#include "table/autorail.h"
#include "spritecache.h"
#include "command_func.h"

namespace blueprint {
    std::unique_ptr<CommandContainer> blueprint::BlueprintTunnelItem::MakeCommand(TileIndex tile_index) {
        return std::make_unique<CommandContainer>(NewCommandContainerBasic(
            AddTileIndexDiffCWrap(tile_index, PositionToDiff(-this->offset)),
            this->rail_type | (TRANSPORT_RAIL << 8),
            0,
            CMD_BUILD_TUNNEL | CMD_MSG(STR_ERROR_CAN_T_BUILD_TUNNEL_HERE),
            CcPlaySound_CONSTRUCTION_RAIL
        ));
    }

    void blueprint::BlueprintTunnelItem::Draw(const TileInfo *tile_info) {
        // no-op
    }

    void blueprint::BlueprintTunnelItem::MarkDirty(TileIndex next_origin) {
        // no-op
    }
}

