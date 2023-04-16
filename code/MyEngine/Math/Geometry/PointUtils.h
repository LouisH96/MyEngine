#pragma once

#include <limits>
#include <DataStructures/Array.h>
#include "Math/Value2.h"
#include <Math/Float3.h>
#include "Math/Value3.h"

#undef min;
#undef max;

namespace MyEngine
{
	namespace Math
	{
		namespace Geometry
		{
			class PointUtils
			{
			public:
				template<typename T>
				static void GetBounds2DPoints(const Ds::Array<Value2<T>>& points, Value2<T>& min, Value2<T>& max);

				template<typename T>
				static void Center2DPoints(Ds::Array<Value2<T>>& points);

				template<typename T> static void Scale(Ds::Array<Value2<T>>& points, T scale);
				template<typename T> static void Scale(Ds::Array<Value3<T>>& points, T scale);

				template<typename T> static void Translate(Ds::Array<Value3<T>>& points, const Value3<T>& translation);
			};

			//---| Definitions |---

			template <typename T>
			void PointUtils::GetBounds2DPoints(const Ds::Array<Value2<T>>& points, Value2<T>& min, Value2<T>& max)
			{
				min.x = std::numeric_limits<T>::max();
				min.y = min.x;
				max.x = -min.x;
				max.y = max.x;
				for (int i = 0; i < points.GetSize(); i++)
				{
					const auto& point{ points[i] };
					if (point.x < min.x) min.x = point.x;
					else if (point.x > max.x) max.x = point.x;
					if (point.y < min.y) min.y = point.y;
					else if (point.y > max.y) max.y = point.y;
				}
			}

			template <typename T>
			void PointUtils::Center2DPoints(Ds::Array<Value2<T>>& points)
			{
				Value2<T> min, max;
				GetBounds2DPoints(points, min, max);
				const Value2<T> center{ (min + max) / static_cast<T>(2) };
				for (int i = 0; i < points.GetSize(); i++)
					points[i] -= center;
			}

			template<typename T>
			void PointUtils::Scale(Ds::Array<Value2<T>>& points, T scale)
			{
				for (int i = 0; i < points.GetSize(); i++)
					points[i] *= scale;
			}

			template <typename T>
			void PointUtils::Scale(Ds::Array<Value3<T>>& points, T scale)
			{
				for (int i = 0; i < points.GetSize(); i++)
					points[i] *= scale;
			}

			template <typename T>
			void PointUtils::Translate(Ds::Array<Value3<T>>& points, const Value3<T>& translation)
			{
				for (int i = 0; i < points.GetSize(); i++)
					points[i] += translation;
			}
		}
	}
}
