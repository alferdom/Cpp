//
// Created by Dominik Alfery on 09.05.2021.
//

#pragma once

#include <string>
#include <memory>
#include <vector>
#include <list>
#include <random>

#include "CCoord.h"
#include "CBonus.h"
#include "EDirection.h"
#include "EField.h"
#include "CInterface.h"
#include "CBullet.h"
#include "ncurses.h"

/**
 * abstract class representing in game character
 */
class CCharacter {
protected:
    /**@var health of character */
    int health;
    /**@var damage made by character */
    unsigned int damage;
    /**@var current coordinates */
    CCoord coordinates;
    /**@var old coordinates */
    CCoord oldcoordinates;
    /**@var facing direction*/
    EDirection direction;
    /**@var face of character */
    EField charFace;
    /**@var bool if is reloading */
    bool isReloading = false;
    /**@var bool is dead */
    bool isDead = false;
    /**@var starting ammo count*/
    unsigned int ammoCount = 35;
    /**@var ammo in magazine */
    unsigned int ammoMagazine = 7;
    /**@var maximum ammo in magazine */
    unsigned int maxAmmoMagazine = ammoMagazine;
    /**@var reloading time */
    unsigned int reloadTime = 40;
    /**@var current reload time */
    unsigned int currReloadTime = 0;
    /**@var characters bullets */
    std::list<CBullet> bullets;
    /**@var stored bonus */
    std::shared_ptr<CBonus> bonus = nullptr;

public:
    /**
     * constructor of abstract class
     * @param numH starting health
     * @param numD starting damage
     * @param startCoords starting coordinates
     * @param dir facing direction
     * @param chFace how will be character represent
     */
    CCharacter(int numH, unsigned int numD, const CCoord &startCoords, const EDirection &dir, const EField &chFace);

    /**
     * virtual pure method for moving
     * @param gamewin where to display movement
     * @param myInferface interface that manages displaying
     * @returns true if moved else false
     */
    virtual bool Move(WINDOW *gamewin, const CInterface &myInferface) = 0;

    /**
     * virtual pure method to Clone *this
     * @return unique_ptr of abstract class
     */
    virtual std::unique_ptr<CCharacter> Clone() const = 0;

    /**
     * getter for coordinates
     * @return CCoord coordinates
     */
    CCoord getCoordinates() const;

    /**
     * getter for old coordinates
     * @return CCoord oldcoordinates
     */
    CCoord getOldCoordinates() const;

    /**
     * getter to get character's health
     * @return int health
     */
    int getHealth() const;

    /**
     * getter to get character's damage
     * @return unsigned int damage
     */
    unsigned getDamage() const;

    /**
     * getter to get character's face
     * @return char charFace
     */
    char getFace() const;

    /**
     * to hit character
     * @param cntdamage what damage inflicted
     */
    void getHit(const unsigned int &cntdamage);

    /**
     * setter for coordinates
     * @param coord new coordinates
     */
    void SetCoordinates(const CCoord &coord);

    /**
     * getter for character's ammo and magazine
     * @return std::pair<unsigned, unsigned> ammomagazine and ammocount
     */
    std::pair<unsigned, unsigned> getAmmo() const;

    /**
     * reverse facing direction
     */
    void ChangeDirection();

    /**
     * getter for reference to list of bullets
     * @return std::list<CBullet> & bullets
     */
    std::list<CBullet> &getBullets();

    /**
     * getter for isDead bool
     * @return bool isDead
     */
    bool IsDead() const;

    /**
     * setter to get bonus
     * @param bon obtained bonus
     */
    void setBonus(const std::shared_ptr<CBonus> &bon);

    /**
     * getter for bonus
     * @return current stored bonus
     */
    std::shared_ptr<CBonus> getBonus() const;
};

/**
 * derived class to represents playable character
 */
class MainPlayer : public CCharacter {
protected:
public:
    /**
     * constructor
     * @param numH starting health
     * @param numD starting damage
     * @param startCoords starting coordinates
     * @param dir facing direction
     * @param chFace how will be character represent
     */
    MainPlayer(int numH, unsigned int numD, const CCoord &startCoords, const EDirection &dir, const EField &chFace);

    /**
     * overriden virtual method Move to manage playable player movement
     * @param gamewin from which window to get input
     * @param myInferface interface managing drawing and input
     * @return true if moved else false if player hit quit key
     */
    bool Move(WINDOW *gamewin, const CInterface &myInferface) override;

    /**
     * method for make shots
     */
    void Shoot();

    /**
     * overriden method to get Clone of class MainPlayer
     * @return std::unique_ptr<CCharacter> of *this
     */
    std::unique_ptr<CCharacter> Clone() const override;
};

/**
 * derived class to represents enemy AI character
 */
class Enemy : public CCharacter {
protected:
public:
    /**
   * constructor
   * @param numH starting health
   * @param numD starting damage
   * @param startCoords starting coordinates
   * @param dir facing direction
   * @param chFace how will be character represent
   */
    Enemy(int numH, unsigned int numD, const CCoord &startCoords, const EDirection &dir, const EField &chFace);

    /**
    * overriden virtual method Move to manage enemy AI movement
    * @param gamewin from which window to get input
    * @param myInferface interface managing drawing
    * @return true
    */
    bool Move(WINDOW *gamewin, const CInterface &myInterface) override;

    /**
     * overriden method to get Clone of class Enemy
     * @return std::unique_ptr<CCharacter> of *this
     */
    std::unique_ptr<CCharacter> Clone() const override;
};
