// Header that implements Axis-Aligned Bounding Box (AABB) collision detection

#if !defined(RECT_INT_TYPE)
#define AABB_INT_TYPE int
#endif

typedef enum
{
    IntersectInclusive, // common points on the edge count
    IntersectExclusive,
} AABB_INTERSECT_TYPE;

typedef struct
{
    AABB_INT_TYPE minX;
    AABB_INT_TYPE minY;

    AABB_INT_TYPE maxX;
    AABB_INT_TYPE maxY;
} Rect;

AABB_INT_TYPE aabb_min(AABB_INT_TYPE a, AABB_INT_TYPE b)
{
    return a < b ? a : b;
}

AABB_INT_TYPE aabb_max(AABB_INT_TYPE a, AABB_INT_TYPE b)
{
    return a > b ? a : b;
}

// ---------1----4---4---3---1----3----2----2---
bool intersect_range(AABB_INT_TYPE min1, AABB_INT_TYPE max1, AABB_INT_TYPE min2, AABB_INT_TYPE max2, AABB_INTERSECT_TYPE intersectType)
{
    if (IntersectInclusive == intersectType)
    {
        return aabb_max(min1, min2) <= aabb_min(max1, max2);
    }
    else if (IntersectExclusive == intersectType)
    {
        return aabb_max(min1, min2) < aabb_min(max1, max2);
    }
}

bool intersect_rect(const Rect r1, const Rect r2, AABB_INTERSECT_TYPE intersectType)
{
    return intersect_range(r1.minX, r1.maxX, r2.minX, r2.maxX, intersectType) &&
           intersect_range(r1.minY, r1.maxY, r2.minY, r2.maxY, intersectType);
}
