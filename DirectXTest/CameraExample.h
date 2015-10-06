#ifndef CAMERA_H
#define CAMERA_H

#include "Vector.h"

const float kCamDist = 2.5f; // Default amount for the eye to be away from the camera's target
const CVector kWorldUp(0, 1, 0); // Direction of up (+Y axis) in world space

class Camera
{
public:
	Camera();

	void rotateY(float angle, const CPos &target); // Rotates around world Y-axis
	void pitch(float angle, const CPos &target); // Rotates around the camera's X-axis

	void setEye(const CPos &eyePos) { eye = eyePos; } // Sets the eye position of the camera
	void setTarget(const CPos &target); // Sets the camera to look at "target"

	// Returns what the camera's target is
	CPos getTarget() const { return eye + (forward * kCamDist); }

	// Data Access ***

	CPos getEye() const { return eye; }
	CVector getCamForward() const { return forward; } // Returns camera's forward vector
	CVector getCamUp() const { return up; } // Returns camera's up vector
	CVector getCamRight() const { return right; } // Returns camera's right vector

	// *** End Data Access

private:

	CVector right; // The local normalized axis that points to the "right"
	CVector up; // The local normalized axis that points "up"
	CVector forward; // The local normalized axis that points "forward"

	CPos eye; // Where we are looking from
};

// Externed camera
extern Camera *gCamera;

#endif
