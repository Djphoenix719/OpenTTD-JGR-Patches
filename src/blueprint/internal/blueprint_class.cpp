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
#include "spritecache.h"
#include "gfx_type.h"
#include "zoom_func.h"
#include "table/sprites.h"
#include "command_func.h"
#include "blueprint/internal/item/blueprint_item_signal.hpp"
#include "blueprint/internal/item/blueprint_item_bridge.hpp"
#include "blueprint/internal/item/blueprint_item_tunnel.hpp"
#include "blueprint/internal/item/blueprint_item_terrain.hpp"
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
                    case RAIL_TILE_SIGNALS: {
                        auto maybe_add_signal = [&](
                            byte signal_bit, uint position, Track track
                        ) {
                            if (!IsSignalPresent(tile_index, signal_bit))
                                return;

                            // TODO: Certain signals can exist on both sides - these are not placed properly
                            auto item = std::make_unique<BlueprintSignalItem>(
                                BlueprintSignalItem(
                                    tile_index,
                                    offset,
                                    track,
                                    GetSignalType(tile_index, track),
                                    GetSignalVariant(tile_index, track),
                                    signal_bit % 2 == 0
                                )
                            );
                            items.emplace_back(std::move(item));
                        };

                        auto rails = GetTrackBits(tile_index);
                        if (!(rails & TRACK_BIT_Y)) {
                            if (!(rails & TRACK_BIT_X)) {
                                if (rails & TRACK_BIT_LEFT) {
                                    maybe_add_signal(2, 0, TRACK_LEFT);
                                    maybe_add_signal(3, 1, TRACK_LEFT);
                                }
                                if (rails & TRACK_BIT_RIGHT) {
                                    maybe_add_signal(0, 2, TRACK_RIGHT);
                                    maybe_add_signal(1, 3, TRACK_RIGHT);
                                }
                                if (rails & TRACK_BIT_UPPER) {
                                    maybe_add_signal(3, 4, TRACK_UPPER);
                                    maybe_add_signal(2, 5, TRACK_UPPER);
                                }
                                if (rails & TRACK_BIT_LOWER) {
                                    maybe_add_signal(1, 6, TRACK_LOWER);
                                    maybe_add_signal(0, 7, TRACK_LOWER);
                                }
                            } else {
                                maybe_add_signal(3, 8, TRACK_X);
                                maybe_add_signal(2, 9, TRACK_X);
                            }
                        } else {
                            maybe_add_signal(3, 10, TRACK_Y);
                            maybe_add_signal(2, 11, TRACK_Y);
                        }
                        FALLTHROUGH;
                    }
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
                                auto item = std::make_unique<BlueprintItemTrack>(
                                    BlueprintItemTrack(
                                        tile_index,
                                        offset,
                                        GetRailType(tile_index),
                                        _track_iterate_dir[track]
                                    )
                                );
                                items.emplace_back(std::move(item));
                            }
                        }
                        break;
                    }
                    default:
                        NOT_REACHED();
                }
                break;
            case MP_TUNNELBRIDGE: {
                if (GetTunnelBridgeTransportType(tile_index) != TRANSPORT_RAIL)
                    break;

                auto other_end = GetOtherTunnelBridgeEnd(tile_index);

                if (IsTunnel(tile_index)) {
                    auto item = std::make_unique<BlueprintTunnelItem>(
                        BlueprintTunnelItem(
                            tile_index,
                            offset,
                            TileIndexToTileIndexDiffC(other_end, tile_index),
                            GetRailType(tile_index),
                            GetTunnelBridgeDirection(tile_index)
                        )
                    );
                    items.emplace_back(std::move(item));
                } else {
                    auto item = std::make_unique<BlueprintBridgeItem>(
                        BlueprintBridgeItem(
                            tile_index,
                            offset,
                            TileIndexToTileIndexDiffC(other_end, tile_index),
                            GetBridgeType(tile_index)
                        )
                    );
                    items.emplace_back(std::move(item));
                }
                break;
            }

                // Nothing else gets copied, do nothing
            default:
                break;
        }

        // Add terrain offsets
        uint start_height = TileHeight(PositionToIndex(start_position));
        uint tile_height = TileHeight(tile_index);

        if (start_height != tile_height) {
            auto item = std::make_unique<BlueprintItemTerrain>(
                BlueprintItemTerrain(tile_index, offset, (int) tile_height - (int) start_height)
            );
            items.emplace_back(std::move(item));
        }

        return items;
    }

    void Blueprint::Add(const std::shared_ptr<BlueprintItemBase> &item) {
        this->items.emplace_back(item);
        this->tiles.emplace(std::make_pair(item->GetStartOffset(), item));
    }

    void Blueprint::Load() {
        this->tiles.clear();

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

    void Blueprint::Paste(TileIndex start_tile) {
        // Three loops - 1st for terrain, 2nd for non-signals, 3rd for signals
        // TODO: There's a infinite loop here when the north corner of the start tile is down one
        auto done = false;
        while (!done) {
            done = true;
            for (auto &item: this->items) {
                auto terrain_item = dynamic_cast<BlueprintItemTerrain *>(item.get());
                if (terrain_item == nullptr) {
                    continue;
                }

                auto command = item->MakeCommand(start_tile);
                if (command == nullptr) {
                    continue;
                }

                DoCommand(command.get(), DC_EXEC);
                done = false;
            }
        }

        // Loop 2 - non-signals
        for (auto &item: this->items) {
            // If this item is a signal, skip on the first loop
            if (dynamic_cast<BlueprintSignalItem *>(item.get()) != nullptr) {
                continue;
            }
            // Skip any terrain, we're done with those
            if (dynamic_cast<BlueprintItemTerrain *>(item.get()) != nullptr) {
                continue;
            }

            auto command = item->MakeCommand(start_tile);
            DoCommand(command.get(), DC_EXEC);
        }

        // Loop 3 - signals
        for (auto &item: this->items) {
            // If this item is NOT a signal, skip on the second loop
            if (dynamic_cast<BlueprintSignalItem *>(item.get()) == nullptr) {
                continue;
            }
            // Skip any terrain, we're done with those
            if (dynamic_cast<BlueprintItemTerrain *>(item.get()) != nullptr) {
                continue;
            }

            auto command = item->MakeCommand(start_tile);
            DoCommand(command.get(), DC_EXEC);
        }
    }
}