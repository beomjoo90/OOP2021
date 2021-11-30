#pragma once
#include "Screen.h"
#include "Component.h"

class Renderer :
    public Component
{
    string      shape;
    Dimension	dim;
    int         capacity;
    Transform*  transform;
    Screen*     screen;

public:
    Renderer(GameObject* gameObject, const char* face, const Dimension& dim)
        :   Component(gameObject), transform( getTransform()), shape(face), dim(dim), capacity((size_t)dim.x* dim.y),
            screen(Screen::GetInstance())
    {
        if (face == nullptr || shape == "") {
            shape.assign(capacity, ' ');
        }
    }
    virtual ~Renderer() {}

    int getWidth() const { return dim.x; }
    int getHeight() const { return dim.y; }

    int getCapacity() const { return capacity; }

	const Dimension& getDimension() const { return dim; } // why getDimension returns reference value?
	void setDimension(const Dimension& dim) { this->dim = dim; }

	
	virtual void draw() { screen->draw(local2Screen(), shape, dim); }

	const char* getShape() const { return shape; }
	void setShape(const char* face) {
		memset(shape, ' ', sizeof(char) * capacity);
		if (face == nullptr || face[0] == '\0') {
			return;
		}
		strncpy(shape, face, min(capacity, strlen(face)));
	}
	void setShape(char shape, int offset) {
		if (offset < 0 || offset >= capacity) return;
		this->shape[offset] = shape;
	}
	void setShape(char shape, const Position& pos) { setShape(shape, pos2Offset(pos)); }


};

