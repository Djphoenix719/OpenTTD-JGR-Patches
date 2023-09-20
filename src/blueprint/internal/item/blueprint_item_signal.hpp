/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef OPENTTD_BLUEPRINT_ITEM_SIGNAL_HPP
#define OPENTTD_BLUEPRINT_ITEM_SIGNAL_HPP

#include "stdafx.h"
#include "command_type.h"
#include "rail_type.h"
#include "track_type.h"
#include "blueprint/internal/blueprint_item.hpp"

namespace blueprint {
    class BlueprintSignalItem : public BlueprintItemBase {
    public:
        std::unique_ptr<CommandContainer> MakeCommand(TileIndex tile_index) override;
        void Draw(const TileInfo *tile_info) override;
        void MarkDirty(TileIndex next_origin) override;

        explicit BlueprintSignalItem(TileIndex start_index, Position offset, Track track, SignalType type,
                                     SignalVariant variant, bool backward)
            : BlueprintItemBase(start_index, offset) {
            this->track = track;
            this->type = type;
            this->variant = variant;
            this->backward = backward;
        }

    private:
        /**
         * Track direction this signal was aligned to
         */
        Track track;
        /**
         * Type of this signal.
         */
        SignalType type;
        /**
         * Variant of this signal.
         */
        SignalVariant variant;
        /**
         * Was this signal placed backwards?
         */
        bool backward;
    };
}

#endif //OPENTTD_BLUEPRINT_ITEM_SIGNAL_HPP
