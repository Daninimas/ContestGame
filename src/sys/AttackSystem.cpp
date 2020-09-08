#include "AttackSystem.hpp"

#include <eng/GameEngine.hpp>
#include <iostream>

AttackSystem::AttackSystem() {}

AttackSystem::~AttackSystem() {}


void AttackSystem::update(GameEngine& gameContext) const {
	//manage COOLDOWN for all attacks
	addCooldownTimeToWeapons(gameContext);

	checkPlayerAttacking(gameContext);
}


void AttackSystem::addCooldownTimeToWeapons(GameEngine& gameContext) const {
	auto& meleeWeapons = gameContext.entityMan.getComponents<MeleeWeaponComponent>();
	float deltaTime = gameContext.getDeltaTime();

	for (auto& meleeWeap : meleeWeapons) {
		meleeWeap.cooldown += deltaTime;
	}
}



void AttackSystem::checkPlayerAttacking(GameEngine& gameContext) const {
	InputComponent& playerInput = gameContext.entityMan.getComponent<InputComponent>(gameContext.playerId);


	if (playerInput.attacking) {
		// TODO attack mele or distance
		createMeleeAttack(gameContext, gameContext.playerId);
	}
}

void AttackSystem::createMeleeAttack(GameEngine& gameContext, int attackerID) const {
	MeleeWeaponComponent& meleeWeapon = gameContext.entityMan.getComponent<MeleeWeaponComponent>(attackerID);

	if (meleeWeapon.cooldown > meleeWeapon.maxCooldown) {
		SituationComponent& attackerSit = gameContext.entityMan.getComponent<SituationComponent>(attackerID);

		int attackId = gameContext.entityMan.createAttack(gameContext, attackerSit.x, attackerSit.y, 0.f, GameObjectType::MELEE_ATTACK);



		meleeWeapon.cooldown = 0.f;
	}
}

