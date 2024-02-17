#include "TransformComponent.h"

namespace CompTK
{
	TransformComponent::TransformComponent(glm::vec3 position, glm::vec3 worldUp, float yaw, float pitch)
		: position_{ position }, worldUp_{ worldUp }, yaw_{ yaw }, pitch_{ pitch }
	{
		clampPitch_ = false;
		minPitch_ = 0.0f;
		maxPitch_ = 0.0f;

		clampYaw_ = false;
		minYaw_ = 0.0f;
		minPitch_ = 0.0f;

		updateVectors();
	}

	void TransformComponent::translate(glm::vec3 translation)
	{
		position_ += translation;
	}

	void TransformComponent::rotate(float xOffset, float yOffset)
	{
		yaw_ += xOffset;
		pitch_ += yOffset;

		if (clampPitch_)
		{
			if (pitch_ > maxPitch_)
			{
				pitch_ = maxPitch_;
			}
			else if (pitch_ < minPitch_)
			{
				pitch_ = minPitch_;
			}
		}

		if (clampYaw_)
		{
			if (yaw_ > maxYaw_)
			{
				yaw_ = maxYaw_;
			}
			else if (yaw_ < minYaw_)
			{
				yaw_ = minYaw_;
			}
		}

		updateVectors();
	}

	void TransformComponent::clampPitch(float minPitch, float maxPitch)
	{
		clampPitch_ = true;
		minPitch_ = minPitch;
		maxPitch_ = maxPitch;
	}

	void TransformComponent::clampYaw(float minYaw, float maxYaw)
	{
		clampYaw_ = true;
		minYaw_ = minYaw;
		maxYaw_ = maxYaw;
	}

	void TransformComponent::updateVectors()
	{
		glm::vec3 front{};
		front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
		front.y = sin(glm::radians(pitch_));
		front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
		front_ = glm::normalize(front);

		right_ = glm::normalize(glm::cross(front_, worldUp_));
		up_ = glm::normalize(glm::cross(right_, front_));
	}
}