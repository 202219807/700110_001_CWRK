#pragma once

#define _GRAVITATIONAL_ACCELERATION    -9.80f
#define _DRAG_COEFFICIENT				0.40f
#define _RESTITUTION					1.0f
#define _TIMESTEP						0.01f
#define _TERRAIN_LEVEL					1.00f
#define _NUM_OF_AGENTS_PER_TEAM			4
#define _NUM_TOWERS						4
#define _LOC_TOWERS						\
{										\
  vec3(21.0f, 0.0f, 8.0f),				\
  vec3(8.0f, 0.0f, 20.0f),				\
  vec3(35.0f, 0.0f, 35.0f),				\
  vec3(15.0f, 0.0f, 37.0f)				\
}
#define _NUM_SPAWN_BASES				2
#define _LOC_SPAWN_BASES				\
{										\
  vec3(4.0f, 1.0f, 40.0f),				\
  vec3(40.0f, 1.0f, 5.0f)				\
}
