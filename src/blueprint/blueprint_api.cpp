/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <http://www.gnu.org/licenses/>.
 */

#include "tile_type.h"
#include "blueprint_class.hpp"

namespace blueprint {
    /**
     * Copy the area between the start and end tiles and store it as the active blueprint.
     */
    void Copy(TileIndex start_tile, TileIndex end_tile) {}
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
}