#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
class AxisAlignedBB {
public:
    AxisAlignedBB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) {
        this->minX = minX;
        this->minY = minY;
        this->minZ = minZ;
        this->maxX = maxX;
        this->maxY = maxY;
        this->maxZ = maxZ;
    }
     AxisAlignedBB* setBounds(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) {
        this->setMinX(minX);
        this->setMinY(minY);
        this->setMinZ(minZ);
        this->setMaxX(maxX);
        this->setMaxY(maxY);
        this->setMaxZ(maxZ);
        return this;
    }

     AxisAlignedBB* addCoord(float x, float y, float z) {
        float minX = this->getMinX();
        float minY = this->getMinY();
        float minZ = this->getMinZ();
        float maxX = this->getMaxX();
        float maxY = this->getMaxY();
        float maxZ = this->getMaxZ();

        if (x < 0) minX += x;
        if (x > 0) maxX += x;

        if (y < 0) minY += y;
        if (y > 0) maxY += y;

        if (z < 0) minZ += z;
        if (z > 0) maxZ += z;

        return new  AxisAlignedBB (minX, minY, minZ, maxX, maxY, maxZ);
    }

     AxisAlignedBB* grow(float x, float y, float z) {
        return new  AxisAlignedBB (this->getMinX() - x, this->getMinY() - y, this->getMinZ() - z, this->getMaxX() + x, this->getMaxY() + y, this->getMaxZ() + z);
    }

     AxisAlignedBB* expand(float x, float y, float z)

    {
        this->setMinX(this->getMinX() - x);
        this->setMinY(this->getMinY() - y);
        this->setMinZ(this->getMinZ() - z);
        this->setMaxX(this->getMaxX() + x);
        this->setMaxY(this->getMaxY() + y);
        this->setMaxZ(this->getMaxZ() + z);

        return this;
    }

      AxisAlignedBB* offset(float x, float y, float z) {
        this->setMinX(this->getMinX() + x);
        this->setMinY(this->getMinY() + y);
        this->setMinZ(this->getMinZ() + z);
        this->setMaxX(this->getMaxX() + x);
        this->setMaxY(this->getMaxY() + y);
        this->setMaxZ(this->getMaxZ() + z);
      
        return this;
    }

     AxisAlignedBB* shrink(float x, float y, float z) {
        return new AxisAlignedBB(this->getMinX() + x, this->getMinY() + y, this->getMinZ() + z, this->getMaxX() - x, this->getMaxY() - y, this->getMaxZ() - z);
    }

     AxisAlignedBB* contract(float x, float y, float z) {
        this->setMinX(this->getMinX() + x);
        this->setMinY(this->getMinY() + y);
        this->setMinZ(this->getMinZ() + z);
        this->setMaxX(this->getMaxX() - x);
        this->setMaxY(this->getMaxY() - y);
        this->setMaxZ(this->getMaxZ() - z);

        return this;
    }

     AxisAlignedBB* setBB(AxisAlignedBB bb) {
        this->setMinX(bb.getMinX());
        this->setMinY(bb.getMinY());
        this->setMinZ(bb.getMinZ());
        this->setMaxX(bb.getMaxX());
        this->setMaxY(bb.getMaxY());
        this->setMaxZ(bb.getMaxZ());
        return this;
    }

     AxisAlignedBB* getOffsetBoundingBox(float x, float y, float z) {
        return new AxisAlignedBB(this->getMinX() + x, this->getMinY() + y, this->getMinZ() + z, this->getMaxX() + x, this->getMaxY() + y, this->getMaxZ() + z);
    }

     float calculateXOffset(AxisAlignedBB bb, float x) {
        if (bb.getMaxY() <= this->getMinY() || bb.getMinY() >= this->getMaxY()) {
            return x;
        }
        if (bb.getMaxZ() <= this->getMinZ() || bb.getMinZ() >= this->getMaxZ()) {
            return x;
        }
        if (x > 0 && bb.getMaxX() <= this->getMinX()) {
            float x1 = this->getMinX() - bb.getMaxX();
            if (x1 < x) {
                x = x1;
            }
        }
        if (x < 0 && bb.getMinX() >= this->getMaxX()) {
            float x2 = this->getMaxX() - bb.getMinX();
            if (x2 > x) {
                x = x2;
            }
        }

        return x;
    }

     float calculateYOffset(AxisAlignedBB bb, float y) {
        if (bb.getMaxX() <= this->getMinX() || bb.getMinX() >= this->getMaxX()) {
            return y;
        }
        if (bb.getMaxZ() <= this->getMinZ() || bb.getMinZ() >= this->getMaxZ()) {
            return y;
        }
        if (y > 0 && bb.getMaxY() <= this->getMinY()) {
            float y1 = this->getMinY() - bb.getMaxY();
            if (y1 < y) {
                y = y1;
            }
        }
        if (y < 0 && bb.getMinY() >= this->getMaxY()) {
            float y2 = this->getMaxY() - bb.getMinY();
            if (y2 > y) {
                y = y2;
            }
        }

        return y;
    }

     float calculateZOffset(AxisAlignedBB bb, float z) {
        if (bb.getMaxX() <= this->getMinX() || bb.getMinX() >= this->getMaxX()) {
            return z;
        }
        if (bb.getMaxY() <= this->getMinY() || bb.getMinY() >= this->getMaxY()) {
            return z;
        }
        if (z > 0 && bb.getMaxZ() <= this->getMinZ()) {
            float z1 = this->getMinZ() - bb.getMaxZ();
            if (z1 < z) {
                z = z1;
            }
        }
        if (z < 0 && bb.getMinZ() >= this->getMaxZ()) {
            float z2 = this->getMaxZ() - bb.getMinZ();
            if (z2 > z) {
                z = z2;
            }
        }

        return z;
    }

     bool intersectsWith(AxisAlignedBB bb) {
        if (bb.getMaxY() > this->getMinY() && bb.getMinY() < this->getMaxY()) {
            if (bb.getMaxX() > this->getMinX() && bb.getMinX() < this->getMaxX()) {
                return bb.getMaxZ() > this->getMinZ() && bb.getMinZ() < this->getMaxZ();
            }
        }

        return false;
    }

     bool isVectorInside(glm::vec3 vector) {
        return vector.x >= this->getMinX() && vector.x <= this->getMaxX() && vector.y >= this->getMinY() && vector.y <= this->getMaxY() && vector.z >= this->getMinZ() && vector.z <= this->getMaxZ();

    }

     float getAverageEdgeLength() {
        return (this->getMaxX() - this->getMinX() + this->getMaxY() - this->getMinY() + this->getMaxZ() - this->getMinZ()) / 3;
    }

     bool isVectorInYZ(glm::vec3 vector) {
        return vector.y >= this->getMinY() && vector.y <= this->getMaxY() && vector.z >= this->getMinZ() && vector.z <= this->getMaxZ();
    }

     bool isVectorInXZ(glm::vec3 vector) {
        return vector.x >= this->getMinX() && vector.x <= this->getMaxX() && vector.z >= this->getMinZ() && vector.z <= this->getMaxZ();
    }

     bool isVectorInXY(glm::vec3 vector) {
        return vector.x >= this->getMinX() && vector.x <= this->getMaxX() && vector.y >= this->getMinY() && vector.y <= this->getMaxY();
    }



     float getMinX() {
        return minX;
    }


     void setMinX(float minX) {
        this->minX = minX;
    }


     float getMinY() {
        return minY;
    }


     void setMinY(float minY) {
        this->minY = minY;
    }


     float getMinZ() {
        return minZ;
    }


     void setMinZ(float minZ) {
        this->minZ = minZ;
    }


     float getMaxX() {
        return maxX;
    }


     void setMaxX(float maxX) {
        this->maxX = maxX;
    }


     float getMaxY() {
        return maxY;
    }


     void setMaxY(float maxY) {
        this->maxY = maxY;
    }


     float getMaxZ() {
        return maxZ;
    }


    void setMaxZ(float maxZ) {
        this->maxZ = maxZ;
    }

    //   AxisAlignedBB clone();

     float minX;
     float minY;
     float minZ;
     float maxX;
     float maxY;
     float maxZ;

     AxisAlignedBB() = default;
      static int floorFloat(float n) {
         int i = (int)n;

         return n >= i ? i : i - 1;
     }

      static int ceilFloat(float n) {
         int i = (int)(n + 1);
         return n >= i ? i : i - 1;
     }
      glm::vec3 getMidPoint() {
          return glm::vec3((getMinX() + getMaxX()) / 2.0f, (getMinY() + getMaxY()) / 2.0f, (getMinZ() + getMaxZ()) / 2.0f);
      }
};
