#pragma once
#include "Panel.h"
class Clickable :
    public Panel
{
    bool clicked;

    bool isInside(const Position& pos) const {
        auto myPos = getPos();
        return 0 <= pos.x && dim.x >= pos.x && 0 <= pos.y && dim.y >= pos.y;
    }

public:
    Clickable(const string& title, const Position& pos, int width, int height, GameObject* parent) 
        : Panel("", pos, width, height, parent), clicked(false) {
        setShape(title.c_str());
    }

    bool isClicked() const { return clicked; }

    void update()
    {
        if (input->getMouseButtonDown(0)) {
            auto pos = screen2Local(input->getMousePosition());
            if (isInside(pos)) {
                clicked = true;
            }
        }
    }

    void draw()
    {
        GameObject::draw();
        screen->render();
        Panel::draw();
        screen->render();
    }
};

