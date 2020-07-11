#pragma once

namespace infra
{
	namespace mem
	{
		template <typename T>
		struct chunk
		{
			int size;
			T* ptr;

			T&
			operator[](unsigned int index)
			{
				return ptr[index];
			}
		};
	}
}