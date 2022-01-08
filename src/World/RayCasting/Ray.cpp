#include "Ray.h"

#include "AxisPlane.h"

Ray::Ray(glm::vec3 position, glm::vec3 direction, World& world, float reach = 10.0f) {
  successful = false;

  std::array<AxisPlane, 3> planes = {{
     AxisPlane{{1, 0, 0}, position, direction},
     AxisPlane{{0, 1, 0}, position, direction},
     AxisPlane{{0, 0, 1}, position, direction},
  }};

  std::sort(planes.begin(), planes.end());
  glm::vec3 prevClosestHit = position;  // the player might be inside a block

  while (!successful && planes[0].getHitDistance() <= reach) {
    // todo check the distance between the two hit positions
    std::optional<glm::ivec3> maybeBlockPosition =
       AxisPlane::rayHitsToBlockPosition(planes[0].getHitPosition(), prevClosestHit);
    prevClosestHit = planes[0].getHitPosition();


    if (!maybeBlockPosition.has_value() || !World::isValidBlockPosition(maybeBlockPosition.value())) {
      planes[0].advanceOffset();
      std::sort(planes.begin(), planes.end());
      continue;
    }

    glm::vec3 blockPosition = maybeBlockPosition.value();

    BlockData block = world.getBlockAt(blockPosition);
    if (block.type == BlockData::BlockType::air) {
      planes[0].advanceOffset();
      std::sort(planes.begin(), planes.end());
    } else {
      successful = true;
      hitTarget = {blockPosition, block};
    }
  }
}
