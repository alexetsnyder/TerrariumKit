#pragma once

#include <glm/glm.hpp>

namespace CompTK
{
	class TransformComponent
	{
		public:
			TransformComponent(glm::vec3 position, glm::vec3 worldUp, float yaw, float pitch);

			glm::vec3 position() const { return position_; }
			glm::vec3 front() const { return front_; }
			glm::vec3 right() const { return right_; }
			glm::vec3 up() const { return up_; }

			void translate(glm::vec3 translation);
			void rotate(float xOffset, float yOffset);

			void clampPitch(float minPitch, float maxPitch);
			void clampYaw(float minYaw, float maxYaw);

		private:
			void updateVectors();

			glm::vec3 position_;
			glm::vec3 up_;
			glm::vec3 front_;
			glm::vec3 right_;
			glm::vec3 worldUp_;

			float yaw_;
			float pitch_;

			bool clampPitch_;
			float minPitch_;
			float maxPitch_;

			bool clampYaw_;
			float minYaw_;
			float maxYaw_;
	};
}
