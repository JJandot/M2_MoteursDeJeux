#ifndef QUADTREE_H
#define QUADTREE_H

#include <iostream>

class Quadtree{
public:
    Quadtree(int x, int y, int width, int height);
    void subdivide();
    void printInfo();

    Quadtree *getChildren() const;

private:
    Quadtree *children;
    int x, y, width, height;
};

#endif // QUADTREE_H
