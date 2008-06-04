/*
 *  ppui/sdl/DisplayDeviceOGL_SDL.cpp
 *
 *  Copyright 2008 Peter Barth, Christopher O'Neill
 *
 *  This file is part of Milkytracker.
 *
 *  Milkytracker is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Milkytracker is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Milkytracker.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "DisplayDeviceOGL_SDL.h"
#include "Graphics.h"

PPDisplayDeviceOGL::PPDisplayDeviceOGL(SDL_Surface*& screen, 
									   pp_int32 width, 
									   pp_int32 height, 
									   pp_int32 bpp,
									   bool fullScreen, 
									   Orientations theOrientation/* = ORIENTATION_NORMAL*/, 
									   bool swapRedBlue/* = false*/) :
	PPDisplayDevice(screen, width, height, bpp, fullScreen, theOrientation)
{
	const SDL_VideoInfo* videoinfo;

	/* Some SDL to get display format */
	videoinfo = SDL_GetVideoInfo();
	if (bpp == -1) 
	{
		bpp = videoinfo->vfmt->BitsPerPixel > 16 ? videoinfo->vfmt->BitsPerPixel : 16;
	}

    Uint32 videoFlags = SDL_OPENGL;    /* Enable OpenGL in SDL          */
    videoFlags |= SDL_GL_DOUBLEBUFFER; /* Enable double buffering       */
    videoFlags |= SDL_HWPALETTE;       /* Store the palette in hardware */

    /* This checks to see if surfaces can be stored in memory */
    if (videoinfo->hw_available)
	videoFlags |= videoinfo->hw_available ? SDL_HWSURFACE : SDL_SWSURFACE;

    /* This checks if hardware blits can be done */
    if (videoinfo->blit_hw)
		videoFlags |= SDL_HWACCEL;

    /* Sets up OpenGL double buffering */
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	/* Set a video mode */
	theSurface = screen = CreateScreen(realWidth, realHeight, 
									   bpp, videoFlags | (bFullScreen==true ? SDL_FULLSCREEN : 0));
	if ( screen == NULL ) 
	{
		fprintf(stderr, "Could not set video mode: %s\n", SDL_GetError());	
		exit(2);
	}	

	currentGraphics = new PPGraphics_OGL(width, height, 0, NULL);	
	currentGraphics->lock = true;
}

PPDisplayDeviceOGL::~PPDisplayDeviceOGL()
{	
	// base class is responsible for deleting currentGraphics
}

PPGraphicsAbstract* PPDisplayDeviceOGL::open()
{
	if (!isEnabled())
		return NULL;

	if (currentGraphics->lock)
	{
		if (SDL_LockSurface(theSurface) < 0)
			return NULL;

		currentGraphics->lock = false;

		return currentGraphics;
	}
	
	return NULL;
}

void PPDisplayDeviceOGL::close()
{
	SDL_UnlockSurface(theSurface);

	currentGraphics->lock = true;
}

void PPDisplayDeviceOGL::update()
{
	if (!isUpdateAllowed() || !isEnabled())
		return;
	
	if (theSurface->locked)
	{
		return;
	}
	
	SDL_UpdateRect(theSurface, 0, 0, 0, 0);
}

void PPDisplayDeviceOGL::update(const PPRect& r)
{
	if (!isUpdateAllowed() || !isEnabled())
		return;
	
	if (theSurface->locked)
	{
		return;
	}

	PPRect r3 = r;
	transformInverse(r3);

	SDL_UpdateRect(theSurface, r3.x1, r3.y1, r3.x2-r3.x1, r3.y2-r3.y1);
}

