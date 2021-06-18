//
// Created by Dominik Alfery on 09.05.2021.
//

#pragma once

/** struct to represent coordinates */
struct CCoord {
    /** @var x coordinate */
    unsigned m_X;
    /** @var y coordinate */
    unsigned m_Y;

    /**
     * constructor
     * @param startX x position
     * @param startY y position
     */
    CCoord(const unsigned startX, const unsigned startY) : m_X(startX), m_Y(startY) {};

    /**
     * overloaded operator for coordinates compare
     * @param rhs Coordinates on the right side
     * @return true if they are equal else false if they are not
     */
    bool operator==(const CCoord &rhs) const {
        return (m_X == rhs.m_X) && (m_Y == rhs.m_Y);
    }
};
