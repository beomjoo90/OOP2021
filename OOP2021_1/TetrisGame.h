#pragma once

#include "Position.h"
#include "GameObject.h"
#include "MoveScript.h"
#include "RendererEnabler.h"
#if 0
#include "Block.h"
#include "Score.h"
#include "Map.h"
#include "ConfirmationPanel.h"
#endif

class TetrisGame
{
    bool                isCompleted;
    GameObject*         banner;

#if 0
    Score* score;  // cache
    Map*                map;    // cache
    Panel*              nextPanel;
    Block*              currentBlock;
    Block*              nextBlock;
    ConfirmationPanel*  confirm;
#endif


public:

    TetrisGame() : isCompleted(false), banner(nullptr) {
        banner = new GameObject("test", "test", "hello world", { (int)strlen("hello world"), 1 });

        auto child = new GameObject("banner child", "test", "I am banners child", { (int)strlen("I am banners child"), 1 }, { 2, 2 }, Position::zeros, banner);
        banner->addComponent<MoveScript>();
        child->addComponent<RendererEnabler>();
    }

    bool isGameOver() const { return isCompleted; }

    void update() {
        banner->internalUpdate();
        banner->internalRender();
    }

#if 0
    TetrisGame() 
        : Panel("", Position{ 1, 1 }, 77, 30, nullptr), 
        isCompleted(false), score(nullptr), nextPanel(nullptr), 
        currentBlock(nullptr), nextBlock(nullptr),
        confirm(nullptr)
    {
        map = new Map{ Position{5,5}, 10, 20, this };
        auto blockShape = Block::chooseShape();
        currentBlock = new Block{ blockShape.shape.c_str(), 
            Position{5,0},
            blockShape.dim,
            map
        }; // make a block to be a child of map object
        currentBlock->setMap(map);

        blockShape = Block::chooseShape();
        nextPanel = new Panel{ " Next", Position{25, 5}, 10, 5, this };
        nextBlock = new Block{ blockShape.shape.c_str(), Position{ 3, 1 },
                blockShape.dim,
                nextPanel,
                false, // make interactable false
        }; // map a nextblock to place inside the next panel        

        score = new Score{ Position{4, 2}, 
            new Panel{ " Score", Position{25, 20}, 10, 5, this } 
        }; // create a ScorePanel and make a "score" its child.

        map->setScore(score);
    }

    bool isGameOver() const { return isCompleted; }

    void update() override {
        
        children.erase(
            std::remove_if(children.begin(), children.end(), 
                [](auto child) { return child->isActive() == false; }),
            children.end());
        

        if (currentBlock->isActive() == false) {
            map->remove(currentBlock);
            delete currentBlock;
            nextPanel->remove(nextBlock);
            currentBlock = nextBlock;
            currentBlock->setInteractable(true);
            currentBlock->setMap(map);
            currentBlock->setParent(map);
            currentBlock->reset();

            auto blockShape = Block::chooseShape();
            nextBlock = new Block{ blockShape.shape.c_str(),
                Position{ 3, 1 },
                blockShape.dim,
                nextPanel,
                false
            };
        }
        if (confirm && confirm->isActive() == false) {
            remove(confirm);
            delete confirm;
            confirm = nullptr;
        }
        if (map->isDone()) {
            isCompleted = true;
            return;
        }
        if (input->getKey(VK_ESCAPE)) {
            auto dim = getDimension();
            confirm = new ConfirmationPanel{ getPos() + Position{ dim.x / 2 - 25/2, dim.y / 2 - 5/2}, 25, 5, this,
                [&]() { map->setFreeze(false); }, // if clicked cancel
                [&]() { isCompleted = true; } // if clicked okay
            };
            map->setFreeze(true);
        }
        
    }
#endif
};

