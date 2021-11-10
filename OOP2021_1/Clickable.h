#pragma once
#include <functional>
#include "Panel.h"
class Clickable :
    public Panel
{
    function<void(void)> onClick;

    bool isInside(const Position& pos) const {
        auto myPos = getPos();
        return 0 <= pos.x && dim.x >= pos.x && 0 <= pos.y && dim.y >= pos.y;
    }

public:
    Clickable(const string& title, const Position& pos, int width, int height, GameObject* parent, std::function<void(void)> onClick = []() {} )
        : Panel("", pos, width, height, parent), onClick(onClick) {
        setShape(title.c_str());
    }

    void update()
    {
        if (input->getMouseButtonDown(0)) {
            auto pos = screen2Local(input->getMousePosition());
            if (isInside(pos)) {
                onClick();
                setActive(false);
            }
        }
    }

    void draw()
    {
        GameObject::draw();       
        Panel::draw();        
    }
};

