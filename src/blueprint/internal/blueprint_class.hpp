/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef OPENTTD_BLUEPRINT_TYPE_HPP
#define OPENTTD_BLUEPRINT_TYPE_HPP

#include <vector>
#include <memory>
#include <unordered_map>
#include "stdafx.h"
#include "blueprint_position.hpp"
#include "blueprint_item.hpp"
#include "tile_cmd.h"
#include "viewport_func.h"
#include "spritecache.h"

namespace blueprint {
    class Blueprint {
    public:
        Blueprint(TileIndex start, TileIndex end) {
            this->start_position = IndexToPosition(start);
            this->end_position = IndexToPosition(end);
            this->last_rendered_tile = -1;
        }

        /**
         * Get the start position of this blueprint.
         */
        [[nodiscard]] inline Position GetStartPosition() const noexcept { return this->start_position; }

        /**
         * Get the end position of this blueprint.
         */
        [[nodiscard]] inline Position GetEndPosition() const noexcept { return this->end_position; }

        /**
         * Add an item to this blueprint.
         */
        void Add(const std::shared_ptr<BlueprintItemBase> &item);

        /**
         * Load the blueprint's internal state given it's start and ending position, copying required
         *  data into items and setting up other variables needed to paste the blueprint.
         */
        void Load();

        /**
         * If the blueprint were to be positioned at the mouse cursor and has at least one item at
         *  the tile, render the items on the tile.
         * @param tile_info The location where rendering is occurring.
         */
        void Draw(const TileInfo *tile_info);

        /**
         * Mark all items in the blueprint dirty, recalculating their sprites.
         */
        void MarkDirty(TileIndex next_origin);

        void DrawSelectionOverlay(const SpritePointerHolder &sprite_store, const DrawPixelInfo *dpi);

    private:
        /**
         * Start position of this blueprint (world position).
         */
        Position start_position;
        /**
         * End position of this blueprint (world position).
         */
        Position end_position;

        /**
         * Vector of items (e.g. individual tracks, signals, etc) contained within this blueprint.
         */
        std::vector<std::shared_ptr<BlueprintItemBase>> items;

        /**
         * Unordered multi-map of offsets to items which should be rendered at that offset.
         */
        std::unordered_multimap<Position, std::shared_ptr<BlueprintItemBase>> tiles;

        TileIndex last_rendered_tile;
    };
}

#endif //OPENTTD_BLUEPRINT_TYPE_HPP
