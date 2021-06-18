//
// Created by Dominik Alfery on 10.06.2021.
//

#include "CCharacter.h"

CCharacter::CCharacter(int numH, unsigned int numD, const CCoord &startCoords, const EDirection &dir,
                       const EField &chFace) : health(numH),
                                               damage(numD),
                                               coordinates(startCoords),
                                               oldcoordinates(startCoords),
                                               direction(dir),
                                               charFace(chFace),
                                               bullets() {
}

CCoord CCharacter::getCoordinates() const {
    return coordinates;
}

CCoord CCharacter::getOldCoordinates() const {
    return oldcoordinates;
}

void CCharacter::SetCoordinates(const CCoord &coord) {
    coordinates.m_X = coord.m_X;
    coordinates.m_Y = coord.m_Y;
}


int CCharacter::getHealth() const {
    return health;
}

void CCharacter::ChangeDirection() {
    switch (direction) {
        default:
            break;
        case EDirection::UP:
            direction = EDirection::DOWN;
            break;
        case EDirection::DOWN:
            direction = EDirection::UP;
            break;
        case EDirection::LEFT:
            direction = EDirection::RIGHT;
            break;
        case EDirection::RIGHT:
            direction = EDirection::LEFT;
            break;
    }
}

std::list<CBullet> &CCharacter::getBullets() {
    return bullets;
}

std::pair<unsigned, unsigned> CCharacter::getAmmo() const {
    return std::make_pair(ammoMagazine, ammoCount);
}

void CCharacter::getHit(const unsigned int &cntdamage) {
    health -= cntdamage;
    if (health <= 0)
        isDead = true;
}

bool CCharacter::IsDead() const {
    return isDead;
}

char CCharacter::getFace() const {
    return static_cast<char>(charFace);
}

unsigned CCharacter::getDamage() const {
    return damage;
}

void CCharacter::setBonus(const std::shared_ptr<CBonus> &bon) {
    bonus = bon;
}

std::shared_ptr<CBonus> CCharacter::getBonus() const {
    return bonus;
}

MainPlayer::MainPlayer(int numH, unsigned int numD, const CCoord &startCoords, const EDirection &dir,
                       const EField &chFace) : CCharacter(numH, numD, startCoords, dir, chFace) {
}

std::unique_ptr<CCharacter> MainPlayer::Clone() const {
    return std::make_unique<MainPlayer>(*this);
}

bool MainPlayer::Move(WINDOW *gamewin, const CInterface &myInterface) {

    if (!(oldcoordinates == coordinates))
        myInterface.PrintCharacter(gamewin, oldcoordinates, ' ');
    myInterface.PrintCharacter(gamewin, coordinates, getFace());

    if (isReloading) {
        currReloadTime++;
        if (currReloadTime == reloadTime) {
            isReloading = false;
            if (maxAmmoMagazine > ammoCount)
                ammoMagazine = ammoCount;
            else
                ammoMagazine = maxAmmoMagazine;
            currReloadTime = 0;
        }
    }

    switch (myInterface.getInput(gamewin)) {
        default:
            break;
        case EDirection::UP:
            oldcoordinates = coordinates;
            coordinates.m_Y--;
            direction = EDirection::UP;
            break;
        case EDirection::DOWN:
            oldcoordinates = coordinates;
            coordinates.m_Y++;
            direction = EDirection::DOWN;
            break;
        case EDirection::LEFT:
            oldcoordinates = coordinates;
            coordinates.m_X--;
            direction = EDirection::LEFT;
            break;
        case EDirection::RIGHT:
            oldcoordinates = coordinates;
            coordinates.m_X++;
            direction = EDirection::RIGHT;
            break;

        case EDirection::USE:
            if (bonus != nullptr) {
                ammoCount += bonus->Use()[0];
                maxAmmoMagazine += bonus->Use()[1];
                if (bonus->Use()[1]) {
                    if (maxAmmoMagazine > ammoCount)
                        ammoMagazine = ammoCount;
                    else
                        ammoMagazine = maxAmmoMagazine;
                }
                health += bonus->Use()[2];
                if (reloadTime)
                    reloadTime -= bonus->Use()[3];
                damage += bonus->Use()[4];
                bonus = nullptr;
            }
            break;
        case EDirection::SHOOT:
            if (!isReloading) {
                Shoot();
                if (!ammoMagazine)
                    isReloading = true;
            }
            break;
        case EDirection::QUIT:
            return false;
    }
    return true;
}

void MainPlayer::Shoot() {
    ammoCount--;
    ammoMagazine--;
    bullets.emplace_back(CCoord(coordinates.m_X, coordinates.m_Y), direction, damage);
}


bool Enemy::Move(WINDOW *gamewin, const CInterface &myInterface) {
    if (!(oldcoordinates == coordinates))
        myInterface.PrintCharacter(gamewin, oldcoordinates, ' ');
    myInterface.PrintCharacter(gamewin, coordinates, getFace());

    switch (direction) {
        default:
            break;
        case EDirection::UP:
            oldcoordinates = coordinates;
            coordinates.m_Y--;
            direction = EDirection::UP;
            break;
        case EDirection::DOWN:
            oldcoordinates = coordinates;
            coordinates.m_Y++;
            direction = EDirection::DOWN;
            break;
        case EDirection::LEFT:
            oldcoordinates = coordinates;
            coordinates.m_X--;
            direction = EDirection::LEFT;
            break;
        case EDirection::RIGHT:
            oldcoordinates = coordinates;
            coordinates.m_X++;
            direction = EDirection::RIGHT;
            break;
    }
    return true;
}

std::unique_ptr<CCharacter> Enemy::Clone() const {
    return std::make_unique<Enemy>(*this);
}

/*void Enemy::Shoot() {
    bullets.emplace_back(CCoord(coordinates.m_X, coordinates.m_Y), direction, damage);
}*/

Enemy::Enemy(int numH, unsigned int numD, const CCoord &startCoords, const EDirection &dir, const EField &chFace)
        : CCharacter(numH, numD, startCoords, dir, chFace) {
}

