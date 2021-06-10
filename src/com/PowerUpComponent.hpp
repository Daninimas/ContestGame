#pragma once

#include <com/Component.hpp>
#include <tools/Color.hpp>

class PowerUpComponent : public Component
{
public:
    explicit PowerUpComponent(const int id);
    ~PowerUpComponent() override;

	enum 
	{
		Shield,
		Fury,
		ExtraLife
	};
	uint8_t type;


	// ----------------- SHIELD DATA ------------------
	float shieldColliderIncFactor{ 1.f }; // Always more than 1. It make the shield collider be bigger than the objective collider

	// Por ahora lo dejo todo en el entity manager al crear el escudo, pero si hay que hacer distintos escudos, esos datos se pondran aqui


	// ----------------- FURY DATA ------------------
	float furyTimersSpeedIncFactor{ 0.7f }; // incremento que hace que las animaciones o cooldowns vayan mas rapidos y taque mas rapido o cosas asi (hay que tener en cuanta que ya se ha hecho uno en su sistema correspondiente)

	float furyTotalLifeTime{ 5.f };

	float furySpeedIncFactor{ 1.f };

	Color furyColor;
	
};

