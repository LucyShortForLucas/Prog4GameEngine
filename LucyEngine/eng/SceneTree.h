#pragma once

#include "Actor.h"

namespace eng {
																					/// A wrapper around a root actor to act as and hold some additional information of the Scene Tree as a whole
class SceneTree final :
	AbstractEventListener<event::ActorDestroyed> {
public:
	//---- Constructors														 
																					 /// Creates a new Scene Tree and load the json file at path as its root's first child
	SceneTree(const std::string& path = "");

	//---- Destructor, Copy, Move
	~SceneTree() = default;
	SceneTree(const SceneTree&) = delete;
	SceneTree& operator=(const SceneTree&) = delete;
	SceneTree(SceneTree&&) = delete;
	SceneTree& operator=(SceneTree&&) = delete;

	//---- Getters
	Actor& Root();

	//---- Scenegraph methods
																					/// Creates a new actor as child of root fron [DATA]/path. Returns pointer to the new actor, or nullpt if the json is invalid
	Actor* LoadActor(const std::string& path);										/// Clears all children of root not tagged with PreserveOnParentClear
	void ClearActors();																/// Clears all existing children of root not tagged with PreserveOnParentClear, then loads a new actor
	Actor* ClearAndLoad(const std::string& path);																									

	//---- Tag methods																
																					/// 'Tags' an actor aith an id, so it can be retrieved later.
	void TagActor(unsigned int tag, Actor* actorPtr);								/// Removes tag from actor. Safe to call even when the actor does not have the tag, in which case this is a no-op
	void UnTagActor(unsigned int tag, Actor*);										/// Returns the first actor tagged with tag, or nullptr if none found
	Actor* GetActorWithTag(unsigned int tag);										/// Returns a vector of all actors tagged with tag.
	const std::vector<Actor*>& GetAllActorsWithTag(unsigned int tag) const;
	
	//---- Event Handlers
	void OnEvent(const event::ActorDestroyed& event) override;

private:
	std::unique_ptr<Actor> m_RootUptr{};
	mutable std::map<unsigned int, std::vector<Actor*>> m_TaggedActors;
};

}