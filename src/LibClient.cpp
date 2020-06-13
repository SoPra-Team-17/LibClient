/**
 * @file    LibClient.cpp
 * @author  Marco
 * @date    14.04.2020 (creation)
 * @brief   Implementation of LibClient
 */

#include "LibClient.hpp"

#include <utility>
#include <string>
#include <datatypes/gameplay/BaseOperation.hpp>
#include <datatypes/gameplay/GadgetAction.hpp>
#include <datatypes/gameplay/SpyAction.hpp>
#include <datatypes/gameplay/GambleAction.hpp>
#include <datatypes/gameplay/PropertyAction.hpp>
#include <datatypes/gameplay/CharacterOperation.hpp>
#include <datatypes/gameplay/Movement.hpp>
#include <datatypes/gameplay/CatAction.hpp>
#include <datatypes/gameplay/JanitorAction.hpp>
#include <datatypes/gameplay/Exfiltration.hpp>
#include <datatypes/gameplay/RetireAction.hpp>

namespace libclient {

    const std::optional<spy::util::UUID> &LibClient::getId() const {
        return model->clientState.id;
    }

    const std::string &LibClient::getName() const {
        return model->clientState.name;
    }

    bool LibClient::isConnected() const {
        return model->clientState.isConnected;
    }

    const spy::network::RoleEnum &LibClient::getRole() const {
        return model->clientState.role;
    }

    const spy::util::UUID &LibClient::getSessionId() const {
        return model->clientState.sessionId;
    }

    const spy::util::UUID &LibClient::getPlayerOneId() const {
        return model->clientState.playerOneId;
    }

    const spy::util::UUID &LibClient::getPlayerTwoId() const {
        return model->clientState.playerTwoId;
    }

    const std::string &LibClient::getPlayerOneName() const {
        return model->clientState.playerOneName;
    }

    const std::string &LibClient::getPlayerTwoName() const {
        return model->clientState.playerTwoName;
    }

    const spy::util::UUID &LibClient::getActiveCharacter() const {
        return model->clientState.activeCharacter;
    }

    const std::optional<spy::util::UUID> &LibClient::getLeftUserId() const {
        return model->clientState.leftUserId;
    }

    const std::optional<spy::network::ErrorTypeEnum> &LibClient::getErrorReason() const {
        return model->clientState.errorReason;
    }

    bool LibClient::isGamePaused() const {
        return model->clientState.gamePaused;
    }

    bool LibClient::isServerEnforced() const {
        return model->clientState.serverEnforced;
    }

    const unsigned int &LibClient::getStrikeNr() const {
        return model->clientState.strikeNr;
    }

    const unsigned int &LibClient::getStrikeMax() const {
        return model->clientState.strikeMax;
    }

    const std::string &LibClient::getStrikeReason() const {
        return model->clientState.strikeReason;
    }

    const spy::scenario::Scenario &LibClient::getLevel() const {
        return model->gameState.level;
    }

    const spy::MatchConfig &LibClient::getSettings() const {
        return model->gameState.settings;
    }

    const std::vector<spy::character::CharacterInformation> &LibClient::getCharacterSettings() const {
        return model->gameState.characterSettings;
    }

    const std::vector<spy::util::UUID> &LibClient::getOfferedCharacters() const {
        return model->gameState.offeredCharacters;
    }

    const std::vector<spy::gadget::GadgetEnum> &LibClient::getOfferedGadgets() const {
        return model->gameState.offeredGadgets;
    }

    const std::vector<spy::util::UUID> &LibClient::getChosenCharacters() const {
        return model->gameState.chosenCharacter;
    }

    const std::vector<spy::gadget::GadgetEnum> &LibClient::getChosenGadgets() const {
        return model->gameState.chosenGadget;
    }

    const std::map<spy::util::UUID, std::set<spy::gadget::GadgetEnum>> &LibClient::getEquipmentMap() const {
        return model->gameState.equipmentMap;
    }

    const std::vector<std::shared_ptr<const spy::gameplay::BaseOperation>> &LibClient::getOperations() const {
        return model->gameState.operations;
    }

    bool LibClient::isGameOver() const {
        return model->gameState.isGameOver;
    }

    const std::optional<spy::util::UUID> &LibClient::getWinner() const {
        return model->gameState.winner;
    }

    const std::optional<spy::statistics::Statistics> &LibClient::getStatistics() const {
        return model->gameState.statistics;
    }

    const std::optional<spy::statistics::VictoryEnum> &LibClient::getWinningReason() const {
        return model->gameState.winningReason;
    }

    bool LibClient::hasReplay() const {
        return model->gameState.hasReplay;
    }

    const spy::gameplay::State &LibClient::getState() const {
        return model->gameState.state;
    }

    const spy::util::UUID &LibClient::getLastActiveCharacter() const {
        return model->gameState.lastActiveCharacter;
    }

    const std::map<spy::network::messages::MetaInformationKey, spy::network::messages::MetaInformation::Info> &
    LibClient::getInformation() const {
        return model->clientState.information;
    }

    const std::optional<std::string> &LibClient::getDebugMessage() const {
        return model->clientState.debugMessage;
    }

    LibClient::LibClient(Callback *callback) : model(std::make_shared<Model>()),
                                               network(callback, model) {}

    bool LibClient::setName(const std::string &name) {
        if (network.getState() != Network::NetworkState::NOT_CONNECTED &&
            network.getState() != Network::NetworkState::CONNECTED) {
            return false;
        }
        model->clientState.name = name;
        return true;
    }

    bool LibClient::setRole(const spy::network::RoleEnum &role) {
        if (network.getState() != Network::NetworkState::NOT_CONNECTED &&
            network.getState() != Network::NetworkState::CONNECTED) {
            return false;
        }
        model->clientState.role = role;
        return true;
    }

    bool LibClient::setFaction(spy::util::UUID id, spy::character::FactionEnum faction) {
        using namespace spy::character;

        if (model->clientState.role != spy::network::RoleEnum::SPECTATOR) {
            auto iAmPlayer1 = amIPlayer1();
            if (!iAmPlayer1.has_value()) {
                return false;
            }
            if (faction == FactionEnum::INVALID ||
                faction == (iAmPlayer1.value() ? FactionEnum::PLAYER1 : FactionEnum::PLAYER2)) {
                return false;
            }

            return model->aiState.addFaction(id, faction == FactionEnum::NEUTRAL ? model->aiState.npcFaction
                                                                                 : model->aiState.enemyFaction);
        } else {
            // client is spectator
            switch (faction) {
                case spy::character::FactionEnum::NEUTRAL:
                    return model->aiState.addFaction(id, model->aiState.npcFaction);
                case spy::character::FactionEnum::PLAYER1:
                    return model->aiState.addFaction(id, model->aiState.myFaction);
                case spy::character::FactionEnum::PLAYER2:
                    return model->aiState.addFaction(id, model->aiState.enemyFaction);
                default:
                    return false;
            }
        }
    }

    std::optional<bool> LibClient::amIPlayer1() {
        if (network.getState() != Network::NetworkState::IN_GAME &&
            network.getState() != Network::NetworkState::IN_GAME_ACTIVE &&
            network.getState() != Network::NetworkState::PAUSE &&
            network.getState() != Network::NetworkState::GAME_OVER) {
            return std::nullopt;
        }
        if (model->clientState.role == spy::network::RoleEnum::SPECTATOR) {
            return std::nullopt;
        }
        return model->clientState.amIPlayer1();
    }

    auto
    LibClient::getUnknownFactionList() -> const std::map<spy::util::UUID, std::vector<std::pair<spy::character::FactionEnum, std::vector<double>>>> & {
        return model->aiState.unknownFaction;
    }

    auto
    LibClient::getUnknownGadgetsList() -> const std::map<std::shared_ptr<spy::gadget::Gadget>, std::vector<std::pair<spy::util::UUID, std::vector<double>>>, util::cmpGadgetPtr> & {
        return model->aiState.unknownGadgets;
    }

    std::optional<double>
    LibClient::hasCharacterFaction(const spy::util::UUID &id, spy::character::FactionEnum faction) {
        return model->aiState.hasCharacterFaction(id, faction, amIPlayer1() ? spy::character::FactionEnum::PLAYER1
                                                                            : spy::character::FactionEnum::PLAYER2);
    }

    std::optional<double> LibClient::hasCharacterGadget(const spy::util::UUID &id, spy::gadget::GadgetEnum type) {
        return model->aiState.hasCharacterGadget(id, type);
    }

    unsigned int LibClient::safePosToIndex(const spy::gameplay::State &s, const spy::util::Point &p) {
        return model->aiState.safePosToIndex(s, p);
    }

    const std::set<unsigned int> &LibClient::getOpenedSafes() {
        return model->aiState.openedSafes;
    }

    const std::map<unsigned int, int> &LibClient::getTriedSafes() {
        return model->aiState.triedSafes;
    }

    const std::set<spy::util::UUID> &LibClient::getCombinationsFromNpcs() {
        return model->aiState.combinationsFromNpcs;
    }

    const std::set<spy::util::UUID> &LibClient::getMyFactionList() {
        return model->aiState.myFaction;
    }

    const std::set<spy::util::UUID> &LibClient::getEnemyFactionList() {
        return model->aiState.enemyFaction;
    }

    const std::set<spy::util::UUID> &LibClient::getNpcFactionList() {
        return model->aiState.npcFaction;
    }

    std::string LibClient::operationToString(std::shared_ptr<const spy::gameplay::BaseOperation> op) const {
        using spy::gameplay::OperationEnum;

        std::string operationString;

        auto variant = this->getInformation().at(
                spy::network::messages::MetaInformationKey::CONFIGURATION_CHARACTER_INFORMATION);
        auto characterInformation = std::get<std::vector<spy::character::CharacterInformation>>(variant);

        auto operationType = op->getType();

        std::string characterName;

        bool isCharacterAction = not(operationType == OperationEnum::INVALID or
                                     operationType == OperationEnum::CAT_ACTION or
                                     operationType == OperationEnum::JANITOR_ACTION);
        if (isCharacterAction) {
            auto characterOperation = std::dynamic_pointer_cast<const spy::gameplay::CharacterOperation>(op);
            auto executingCharInfo = std::find_if(characterInformation.begin(), characterInformation.end(),
                                                  [characterOperation](const spy::character::CharacterInformation &ci) {
                                                      return characterOperation->getCharacterId() ==
                                                             ci.getCharacterId();
                                                  });
            characterName = executingCharInfo->getName();
        }


        switch (operationType) {
            case OperationEnum::INVALID:
                break;
            case OperationEnum::GADGET_ACTION: {
                auto gadgetOperation = std::dynamic_pointer_cast<const spy::gameplay::GadgetAction>(op);

                auto gadgetType = gadgetOperation->getGadget();
                nlohmann::json gadgetTypeJson = gadgetType;
                std::string gadgetTypeStr = gadgetTypeJson.dump();
                std::for_each(gadgetTypeStr.begin(), gadgetTypeStr.end(), [](char &c) {
                    c = std::tolower(c);
                });

                auto targetCoords = gadgetOperation->getTarget();

                operationString = "Gadget Op. " + gadgetTypeStr + " by " + characterName + " on x=" +
                                  std::to_string(targetCoords.x) + " y=" + std::to_string(targetCoords.y);
                break;
            }
            case OperationEnum::SPY_ACTION: {
                auto spyOperation = std::dynamic_pointer_cast<const spy::gameplay::SpyAction>(op);

                auto targetCoords = spyOperation->getTarget();

                operationString =
                        "Spy op. by " + characterName + " on x=" + std::to_string(targetCoords.x) + " y=" +
                        std::to_string(targetCoords.y);
                break;
            }
            case OperationEnum::GAMBLE_ACTION: {
                auto gambleOperation = std::dynamic_pointer_cast<const spy::gameplay::GambleAction>(op);

                auto targetCoords = gambleOperation->getTarget();

                operationString =
                        "Gamble op. by " + characterName + ", bet " + std::to_string(gambleOperation->getStake()) +
                        " Chips on x=" + std::to_string(targetCoords.x) + " y=" + std::to_string(targetCoords.y);
                break;
            }
            case OperationEnum::PROPERTY_ACTION: {
                auto propertyOperation = std::dynamic_pointer_cast<const spy::gameplay::PropertyAction>(op);

                nlohmann::json propertyJson = propertyOperation->getUsedProperty();
                std::string propertyString = propertyJson.dump();
                std::for_each(propertyString.begin(), propertyString.end(), [](char &c) {
                    c = std::tolower(c);
                });

                auto targetCoords = propertyOperation->getTarget();

                operationString =
                        "Property op. by " + characterName + "on x=" + std::to_string(targetCoords.x) + " y=" +
                        std::to_string(targetCoords.y);

                break;
            }
            case OperationEnum::MOVEMENT: {
                auto movementOperation = std::dynamic_pointer_cast<const spy::gameplay::Movement>(op);

                auto from = movementOperation->getFrom();
                auto to = movementOperation->getTarget();

                operationString = "Movement op. by " + characterName + " from x=" + std::to_string(from.x) + " y=" +
                                  std::to_string(from.y) + " to x=" + std::to_string(to.x) + " y=" +
                                  std::to_string(to.y);
                break;
            }
            case OperationEnum::CAT_ACTION: {
                auto catOperation = std::dynamic_pointer_cast<const spy::gameplay::CatAction>(op);

                operationString = "Cat. op on x=" + std::to_string(catOperation->getTarget().x) + " y=" +
                                  std::to_string(catOperation->getTarget().y);
                break;
            }
            case OperationEnum::JANITOR_ACTION: {
                auto janitorOperation = std::dynamic_pointer_cast<const spy::gameplay::JanitorAction>(op);

                operationString = "Janitor op on x=" + std::to_string(janitorOperation->getTarget().x) + " y=" +
                                  std::to_string(janitorOperation->getTarget().y);
                break;
            }
            case OperationEnum::EXFILTRATION: {
                auto exfiltrationOperation = std::dynamic_pointer_cast<const spy::gameplay::Exfiltration>(op);

                auto from = exfiltrationOperation->getFrom();
                auto to = exfiltrationOperation->getTarget();

                operationString = characterName + " was exfiltrated from x=" + std::to_string(from.x) + " y=" +
                                  std::to_string(from.y) + " to x=" + std::to_string(to.x) + " y=" +
                                  std::to_string(to.y);

                break;
            }
            case OperationEnum::RETIRE: {
                auto retireOperation = std::dynamic_pointer_cast<const spy::gameplay::RetireAction>(op);

                auto target = retireOperation->getTarget();

                operationString =
                        characterName + " retired on x=" + std::to_string(target.x) + " y=" + std::to_string(target.y);
                break;
            }
            default:
                break;
        }

        return operationString;
    }

    bool LibClient::lastOpSuccessful() const {
        return model->gameState.lastOpSuccessful;
    }

    std::optional<std::pair<bool, spy::util::UUID>> LibClient::isEnemy() const {
        return model->gameState.isEnemy;
    }
}
