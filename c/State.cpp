#include "../h/common.h"

State::State () {
	RotationPre = glm::vec3(0.0f);
	RotationPost = glm::vec3(0.0f);
	TranslationPre = glm::vec3(0.0f);
	TranslationPost = glm::vec3(0.0f);
	Scale = glm::vec3(1.0f);

	Color[0] = 1.0;
	Color[1] = 1.0;
	Color[2] = 1.0;
}

State::State (const State& initialState) {
	this->RotationPre = initialState.RotationPre;
	this->RotationPost = initialState.RotationPost;
	this->TranslationPre = initialState.TranslationPre;
	this->TranslationPost = initialState.TranslationPost;
	this->Scale = initialState.Scale;
	this->Color[0] = initialState.Color[0];
	this->Color[1] = initialState.Color[1];
	this->Color[2] = initialState.Color[2];
}

void State::set(State newState) {
	this->RotationPre = newState.RotationPre;
	this->RotationPost = newState.RotationPost;
	this->TranslationPre = newState.TranslationPre;
	this->TranslationPost = newState.TranslationPost;
	this->Scale = newState.Scale;
	this->Color[0] = newState.Color[0];
	this->Color[1] = newState.Color[1];
	this->Color[2] = newState.Color[2];
}

State State::operator= (State other) {
	
	State result;

	result.RotationPre = other.RotationPre;
	result.RotationPost = other.RotationPost;
	result.TranslationPre = other.TranslationPre;
	result.TranslationPost = other.TranslationPost;
	result.Scale = other.Scale;

	result.Color[0] = other.Color[0];
	result.Color[1] = other.Color[1];
	result.Color[2] = other.Color[2];

	return result;
}