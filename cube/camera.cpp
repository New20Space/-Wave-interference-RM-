#include <GL/glew.h>
#include <GL/freeglut.h>
#include <windows.h>
#define _USE_MATH_DEFINES
#include <math.h>

#include "camera.h"

struct SCamera camera = { 10*0.3,100*0.3,1.7+10,40,270 };

void Camera_Apply() {
	glRotatef(-camera.Xrot, 1, 0, 0);
	glRotatef(-camera.Zrot, 0, 0, 1);
	glTranslatef(-camera.x, -camera.y, -camera.z);
}
void Camera_Rotation(float xAngle, float zAngle)
{
	camera.Zrot += zAngle;
	if (camera.Zrot < 0)camera.Zrot += 360;
	if (camera.Zrot > 360)camera.Zrot -= 360;

	camera.Xrot += xAngle;
	if (camera.Xrot < 0)camera.Xrot = 0;
	if (camera.Xrot > 180)camera.Xrot = 180;
}
void Camera_AutoMoveByMouse(int centerX, int centerY, float speed) {

	POINT cur;
	POINT base = { centerX, centerY };
	GetCursorPos(&cur);
	Camera_Rotation((base.y - cur.y) * speed, (base.x - cur.x) * speed);
	SetCursorPos(base.x,base.y);
	
}
void Camera_MoveDirection(int vMove, int hMove,float speed) {
	float ang = -camera.Zrot / 180 * M_PI;
	if (vMove > 0)
		ang += hMove > 0 ? M_PI_4 : (hMove < 0 ? -M_PI_4 :0);
	if (vMove < 0)
		ang += M_PI + (hMove > 0 ? -M_PI_4 : (hMove < 0 ? M_PI_4 : 0));

	if (vMove == 0) {
		ang += hMove > 0 ? M_PI_2 : -M_PI_2;
		if (hMove == 0) {
			speed = 0;
		}
	}
	if (speed != 0) {
		camera.x += sin(ang) * speed;
		camera.y += cos(ang) * speed;

	}

}
void Camera_MoveUp(int Move, float speed) {
	if (Move>0) {
		camera.z += speed;
	}
	else if (Move<0) {
		camera.z -= speed;
	}
}