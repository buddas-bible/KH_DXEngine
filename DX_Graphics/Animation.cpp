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

	void Animation::InitFramerate()
	{
		using namespace DirectX;
		if (rotList.size() == 0)
		{
			return;
		}
		
		for (size_t i = 0; i < rotList.size(); i++)
		{
			XMVECTOR quaternion =
				XMQuaternionRotationAxis(
					ConvertToXMVECTOR({ rotList[i]->axis, 0.f }), rotList[i]->angle);
			quaternion = XMQuaternionNormalize(quaternion);

			if (i == 0)
			{
				rotList[i]->quaternion = ConvertToKHVector4D(quaternion);
			}
			else
			{
				XMVECTOR prev = ConvertToXMVECTOR(rotList[i - 1]->quaternion);
				XMVECTOR result = XMQuaternionMultiply(prev, quaternion);

				result = XMQuaternionNormalize(result);

				rotList[i]->quaternion = ConvertToKHVector4D(result);
			}
		}
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