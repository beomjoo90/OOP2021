#pragma once

#include "Position.h"
#include "Block.h"
#include "Score.h"
#include "Map.h"

class TetrisGame :
    public Panel
{
    bool        isCompleted;
    Score*      score;  // cache
    Map*        map;    // cache

public:
    TetrisGame() 
        : Panel("", Position{ 1, 1 }, 77, 30, nullptr), 
        isCompleted(false), score(nullptr)
    {
        map = new Map{ Position{5,5}, 10, 20, this };
        auto block = new Block{ " \xdb \xdb\xdb\xdb", 
            Position{5,0},
            Dimension{3, 2},
            map
        }; // make a block to be a child of map object
        new Panel{ " Next", Position{25, 5}, 10, 5, this };
        score = new Score{ Position{4, 2}, 
            new Panel{ " Score", Position{25, 20}, 10, 5, this } 
        }; // create a ScorePanel and make a "score" its child.
        map->setScore(score);
        block->setMap(map);
    }

    bool isGameOver() const { return isCompleted; }

    void update() override {
        if (map->isDone()) {
            isCompleted = true;
            return;
        }
        if (input->getKey(VK_ESCAPE)) 
            isCompleted = true;
    }
};

