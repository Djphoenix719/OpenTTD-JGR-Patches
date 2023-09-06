/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <http://www.gnu.org/licenses/>.
 */

#include "blueprint_class.hpp"
#include "blueprint_item.hpp"
#include "viewport_func.h"
#include "rail_map.h"
#include "blueprint/internal/item/blueprint_item_track.hpp"
#include <algorithm>

namespace blueprint {
    std::unique_ptr<Position> GetCursorPosition() {
        auto cursor_virtual = GetTileBelowCursor();
        if (cursor_virtual.x == -1 || cursor_virtual.y == -1)
            return nullptr;
        auto cursor_world = TileVirtXY(cursor_virtual.x, cursor_virtual.y);
        return std::make_unique<Position>(IndexToPosition(cursor_world));
    }

    std::vector<std::unique_ptr<BlueprintItemBase>> MakeItemsFromTile(Position start_position, TileIndex tile_index) {
        std::vector<std::unique_ptr<BlueprintItemBase>> items;
        Position offset = start_position - IndexToPosition(tile_index);
        switch (GetTileType(tile_index)) {
            case MP_STATION:
                break;
            case MP_RAILWAY:
                switch (GetRailTileType(tile_index)) {
//                    case RAIL_TILE_DEPOT: {
//                        blueprint->AddRailDepot(tile_index, tile_diff);
//                        break;
//                    }
//                    case RAIL_TILE_SIGNALS: {
//                        blueprint->AddSignal(tile_index, tile_diff);
//                        FALLTHROUGH;
//                    }
                    case RAIL_TILE_NORMAL: {
                        static const Trackdir _track_iterate_dir[TRACK_END] = {
                            TRACKDIR_X_SW,
                            TRACKDIR_Y_SE,
                            TRACKDIR_UPPER_E,
                            TRACKDIR_LOWER_E,
                            TRACKDIR_LEFT_S,
                            TRACKDIR_RIGHT_S
                        };

                        // TODO: Optimization - can store items for lengths of straight track, then place lengths at a time
                        // When there are multiple tracks on the same tile we need to decompress the bits
                        for (Track track = TRACK_BEGIN; track < TRACK_END; track++) {
                            if (HasBit(GetTrackBits(tile_index), track)) {
                                auto bp = std::make_unique<BlueprintItemTrack>(
                                    BlueprintItemTrack(
                                        tile_index,
                                        offset,
                                        GetRailType(tile_index),
                                        _track_iterate_dir[track]
                                    )
                                );
                                items.emplace_back(std::move(bp));
                            }
                        }
                        break;
                    }
                    default:
                        NOT_REACHED();
                }
                break;
//            case MP_TUNNELBRIDGE:
//                if (GetTunnelBridgeTransportType(tile_index) != TRANSPORT_RAIL)
//                    break;
//                blueprint->AddTunnelBridge(tile_index, tile_diff);
//                break;

                // Nothing else gets copied, do nothing
            default:
                break;
        }
        return items;
    }

    void Blueprint::Add(const std::shared_ptr<BlueprintItemBase> &item) {
        this->items.emplace_back(item);
        this->tiles.emplace(std::make_pair(item->GetStartOffset(), item));
    }

    void Blueprint::Load() {
        auto area = TileArea{
            PositionToIndex(this->start_position),
            PositionToIndex(this->end_position)
        };

        for (auto tile: area) {
            auto items_on_tile = MakeItemsFromTile(this->start_position, tile);
            for (auto &new_item: items_on_tile) {
                this->Add(std::move(new_item));
            }
        }
    }


    void Blueprint::Draw(const TileInfo *tile_info) {
        auto cursor = GetCursorPosition();
        // Cursor above some GUI element or otherwise off-map
        if (cursor == nullptr)
            return;

        auto render_position = IndexToPosition(tile_info->tile);
        auto offset = *cursor - render_position;
        auto range = this->tiles.equal_range(offset);
        // No tiles are at that offset
        if (range.first == this->tiles.end())
            return;

        for (auto pair = range.first; pair != range.second; pair++) {
            auto item = pair->second;
            item->Draw(tile_info);
        }
    }
}