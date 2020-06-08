//
// Created by Carolin on 07.06.2020.
//

#include "GameState.hpp"
#include "datatypes/gameplay/OperationEnum.hpp"
#include "datatypes/gameplay/CharacterOperation.hpp"
#include "datatypes/gameplay/PropertyAction.hpp"
#include "datatypes/character/PropertyEnum.hpp"
#include "util/GameLogicUtils.hpp"

void libclient::model::GameState::handleLastClientOperation() {
    using spy::gameplay::OperationEnum;
    const auto &operations_vec = this->operations;

    if (operations_vec.empty()) {
        return;
    }

    for (const auto &base_op : operations_vec) {
        auto op_type = base_op->getType();
        bool skip = (op_type == OperationEnum::CAT_ACTION
                     or op_type == OperationEnum::JANITOR_ACTION
                     or op_type == OperationEnum::EXFILTRATION);
        if (skip) {
            continue;
        }

        const auto &character_op = std::dynamic_pointer_cast<const spy::gameplay::CharacterOperation>(base_op);
        const auto &character_id = character_op->getCharacterId();

        // check if executing character is in chosen characters
        bool own = false;
        for (const auto &char_id : this->chosenCharacter) {
            if (char_id == character_id) {
                own = true;
                break;
            }
        }
        // if executing character was not one of clients characters, return
        if (!own) {
            return;
        }

        this->lastOpSuccessful = character_op->isSuccessful();
        this->isEnemy = std::nullopt;

        // if observation was used, set pair of <isEnemy, UUID of observed char>
        if (character_op->getType() == OperationEnum::PROPERTY_ACTION) {
            const auto property_op = std::dynamic_pointer_cast<const spy::gameplay::PropertyAction>(character_op);

            if (property_op->getUsedProperty() != spy::character::PropertyEnum::OBSERVATION) {
                return;
            }

            if (property_op->getIsEnemy().has_value()) {
                bool enemy = property_op->getIsEnemy().value();
                spy::util::UUID id = spy::util::GameLogicUtils::findInCharacterSetByCoordinates(state.getCharacters(),
                                                                                                property_op->getTarget())->getCharacterId();
                this->isEnemy = std::pair<bool, spy::util::UUID>(enemy, id);
            }
        }
    }

}
