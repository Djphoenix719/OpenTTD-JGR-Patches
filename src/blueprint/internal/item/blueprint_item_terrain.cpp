/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <http://www.gnu.org/licenses/>.
 */

#include "blueprint_item_terrain.hpp"

namespace blueprint {
    std::unique_ptr<CommandContainer> blueprint::BlueprintItemTerrain::MakeCommand(TileIndex tile_index) {
        auto target_tile = AddTileIndexDiffCWrap(
            tile_index, PositionToDiff(-this->offset)
        );
        auto start_tile_height = (int) TileHeight(tile_index);
        auto target_tile_height = (int) TileHeight(target_tile);
        auto current_offset = target_tile_height - start_tile_height;

        if (current_offset != this->height_offset) {
            // DoCommandX(start_tile, SLOPE_N, 0, DC_EXEC, CMD_TERRAFORM_LAND);
            return std::make_unique<CommandContainer>(NewCommandContainerBasic(
                target_tile,
                SLOPE_N,
                (current_offset > this->height_offset) ? 0 : 1,
                CMD_TERRAFORM_LAND
            ));
        } else {
            return nullptr;
        }
    }

    void blueprint::BlueprintItemTerrain::Draw(const TileInfo *tile_info) {
        // no-op
    }

    void blueprint::BlueprintItemTerrain::MarkDirty(TileIndex next_origin) {
        // no-op
    }
}

