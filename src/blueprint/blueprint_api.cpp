/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <http://www.gnu.org/licenses/>.
 */

#include "stdafx.h"
#include "tile_type.h"
#include "internal/blueprint_class.hpp"
#include "blueprint_api.hpp"
#include "tile_cmd.h"
#include "tilehighlight_type.h"
#include "blueprint/internal/blueprint_state.hpp"

namespace blueprint {
    /**
     * Copy the area between the start and end tiles and store it as the active blueprint.
     */
    std::shared_ptr<Blueprint> Copy(TileIndex start_tile, TileIndex end_tile) {
        auto blueprint = std::make_shared<Blueprint>(start_tile, end_tile);
        blueprint->Load();
        AddBlueprintToHistory(blueprint);
        return blueprint;
    }

    /**
     * Paste the active blueprint at the specified start tile.
     * @param start_tile The tile to start pasting in - should correspond to the equivalent start_tile from the copy.
     * @param blueprint The blueprint to paste - if nullptr use the globally available "last copied blueprint".
     */
    void Paste(TileIndex start_tile, const Blueprint *blueprint) {}

    /**
     * Reset the last copied blueprint.
     */
    void Reset() {}

    bool DrawTileSelection(const TileInfo *tile_info, const TileHighlightData &tile_highlight_data) {
        if (tile_highlight_data.place_mode != HT_PASTE) {
            // If we're not pasting a blueprint, we want to do nothing
            return false;
        }

        auto blueprint = GetLastBlueprint();
        if (blueprint == nullptr) {
            // If we are pasting a blueprint, this should never be nullptr, but for safety if it is do nothing
            return false;
        }

        blueprint->Draw(tile_info);
        return true;
    }
}