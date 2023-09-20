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
#include "viewport_func.h"

namespace blueprint {
    /**
     * Copy the area between the start and end tiles and store it as the active blueprint.
     */
    std::shared_ptr<Blueprint> Copy(TileIndex start_tile, TileIndex end_tile) {
        auto blueprint = std::make_shared<Blueprint>(start_tile, end_tile);
        blueprint->Load();
        SetActiveBlueprint(blueprint);
        return blueprint;
    }

    /**
     * Paste the active blueprint at the specified start tile.
     * @param start_tile The tile to start pasting in - should correspond to the equivalent start_tile from the copy.
     * @param blueprint The blueprint to paste - if nullptr use the globally available "last copied blueprint".
     */
    void Paste(TileIndex start_tile) {
        auto blueprint = GetActiveBlueprint();
        if (blueprint == nullptr)
            return;

        blueprint->Paste(start_tile);
    }

    /**
     * Reset the current blueprint to a new blueprint.
     */
    void Reset() {
        auto last_blueprint = GetActiveBlueprint();
        if (last_blueprint == nullptr)
            return;

        auto next_blueprint = std::make_shared<Blueprint>();
        SetActiveBlueprint(next_blueprint);
    }

    bool DrawTileSelection(const TileInfo *tile_info, const TileHighlightData &tile_highlight_data) {
        if (tile_highlight_data.place_mode != HT_PASTE) {
            // If we're not pasting a blueprint, we want to do nothing
            return false;
        }

        auto blueprint = GetActiveBlueprint();
        if (blueprint == nullptr) {
            // If we are pasting a blueprint, this should never be nullptr, but for safety if it is, do nothing
            return false;
        }

        blueprint->Draw(tile_info);
        return true;
    }

    void UpdateTileSelection(TileHighlightData &tile_highlight_data, HighLightStyle new_draw_style) {
        // If old or new draw style is pasting
        if (tile_highlight_data.drawstyle & HT_PASTE || new_draw_style & HT_PASTE) {
            // If the new style is not pasting, we're "stopping paste"
            if (!(new_draw_style & HT_PASTE)) {
                Reset();
                MarkWholeNonMapViewportsDirty();
                return;
            }

            if (tile_highlight_data.place_mode & HT_PASTE) {
                auto cursor_virtual = GetTileBelowCursor();
                if (cursor_virtual.x == -1 || cursor_virtual.y == -1)
                    return;

                auto blueprint = GetActiveBlueprint();
                if (blueprint == nullptr)
                    return;

                MarkWholeNonMapViewportsDirty();
            }
        }
    }
}