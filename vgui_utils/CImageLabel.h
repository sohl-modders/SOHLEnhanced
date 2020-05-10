//========= Copyright © 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================
#ifndef VGUI_UTILS_CIMAGELABEL_H
#define VGUI_UTILS_CIMAGELABEL_H

#include <VGUI_Label.h>
#include <VGUI_BitmapTGA.h>

// Wrapper for an Image Label without a background
class CImageLabel : public vgui::Label
{
public:
	vgui::BitmapTGA	*m_pTGA;

public:
	void LoadImage( const char * pImageName );
	CImageLabel( const char* pImageName, int x, int y );
	CImageLabel( const char* pImageName, int x, int y, int wide, int tall );

	virtual int getImageTall();
	virtual int getImageWide();

	virtual void paintBackground()
	{
		// Do nothing, so the background's left transparent.
	}
};

#endif //VGUI_UTILS_CIMAGELABEL_H
