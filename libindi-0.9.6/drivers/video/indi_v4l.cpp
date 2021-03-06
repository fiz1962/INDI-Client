#if 0
    V4L INDI Driver
    INDI Interface for V4L devices
    Copyright (C) 2003-2005 Jasem Mutlaq (mutlaqja@ikarustech.com)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

#endif

#include "v4ldriver.h"

V4L_Driver *MainCam = NULL;		/* Main and only camera */

/* send client definitions of all properties */
void ISInit()
{
  if (MainCam == NULL)
  {
    MainCam = new V4L_Driver();
    MainCam->initProperties("Generic Video4Linux");
    MainCam->initCamBase();
  }
}
    
void ISGetProperties (const char *dev)
{ 
   ISInit();
  
  MainCam->ISGetProperties(dev);
}


void ISNewSwitch (const char *dev, const char *name, ISState *states, char *names[], int n)
{
	
  ISInit();
  
  MainCam->ISNewSwitch(dev, name, states, names, n);
}

void ISNewText (const char *dev, const char *name, char *texts[], char *names[], int n)
{
  
   ISInit();
   
   MainCam->ISNewText(dev, name, texts, names, n);
}


void ISNewNumber (const char *dev, const char *name, double values[], char *names[], int n)
{
      
  ISInit();
  
  MainCam->ISNewNumber(dev, name, values, names, n);
}

void ISNewBLOB (const char *dev, const char *name, int sizes[], int blobsizes[], char *blobs[], char *formats[], char *names[], int n) {}
void ISSnoopDevice (XMLEle *root) {}
