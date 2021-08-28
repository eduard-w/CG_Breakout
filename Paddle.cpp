#include "Paddle.hpp"
#include "GameObject.hpp"

Paddle::Paddle() : GameObject{ "cube.obj", glm::vec3{0,0,-2}, glm::vec3{1,1,0.1}}
{
}
