#ifndef STATE_H
#define STATE_H

// Stores the state of a SceneObject
// Includes Operators that would be useful
// for interpolating animations

class State {

public:
	glm::vec3 RotationPre, 
			  RotationPost, 
			  TranslationPre, 
			  TranslationPost, 
			  Scale;
	float Color[3];
	State();
	State(const State&);
	void set(State newState);
	// State operator + (State);
	// State operator * (float);
	State operator = (State);

};

#endif //STATE_H