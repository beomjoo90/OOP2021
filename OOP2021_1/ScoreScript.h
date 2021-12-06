#pragma once
#include "GameObject.h"
#include "Behaviour.h"
class ScoreScript :
    public Behaviour
{
    int value;
    char buffer[3];

public:
	ScoreScript(GameObject* gameObject) : Behaviour(gameObject), value(0) {
		sprintf(buffer, "%3d", value % 1000);
	}

	void update() override
	{
		sprintf(buffer, "%3d", value % 1000);
		renderer->setShape(buffer);
	}

	void setValue(int value) { this->value = value; }
	int getValue() const { return value; }
};

