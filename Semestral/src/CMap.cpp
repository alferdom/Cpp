//
// Created by Dominik Alfery on 08.06.2021.
//

#include "CMap.h"


CMap::CMap() : mapHeight(0), mapWidth(0) {

}

CMap::CMap(const size_t &Height, const size_t &Width, std::map<unsigned, std::string> &loadMAP) : mapHeight(Height),
                                                                                                  mapWidth(Width),
                                                                                                  mapDATA(std::move(
                                                                                                          loadMAP)) {

}

size_t CMap::GetHeight() const {
    return mapHeight;
}

size_t CMap::GetWidth() const {
    return mapWidth;
}

std::map<unsigned, std::string> CMap::GetMapData() const {
    return mapDATA;
}

