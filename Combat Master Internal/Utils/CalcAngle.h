#pragma once

Vector2 CalcAngle(Vector3 src, Vector3 dst)
{
	Vector2 angle;
	angle.x = -atan2f(dst.x - src.x, dst.y - src.y) / IM_PI * 180.0f + 180.0f;
	angle.y = asinf((dst.z - src.z) / Vector3::Distance(src, dst)) * 180.0f / IM_PI;

	return angle;
}