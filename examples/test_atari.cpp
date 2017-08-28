// Copyright (c) 2017 Baidu Inc. All Rights Reserved.

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0

// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "games/arcade/arcade_simulator.h"

using namespace simulator::arcade_game;
using namespace simulator;

DECLARE_int32(context);

int main(int argc, char** argv) {
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    std::string ale_rom = "/tmp/breakout.bin";
    std::shared_ptr<ArcadeGame> atari(ArcadeGame::create(ale_rom));
    atari->reset_game();

    auto num_actions = atari->get_num_actions();
    int act_rep = FLAGS_context;

    double reward = 0;
    double reward_per_game = 0;
    double r = 0;

    for (int i = 0; i < 100; i++) {
        auto game_over_str =
            GameSimulator::decode_game_over_code(atari->game_over());
        if (game_over_str != "alive" && game_over_str != "lost_life") {
            LOG(INFO) << "game over because of " + game_over_str;
            atari->reset_game();
            reward_per_game = 0;
            continue;
        }
        atari->show_screen(reward_per_game);

        StatePacket state;
        // You can choose to store the immediate reward r in the state
        // Or you can just ignore the first argument
        atari->get_state_data(r, state);

        StatePacket actions;
        actions.add_buffer_id("action", {util::get_rand_ind(num_actions)});
        r = atari->take_actions(actions, act_rep);
        reward_per_game += r;
        reward += r;
        LOG(INFO) << r;
    }

    LOG(INFO) << "total reward " << reward;
    return 0;
}
