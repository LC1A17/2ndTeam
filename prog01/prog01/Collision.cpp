#include "Collision.h"

using namespace DirectX;

<<<<<<< HEAD
bool Collision::CheckSphere2Plane(const Sphere& sphere, const Plane& plane,
	DirectX::XMVECTOR* inter)
{
	//座標系の原点から球の中心座標への距離
	XMVECTOR distV = XMVector3Dot(sphere.center, plane.nomal);
	//平面の原点距離を減算することで、平面と球の中心との距離が出る
	float dist = distV.m128_f32[0] - plane.distance;
=======
bool Collision::CheckSphere2Plane(const Sphere& sphere, const Plane& plane, DirectX::XMVECTOR* inter)
{
	//座標系の原点から球の中心座標への距離
	XMVECTOR distV = XMVector3Dot(sphere.center, plane.nomal);
	
	//平面の原点距離を減算することで、平面と球の中心との距離が出る
	float dist = distV.m128_f32[0] - plane.distance;
	
>>>>>>> e29bad67696d15a8f797935b7f879313ababe24d
	//距離の絶対値が半径より大きければ当たっていない
	if (fabsf(dist) > sphere.radius)
	{
		return false;
	}
<<<<<<< HEAD
=======
	
>>>>>>> e29bad67696d15a8f797935b7f879313ababe24d
	//擬似交点を計算
	if (inter)
	{
		//平面上の最近接点を擬似交点とする
		*inter = -dist * plane.nomal + sphere.center;
	}

	return true;
}

<<<<<<< HEAD
void Collision::ClosestPtPoint2Triangle(const DirectX::XMVECTOR& point,
	const Triangle& triangle, DirectX::XMVECTOR* closest)
{
	// pointがp0の外側の頂点領域の中にあるかどうかチェック
=======
void Collision::ClosestPtPoint2Triangle(const DirectX::XMVECTOR& point, const Triangle& triangle, DirectX::XMVECTOR* closest)
{
	//pointがp0の外側の頂点領域の中にあるかどうかチェック
>>>>>>> e29bad67696d15a8f797935b7f879313ababe24d
	XMVECTOR p0_p1 = triangle.p1 - triangle.p0;
	XMVECTOR p0_p2 = triangle.p2 - triangle.p0;
	XMVECTOR p0_pt = point - triangle.p0;

	XMVECTOR d1 = XMVector3Dot(p0_p1, p0_pt);
	XMVECTOR d2 = XMVector3Dot(p0_p2, p0_pt);

	if (d1.m128_f32[0] <= 0.0f && d2.m128_f32[0] <= 0.0f)
	{
		// p0が最近傍
		*closest = triangle.p0;
		return;
	}

<<<<<<< HEAD
	// pointがp1の外側の頂点領域の中にあるかどうかチェック
=======
	//pointがp1の外側の頂点領域の中にあるかどうかチェック
>>>>>>> e29bad67696d15a8f797935b7f879313ababe24d
	XMVECTOR p1_pt = point - triangle.p1;

	XMVECTOR d3 = XMVector3Dot(p0_p1, p1_pt);
	XMVECTOR d4 = XMVector3Dot(p0_p2, p1_pt);

	if (d3.m128_f32[0] >= 0.0f && d4.m128_f32[0] <= d3.m128_f32[0])
	{
		// p1が最近傍
		*closest = triangle.p1;
		return;
	}

<<<<<<< HEAD
	// pointがp0_p1の辺領域の中にあるかどうかチェックし、あればpointのp0_p1上に対する射影を返す
	float vc = d1.m128_f32[0] * d4.m128_f32[0] - d3.m128_f32[0] * d2.m128_f32[0];
=======
	//pointがp0_p1の辺領域の中にあるかどうかチェックし、あればpointのp0_p1上に対する射影を返す
	float vc = d1.m128_f32[0] * d4.m128_f32[0] - d3.m128_f32[0] * d2.m128_f32[0];
	
>>>>>>> e29bad67696d15a8f797935b7f879313ababe24d
	if (vc <= 0.0f && d1.m128_f32[0] >= 0.0f && d3.m128_f32[0] <= 0.0f)
	{
		float v = d1.m128_f32[0] / (d1.m128_f32[0] - d3.m128_f32[0]);
		*closest = triangle.p0 + v * p0_p1;
		return;
	}

<<<<<<< HEAD
	// pointがp2の外側の頂点領域の中にあるかどうかチェック
=======
	//pointがp2の外側の頂点領域の中にあるかどうかチェック
>>>>>>> e29bad67696d15a8f797935b7f879313ababe24d
	XMVECTOR p2_pt = point - triangle.p2;

	XMVECTOR d5 = XMVector3Dot(p0_p1, p2_pt);
	XMVECTOR d6 = XMVector3Dot(p0_p2, p2_pt);
<<<<<<< HEAD
=======
	
>>>>>>> e29bad67696d15a8f797935b7f879313ababe24d
	if (d6.m128_f32[0] >= 0.0f && d5.m128_f32[0] <= d6.m128_f32[0])
	{
		*closest = triangle.p2;
		return;
	}

<<<<<<< HEAD
	// pointがp0_p2の辺領域の中にあるかどうかチェックし、あればpointのp0_p2上に対する射影を返す
	float vb = d5.m128_f32[0] * d2.m128_f32[0] - d1.m128_f32[0] * d6.m128_f32[0];
=======
	//pointがp0_p2の辺領域の中にあるかどうかチェックし、あればpointのp0_p2上に対する射影を返す
	float vb = d5.m128_f32[0] * d2.m128_f32[0] - d1.m128_f32[0] * d6.m128_f32[0];
	
>>>>>>> e29bad67696d15a8f797935b7f879313ababe24d
	if (vb <= 0.0f && d2.m128_f32[0] >= 0.0f && d6.m128_f32[0] <= 0.0f)
	{
		float w = d2.m128_f32[0] / (d2.m128_f32[0] - d6.m128_f32[0]);
		*closest = triangle.p0 + w * p0_p2;
		return;
	}

<<<<<<< HEAD
	// pointがp1_p2の辺領域の中にあるかどうかチェックし、あればpointのp1_p2上に対する射影を返す
	float va = d3.m128_f32[0] * d6.m128_f32[0] - d5.m128_f32[0] * d4.m128_f32[0];
=======
	//pointがp1_p2の辺領域の中にあるかどうかチェックし、あればpointのp1_p2上に対する射影を返す
	float va = d3.m128_f32[0] * d6.m128_f32[0] - d5.m128_f32[0] * d4.m128_f32[0];
	
>>>>>>> e29bad67696d15a8f797935b7f879313ababe24d
	if (va <= 0.0f && (d4.m128_f32[0] - d3.m128_f32[0]) >= 0.0f && (d5.m128_f32[0] - d6.m128_f32[0]) >= 0.0f)
	{
		float w = (d4.m128_f32[0] - d3.m128_f32[0]) / ((d4.m128_f32[0] - d3.m128_f32[0]) + (d5.m128_f32[0] - d6.m128_f32[0]));
		*closest = triangle.p1 + w * (triangle.p2 - triangle.p1);
		return;
	}

	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	*closest = triangle.p0 + p0_p1 * v + p0_p2 * w;
}

<<<<<<< HEAD
bool Collision::CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle,
	DirectX::XMVECTOR* inter)
{
	XMVECTOR p;
	//球の中心に対する最近接点である三角形上にある点pを見つける
	ClosestPtPoint2Triangle(sphere.center, triangle, &p);
	//点pと球の中心の差分ベクトル
	XMVECTOR v = p - sphere.center;
	//距離の二乗を求める
	//(同じベクトル同士の内積は三平方の定理のルート内部の式と一致する)
	v = XMVector3Dot(v, v);
=======
bool Collision::CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle, DirectX::XMVECTOR* inter)
{
	XMVECTOR p;

	//球の中心に対する最近接点である三角形上にある点pを見つける
	ClosestPtPoint2Triangle(sphere.center, triangle, &p);
	
	//点pと球の中心の差分ベクトル
	XMVECTOR v = p - sphere.center;
	
	//距離の二乗を求める
	//(同じベクトル同士の内積は三平方の定理のルート内部の式と一致する)
	v = XMVector3Dot(v, v);
	
>>>>>>> e29bad67696d15a8f797935b7f879313ababe24d
	//球と三角形の距離が半径以下なら当たっていない
	if (v.m128_f32[0] > sphere.radius * sphere.radius)
	{
		return false;
	}
<<<<<<< HEAD
=======
	
>>>>>>> e29bad67696d15a8f797935b7f879313ababe24d
	//擬似交点を計算
	if (inter)
	{
		//三角形上の最近接点pを擬似交点とする
		*inter = p;
	}

	return true;
}