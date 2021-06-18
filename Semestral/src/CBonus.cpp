//
// Created by Dominik Alfery on 14.06.2021.
//

#include "CBonus.h"

#include <utility>

CBonus::CBonus(const std::string &name, const CCoord &coord) : type(name), coordinates(coord) {

}

CCoord CBonus::getCoordinates() const {
    return coordinates;
}

std::string CBonus::getType() const {
    return type;
}

CAmmo::CAmmo(const CCoord &coord) : CBonus("Ammo", coord) {

}

std::vector<int> CAmmo::Use() const {
    return std::vector<int>{20, 0, 0, 0, 0};
}

std::shared_ptr<CBonus> CAmmo::Clone() const {
    return std::make_shared<CAmmo>(*this);
}

CMagazine::CMagazine(const CCoord &coord) : CBonus("Magazine", coord) {

}

std::vector<int> CMagazine::Use() const {
    return std::vector<int>{0, 5, 0, 0, 0};
}

std::shared_ptr<CBonus> CMagazine::Clone() const {
    return std::make_shared<CMagazine>(*this);

}


CHealth::CHealth(const CCoord &coord) : CBonus("Health", coord) {

}

std::vector<int> CHealth::Use() const {
    return std::vector<int>{0, 0, 20, 0, 0};
}

std::shared_ptr<CBonus> CHealth::Clone() const {
    return std::make_shared<CHealth>(*this);
}


CReloading::CReloading(const CCoord &coord) : CBonus("Reloading", coord) {

}

std::vector<int> CReloading::Use() const {
    return std::vector<int>{0, 0, 0, 4, 0};
}

std::shared_ptr<CBonus> CReloading::Clone() const {
    return std::make_shared<CReloading>(*this);
}


CDamage::CDamage(const CCoord &coord) : CBonus("Damage", coord) {

}

std::vector<int> CDamage::Use() const {
    return std::vector<int>{0, 0, 0, 0, 10};
}

std::shared_ptr<CBonus> CDamage::Clone() const {
    return std::make_shared<CDamage>(*this);
}

