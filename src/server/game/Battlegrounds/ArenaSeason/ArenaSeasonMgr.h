/*
 * This file is part of the AzerothCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Affero General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _ARENASEASONMGR_H
#define _ARENASEASONMGR_H

#include "Common.h"
#include <vector>
#include <unordered_map>

enum ArenaSeasonState
{
    ARENA_SEASON_STATE_DISABLED    = 0,
    ARENA_SEASON_STATE_IN_PROGRESS = 1
};

enum ArenaSeasonRewardType
{
    ARENA_SEASON_REWARD_TYPE_ITEM = 1,
    ARENA_SEASON_REWARD_TYPE_ACHIEVEMENT = 2
};

// ArenaSeasonReward represents one reward, it can be an item or achievement.
struct ArenaSeasonReward
{
    ArenaSeasonReward() = default;

    // Item or acheivement entry.
    uint32 entry{0};

    ArenaSeasonRewardType type{ARENA_SEASON_REWARD_TYPE_ITEM};

    bool operator==(const ArenaSeasonReward& other) const
    {
        return entry == other.entry && type == other.type;
    }
};

struct ArenaSeasonRewardGroup
{
    ArenaSeasonRewardGroup() = default;

    uint8 season{0};

    float minPctCriteria{0};
    float maxPctCriteria{0};

    uint32 rewardMailTemplateID{0};

    std::vector<ArenaSeasonReward> itemRewards{};
    std::vector<ArenaSeasonReward> achievementRewards{};

    bool operator<(const ArenaSeasonRewardGroup& other) const
    {
        return this->minPctCriteria < other.minPctCriteria;
    }

    bool operator==(const ArenaSeasonRewardGroup& other) const
    {
        return minPctCriteria == other.minPctCriteria &&
               maxPctCriteria == other.maxPctCriteria &&
               itemRewards == other.itemRewards &&
               achievementRewards == other.achievementRewards;
    }
};

class ArenaSeasonMgr
{
public:
    static ArenaSeasonMgr* instance();

    typedef std::unordered_map<uint8, std::vector<ArenaSeasonRewardGroup>> ArenaSeasonRewardGroupsBySeasonContainer;

    // Loading functions
    void LoadRewards();
    void LoadActiveSeason();

    // Season management functions
    void ChangeCurrentSeason(uint8 season);
    uint8 GetCurrentSeason() { return _currentSeason; }

    void SetSeasonState(ArenaSeasonState state);
    ArenaSeasonState GetSeasonState() { return _currentSeasonState; }

    // Season completion functions
    void RewardTeamsForTheSeason();
    bool CanDeleteArenaTeams();
    void DeleteArenaTeams();

private:
    uint16 GameEventForArenaSeason(uint8 season);
    void BroadcastUpdatedWorldState();

    ArenaSeasonRewardGroupsBySeasonContainer _arenaSeasonRewardGroupsStore;

    uint8 _currentSeason;
    ArenaSeasonState _currentSeasonState;
};

#define sArenaSeasonMgr ArenaSeasonMgr::instance()

#endif // _ARENASEASONMGR_H
