//////////////////////////////////////////////////////////////////////////////
// NoLifeNx - Part of the NoLifeStory project                               //
// Copyright © 2013 Peter Atashian                                          //
//                                                                          //
// This program is free software: you can redistribute it and/or modify     //
// it under the terms of the GNU Affero General Public License as           //
// published by the Free Software Foundation, either version 3 of the       //
// License, or (at your option) any later version.                          //
//                                                                          //
// This program is distributed in the hope that it will be useful,          //
// but WITHOUT ANY WARRANTY; without even the implied warranty of           //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            //
// GNU Affero General Public License for more details.                      //
//                                                                          //
// You should have received a copy of the GNU Affero General Public License //
// along with this program.  If not, see <http://www.gnu.org/licenses/>.    //
//////////////////////////////////////////////////////////////////////////////
#include "nx.hpp"
#include "file.hpp"
#include "node.hpp"

#include <fstream>
#include <vector>
#include <memory>
#include <stdexcept>

namespace nl
{
    namespace nx
    {
        std::vector<std::unique_ptr<file>> files;

        bool exists(std::string name)
        {
            return std::ifstream("HeavenClient/"+name).is_open();
        }

        node add_file(std::string name)
        {
            if (!exists(name))
                return {};

            files.emplace_back(new file(name));

            return *files.back();
        }

        node base, character, effect, etc, item, map, mapPretty, mapLatest, map001, mob, morph, npc, quest, reactor, skill, sound, string, tamingmob, ui;

        void load_all()
        {
            printf("[*] Checking for base vs Data nx.\n");
            if (exists("Base.nx"))
            {
                printf("[*] Checking for base vs Data nx.\n");
                printf("[*] init base.nx\n");
                base = add_file("Base.nx");
                printf("[*] init character.nx\n");
                character = add_file("Character.nx");
                printf("[*] init effect.nx\n");
                effect = add_file("Effect.nx");
                printf("[*] init etc.nx\n");
                etc = add_file("Etc.nx");
                printf("[*] init item.nx\n");
                item = add_file("Item.nx");
                printf("[*] init map.nx\n");
                map = add_file("Map.nx");
                /*printf("[*] init mappretty.nx\n");
                mapPretty = add_file("MapPretty.nx");
                printf("[*] init maplatest.nx\n");
                mapLatest = add_file("MapLatest.nx");
                */
                printf("[*] init map001.nx\n");
                map001 = add_file("Map001.nx");
                printf("[*] init mob.nx\n");
                mob = add_file("Mob.nx");
                printf("[*] init morph.nx\n");
                morph = add_file("Morph.nx");
                printf("[*] init npc.nx\n");
                npc = add_file("Npc.nx");
                printf("[*] init quest.nx\n");
                quest = add_file("Quest.nx");
                printf("[*] init reactor.nx\n");
                reactor = add_file("Reactor.nx");
                printf("[*] init skill.nx\n");
                skill = add_file("Skill.nx");
                printf("[*] init sound.nx\n");
                sound = add_file("Sound.nx");
                printf("[*] init string.nx\n");
                string = add_file("String.nx");
                printf("[*] init tamingmob.nx\n");
                tamingmob = add_file("TamingMob.nx");
                printf("[*] init ui.nx\n");
                ui = add_file("UI.nx");
            }
            else if (exists("Data.nx"))
            {
                base = add_file("Data.nx");
                character = base["Character"];
                effect = base["Effect"];
                etc = base["Etc"];
                item = base["Item"];
                map = base["Map"];
                mob = base["Mob"];
                morph = base["Morph"];
                npc = base["Npc"];
                quest = base["Quest"];
                reactor = base["Reactor"];
                skill = base["Skill"];
                sound = base["Sound"];
                string = base["String"];
                tamingmob = base["TamingMob"];
                ui = base["UI"];
            }
            else
            {
                throw std::runtime_error("Failed to locate nx files.");
            }
        }
    }
}