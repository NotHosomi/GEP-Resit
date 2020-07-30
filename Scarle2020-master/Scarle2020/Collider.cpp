#include "pch.h"
#include "Collider.h"

Collider::Collider(float _x, float _y, float _w, float _h)
{
    x = _x;
    y = _y;
    width = _w;
    height = _h;
}

bool Collider::checkOverlap(float x_, float y_) const
{
    return (x_ >= this->x && x_ <= this->x + this->width) &&
        (y_ >= this->y && y_ <= this->y + this->height);
}

bool Collider::checkOverlap(const Collider& rhs) const
{
    const auto& lhs = *this;

    bool x_overlap = isBetween(lhs.x, rhs.x, rhs.x + rhs.width) ||
        isBetween(rhs.x, lhs.x, lhs.x + lhs.width);

    bool y_overlap = isBetween(lhs.y, rhs.y, rhs.y + rhs.height) ||
        isBetween(rhs.y, lhs.y, lhs.y + lhs.height);

    return x_overlap && y_overlap;
}

bool Collider::isBetween(float value, float min, float max) const
{
    return (value >= min) && (value <= max);
}