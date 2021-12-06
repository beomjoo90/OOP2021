#pragma once

#include "Position.h"
#include "GameObject.h"
#include "MoveScript.h"
#include "RendererEnabler.h"
#include "BlockScript.h"
#include "PanelScript.h"
#include "ScoreScript.h"
#include "MapScript.h"
#if 0
#include "Block.h"
#include "Score.h"
#include "Map.h"
#include "ConfirmationPanel.h"
#endif

struct BlockShape {
    string		shape;
    Dimension	dim;
};

class TetrisGame
{
    bool                isCompleted;
    GameObject*         banner;

    static BlockShape chooseShape() {
        static vector<BlockShape> candidates{
            { "\xDB\xDB \xDB \xDB", {2, 3}	},
            { "\xDB\xDB\xDB\xDB",	{2, 2}	},
            { "\xDB\xDB\xDB\xDB",	{4, 1}	},
            { "\xDB\xDB\xDB \xDB ", {2, 3}	},
            { " \xDB\xDB\xDB\xDB ", {2, 3}	},
            { " \xDB\xDB\xDB \xDB", {2, 3}	},
            { "\xDB \xDB\xDB \xDB", {2, 3}  }
        };
        return candidates[rand() % candidates.size()];
    }

#if 0
    Score* score;  // cache
    Map*                map;    // cache
    Panel*              nextPanel;
    Block*              currentBlock;
    Block*              nextBlock;
    ConfirmationPanel*  confirm;
#endif
    BlockShape          nextBlockShape;
    GameObject*         currentBlock;
    GameObject*         map;
    MapScript*          mapScript;
    GameObject*         next;
    GameObject*         nextBlock;


    vector<GameObject*> rootChildren;

public:

    TetrisGame() : isCompleted(false), currentBlock(nullptr), mapScript(nullptr), 
        map(nullptr), next(nullptr) {
        map = new GameObject("map", "panel", nullptr, { 10, 20 }, { 5,5 }, Position::zeros, nullptr);
        rootChildren.push_back(map);
        auto panelScript = map->getOrAddComponent<PanelScript>();
        panelScript->setTitle(" Tetris");
        mapScript = map->getOrAddComponent<MapScript>();

        auto blockShape = TetrisGame::chooseShape();
        currentBlock = new GameObject{ "currentBlock", "block", blockShape.shape.c_str(),
            blockShape.dim, {5,0},  Position::zeros, map };
        currentBlock->addComponent<BlockScript>();

        next = new GameObject("nextPanel", "panel", nullptr, { 10,5 } , { 25, 5 }, Position::zeros, nullptr);
        rootChildren.push_back(next);
        panelScript = next->getOrAddComponent<PanelScript>();
        panelScript->setTitle(" Next");

        nextBlockShape = TetrisGame::chooseShape();
        nextBlock = new GameObject{ "nextBlock", "block", nextBlockShape.shape.c_str(),
            nextBlockShape.dim, {3,1}, Position::zeros, next };

        auto score = new GameObject("scorePanel", "panel", nullptr, { 10,5 }, { 25, 15 },  Position::zeros, nullptr);
        rootChildren.push_back(score);
        panelScript = score->getOrAddComponent<PanelScript>();
        panelScript->setTitle(" Score");

        auto scoreScript = score->getOrAddComponent<ScoreScript>();

        mapScript->setScore( scoreScript );
    }

    bool isGameOver() const { return isCompleted; }

    void update() {
        if (currentBlock->isActive() == false) {
            map->remove(currentBlock);
            delete currentBlock;
            nextBlock->setParent(map);
            currentBlock = nextBlock;
            currentBlock->addComponent<BlockScript>();
            currentBlock->getComponent<BlockScript>()->reset();

            nextBlockShape = chooseShape();
            nextBlock = new GameObject{ "nextBlock", "block", nextBlockShape.shape.c_str(),
                    nextBlockShape.dim, {3,1}, Position::zeros, next };
        }
        if (mapScript->isDone()) {
            isCompleted = true;
            return;
        }

        for (auto child : rootChildren) child->internalUpdate();
        rootChildren.erase(
            std::remove_if(rootChildren.begin(), rootChildren.end(),
                [](auto child) { return child->isActive() == false; }),
            rootChildren.end());
        for (auto child : rootChildren) child->internalRemove();
        for (auto child : rootChildren) child->internalRender();
        
    }

#if 0
    void update() override {
        if (confirm && confirm->isActive() == false) {
            remove(confirm);
            delete confirm;
            confirm = nullptr;
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

