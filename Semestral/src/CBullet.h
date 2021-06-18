//
// Created by Dominik Alfery on 11.06.2021.
//

#pragma once

#include "CCoord.h"
#include "EField.h"
#include "EDirection.h"
/** struct represents one bullet */
struct CBullet {
    /** @var bullet coordinates */
    CCoord coordinates;
    /** bullet direction */
    EDirection direction;
    /** @var if it was just shot */
    bool firstshot = true;
    /** @var bullet damage */
    unsigned int damage;

    /**
     * construcot
     * @param coord where to make bullets
     * @param dir direction of bullet
     * @param damage damage of bullet
     */
    CBullet(const CCoord &coord, const EDirection &dir, unsigned int damage);

    /**
     * move bullet in the direction
     */
    void Move();
};