/*
    Realm of Aesir
    Copyright (C) 2019  Michael de Lang

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "load_spawners.h"
#include <rapidjson/document.h>
#include <working_directory_manipulation.h>
#include "spdlog/spdlog.h"
#include <range/v3/all.hpp>
#include <entt/entity/registry.hpp>
#include <charconv>

using namespace std;
using namespace rapidjson;
using namespace lotr;

void emplace_npc_ids(Value const &current_script, vector<spawner_npc_id> &npc_ids, entt::registry &registry) {
    auto &npc_ids_json = current_script["npcIds"];
    npc_ids.reserve(npc_ids_json.Size());
    for (SizeType npc_i = 0; npc_i < npc_ids_json.Size(); npc_i++) {
        auto npc_id = npc_ids_json[npc_i]["name"].GetString();
        auto chance = npc_ids_json[npc_i]["chance"].GetUint();
        spdlog::trace("[{}] npc id {} {}", __FUNCTION__, npc_id, chance);

        auto gnpc = lotr::get_global_npc_by_npc_id(registry, npc_id);

        if (gnpc == nullptr) {
            spdlog::error("[{}] npc id {} not found in global npcs", __FUNCTION__, npc_id);
            continue;
        }

        npc_ids.emplace_back(chance, *gnpc);
    }
}

void emplace_paths(Value const &current_script, vector<vector<npc_path>> &paths) {
    if(current_script.HasMember("paths")) {
        auto &paths_json = current_script["paths"];
        paths.reserve(paths_json.Size());
        for (SizeType path_i = 0; path_i < paths_json.Size(); path_i++) {
            string path = paths_json[path_i].GetString();
            spdlog::trace("[{}] npc path {}", __FUNCTION__, path);
            vector<string_view> split_path = path | ranges::views::split(' ') | ranges::views::transform([](auto &&rng){return string_view(&*rng.begin(), ranges::distance(rng));}) | ranges::to_vector;
            vector<npc_path> spawn_paths;
            spawn_paths.reserve(split_path.size());
            for(auto const &split : split_path) {
                vector<string_view> temp = split | ranges::views::split('-') | ranges::views::transform([](auto &&rng){return string_view(&*rng.begin(), ranges::distance(rng));}) | ranges::to_vector;
                if(temp.size() != 2) {
                    spdlog::error("[{}] path {} split {} temp size not equal to 2 but is {}", __FUNCTION__, path, split, temp.size());
                    continue;
                }
                uint32_t value;
                if(auto [p, ec] = from_chars(temp[0].data(), temp[0].data() + temp[0].size(), value); ec != errc()) {
                    spdlog::error("[{}] path {} split {} couldn't convert to integer", __FUNCTION__, path, split);
                    continue;
                }
                movement_direction dir;
                if(temp[1] == north_direction) {
                    dir = movement_direction::North;
                } else if(temp[1] == east_direction) {
                    dir = movement_direction::East;
                } else if(temp[1] == south_direction) {
                    dir = movement_direction::South;
                } else if(temp[1] == west_direction) {
                    dir = movement_direction::West;
                } else if(temp[1] == north_east_direction)  {
                    dir = movement_direction::NorthEast;
                } else if(temp[1] == north_west_direction)  {
                    dir = movement_direction::NorthWest;
                } else if(temp[1] == south_east_direction)  {
                    dir = movement_direction::SouthEast;
                } else if(temp[1] == south_west_direction)  {
                    dir = movement_direction::SouthWest;
                } else {
                    spdlog::error("[{}] could not decode path {} split {} missing direction {}", __FUNCTION__, path, split, temp[1]);
                    continue;
                }
                spawn_paths.emplace_back(dir, value);
            }
            paths.emplace_back(move(spawn_paths));
        }
    }
}

void emplace_npc_ais(Value const &current_script, vector<string> &npc_ai_settings) {
    if(current_script.HasMember("npcAISettings")) {
        auto &ais = current_script["npcAISettings"];
        npc_ai_settings.reserve(ais.Size());
        for (SizeType ai_i = 0; ai_i < ais.Size(); ai_i++) {
            npc_ai_settings.emplace_back(ais[ai_i].GetString());
        }
    }
}

vector<spawner_script> lotr::load_global_spawners_from_file(const string &file, entt::registry &registry) {
    spdlog::debug("[{}] Loading global spawners from file {}", __FUNCTION__, file);
    auto env_contents = read_whole_file(file);

    if(!env_contents) {
        return {};
    }

    Document d;
    d.Parse(env_contents->c_str(), env_contents->size());

    if (d.HasParseError() || !d.IsArray()) {
        spdlog::error("[{}] deserialize {} failed", __FUNCTION__, file);
        return {};
    }

    vector<spawner_script> scripts;
    scripts.reserve(d.Size());
    string yes = "yes";

    for(SizeType i = 0; i < d.Size(); i++) {
        auto &current_script = d[i];
        spawner_script script;
        spdlog::trace("[{}] script {}: {}", __FUNCTION__, i, current_script["tag"].GetString());

        script.tag = current_script["tag"].GetString();
        script.respawn_rate = current_script["respawnRate"].GetUint();
        script.initial_spawn = current_script["initialSpawn"].GetUint();
        script.max_spawn = current_script["maxSpawn"].GetUint();
        script.max_creatures = current_script["maxCreatures"].GetUint();
        script.spawn_radius = current_script["spawnRadius"].GetUint();
        script.random_walk_radius = current_script["randomWalkRadius"].GetUint();
        script.leash_radius = current_script["leashRadius"].GetUint();
        script.elite_tick_cap = current_script["eliteTickCap"].GetUint();
        script.should_serialize = current_script["shouldSerialize"].GetString() == yes;
        script.always_spawn = current_script["alwaysSpawn"].GetString() == yes;
        script.require_dead_to_respawn = current_script["requireDeadToRespawn"].GetString() == yes;
        script.can_slow_down = current_script["canSlowDown"].GetString() == yes;
        script.do_initial_spawn_immediately = current_script["doInitialSpawnImmediately"].GetString() == yes;

        emplace_npc_ids(current_script, script.npc_ids, registry);

        emplace_paths(current_script, script.paths);

        emplace_npc_ais(current_script, script.npc_ai_settings);

        scripts.emplace_back(move(script));
    }

    return scripts;
}

