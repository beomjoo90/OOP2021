#pragma once
#include "Clickable.h"
class ConfirmationPanel :
    public Panel
{
    Clickable* cancel;
    Clickable* okay;

    string result;
public:
    ConfirmationPanel(const Position& pos, int width, int height, GameObject* parent)
        : Panel(" Do you want to exit?", pos, width, height, parent) {
        cancel = new Clickable("cancel", { 2, (height-2)/2+1 }, 6, 1, this);
        okay = new Clickable("okay", { width/2 +2, (height-2)/2+1}, 6, 1, this);
    }
    const string& getResponse() const { return result; }

    void update() {
        if (cancel->isClicked()) {
            result = "cancel";
            setActive(false);
        }
        if (okay->isClicked()) {
            result = "okay";
            setActive(false);
        }
    }

};

