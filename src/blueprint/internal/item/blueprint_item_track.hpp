//
// Created by Andy on 9/4/2023.
//

#ifndef OPENTTD_BLUEPRINT_ITEM_TRACK_HPP
#define OPENTTD_BLUEPRINT_ITEM_TRACK_HPP

#include "command_type.h"
#include "../blueprint_item.hpp"
#include "../blueprint_position.hpp"

namespace blueprint {
    class BlueprintItemTrack : public BlueprintItemBase {
    public:
        std::unique_ptr<CommandContainer> MakeCommand(TileIndex tile_index) override;

        explicit BlueprintItemTrack(TileIndex start_index, Position offset, RailType type, Trackdir direction)
            : BlueprintItemBase(start_index, offset) {
            this->type = type;
            this->direction = direction;
        }

    private:
        /**
         * Type of this track.
         */
        RailType type;
        /**
         * Direction of this track.
         */
        Trackdir direction;
    };
}

#endif //OPENTTD_BLUEPRINT_ITEM_TRACK_HPP
