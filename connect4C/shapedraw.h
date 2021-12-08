#pragma once
#include "ezsdl.h"

void EZ_FillRect(EZ_SDL_Context* context, int x, int y, int w, int h) {
    SDL_SetRenderDrawColor(context->renderer,
        context->c.r, context->c.g, context->c.b, context->c.a);
    SDL_Rect r;
    r.x = x;
    r.y = y;
    r.w = w;
    r.h = h;
    SDL_RenderFillRect(context->renderer, &r);
}

void EZ_DrawRect(EZ_SDL_Context* context, int x, int y, int w, int h) {
    SDL_SetRenderDrawColor(context->renderer,
        context->c.r, context->c.g, context->c.b, context->c.a);
    SDL_Rect r;
    r.x = x;
    r.y = y;
    r.w = w;
    r.h = h;
    SDL_RenderDrawRect(context->renderer, &r);
}

void EZ_DrawCircle(EZ_SDL_Context* context, int32_t centreX, int32_t centreY, int32_t radius){
    const int32_t diameter = (radius * 2);

    int32_t x = (radius - 1);
    int32_t y = 0;
    int32_t tx = 1;
    int32_t ty = 1;
    int32_t error = (tx - diameter);

    SDL_SetRenderDrawColor(context->renderer,
        context->c.r, context->c.g, context->c.b, context->c.a);

    while (x >= y)
    {
        //  Each of the following renders an octant of the circle
        SDL_RenderDrawPoint(context->renderer, centreX + x, centreY - y);
        SDL_RenderDrawPoint(context->renderer, centreX + x, centreY + y);
        SDL_RenderDrawPoint(context->renderer, centreX - x, centreY - y);
        SDL_RenderDrawPoint(context->renderer, centreX - x, centreY + y);
        SDL_RenderDrawPoint(context->renderer, centreX + y, centreY - x);
        SDL_RenderDrawPoint(context->renderer, centreX + y, centreY + x);
        SDL_RenderDrawPoint(context->renderer, centreX - y, centreY - x);
        SDL_RenderDrawPoint(context->renderer, centreX - y, centreY + x);

        if (error <= 0)
        {
            ++y;
            error += ty;
            ty += 2;
        }

        if (error > 0)
        {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }
}

//void EZ_FillCircle(EZ_SDL_Context* context, int x, int y, int radius){
//
//    SDL_SetRenderDrawColor(context->renderer, 
//        context->c.r, context->c.g, context->c.b, context->c.a);
//
//    for (int w = 0; w < radius * 2; w++)
//    {
//        for (int h = 0; h < radius * 2; h++)
//        {
//            int dx = radius - w; // horizontal offset
//            int dy = radius - h; // vertical offset
//            if ((dx * dx + dy * dy) <= (radius * radius))
//            {
//                SDL_RenderDrawPoint(context->renderer, x + dx, y + dy);
//            }
//        }
//    }
//}

void EZ_FillCircle(EZ_SDL_Context* context, int cx, int cy, int radius)
{
    // Note that there is more to altering the bitrate of this 
    // method than just changing this value.  See how pixels are
    // altered at the following web page for tips:
    //   http://www.libsdl.org/intro.en/usingvideo.html
    static const int BPP = 4;
    SDL_SetRenderDrawColor(context->renderer, context->c.r, context->c.g, context->c.b, context->c.a);
    for (int dy = 1; dy <= radius; dy += 1)
    {
        int dx = (int)floor(sqrt((2.0 * radius * dy) - (dy * dy)));
        int x = cx - dx;
        SDL_RenderDrawLine(context->renderer, cx - dx, cy + dy - radius, cx + dx, cy + dy - radius);
        SDL_RenderDrawLine(context->renderer, cx - dx, cy - dy + radius, cx + dx, cy - dy + radius);
    }
}