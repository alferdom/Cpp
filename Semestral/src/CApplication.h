//
// Created by Dominik Alfery on 09.05.2021.
//
#pragma once

#include "CInterface.h"

/** class respresenting whole app */
class CApplication {
private:
    /** @var CInterface type to start interface actions */
    CInterface myInterface;

public:
    /**
     * Constructor
     * @param interface
     */
    explicit CApplication(CInterface &interface);

    /**
     * deleted CApplication copying
     */
    CApplication(const CApplication &application) = delete;

    CApplication &operator=(const CApplication &application) = delete;

    ~CApplication() = default;

    /**
     * Start the app
     */
    void Run() const;

};