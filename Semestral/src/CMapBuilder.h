//
// Created by Dominik Alfery on 08.06.2021.
//

#pragma once

#include <memory>
#include <string>
#include <iostream>
#include <fstream>

#include "CMap.h"

/** class representing map builder from file generator */
class CMapBuilder {
private:
    /** @var name of the file */
    const std::string mapFilename;
    /** @var file ending */
    const std::string mapNameEnding;

public:
    /**
     * constructor
     */
    CMapBuilder();

    /**
     * check if all level files are present
     * @param maxLevels number of levels
     * @return true if they all exists else false if they do not
     */
    bool CheckMapFiles(int maxLevels) const;

    /**
     * generate map level from file
     * @param currLevel what current level to generate
     * @return std::unique_ptr<CMap> to loaded map
     */
    std::unique_ptr<CMap> Generate(int currLevel) const;
};


