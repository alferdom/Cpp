//
// Created by Dominik Alfery on 08.06.2021.
//

#include "CMapBuilder.h"

CMapBuilder::CMapBuilder() : mapFilename("./examples/Level"), mapNameEnding(".ini") {
}


bool CMapBuilder::CheckMapFiles(int maxLevels) const {
    for (int i = 1; i <= maxLevels; ++i) {
        std::string filename = mapFilename + std::to_string(maxLevels) + mapNameEnding;
        std::ifstream infile(filename);

        if (!infile) {
            return false;
        }

        infile.close();
    }
    return true;
}


std::unique_ptr<CMap> CMapBuilder::Generate(int currLevel) const {
    std::map<unsigned, std::string> readMap;
    std::string mapLine;
    std::string filename = mapFilename + std::to_string(currLevel) + mapNameEnding;
    std::ifstream MyReadFile(filename);
    unsigned int row = 0;
    while (getline(MyReadFile, mapLine)) {
        readMap[row] = {std::move(mapLine)};
        row++;
    }

    CMap tmp(row, readMap.begin()->second.size(), readMap);

    MyReadFile.close();
    return std::make_unique<CMap>(tmp);
}


