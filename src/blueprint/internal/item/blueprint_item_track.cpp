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
#include "rail_map.h"
#include "table/sprites.h"
#include "viewport_func.h"
#include "rail.h"
#include "tilehighlight_type.h"
#include "table/autorail.h"
#include "spritecache.h"

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

    void BlueprintItemTrack::Draw(const TileInfo *tile_info) {
        auto rti = GetRailTypeInfo(GetRailType(PositionToIndex(this->origin)));

        SpriteID sprite_id;
        switch (this->direction) {
            case TRACKDIR_UPPER_W:
            case TRACKDIR_UPPER_E:
                sprite_id = rti->base_sprites.single_n;
                break;
            case TRACKDIR_RIGHT_S:
            case TRACKDIR_RIGHT_N:
                sprite_id = rti->base_sprites.single_e;
                break;
            case TRACKDIR_LOWER_E:
            case TRACKDIR_LOWER_W:
                sprite_id = rti->base_sprites.single_s;
                break;
            case TRACKDIR_LEFT_S:
            case TRACKDIR_LEFT_N:
                sprite_id = rti->base_sprites.single_w;
                break;
            case TRACKDIR_Y_SE:
            case TRACKDIR_Y_NW:
                sprite_id = rti->base_sprites.single_y;
                break;
            case TRACKDIR_X_SW:
            case TRACKDIR_X_NE:
                sprite_id = rti->base_sprites.single_x;
                break;
            default:
                NOT_REACHED();
        }

        DrawGroundSpriteAt(sprite_id, PAL_NONE, 0, 0, 0);
    }

    void BlueprintItemTrack::MarkDirty(TileIndex next_origin) {
        this->ghost = std::make_unique<BlueprintGhost>();
        auto real_position = IndexToPosition(next_origin) + this->offset;
        auto real_index = PositionToIndex(real_position);
        this->ghost->position = RemapCoords(
            real_position.x * (int) TILE_SIZE,
            real_position.y * (int) TILE_SIZE,
            (int) TileHeight(real_index) * 7
        );
        this->ghost->sprite_id = SPR_AUTORAIL_BASE + _AutorailTilehSprite[0][TrackdirToDirection(this->direction)];
        this->ghost->palette_id = PAL_NONE;
    }
}