#pragma once

#pragma once

#include <concepts>
#include <optional>
#include <string>
#include <set>

#include <nlohmann/json.hpp>

#include <iostream>

namespace eng {

class Actor; //forward declaration

/// <summary>
/// An abstract base class for Components
/// </summary>
class AbstractComponent {
public: 
	//---- Virtual Destructor 
	AbstractComponent(Actor& owner);
	virtual ~AbstractComponent() = default;


	//---- Serialization

	virtual nlohmann::ordered_json Serialize() { return nlohmann::ordered_json{}; }
	virtual const std::string& TypeName() = 0;

	//---- Gameloop methods 
																					/// Called once before the first frame of the owning object. This method should be used to initialize the component internally with its Actor.
	virtual void Start() {};														/// Called when the object becomes enabled from a disabled state, including during Start() (even if the object starts in a disabled state) and when the owning Actor is finished moving across the scenegraph to a new parent. Any initializing that depends on the scenegraph should happen here to ensure a valid state for the component.
	virtual void OnEnable() {};														/// Called once per frame.
	virtual void Update() {};														/// Called once per frame, after Update() has been called on every other Actor/Component.
	virtual void LateUpdate() {};													/// Called once per 'physics tick', should be used for any physics dependent code. May be called 0 or more times per update. NOT IMPLEMENTED
	virtual void FixedUpdate() {};													/// Called once per frame by the renderer. Any rendering should go here.
	virtual void Render() {};														/// Called once per frame by the renderer, after all other rendering. Any Imgui rendering should go here.
	virtual void RenderImgui() {};													/// Called when the object becomes disabled from a enabled state, including during Start() if the objects starts disabled and before the owning Actor begins moves across the scenegraph to a new parent. Any initializing done in OnEnable() should be undone here to keep the component in a valid state.
	virtual void OnDisable() {};
	virtual void OnDestroy() {};

public: //--------------------- Relationship Methods -------------------------

	Actor& Owner();
	const Actor& Owner() const;

private: //-------------------- Fields --------------------------
	Actor& m_Owner;
}; // !AbstractComponent

}