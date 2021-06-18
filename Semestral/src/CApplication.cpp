#include "CApplication.h"


CApplication::CApplication(CInterface &interface) : myInterface(std::move(interface)) {}

void CApplication::Run() const{
    myInterface.StartDisplay();
    myInterface.MakeMenu();
}
