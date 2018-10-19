#include "quadtree.h"

Quadtree::Quadtree(int x, int y, int width, int height){
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
}

void Quadtree::subdivide(){
    children = (Quadtree*)malloc(sizeof(Quadtree) * 4);
    int h = height / 2;
    int w = width / 2;
    children[0] = Quadtree(x, y, w, h);
    children[1] = Quadtree(x, y + h, w, h);
    children[2] = Quadtree(x + w, y, w, h);
    children[3] = Quadtree(x + w, y + h, w, h);
}

void Quadtree::printInfo()
{
    std::cout << "x = " << x << ", y = " << y << ", width = " << width << ", height = " << height << std::endl;
}

Quadtree *Quadtree::getChildren() const
{
    return children;
}
