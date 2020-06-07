/**
 * @file    LibClient.cpp
 * @author  Marco
 * @date    14.04.2020 (creation)
 * @brief   Implementation of LibClient
 */
#include "LibClient.hpp"

#include <utility>

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
    }

    std::optional<bool> LibClient::amIPlayer1() {
        if (network.getState() != Network::NetworkState::IN_GAME &&
            network.getState() != Network::NetworkState::IN_GAME_ACTIVE &&
            network.getState() != Network::NetworkState::PAUSE &&
            network.getState() != Network::NetworkState::GAME_OVER) {
            return std::nullopt;
        }
        return model->clientState.amIPlayer1(model->clientState.role);
    }

    auto
    LibClient::getUnknownFactionList() -> std::map<spy::util::UUID, std::vector<std::pair<spy::character::FactionEnum, float>>> {
        return model->aiState.unknownFaction;
    }

    std::vector<spy::util::UUID> LibClient::getMyFactionList() {
        return model->aiState.myFaction;
    }

    std::vector<spy::util::UUID> LibClient::getEnemyFactionList() {
        return model->aiState.enemyFaction;
    }

    std::vector<spy::util::UUID> LibClient::getNpcFactionList() {
        return model->aiState.npcFaction;
    }

    auto
    LibClient::getUnknownGadgetsList() -> std::unordered_map<std::shared_ptr<spy::gadget::Gadget>, std::vector<std::pair<spy::util::UUID, float>>> {
        return model->aiState.unknownGadgets;
    }
}