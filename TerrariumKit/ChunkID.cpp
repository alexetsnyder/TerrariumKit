#include "ChunkID.h"

ChunkID::ChunkID()
	: _id{}, _chunkSize{}
{

}

ChunkID::ChunkID(ChunkSize chunkSize, glm::vec3 worldPos)
{
	init(chunkSize, worldPos);
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

void ChunkID::setPosition(const glm::vec3& worldPos)
{
	float x = floor(worldPos.x);
	float z = floor(worldPos.z);

	float chunkX = floor(x / _chunkSize.xWidth);
	float chunkZ = floor(z / _chunkSize.zWidth);

	_id[0] = chunkX;
	_id[1] = chunkZ;
}

void ChunkID::init(ChunkSize chunkSize, glm::vec3 worldPos)
{
	_chunkSize = chunkSize;
	setPosition(worldPos);
}
