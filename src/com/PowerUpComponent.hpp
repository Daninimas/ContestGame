#pragma once

#include <com/Component.hpp>

class PowerUpComponent : public Component
{
public:
    explicit PowerUpComponent(const int id);
    ~PowerUpComponent() override;

	enum 
	{
		Shield,
		Fury
	};
	uint8_t type;


	// ----------------- SHIELD DATA ------------------
	float colliderIncFactor{ 1.f }; // Always more than 1. It make the shield collider be bigger than the objective collider

	// Por ahora lo dejo todo en el entity manager al crear el escudo, pero si hay que hacer distintos escudos, esos datos se pondran aqui
	
};

