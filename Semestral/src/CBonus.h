//
// Created by Dominik Alfery on 14.06.2021.
//

#pragma once

#include <string>
#include <vector>
#include <memory>

#include "CCoord.h"
/** abstract class represents bonus */
class CBonus {
    /** @var bonus type */
    const std::string type;
    /** @var  bullet coordinates */
    CCoord coordinates;
public:
    /**
     * constructor
     * @param name name of bonus
     * @param coord where to spawn bonus
     */
    explicit CBonus(const std::string &name, const CCoord &coord);

    /**
     * virtual pure method to Clone *this
     * @return std::shared_ptr<CBonus> to *this
     */
    virtual std::shared_ptr<CBonus> Clone() const = 0;

    /**
     * virtual pure method to Use the defined bonus
     * @return vstd::vector<int> of defined numbers
     */
    virtual std::vector<int> Use() const = 0;

    /**
     * getter for coordinates
     * @return CCoord coordinates
     */
    CCoord getCoordinates() const;

    /**
     * getter for bonus type
     * @return std::string type
     */
    std::string getType() const;
};

/** derived class for bonus ammo */
class CAmmo : public CBonus {
public:
    /**
     * constructor
     * @param coord where to spawn bonus
     */
    explicit CAmmo(const CCoord &coord);

    /**
     * overriden method to get Clone of class CAmmo
     * @return std::shared_ptr<CBonus> of *this
     */
    std::shared_ptr<CBonus> Clone() const override;

    /**
     * overriden method to use bonus
     * @return std::vector<int> of defined numbers
     */
    std::vector<int> Use() const override;
};

/** derived class for larger magazine */
class CMagazine : public CBonus {
public:
    /**
     * constructor
     * @param coord where to spawn bonus
     */
    explicit CMagazine(const CCoord &coord);

    /**
     * overriden method to get Clone of class CMagazine
     * @return std::shared_ptr<CBonus> of *this
     */
    std::shared_ptr<CBonus> Clone() const override;

    /**
     * overriden method to use bonus
     * @return std::vector<int> of defined numbers
     */
    std::vector<int> Use() const override;
};

/** derived class for bonus health */
class CHealth : public CBonus {
public:
    /**
     * constructor
     * @param coord where to spawn bonus
     */
    explicit CHealth(const CCoord &coord);

    /**
     * overriden method to get Clone of class CHealth
     * @return std::shared_ptr<CBonus> of *this
     */
    std::shared_ptr<CBonus> Clone() const override;

    /**
     * overriden method to use bonus
     * @return std::vector<int> of defined numbers
     */
    std::vector<int> Use() const override;

};

/** derived class for faster reloading */
class CReloading : public CBonus {
public:
    /**
     * constructor
     * @param coord where to spawn bonus
     */
    explicit CReloading(const CCoord &coord);

    /**
     * overriden method to get Clone of class CReloading
     * @return std::shared_ptr<CBonus> of *this
     */
    std::shared_ptr<CBonus> Clone() const override;

    /**
     * overriden method to use bonus
     * @return std::vector<int> of defined numbers
     */
    std::vector<int> Use() const override;

};

/** derived class for extra damage */
class CDamage : public CBonus {
public:
    /**
     * constructor
     * @param coord where to spawn bonus
     */
    explicit CDamage(const CCoord &coord);

    /**
     * overriden method to get Clone of class CDamage
     * @return std::shared_ptr<CBonus> of *this
     */
    std::shared_ptr<CBonus> Clone() const override;

    /**
     * overriden method to use bonus
     * @return std::vector<int> of defined numbers
     */
    std::vector<int> Use() const override;
};