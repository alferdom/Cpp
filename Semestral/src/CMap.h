//
// Created by Dominik Alfery on 08.06.2021.
//
#pragma once

#include <map>
#include <string>
#include <iostream>
#include <memory>

/** class representing game map */
class CMap {
private:
    /** @var height of the map */
    const size_t mapHeight;
    /** @var width of the map */
    const size_t mapWidth;
    /** @var stored loaded map data */
    std::map<unsigned, std::string> mapDATA;

public:
    /**
     * constructor to create empty map
     */
    CMap();

    /**
     * constructor to create map of defined atributes
     * @param Height height of the map
     * @param Width width of the map
     * @param loadMAP loaded mapa data
     */
    CMap(const size_t &Height, const size_t &Width, std::map<unsigned, std::string> &loadMAP);

    /**
     * getter to get map height
     * @return map height
     */
    size_t GetHeight() const;

    /**
     * gettter to get map width
     * @return map width
     */
    size_t GetWidth() const;

    /**
     * getter to get mapdata
     * @return std::map<unsigned, std::string> of map data
     */
    std::map<unsigned, std::string> GetMapData() const;
};