/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef OPENTTD_BLUEPRINT_POSITION_HPP
#define OPENTTD_BLUEPRINT_POSITION_HPP

#include "stdafx.h"
#include "map_type.h"
#include "tile_type.h"

namespace blueprint {
    struct Position {
        int x;
        int y;

        Position() : x{0}, y{0} {}

        explicit Position(int s) : x{s}, y{s} {}

        explicit Position(int x, int y) : x{x}, y{y} {}

        inline Position operator+(const Position &rhs) const noexcept {
            return Position{this->x + rhs.x, this->y + rhs.y};
        }

        inline Position operator-(const Position &rhs) const noexcept {
            return Position{this->x - rhs.x, this->y - rhs.y};
        }

        inline bool operator==(const Position &rhs) const noexcept {
            return this->x == rhs.x && this->y == rhs.y;
        }

        inline bool operator!=(const Position &rhs) const noexcept {
            return this->x != rhs.x || this->y != rhs.y;
        }

        inline Position operator-() const noexcept {
            return Position{-this->x, -this->y};
        }

        /**
         * Returns a new Position which points from this to the other Position (like a Vector does).
         */
        [[nodiscard]] inline Position to(const Position &rhs) const noexcept {
            return Position{rhs.x - this->x, rhs.y - this->y};
        }

        /**
         * Returns a new Position which points from the other Position to this (like a Vector does).
         */
        [[nodiscard]] inline Position from(const Position &rhs) const noexcept {
            return Position{this->x - rhs.x, this->y - rhs.y};
        }
    };

    Position DiffToPosition(TileIndexDiffC diff);
    TileIndexDiffC PositionToDiff(Position position);
    Position IndexToPosition(TileIndex index);
    TileIndex PositionToIndex(Position position);
}

namespace std {
    template<>
    struct std::hash<blueprint::Position> {
        std::size_t operator()(const blueprint::Position &offset) const noexcept {
            // On 32-bit systems this should fill the entire size_t, on 64-bit systems it will fill half
            //  Either way this should be fine and unique w/ no loss in data
            return ((size_t) offset.x << (sizeof(offset.x) * 8)) | offset.y;
        }
    };
}

#endif //OPENTTD_BLUEPRINT_POSITION_HPP
