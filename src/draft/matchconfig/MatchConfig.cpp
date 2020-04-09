/**
 * @file   MatchConfig.cpp
 * @author Dominik Authaler
 * @date   04.04.2020 (creation)
 * @brief  Definition of the match config class.
 */

#include "MatchConfig.hpp"

namespace spy {
    MatchConfig::MatchConfig() {

    }

    unsigned int MatchConfig::getMoledieRange() const {
        return moledieRange;
    }

    double MatchConfig::getBowlerBladeHitChance() const {
        return bowlerBladeHitChance;
    }

    unsigned int MatchConfig::getBowlerBladeRange() const {
        return bowlerBladeRange;
    }

    unsigned int MatchConfig::getBowlerBladeDamage() const {
        return bowlerBladeDamage;
    }

    double MatchConfig::getLaserCompactHitChance() const {
        return laserCompactHitChance;
    }

    unsigned int MatchConfig::getRocketPenDamage() const {
        return rocketPenDamage;
    }

    int MatchConfig::getGasGlossDamage() const {
        return gasGlossDamage;
    }

    unsigned int MatchConfig::getMothballPouchRange() const {
        return mothballPouchRange;
    }

    unsigned int MatchConfig::getMothballPouchDamage() const {
        return mothballPouchDamage;
    }

    unsigned int MatchConfig::getFogTinRange() const {
        return fogTinRange;
    }

    unsigned int MatchConfig::getGrappleRange() const {
        return grappleRange;
    }

    double MatchConfig::getGrappleHitChance() const {
        return grappleHitChance;
    }

    double MatchConfig::getWiretapWithEarplugsFailChance() const {
        return wiretapWithEarplugsFailChance;
    }

    double MatchConfig::getMirrorSwapChance() const {
        return mirrorSwapChance;
    }

    double MatchConfig::getCocktailDodgeChance() const {
        return cocktailDodgeChance;
    }

    unsigned int MatchConfig::getCocktailHealthPoints() const {
        return cocktailHealthPoints;
    }

    double MatchConfig::getSpySuccessChance() const {
        return spySuccessChance;
    }

    double MatchConfig::getBabysitterSuccessChance() const {
        return babysitterSuccessChance;
    }

    double MatchConfig::getHoneyTrapSuccessChance() const {
        return honeyTrapSuccessChance;
    }

    double MatchConfig::getObservationSuccessChance() const {
        return observationSuccessChance;
    }

    unsigned int MatchConfig::getChipsToIpFactor() const {
        return chipsToIpFactor;
    }

    unsigned int MatchConfig::getRoundLimit() const {
        return roundLimit;
    }

    unsigned int MatchConfig::getTurnPhaseLimit() const {
        return turnPhaseLimit;
    }

    unsigned int MatchConfig::getCatIp() const {
        return catIp;
    }

    unsigned int MatchConfig::getStrikeMaximum() const {
        return strikeMaximum;
    }

    void from_json(const nlohmann::json &j, MatchConfig &c) {
        j.at("Moledie_Range").get_to(c.moledieRange);

        j.at("BowlerBlade_Range").get_to(c.bowlerBladeRange);
        j.at("BowlerBlade_HitChance").get_to(c.bowlerBladeHitChance);
        j.at("BowlerBlade_Damage").get_to(c.bowlerBladeDamage);

        j.at("LaserCompact_HitChance").get_to(c.laserCompactHitChance);

        j.at("RocketPen_Damage").get_to(c.rocketPenDamage);

        j.at("GasGloss_Damage").get_to(c.gasGlossDamage);

        j.at("MothballPouch_Range").get_to(c.mothballPouchRange);
        j.at("MothballPouch_Damage").get_to(c.mothballPouchDamage);

        j.at("FogTin_Range").get_to(c.fogTinRange);

        j.at("Grapple_Range").get_to(c.grappleRange);
        j.at("Grapple_HitChance").get_to(c.grappleHitChance);

        j.at("WiretapWithEarplugs_FailChance").get_to(c.wiretapWithEarplugsFailChance);

        j.at("Mirror_SwapChance").get_to(c.mirrorSwapChance);

        j.at("Cocktail_DodgeChance").get_to(c.cocktailDodgeChance);

        j.at("Cocktail_Hp").get_to(c.cocktailHealthPoints);

        j.at("Spy_SuccessChance").get_to(c.spySuccessChance);

        j.at("Babysitter_SuccessChance").get_to(c.babysitterSuccessChance);

        j.at("HoneyTrap_SuccessChance").get_to(c.honeyTrapSuccessChance);

        j.at("Observation_SuccessChance").get_to(c.observationSuccessChance);

        j.at("ChipsToIpFaktor").get_to(c.chipsToIpFactor);

        j.at("RoundLimit").get_to(c.roundLimit);

        j.at("TurnPhaseLimit").get_to(c.turnPhaseLimit);

        j.at("CatIp").get_to(c.catIp);

        j.at("StrikeMaximum").get_to(c.strikeMaximum);
    }
}
