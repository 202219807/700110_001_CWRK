#pragma once
#include "BoundingSphere.h"
#include "CollisionObject.h"
#include <memory>
#include <vector>

using namespace std;

// tree structure and methods to build and traverse the tree
// e.g. create, insert, update, remove, split, queryCollisions
class BoundingVolumeHierarchy
{
public:
	struct Node
	{
		BoundingSphere boundingSphere;
		std::unique_ptr<CollisionObject> object;
		Node* left;
		Node* right;
		Node() : left(nullptr), right(nullptr) {}
	};

private:
	Node* m_Root;

public:
	BoundingVolumeHierarchy() 
	{
		m_Root = new Node();
	}

	vector<unique_ptr<CollisionObject>> queryCollisions(BoundingSphere sphere) 
	{
		vector<unique_ptr<CollisionObject>> collidingObjects;
		query(m_Root, sphere, collidingObjects);
		return collidingObjects;
	}
	
	void create(vector<unique_ptr<CollisionObject>>& objects)
	{
		m_Root = create(objects, 0, objects.size());
	}

	void insert(unique_ptr<CollisionObject> object)
	{
		//create a new leaf node for the object
		Node* leaf = new Node();
		leaf->object = std::move(object);
		leaf->boundingSphere = leaf->object->GetBoundingSphere();

		if (!m_Root) {
			m_Root = leaf;
		}
		else {
			insert(m_Root, leaf);
		}
	}

	void remove(unique_ptr<CollisionObject> object) 
	{
		remove(m_Root, std::move(object));
	}
	
	void update(unique_ptr<CollisionObject> object) 
	{
		update(m_Root, std::move(object));
	}

private:
	Node* create(vector<unique_ptr<CollisionObject>>& objects, int start, int end)
	{
		if (start >= end) return nullptr;
		Node* node = new Node();
		if (end - start == 1)
		{
			node->object = std::move(objects[start]);
			node->boundingSphere = node->object->GetBoundingSphere();
		}
		else
		{
			int mid = (start + end) / 2;
			node->left = create(objects, start, mid);
			node->right = create(objects, mid, end);
			if (node->left != nullptr && node->right != nullptr)
				node->boundingSphere = node->left->boundingSphere.merge(node->right->boundingSphere);
			else if (node->left != nullptr)
				node->boundingSphere = node->left->boundingSphere;
			else if (node->right != nullptr)
				node->boundingSphere = node->right->boundingSphere;
		}
		return node;
	}

	void insert(Node* current, Node* leaf)
	{
		// check if leaf can be added as child of current
		if (leaf->boundingSphere.intersects(current->boundingSphere))
		{
			// leaf can be added as child of current
			if (!current->left)
			{
				current->left = leaf;
			}
			else if (!current->right)
			{
				current->right = leaf;
			}
			else
			{
				// current is already a full node, we need to split
				// the node and call insert recursively
				// create new bounding spheres for left and right children
				BoundingSphere leftSphere, rightSphere;
				splitNode(current, leftSphere, rightSphere);
				current->left = new Node();
				current->left->boundingSphere = leftSphere;
				current->right = new Node();
				current->right->boundingSphere = rightSphere;
				insert(current->left, leaf);
			}
		}
	}

	void remove(Node* current, std::unique_ptr<CollisionObject> object) 
	{
		// logic to remove the object from the BVH
		if (current->left == nullptr) 
		{
			if (current->object.get() == object.get()) 
			{
				current->object.reset();
			}
		}
		else 
		{
			remove(current->left, std::move(object));
			remove(current->right, std::move(object));
			current->boundingSphere = current->left->boundingSphere.merge(current->right->boundingSphere);
		}
	}

	void update(Node* current, std::unique_ptr<CollisionObject> object) 
	{
		// logic to update the object in the BVH
		if (current->left == nullptr) {
			if (current->object.get() == object.get()) {
				current->object = std::move(object);
			}
		}
		else {
			update(current->left, std::move(object));
			update(current->right, std::move(object));
			current->boundingSphere = current->left->boundingSphere.merge(current->right->boundingSphere);
		}
	}

	void splitNode(Node* current, BoundingSphere& leftSphere, BoundingSphere& rightSphere) 
	{
		// logic to split the node into two child nodes
		// and update the bounding spheres of the left and right nodes
		if (current->left == nullptr)
		{
			current->left = new Node();
			current->right = new Node();
			std::unique_ptr<CollisionObject> object = std::move(current->object);
			current->left->object = std::move(object);
			current->right->object = std::move(object);
			leftSphere = current->left->boundingSphere;
			rightSphere = current->right->boundingSphere;
		}
		else 
		{
			splitNode(current->left, leftSphere, rightSphere);
			splitNode(current->right, leftSphere, rightSphere);
		}
		current->boundingSphere = leftSphere.merge(rightSphere);
	}
	
	void query(Node* node, BoundingSphere& sphere, vector<unique_ptr<CollisionObject>>& collidingObjects)
	{
		if (!node) return;
		if (node->boundingSphere.intersects(sphere))
		{
			if (node->object)
			{
				collidingObjects.push_back(std::move(node->object));
			}
			query(node->left, sphere, collidingObjects);
			query(node->right, sphere, collidingObjects);
		}
	}
};

