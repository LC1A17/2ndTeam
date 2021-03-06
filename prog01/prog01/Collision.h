#pragma once

#include "CollisionPrimitive.h"

class Collision
{
public:
	//球と平面の当たり判定
	static bool CheckSphere2Plane(const Sphere& sphere, const Plane& plane, DirectX::XMVECTOR* inter = nullptr);
	
	//点と三角形の最近接点を求める
	static void ClosestPtPoint2Triangle(const DirectX::XMVECTOR& point, const Triangle& triangle, DirectX::XMVECTOR* closest);
	
	//球と法線付き三角形の当たり判定
	static bool CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle, DirectX::XMVECTOR* inter = nullptr);
};