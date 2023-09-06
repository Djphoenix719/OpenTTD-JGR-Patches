/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef OPENTTD_BLUEPRINT_ITEM_HPP
#define OPENTTD_BLUEPRINT_ITEM_HPP

#include "stdafx.h"
#include "command_type.h"
#include "rail_type.h"
#include "track_type.h"
#include "blueprint_position.hpp"
#include "tile_cmd.h"

namespace blueprint {
    struct BlueprintGhost {
        Point position;
        SpriteID sprite_id;
        SpriteID palette_id;
    };

    class BlueprintItemBase {
    public:
        /**
         * Get the position of the origin of this blueprint item (e.g. where it was copied from).
         */
        [[nodiscard]] inline Position GetOriginPosition() const noexcept { return this->origin; }

        /**
         * Get the offset from the parent blueprint's start_position.
         */
        [[nodiscard]] inline Position GetStartOffset() const noexcept { return this->offset; }

        /**
         * Get a pointer to the ghost data contained within this blueprint item. Do not free this pointer,
         *  it is managed by the blueprint item.
         */
        [[nodiscard]] inline BlueprintGhost *GetGhost() const noexcept { return this->ghost.get(); }

        /**
         * Make a command that will send this single item over the network.
         * @param tile_index Origin point for the command, not including offset. E.g. this is where the
         *  user clicked, not necessarily where this item should be placed.
         */
        virtual std::unique_ptr<CommandContainer> MakeCommand(TileIndex tile_index) = 0;

        /**
         * Draw this item at the given tile.
         * @param tile_info The tile to draw at.
         */
        virtual void Draw(const TileInfo *tile_info) = 0;

        /**
         * Mark the item dirty, recalculating it's sprite immediately.
         */
        virtual void MarkDirty(TileIndex current_origin) = 0;

    protected:
        /**
         * Tile index of the origin of this blueprint item (e.g. where it was copied from).
         */
        Position origin;
        /**
         * Offset from the parent blueprint's start_position.
         */
        Position offset;
        /**
         * Data required to render this blueprint item's ghost when placing the blueprint.
         */
        std::unique_ptr<BlueprintGhost> ghost;

        explicit BlueprintItemBase(TileIndex start_index, Position offset) {
            this->offset = offset;
            this->origin = IndexToPosition(start_index) + offset;
        }
    };
}

#endif //OPENTTD_BLUEPRINT_ITEM_HPP
