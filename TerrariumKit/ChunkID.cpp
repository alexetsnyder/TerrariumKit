#include "ChunkID.h"

namespace ProcGenTK
{
	ChunkID::ChunkID(ChunkSize chunkSize, float x, float y, float z)
		: chunkSize_{ chunkSize }
	{
		setID(x, y, z);
	}

	ChunkID::ChunkID(ChunkSize chunkSize, const glm::vec3& worldPos)
		: chunkSize_{ chunkSize }
	{
		setPosition(worldPos);
	}

	float ChunkID::x() const
	{
		return id_[0];
	}

	float ChunkID::y() const
	{
		return id_[1];
	}

	float ChunkID::z() const
	{
		return id_[2];
	}

	std::array<float, 3> ChunkID::id() const
	{
		return id_;
	}

	glm::vec3 ChunkID::position() const
	{
		float x = id_[0] * chunkSize_.xWidth;
		float y = id_[1] * chunkSize_.height;
		float z = id_[2] * chunkSize_.zWidth;

		return glm::vec3{ x, y, z };
	}

	glm::vec3 ChunkID::computeRelativeVoxelPosition(const glm::vec3& worldPos) const
	{
		float x = floor(worldPos.x);
		float y = floor(worldPos.y);
		float z = floor(worldPos.z);

		x -= id_[0] * chunkSize_.xWidth;
		y -= id_[1] * chunkSize_.height;
		z -= id_[2] * chunkSize_.zWidth;

		return glm::vec3{ x, y, z };
	}

	bool ChunkID::Equals(const ChunkID& rhs)
	{
		return id_[0] == rhs.id_[0] && id_[1] == rhs.id_[1] && id_[2] == rhs.id_[2];
	}

	void ChunkID::setID(float x, float y, float z)
	{
		id_[0] = x;
		id_[1] = y;
		id_[2] = z;
	}

	void ChunkID::setPosition(const glm::vec3& worldPos)
	{
		float x = floor(worldPos.x);
		float y = floor(worldPos.y);
		float z = floor(worldPos.z);

		float chunkX = floor(x / chunkSize_.xWidth);
		float chunkY = floor(y / chunkSize_.height);
		float chunkZ = floor(z / chunkSize_.zWidth);

		setID(chunkX, chunkY, chunkZ);
	}
}
