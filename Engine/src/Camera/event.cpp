#include "camera.hpp"

void Camera::processMouseMovement(int deltaX, int deltaY) {
    constexpr double sensitivity = 0.1;
    Interval interval(-89.0, 89.0);
    yaw += deltaX * sensitivity;
    pitch += deltaY * sensitivity;
    pitch = interval.clamp(pitch);
    updateCameraVectors();
    pixel_cache.clear();
}

void Camera::processMouseScroll(int deltaY) {
    constexpr double zoom_factor = 0.1;
    Interval interval(-45.0, 45.0);
    zoom += deltaY * zoom_factor;
    zoom = interval.clamp(zoom);
    updateCameraVectors();
    pixel_cache.clear();
}
