#include "ChunkID.h"

namespace ProcGenTK
{
	ChunkID::ChunkID()
		: _id{}, _chunkSize{}
	{

	}

	ChunkID::ChunkID(ChunkSize chunkSize, float x, float y, float z)
	{
		init(chunkSize, x, y, z);
	}

	ChunkID::ChunkID(ChunkSize chunkSize, glm::vec3 worldPos)
	{
		_chunkSize = chunkSize;
		setPosition(worldPos);
	}

	float ChunkID::getX()
	{
		return _id[0];
	}

	float ChunkID::getY()
	{
		return _id[1];
	}

	float ChunkID::getZ()
	{
		return _id[2];
	}

	std::array<float, 3> ChunkID::getID() const
	{
		return _id;
	}

	glm::vec3 ChunkID::getPosition() const
	{
		float x = _id[0] * _chunkSize.xWidth;
		float y = _id[1] * _chunkSize.height;
		float z = _id[2] * _chunkSize.zWidth;

		return glm::vec3{ x, y, z };
	}

	glm::vec3 ChunkID::getRelativeVoxelPosition(const glm::vec3& worldPos) const
	{
		float x = floor(worldPos.x);
		float y = floor(worldPos.y);
		float z = floor(worldPos.z);

		x -= _id[0] * _chunkSize.xWidth;
		y -= _id[1] * _chunkSize.height;
		z -= _id[2] * _chunkSize.zWidth;

		return glm::vec3{ x, y, z };
	}

	void ChunkID::setID(float x, float y, float z)
	{
		_id[0] = x;
		_id[1] = y;
		_id[2] = z;
	}

	void ChunkID::setPosition(const glm::vec3& worldPos)
	{
		float x = floor(worldPos.x);
		float y = floor(worldPos.y);
		float z = floor(worldPos.z);

		float chunkX = floor(x / _chunkSize.xWidth);
		float chunkY = floor(y / _chunkSize.height);
		float chunkZ = floor(z / _chunkSize.zWidth);

		_id[0] = chunkX;
		_id[1] = chunkY;
		_id[2] = chunkZ;
	}

	void ChunkID::init(ChunkSize chunkSize, float x, float y, float z)
	{
		_chunkSize = chunkSize;
		setID(x, y, z);
	}

	bool ChunkID::Equals(const ChunkID& rhs)
	{
		return _id[0] == rhs._id[0] && _id[1] == rhs._id[1] && _id[2] == rhs._id[2];
	}
}
