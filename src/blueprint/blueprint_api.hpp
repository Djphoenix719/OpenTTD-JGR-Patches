/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef OPENTTD_BLUEPRINT_API_HPP
#define OPENTTD_BLUEPRINT_API_HPP

#include <memory>
#include "tile_type.h"
#include "internal/blueprint_class.hpp"
#include "tile_cmd.h"
#include "tilehighlight_type.h"

/**
 * "Public" functions for blueprints - avoid using anything not in this file from outside
 *  the blueprints folder unless you know what you are doing.
 */

namespace blueprint {
    std::shared_ptr<Blueprint> Copy(TileIndex start_tile, TileIndex end_tile);
    void Paste(TileIndex start_tile);
    void Reset();
    bool DrawTileSelection(const TileInfo *tile_info, const TileHighlightData &tile_highlight_data);
    void UpdateTileSelection(TileHighlightData &tile_highlight_data, HighLightStyle new_draw_style);
}

#endif //OPENTTD_BLUEPRINT_API_HPP
