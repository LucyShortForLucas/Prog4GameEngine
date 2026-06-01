#include "SceneTree.h"

#include "Services.h"
#include <algorithm>
#include <ranges>

namespace eng {

SceneTree::SceneTree(const std::string& path) :
	m_RootUptr{std::make_unique<Actor>(this)} {

	if (path == "")
		return;

	auto jPtr {service::resources.Get().LoadJson(path)};

	m_RootUptr->DeserializeChild(*jPtr);
}

Actor& SceneTree::Root() {
	return *m_RootUptr;
}

Actor* SceneTree::LoadActor(const std::string& path) {
	auto jPtr{ service::resources.Get().LoadJson(path) };

	return m_RootUptr->DeserializeChild(*jPtr);
}

void SceneTree::ClearActors() {
	m_RootUptr->ClearChildren();
}

Actor* SceneTree::ClearAndLoad(const std::string& path) {
	m_RootUptr->ClearChildren();
	return LoadActor(path);
}

void SceneTree::TagActor(unsigned int tag, Actor* actorPtr) {
	if (!m_TaggedActors.contains(tag))
		m_TaggedActors[tag] = std::vector<Actor*>{};

	m_TaggedActors[tag].emplace_back(actorPtr);

	actorPtr->SubscribeActorDestroyed(*this);
}

void SceneTree::UnTagActor(unsigned int tag, Actor* actorPtr) {
	if (!m_TaggedActors.contains(tag))
		return;

	auto it = std::ranges::find(m_TaggedActors[tag], actorPtr);
	if (it != m_TaggedActors[tag].end()) 
		m_TaggedActors[tag].erase(it);

	actorPtr->UnsubscribeActorDestroyed(*this);
}

Actor* SceneTree::GetActorWithTag(unsigned int tag) {
	if (!m_TaggedActors.contains(tag))
		return nullptr;

	if (m_TaggedActors[tag].size() < 1)
		return nullptr;

	return *m_TaggedActors[tag].begin();
}

const std::vector<Actor*>& SceneTree::GetAllActorsWithTag(unsigned int tag) {
	if (!m_TaggedActors.contains(tag))
		m_TaggedActors[tag] = std::vector<Actor*>{};

	return m_TaggedActors[tag];
}

void SceneTree::OnEvent(const event::ActorDestroyed& event) {
	event.actorPtr->UnsubscribeActorDestroyed(*this);
}


} // !eng