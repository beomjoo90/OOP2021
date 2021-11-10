#pragma once
#include <functional>
#include "Clickable.h"
class ConfirmationPanel :
    public Panel
{
    Clickable* cancel;
    Clickable* okay;

    string result;
public:
    ConfirmationPanel(const Position& pos, int width, int height, GameObject* parent, std::function<void(void)> onCancel = []() {}, std::function<void(void)> onOkay = []() {})
        : Panel(" Do you want to exit?", pos, width, height, parent) {
        cancel = new Clickable("cancel", { 2, (height-2)/2+1 }, 6, 1, this, onCancel);
        okay = new Clickable("okay", { width/2 +2, (height-2)/2+1}, 6, 1, this, onOkay);
    }
    const string& getResponse() const { return result; }

    void update() {
        if (cancel->isActive() == false || okay->isActive() == false) setActive(false);
    }

};

