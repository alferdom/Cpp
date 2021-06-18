//
// Created by Dominik Alfery on 11.06.2021.
//

#include "CBullet.h"

CBullet::CBullet(const CCoord &coord, const EDirection &dir, unsigned int bulldmg) : coordinates(coord),
                                                                                     direction(dir),
                                                                                     damage(bulldmg) {
}

void CBullet::Move() {
    switch (direction) {
        case EDirection::UP:
            coordinates.m_Y--;
            break;
        case EDirection::DOWN:
            coordinates.m_Y++;
            break;
        case EDirection::LEFT:
            coordinates.m_X--;
            break;
        case EDirection::RIGHT:
            coordinates.m_X++;
            break;
        default:
            break;
    }
}

