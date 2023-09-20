/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <http://www.gnu.org/licenses/>.
 */

#include "blueprint_state.hpp"

namespace blueprint {
    std::shared_ptr<Blueprint> _current;

    /**
     * Get the last blueprint copied by the user.
     * @return A pointer to the last blueprint, or nullptr if none have been copied since startup.
     */
    std::shared_ptr<Blueprint> GetActiveBlueprint() {
        return _current;
    }

    /**
     * Set the currently active blueprint to the provided blueprint;
     */
    void SetActiveBlueprint(const std::shared_ptr<Blueprint> &blueprint) {
        _current = blueprint;
    }
}