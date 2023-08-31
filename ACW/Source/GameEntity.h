#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class IRenderHelpers;
class IGuiHelpers;

class GameEntity
{
private:
	unsigned int m_ID;
	static unsigned int s_NextID;

	glm::mat4 m_Model;
	glm::vec3 m_Colour;
	glm::vec3 m_Position;

public:
	GameEntity() : m_Colour(1.0f, 1.0f, 1.0f), m_ID(s_NextID) 
	{ 
		s_NextID++; 
	}
	GameEntity(unsigned int);
	GameEntity(const GameEntity&) = delete;
	GameEntity& operator=(const GameEntity&) = delete;
	~GameEntity() = default;

	unsigned int ID() const;
	void SetID(unsigned int);

	glm::mat4 GetModel() const;
	glm::mat4& GetModel();
	void SetModel() {};

	glm::vec3 GetColour() const;
	glm::vec3& GetColour();
	void SetColour() {};

	glm::vec3 GetPosition() const;
	glm::vec3& GetPosition();
	void SetPosition(glm::vec3);
	
	void Scale(glm::vec3);
	void Translate(glm::vec3);
	void Rotate(float fRadians, glm::vec3);

	virtual void Render(const IRenderHelpers&) const = 0;
	virtual void RenderGui(const IGuiHelpers&) = 0;
};
