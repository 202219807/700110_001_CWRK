#include "GameEntity.h"
#include "helpers/IRenderHelpers.h"
#include "helpers/IGuiHelpers.h"

unsigned int GameEntity::s_NextID = 0;

GameEntity::GameEntity(unsigned int mID)
{
	SetID(mID);
}

unsigned int GameEntity::ID() const 
{ 
	return m_ID; 
}

void GameEntity::SetID(unsigned int mID)
{
	m_ID = s_NextID;
}

glm::mat4 GameEntity::GetModel() const
{
	return GameEntity::m_Model;
}

glm::mat4& GameEntity::GetModel()
{
	return GameEntity::m_Model;
}

glm::vec3 GameEntity::GetColour() const
{
	return m_Colour;
}

glm::vec3& GameEntity::GetColour()
{
	return m_Colour;
}

glm::vec3 GameEntity::GetPosition() const
{
	return m_Position;
}

glm::vec3& GameEntity::GetPosition()
{
	return m_Position;
}

void GameEntity::SetPosition(glm::vec3 pPosition)
{
	glm::mat4& model = GetModel();
	model[3][0] = pPosition.x;
	model[3][1] = pPosition.y;
	model[3][2] = pPosition.z;

	m_Position = pPosition;
}

void GameEntity::Scale(glm::vec3 pScale)
{
	m_Model = glm::scale(m_Model, pScale);
}

void GameEntity::Translate(glm::vec3 pTranslation)
{
	m_Model = glm::translate(m_Model, pTranslation);
}

void GameEntity::Rotate(float pRadians, glm::vec3 pAxis)
{
	m_Model = glm::rotate(m_Model, pRadians, pAxis);
}
