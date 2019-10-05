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

#include <catch2/catch.hpp>
#include <iostream>
#include <fstream>

#include "asset_loading/load_spawners.h"

using namespace std;
using namespace lotr;

TEST_CASE("spawner loading tests") {
    ofstream map_file("test_spawner.json", ios_base::trunc);
    map_file << R"([
    {
        "npcIds": [
            {
                "name": "Training Dummy",
                "chance": 1
            }
        ],
        "respawnRate": 5,
        "initialSpawn": 1,
        "maxSpawn": 1,
        "maxCreatures": 1,
        "spawnRadius": 0,
        "randomWalkRadius": 0,
        "leashRadius": 0,
        "shouldSerialize": "yes",
        "alwaysSpawn": "yes",
        "requireDeadToRespawn": "yes",
        "canSlowDown": "no",
        "eliteTickCap": 0,
        "npcAISettings": [
            "trainingdummy"
        ],
        "doInitialSpawnImmediately": "yes",
        "tag": "Global Dummy"
    },
    {
        "npcIds": [],
        "respawnRate": 0,
        "initialSpawn": 0,
        "maxSpawn": 1,
        "maxCreatures": 1,
        "spawnRadius": 0,
        "randomWalkRadius": 7,
        "leashRadius": 12,
        "shouldSerialize": "yes",
        "alwaysSpawn": "yes",
        "requireDeadToRespawn": "yes",
        "shouldStrip": "yes",
        "stripOnSpawner": "yes",
        "canSlowDown": "no",
        "doInitialSpawnImmediately": "yes",
        "eliteTickCap": 0,
        "tag": "Global Empty"
    },
    {
        "npcIds": [],
        "respawnRate": 3600,
        "initialSpawn": 1,
        "maxSpawn": 1,
        "maxCreatures": 1,
        "spawnRadius": 0,
        "randomWalkRadius": 2,
        "leashRadius": 10,
        "shouldSerialize": "yes",
        "alwaysSpawn": "yes",
        "requireDeadToRespawn": "yes",
        "shouldStrip": "yes",
        "stripOnSpawner": "yes",
        "canSlowDown": "no",
        "isDangerous": "yes",
        "doInitialSpawnImmediately": "yes",
        "eliteTickCap": 0,
        "tag": "Global Lair"
    },
    {
        "npcIds": [],
        "respawnRate": 1800,
        "initialSpawn": 1,
        "maxSpawn": 1,
        "maxCreatures": 1,
        "spawnRadius": 0,
        "randomWalkRadius": 0,
        "leashRadius": 0,
        "shouldSerialize": "yes",
        "alwaysSpawn": "yes",
        "requireDeadToRespawn": "yes",
        "canSlowDown": "no",
        "doInitialSpawnImmediately": "yes",
        "eliteTickCap": 0,
        "npcAISettings": [
            "resource"
        ],
        "tag": "Global Single-resource"
    },
    {
        "npcIds": [
            {
                "name": "Tutorial Deer",
                "chance": 1
            },
            {
                "name": "Tutorial Wolf",
                "chance": 1
            }
        ],
        "respawnRate": 15,
        "initialSpawn": 2,
        "maxCreatures": 6,
        "spawnRadius": 1,
        "maxSpawn": 1,
        "randomWalkRadius": 5,
        "leashRadius": 10,
        "eliteTickCap": 0,
        "shouldSerialize": "yes",
        "alwaysSpawn": "yes",
        "requireDeadToRespawn": "yes",
        "canSlowDown": "no",
        "doInitialSpawnImmediately": "yes",
        "tag": "Tutorial Monster"
    },
    {
        "npcIds": [
            {
                "name": "Tutorial Townee",
                "chance": 1
            }
        ],
        "paths": [
            "23-E 16-S 23-W 16-N",
            "16-S 23-E 16-N 23-W",
            "8-S 23-E 8-S 23-W 8-N 23-E 8-N 23-W",
            "23-E 8-S 23-W 8-S 23-E 8-N 23-W 8-N"
        ],
        "respawnRate": 15,
        "initialSpawn": 2,
        "maxCreatures": 20,
        "spawnRadius": 0,
        "maxSpawn": 1,
        "randomWalkRadius": 0,
        "leashRadius": 30,
        "eliteTickCap": 0,
        "shouldSerialize": "yes",
        "alwaysSpawn": "yes",
        "requireDeadToRespawn": "yes",
        "canSlowDown": "no",
        "doInitialSpawnImmediately": "yes",
        "tag": "Tutorial Townee"
    },
    {
        "npcIds": [
            {
                "name": "Tutorial Yeti",
                "chance": 1
            }
        ],
        "respawnRate": 30,
        "initialSpawn": 1,
        "maxCreatures": 1,
        "spawnRadius": 1,
        "randomWalkRadius": 5,
        "leashRadius": 10,
        "maxSpawn": 1,
        "eliteTickCap": 0,
        "shouldSerialize": "yes",
        "alwaysSpawn": "yes",
        "requireDeadToRespawn": "yes",
        "canSlowDown": "no",
        "doInitialSpawnImmediately": "yes",
        "tag": "Tutorial Yeti"
    },
    {
        "npcIds": [
            {
                "name": "Basic Copper Vein",
                "chance": 20
            },
            {
                "name": "Rich Copper Vein",
                "chance": 1
            }
        ],
        "respawnRate": 1800,
        "initialSpawn": 1,
        "maxSpawn": 1,
        "maxCreatures": 1,
        "spawnRadius": 0,
        "randomWalkRadius": 0,
        "leashRadius": 0,
        "shouldSerialize": "yes",
        "alwaysSpawn": "yes",
        "requireDeadToRespawn": "yes",
        "canSlowDown": "no",
        "eliteTickCap": 0,
        "npcAISettings": [
            "resource"
        ],
        "doInitialSpawnImmediately": "yes",
        "tag": "Resource Copper"
    }])";

    map_file.close();

    entt::registry registry;
    {
        auto new_entity = registry.create();
        global_npc_component gnpc{};
        gnpc.npc_id = "Training Dummy";
        registry.assign<global_npc_component>(new_entity, move(gnpc));
    }

    auto scripts = load_global_spawners_from_file("test_spawner.json", registry);
    REQUIRE(scripts.size() == 8);
    REQUIRE(scripts[0].tag == "Global Dummy");
    REQUIRE(scripts[0].respawn_rate == 5);
    REQUIRE(scripts[0].initial_spawn == 1);
    REQUIRE(scripts[0].max_spawn == 1);
    REQUIRE(scripts[0].max_creatures == 1);
    REQUIRE(scripts[0].spawn_radius == 0);
    REQUIRE(scripts[0].random_walk_radius == 0);
    REQUIRE(scripts[0].leash_radius == 0);
    REQUIRE(scripts[0].elite_tick_cap == 0);
    REQUIRE(scripts[0].should_serialize == true);
    REQUIRE(scripts[0].always_spawn == true);
    REQUIRE(scripts[0].require_dead_to_respawn == true);
    REQUIRE(scripts[0].can_slow_down == false);
    REQUIRE(scripts[0].do_initial_spawn_immediately == true);
    REQUIRE(scripts[0].npc_ai_settings.size() == 1);
    REQUIRE(scripts[0].npc_ai_settings[0] == "trainingdummy");
    REQUIRE(scripts[0].npc_ids.size() == 1);
    REQUIRE(scripts[0].npc_ids[0].npc_id == "Training Dummy");
    REQUIRE(scripts[0].npc_ids[0].chance == 1);
}
