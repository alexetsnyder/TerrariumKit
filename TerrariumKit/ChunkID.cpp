#include "ChunkID.h"

ChunkID::ChunkID()
	: _id{}, _chunkSize{}
{

}

ChunkID::ChunkID(ChunkSize chunkSize, float x, float z)
{
	init(chunkSize, x, z);
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

float ChunkID::getZ()
{
	return _id[1];
}

std::array<float, 2> ChunkID::getID() const
{
	return _id;
}

glm::vec3 ChunkID::getPosition() const
{
	float x = _id[0] * _chunkSize.xWidth;
	float y = 0.0f;
	float z = _id[1] * _chunkSize.zWidth;

	return glm::vec3{ x, y, z };
}

glm::vec3 ChunkID::getRelativeVoxelPosition(const glm::vec3& worldPos) const
{
	float x = floor(worldPos.x);
	float y = floor(worldPos.y);
	float z = floor(worldPos.z);

	x -= _id[0] * _chunkSize.xWidth;
	z -= _id[1] * _chunkSize.zWidth;

	return glm::vec3{ x, y, z };
}

void ChunkID::setID(float x, float z)
{
	_id[0] = x;
	_id[1] = z;
}

void ChunkID::setPosition(const glm::vec3& worldPos)
{
	float x = floor(worldPos.x);
	float z = floor(worldPos.z);

	float chunkX = floor(x / _chunkSize.xWidth);
	float chunkZ = floor(z / _chunkSize.zWidth);

	_id[0] = chunkX;
	_id[1] = chunkZ;
}

void ChunkID::init(ChunkSize chunkSize, float x, float z)
{
	_chunkSize = chunkSize;
	setID(x, z);
}

bool ChunkID::Equals(const ChunkID& rhs)
{
	return _id[0] == rhs._id[0] && _id[1] == rhs._id[1];
}
