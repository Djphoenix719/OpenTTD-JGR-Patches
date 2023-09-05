/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <http://www.gnu.org/licenses/>.
 */

#include "blueprint_class.hpp"
#include "blueprint_item.hpp"

namespace blueprint {
    bool Blueprint::HasLocalTile(blueprint::Position position) {
        auto range = this->tiles.equal_range(position);
        return range.first != this->tiles.end();
    }

    void Blueprint::Add(std::shared_ptr<BlueprintItemBase> &item) {
        this->items.emplace_back(item);
        this->tiles.emplace(std::make_pair(item->GetStartOffset(), item));
    }
}