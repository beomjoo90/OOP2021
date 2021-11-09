#pragma once

#include <fstream>
#include "Position.h"
#include "MapEditor.h"
#include "ConfirmationPanel.h"

using namespace std;

class TetrisEditor :
    public Panel
{
    bool        isCompleted;
    MapEditor*  map;    // cache
    Clickable*  save;
    Clickable*  terminate;
    Clickable*  load;
    
public:
    TetrisEditor()
        : Panel("", Position{ 1, 1 }, 77, 30, nullptr),
        isCompleted(false), map(nullptr), save(nullptr), terminate(nullptr)
    {
        map = new MapEditor{ Position{5,5}, 10, 20, this };
        save = new Clickable("save", { 20,  10 }, 5, 1, this);
        terminate = new Clickable("exit", { 20, 15 }, 5, 1, this);
        load = new Clickable("load", { 20, 20 }, 5, 1, this);
    }

    bool isGameOver() const { return isCompleted; }

    void update() override {
        if (save->isClicked()) {
            map->save();
            save->setClicked(false);
        }
        else if (terminate->isClicked()) {
            isCompleted = true;
        }
        else if (load->isClicked()) {
            map->load();
            load->setClicked(false);
        }
    }
};

