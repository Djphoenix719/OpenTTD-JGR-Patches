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
         * Make a command that will send this single item over the network.
         * @param tile_index Origin point for the command, not including offset. E.g. this is where the
         *  user clicked, not necessarily where this item should be placed.
         */
        virtual std::unique_ptr<CommandContainer> MakeCommand(TileIndex tile_index) = 0;

        virtual void Draw(const TileInfo *tile_info) = 0;

    protected:
        /**
         * Tile index of the origin of this blueprint item (e.g. where it was copied from).
         */
        Position origin;
        /**
         * Offset from the parent blueprint's start_position.
         */
        Position offset;

        explicit BlueprintItemBase(TileIndex start_index, Position offset) {
            this->offset = offset;
            this->origin = IndexToPosition(start_index) + offset;
        }
    };
}

#endif //OPENTTD_BLUEPRINT_ITEM_HPP
