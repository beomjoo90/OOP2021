#pragma once

#include "Position.h"
#include "GameObject.h"
#include "MoveScript.h"
#include "BlockScript.h"
#include "PanelRenderer.h"
#include "ScoreScript.h"
#include "MapScript.h"

using namespace std;

class Scene : public GameObject
{
    bool                isCompleted;

    BlockShape          nextBlockShape;
    GameObject*         currentBlock;
    GameObject*         map;
    MapScript*          mapScript;
    GameObject*         next;
    GameObject*         nextBlock;
    GameObject*         score;

public:

    Scene() : GameObject{ nullptr, "root", "root", nullptr, {1, 1}, {0, 0}, Position::zeros },
        isCompleted(false), currentBlock(nullptr), mapScript(nullptr), map(nullptr), next(nullptr) 
    {
        map = new GameObject( this, "map", "panel", nullptr, { 10, 20 }, { 5,5 }, Position::zeros);
        map->getOrAddComponent<PanelRenderer>()->setTitle(" Tetris ");
        mapScript = map->getOrAddComponent<MapScript>();

        auto blockShape = BlockScript::ChooseShape();
        currentBlock = new GameObject{ map, "currentBlock", "block", blockShape.shape.c_str(),
            blockShape.dim, {5,0},  Position::zeros};
        currentBlock->addComponent<BlockScript>();

        next = new GameObject(this, "nextPanel", "panel", nullptr, { 10,5 } , { 25, 5 }, Position::zeros);
        next->getOrAddComponent<PanelRenderer>()->setTitle(" Next ");

        nextBlockShape = BlockScript::ChooseShape();
        nextBlock = new GameObject{ next, "nextBlock", "block", nextBlockShape.shape.c_str(),
            nextBlockShape.dim, {3,1}, Position::zeros };

        score = new GameObject(this, "scorePanel", "panel", nullptr, { 10,5 }, { 25, 15 },  Position::zeros);
        score->getOrAddComponent<PanelRenderer>()->setTitle(" Score ");
        auto scoreValue = new GameObject(score, "scoreValue", "value", "0", {3,1}, {3, 2}, Position::zeros);        
        auto scoreScript = scoreValue->getOrAddComponent<ScoreScript>();

        mapScript->setScore( scoreScript );

        internalStart();
    }

    bool isGameOver() const { return isCompleted; }

    void update() override;

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

