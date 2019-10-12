#pragma once

namespace math
{
	struct Mat4f
	{
		float data[4][4];

		float*
		operator[](unsigned int index)
		{
			return data[index];
		}

		Mat4f
		operator*(const Mat4f& other)
		{
			Mat4f result{};
			for (unsigned int row = 0; row < 4; ++row)
			{
				for (unsigned int col = 0; col < 4; ++col)
				{
					result.data[row][col] = data[row][0] * other.data[0][col] +
											data[row][1] * other.data[1][col] +
											data[row][2] * other.data[2][col] +
											data[row][3] * other.data[3][col];
				}
			}
			return result;
		}
	};
};