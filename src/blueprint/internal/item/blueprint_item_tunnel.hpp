/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef OPENTTD_BLUEPRINT_ITEM_TUNNEL_HPP
#define OPENTTD_BLUEPRINT_ITEM_TUNNEL_HPP

#include "stdafx.h"
#include "command_type.h"
#include "rail_type.h"
#include "track_type.h"
#include "blueprint/internal/blueprint_item.hpp"
#include "bridge.h"

namespace blueprint {
    class BlueprintTunnelItem : public BlueprintItemBase {
    public:
        std::unique_ptr<CommandContainer> MakeCommand(TileIndex tile_index) override;
        void Draw(const TileInfo *tile_info) override;
        void MarkDirty(TileIndex next_origin) override;

        explicit BlueprintTunnelItem(TileIndex start_index, Position offset, TileIndexDiffC other_end_offset,
                                     RailType rail_type, DiagDirection direction)
            : BlueprintItemBase(start_index, offset) {
            this->other_end_offset = other_end_offset;
            this->rail_type = rail_type;
            this->direction = direction;
        }

    private:
        /**
         * Offset of the other end, based on start index.
         */
        TileIndexDiffC other_end_offset{};
        /**
         * Rail type within the tunnel.
         */
        RailType rail_type;
        /**
         * Direction of the tunnel.
         */
        DiagDirection direction;
    };
}

#endif //OPENTTD_BLUEPRINT_ITEM_TUNNEL_HPP
