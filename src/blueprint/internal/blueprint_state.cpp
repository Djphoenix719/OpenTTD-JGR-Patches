/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <http://www.gnu.org/licenses/>.
 */

#include "blueprint_state.hpp"

namespace blueprint {
    std::vector<std::shared_ptr<Blueprint>> _previous_blueprints;

    /**
     * Get the last blueprint copied by the user.
     * @return A pointer to the last blueprint, or nullptr if none have been copied since startup.
     */
    std::shared_ptr<Blueprint> GetLastBlueprint() {
        if (_previous_blueprints.empty())
            return nullptr;
        return _previous_blueprints.back();
    }

    /**
     * Add a blueprint to the history.
     */
    void AddBlueprintToHistory(std::shared_ptr<Blueprint> blueprint) {
        _previous_blueprints.emplace_back(blueprint);
    }
}