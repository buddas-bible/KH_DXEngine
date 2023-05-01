#pragma once

#include "DXDefine.h"

class Vector3D;

namespace KH
{
	struct Animation_Pos
	{
		Animation_Pos() = default;
		Animation_Pos(const Animation_Pos& p) : framerate(p.framerate), pos(p.pos)
		{}
		size_t framerate{};
		Vector3D pos;

		bool operator == (int a)
		{
			return this->framerate == a;
		}
	};

	struct Animation_Rot
	{
		Animation_Rot() = default;
		Animation_Rot(const Animation_Rot& p) : framerate(p.framerate), axis(p.axis), angle(p.angle)
		{}
		size_t framerate{};
		Vector3D axis;
		float angle{};
		Vector4D quaternion;

		bool operator == (int a)
		{
			return this->framerate == a;
		}
	};

// 	struct Animation_Scl
// 	{
// 		int framerate;
// 		Vector3D scale;
// 		Vector3D axis;
// 		float scaleaxisang;
// 		Vector4D quaternion;
// 	};

	struct Animation
	{
		std::wstring nodeName;
		std::vector<int> frameList;								// 프레임 리스트?
		std::vector<Animation_Pos*> posList;
		std::vector<Animation_Rot*> rotList;
		// std::vector<Animation_Scl> sclList;

		void AddPositionSample(size_t framerate, const Vector3D& pos);
		void AddRotationSample(size_t framerate, const Vector3D& axis, float angle);
		// void AddScalingSample(size_t framerate, const Vector3D& axis, float scale);
		void InitFramerate();
		//
	};
}