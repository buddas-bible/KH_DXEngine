#include "Animation.h"

#include <algorithm>

namespace KH
{
	void Animation::AddPositionSample(size_t framerate, const Vector3D& pos)
	{
		Animation_Pos* p = new Animation_Pos;
		p->framerate = framerate;
		p->pos = pos;
		posList.push_back(p);
	}

	void Animation::AddRotationSample(size_t framerate, const Vector3D& axis, float angle)
	{
		Animation_Rot* r = new Animation_Rot;
		r->framerate = framerate;
		r->axis = axis;
		r->angle = angle;
		rotList.push_back(r);
	}
	/*
	*/
	void Animation::InitFramerate()
	{
		for (auto& e : posList)
		{
			frameList.push_back(e->framerate);
		}

		for (auto& e : rotList)
		{
			auto itr = std::find(rotList.begin(), rotList.end(), e->framerate);
			if (itr != rotList.end())
			{
				continue;
			}
			else
			{
				frameList.push_back(e->framerate);
			}
		}

		std::sort(frameList.begin(), frameList.end());
	}

	// void Animation::AddScalingSample(size_t framerate, const Vector3D& axis, float scale)
	// {
	// 	Animation_Scl s;
	// 	s.framerate = framerate;
	// 	s.axis = axis;
	// 	s.scaleaxisang = scale;
	// 	sclList.push_back(std::move(s));
	// }
}