/*=========================================================================

  Program: DICOM for VTK

  Copyright (c) 2012-2024 David Gobbi
  All rights reserved.
  See Copyright.txt or http://dgobbi.github.io/bsd3.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkDICOMVR.h"
#include "vtkDICOMValue.h"

ostream& operator<<(ostream& o, const vtkDICOMVR& a)
{
  o << a.GetText();
  return o;
}

namespace {

typedef vtkDICOMVR VR;

const unsigned char AXT[256] = { // AE AS AT
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0, 0, 0, 0, 0, VR::AE, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //  ABCDEFGHIJKLMNO
  0, 0, 0, VR::AS, VR::AT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // PQRSTUVWXYZ
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

const unsigned char CXT[256] = { // CS
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0, 0, 0, 0,      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //  ABCDEFGHIJKLMNO
  0, 0, 0, VR::CS, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // PQRSTUVWXYZ
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

const unsigned char DXT[256] = { // DA DS DT
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0, VR::DA, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //  ABCDEFGHIJKLMNO
  0, 0, 0, VR::DS, VR::DT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // PQRSTUVWXYZ
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

const unsigned char FXT[256] = { // FD FL
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0, 0, 0, 0, VR::FD, 0, 0, 0, 0, 0, 0, 0, VR::FL, 0, 0, 0, //  ABCDEFGHIJKLMNO
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // PQRSTUVWXYZ
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

const unsigned char IXT[256] = { // IS
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //  ABCDEFGHIJKLMNO
  0, 0, 0, VR::IS, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // PQRSTUVWXYZ
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

const unsigned char LXT[256] = { // LO LT
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, VR::LO, //  ABCDEFGHIJKLMNO
  0, 0, 0, 0, VR::LT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // PQRSTUVWXYZ
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

const unsigned char OXT[256] = { // OB OF OL OV OW
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,VR::OB,0,VR::OD,0,VR::OF,0,0,0,0,0,VR::OL,0,0,0, //  ABCDEFGHIJKLMNO
  0, 0, 0, 0, 0, 0, VR::OV, VR::OW, 0, 0, 0, 0, 0, 0, 0, 0, // PQRSTUVWXYZ
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

const unsigned char PXT[256] = { // PN
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, VR::PN, 0, //  ABCDEFGHIJKLMNO
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // PQRSTUVWXYZ
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

const unsigned char SXT[256] = { // SH SL SQ SS ST SV
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0, 0, 0, 0, 0, 0, 0, 0, VR::SH, 0, 0, 0, VR::SL, 0, 0, 0, //  ABCDEFGHIJKLMNO
  0, VR::SQ, 0, VR::SS, VR::ST, 0, VR::SV, 0,0,0,0,0,0,0,0,0, // PQRSTUVWXYZ
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

const unsigned char TXT[256] = { // TM
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, VR::TM, 0, 0, //  ABCDEFGHIJKLMNO
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // PQRSTUVWXYZ
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

const unsigned char UXT[256] = { // UI UL UN US UT UV
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,VR::UC,0,0,0,0,0,VR::UI,0,0,VR::UL,0,VR::UN,0, //  ABCDEFGHIJKLMNO
  0, 0, VR::UR,VR::US,VR::UT, 0, VR::UV, 0,0,0,0,0,0,0,0,0, // PQRSTUVWXYZ
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

const unsigned char XXT[256] = { // Invalid VR
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

} // end anonymous namespace

const unsigned char *vtkDICOMVR::VRTable[256] = {
  XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,
  XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,
  XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,
  XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,
  XXT,AXT,XXT,CXT,DXT,XXT,FXT,XXT,XXT,IXT,XXT,XXT,LXT,XXT,XXT,OXT,
  PXT,XXT,XXT,SXT,TXT,UXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,
  XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,
  XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,
  XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,
  XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,
  XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,
  XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,
  XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,
  XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,
  XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,
  XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,XXT,
};

const unsigned char vtkDICOMVR::TypeTable[37] = {
  VTK_VOID,           // Invalid
  VTK_CHAR,           // AE Application Entity
  VTK_CHAR,           // AS Age String
  VTK_DICOM_TAG,      // AT Attribute Tag
  VTK_CHAR,           // CS Code String
  VTK_CHAR,           // DA Date
  VTK_CHAR,           // DS Decimal String
  VTK_CHAR,           // DT Date Time
  VTK_DOUBLE,         // FD Double
  VTK_FLOAT,          // FL Float
  VTK_CHAR,           // IS Integer String
  VTK_CHAR,           // LO Long String
  VTK_CHAR,           // LT Long Text
  VTK_UNSIGNED_CHAR,  // OB Other Byte
  VTK_DOUBLE,         // OD Other Double
  VTK_FLOAT,          // OF Other Float
  VTK_UNSIGNED_INT,   // OL Other Long
  VTK_UNSIGNED_SHORT, // OW Other Word
  VTK_CHAR,           // PN Personal Name
  VTK_CHAR,           // SH Short String
  VTK_INT,            // SL Signed Long
  VTK_DICOM_ITEM,     // SQ Sequence
  VTK_SHORT,          // SS Signed Short
  VTK_CHAR,           // ST Short Text
  VTK_CHAR,           // TM Time
  VTK_CHAR,           // UC Unlimited Characters
  VTK_CHAR,           // UI UID
  VTK_UNSIGNED_INT,   // UL Unsigned Long
  VTK_UNSIGNED_CHAR,  // UN Unknown
  VTK_CHAR,           // UR URI or URL
  VTK_UNSIGNED_SHORT, // US Unsigned Short
  VTK_CHAR,           // UT Unlimited Text
  VTK_VOID,           // Invalid
  VTK_VOID,           // Invalid
  VTK_UNSIGNED_LONG_LONG, // OV Other Very Long
  VTK_LONG_LONG,      // SV Signed Very Long
  VTK_UNSIGNED_LONG_LONG, // UV Unsigned Very Long
};

const char vtkDICOMVR::TextTable[37][4] = {
  { 0, 0, 0, 0 },     // Invalid
  { 'A', 'E', 0, 0 }, // AE Application Entity
  { 'A', 'S', 0, 0 }, // AS Age String
  { 'A', 'T', 0, 0 }, // AT Attribute Tag
  { 'C', 'S', 0, 0 }, // CS Code String
  { 'D', 'A', 0, 0 }, // DA Date
  { 'D', 'S', 0, 0 }, // DS Decimal String
  { 'D', 'T', 0, 0 }, // DT Date Time
  { 'F', 'D', 0, 0 }, // FD Double
  { 'F', 'L', 0, 0 }, // FL Float
  { 'I', 'S', 0, 0 }, // IS Integer String
  { 'L', 'O', 0, 0 }, // LO Long String
  { 'L', 'T', 0, 0 }, // LT Long Text
  { 'O', 'B', 0, 0 }, // OB Other Byte
  { 'O', 'D', 0, 0 }, // OD Other Double
  { 'O', 'F', 0, 0 }, // OF Other Float
  { 'O', 'L', 0, 0 }, // OL Other Long
  { 'O', 'W', 0, 0 }, // OW Other Word
  { 'P', 'N', 0, 0 }, // PN Personal Name
  { 'S', 'H', 0, 0 }, // SH Short String
  { 'S', 'L', 0, 0 }, // SL Signed Long
  { 'S', 'Q', 0, 0 }, // SQ Sequence
  { 'S', 'S', 0, 0 }, // SS Signed Short
  { 'S', 'T', 0, 0 }, // ST Short Text
  { 'T', 'M', 0, 0 }, // TM Time
  { 'U', 'C', 0, 0 }, // UC Unlimited Characters
  { 'U', 'I', 0, 0 }, // UI UID
  { 'U', 'L', 0, 0 }, // UL Unsigned Long
  { 'U', 'N', 0, 0 }, // UN Unknown
  { 'U', 'R', 0, 0 }, // UR URI or URL
  { 'U', 'S', 0, 0 }, // US Unsigned Short
  { 'U', 'T', 0, 0 }, // UT Unlimited Text
  { 'O', 'X', 0, 0 },     // Invalid
  { 'X', 'S', 0, 0 },     // Invalid
  { 'O', 'V', 0, 0 }, // OV Other Very Long
  { 'S', 'V', 0, 0 }, // SV Signed Very Long
  { 'U', 'V', 0, 0 }, // UV Unsigned Very Long
};
