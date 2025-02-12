#include "UFOAnimator.h"

#include <cmath>
#include <numbers>
#include "drawable.h"

void UFOAnimator::Animate(float deltaTime, Drawable &drawable)
{
    static float mainAngle = 0.0f;
    mainAngle += deltaTime;
    if (mainAngle > 2.0f * std::numbers::pi_v<float>) {
        mainAngle -= 2.0f * std::numbers::pi_v<float>;
    }
    float radius = 10.0f;
    float speed = 1.0f;     // Speed of movement in the circle

    // Compute new position using angle over time
    float angle = speed * mainAngle;
    float newX = radius * cosf(angle);
    float newZ = radius * sinf(angle);

    // Rotations
    float ZRotationSpeed = 120.0f; // Rotates 90° per second
    float newZRotation = drawable.GetZRotation() + ZRotationSpeed * deltaTime;

    float tiltAmount = 10.0f;
    float tiltSpeed = 2.0f;
    // -90, because original model of ufo is rotated
    float newXRotation = -90.0 + tiltAmount * sinf(tiltSpeed * mainAngle);
    float newYRotation = tiltAmount * cosf(tiltSpeed * mainAngle);

    drawable.SetXPosition(newX);
    drawable.SetZPosition(newZ);
    drawable.SetYRotation(std::fmod(newYRotation, 360.0f));
    drawable.SetXRotation(newXRotation);
    drawable.SetZRotation(newZRotation);
}
