/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <http://www.gnu.org/licenses/>.
 */

#include "blueprint_position.hpp"
#include "map_func.h"

namespace blueprint {
    /**
     * Convert a TileIndexDiffC to a position.
     */
    Position DiffToPosition(TileIndexDiffC diff) {
        return Position{(int) diff.x, (int) diff.y};
    }

    /**
     * Convert a Position to a TileIndexDiffC.
     */
    TileIndexDiffC PositionToDiff(Position position) {
        return TileIndexDiffC{(int16) position.x, (int16) position.y};
    }

    /**
     * Convert a TileIndex to a Position.
     */
    Position IndexToPosition(TileIndex index) {
        uint x = TileX(index);
        uint y = TileY(index);
        return Position{(int) x, (int) y};
    }

    /**
     * Convert a Position to a TileIndex.
     */
    TileIndex PositionToIndex(Position position) {
        return TileXY((uint) position.x, (uint) position.y);
    }
}