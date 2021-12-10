#include "Scene.h"

void Scene::update() {
    if (currentBlock->isAlive() == false) {
        map->removeChild(currentBlock);
        delete currentBlock;
        nextBlock->setParent(map);
        currentBlock = nextBlock;
        currentBlock->getOrAddComponent<BlockScript>()->start();

        nextBlockShape = BlockScript::ChooseShape();
        nextBlock = new GameObject{ next, "nextBlock", "block", nextBlockShape.shape.c_str(),
                nextBlockShape.dim, {3,1}, Position::zeros };
    }
    internalRemove();
    if (mapScript->isDone()) {
        isCompleted = true;
        return;
    }
    internalRender(Position::zeros, false);
    internalUpdate();
}
