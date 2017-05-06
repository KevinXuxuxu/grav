#include <cstdio>
#include <cstring>

#include "body.hpp"

#include "octree.hpp"
#include "gravity.hpp"

Body *OctreeNode::auxbody = new Body[N];