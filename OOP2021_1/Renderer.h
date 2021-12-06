#pragma once
#include "Screen.h"
#include "Component.h"

class Renderer :
    public Component
{
    char*       shape;
    int         capacity;

    

protected:
    Transform*  transform;
    Screen*     screen;
    Dimension	dim;

public:
    Renderer(GameObject* gameObject, const char* face, const Dimension& dim)
        :   Component(gameObject), transform( getTransform()), shape(nullptr), dim(dim), capacity((size_t)dim.x* dim.y),
            screen(Screen::GetInstance())
    {
        if (capacity == 0) return;

        shape = new char[capacity];
        memset(shape, ' ', capacity);
        if (face == nullptr) return;

        strncpy(shape, face, min(strlen(face), capacity));
    }
    virtual ~Renderer() {
        if (shape == nullptr) return;
        delete[] shape;
    }

    int pos2Offset(const Position& pos) const { return pos.y * dim.x + pos.x; }
    Position offset2Pos(int offset) const { return Position{ offset % dim.x, offset / dim.x }; }

    int getWidth() const { return dim.x; }
    int getHeight() const { return dim.y; }

    int getCapacity() const { return capacity; }

	const Dimension& getDimension() const { return dim; } // why getDimension returns reference value?
	void setDimension(const Dimension& dim) { this->dim = dim; }
	
    virtual void draw();

	const char* getShape() const { return shape; }
	void setShape(const char* face) {
		memset(shape, ' ', capacity);
		if (face == nullptr || face[0] == '\0') return;
		strncpy(shape, face, min(capacity, strlen(face)));
	}
	void setShape(char shape, int offset) {
        if (!shape || offset < 0 || offset >= capacity) return;
		this->shape[offset] = shape;
	}
	void setShape(char shape, const Position& pos) { setShape(shape, pos2Offset(pos)); }

    virtual void render();
};

