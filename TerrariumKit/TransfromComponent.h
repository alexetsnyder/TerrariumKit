#pragma once

#include "IComponent.h"

#include <glm/glm.hpp>

namespace CompTK
{
	class TransformComponent : public IComponent
	{
		public:
			TransformComponent() {}

			void update() override {}

		private:
			glm::vec3 position_;
			glm::vec3 front_;
			glm::vec3 right_;
			glm::vec3 up_;
	};
}