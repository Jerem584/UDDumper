// UDDumper.cpp : Définit les fonctions exportées de la DLL.
//

#include "pch.h"
#include "framework.h"
#include "UDDumper.h"


// Il s'agit d'un exemple de variable exportée
UDDUMPER_API int nUDDumper=0;

// Il s'agit d'un exemple de fonction exportée.
UDDUMPER_API int fnUDDumper(void)
{
    return 0;
}

// Il s'agit du constructeur d'une classe qui a été exportée.
CUDDumper::CUDDumper()
{
    return;
}
