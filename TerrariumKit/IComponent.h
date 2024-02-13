#pragma once

namespace CompTK
{
	class IComponent
	{
		public:
			virtual void update() = 0;
			virtual ~IComponent() {};
	};
}