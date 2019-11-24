#include <TXLib.h>
#include "Menu.h"
#include "Tools.h"
#include "Buttons.h"

//-----------------------------------------------------------------------------

const int WindWidth = 800;
const int WindHeight = 800;

//-----------------------------------------------------------------------------

enum Result

{

    ResultExit,
    ResultRestart

};

//-----------------------------------------------------------------------------

struct Painter

{

     void (*draw) (COLORREF color, int r);
     void (*use) (COLORREF color, int r, HDC dc);

     int r;

     COLORREF color;

     HDC drawField;

};

//-----------------------------------------------------------------------------

int run (int WindWidth, int WindHeight);

void (*use) (COLORREF color, int r, HDC dc);

void (*draw) (COLORREF color, int r);

void menu (Painter * painter);

void chooseTool (Painter * painter);

void changeColor (Painter * painter);

void drawSubColorBar (int x, int y, int width, int height, COLORREF color, HDC dc);

void drawColorBar (int x, int y, int width, int height, HDC dc);

//-----------------------------------------------------------------------------

int main ()

{

    txCreateWindow (WindWidth, WindHeight);
    txDisableAutoPause ();

    while (true)

    {

        int result = run (WindWidth, WindHeight);

        if (result == ResultExit)

        {

            break;

        }

        if (result == ResultRestart)

        {

        }

    }

    return 0;

}

//-----------------------------------------------------------------------------

int run (int WindWidth, int WindHeight)

{

    Painter painter = {drawDefault, useDefault, 10, TX_WHITE, txCreateCompatibleDC (WindWidth, WindHeight)};

    txSetFillColor (RGB (45, 45, 45), painter.drawField);
    txClear (painter.drawField);

    while (true)

    {

        if (txMouseButtons () == 1)

        {

            painter.use (painter.color, painter.r, painter.drawField);

        }

        if (txMouseButtons () == 2)

        {

            painter.color = txGetPixel (txMouseX (), txMouseY (), painter.drawField);

        }

        if (GetAsyncKeyState (VK_ESCAPE))

        {

            txDeleteDC (painter.drawField);

            return ResultExit;

        }

        if (GetAsyncKeyState ('R'))

        {

            txDeleteDC (painter.drawField);

            return ResultRestart;

        }

        if (GetAsyncKeyState ('M'))

        {

            menu (&painter);

            txSleep (100);

        }

        if (GetAsyncKeyState (VK_RIGHT))

        {

            painter.r ++;

        }

        if (GetAsyncKeyState (VK_LEFT))

        {

            painter.r--;

        }

        txBitBlt (txDC (), 0, 0, 0, 0, painter.drawField);

        painter.draw (painter.color, painter.r);

        txSleep (1);

    }

}

void menu (Painter * painter)

{

    MenuButton buttons[] =

    {

        {false, NULL, "Tools", false},
        {false, NULL, "Color", false},
        {true}

    };

    Menu m = {WindWidth / 2 - 50, WindHeight / 2 - 100, 100, 200, "Menu", "Arial", "Comic Sans MS", TX_WHITE, RGB (45, 45, 45), RGB (200, 200, 200), RGB (45, 45, 45), RGB (45, 45, 45), txDC (), buttons};

    while (!GetAsyncKeyState (VK_ESCAPE))

    {

        m.draw ();

        int result = m.check ();

        if (result == 0)

        {

            txSleep (100);

            chooseTool (painter);

            return;

        }

        if (result == 1)

        {

            txSleep (100);

            changeColor (painter);

            return;

        }

        txSleep (1);

    }

    txSleep (100);

}

//-----------------------------------------------------------------------------

void chooseTool (Painter * painter)

{

    MenuButton buttons[] = {

        {false, NULL,   "Default", false},
        {false, NULL,      "Line", false},
        {false, NULL, "Rectanhle", false},
        {true}

    };

    Menu m = {WindWidth / 2 - 50, WindHeight / 2 - 100, 100, 200, "Tools", "Arial", "Comic Sans MS", TX_WHITE, RGB (45, 45, 45), RGB (200, 200, 200), RGB (45, 45, 45), RGB (45, 45, 45), txDC (), buttons};

    while (!GetAsyncKeyState (VK_ESCAPE))

    {

        m.draw ();

        int result = m.check ();

        switch (result)

        {

            case 0:

            {

                painter -> draw = drawDefault;
                painter -> use = useDefault;
                return;
                break;

            }

            case 1:

            {

                painter -> draw = drawLine;
                painter -> use = useLine;
                return;
                break;

            }

            case 2:

            {

                painter -> draw = drawRectangle;
                painter -> use = useRectangle;
                return;
                break;

            }

        }

        txSleep (1);

    }

}

//-----------------------------------------------------------------------------

void drawColorBar (int x, int y, int width, int height, HDC dc)

{

    int r = 255;
    int g = 0;
    int b = 0;

    double interval = width / (255.0 * 6.0);

    double lineX = 0;

    for (int i = 0; i < 255; i++)

    {

        COLORREF color = RGB (r, g, b);

        txSetColor (color, 1, dc);
        txSetFillColor (color, dc);

        txRectangle (x + lineX, y, x + lineX + interval, y + height, dc);

        lineX += interval;

        g++;

    }

    for (int i = 0; i < 255; i++)

    {

        COLORREF color = RGB (r, g, b);

        txSetColor (color, 1, dc);
        txSetFillColor (color, dc);

        txRectangle (x + lineX, y, x + lineX + interval, y + height, dc);

        lineX += interval;

        r--;

    }

    for (int i = 0; i < 255; i++)

    {

        COLORREF color = RGB (r, g, b);

        txSetColor (color, 1, dc);
        txSetFillColor (color, dc);

        txRectangle (x + lineX, y, x + lineX + interval, y + height, dc);

        b++;

        lineX += interval;

    }

    for (int i = 0; i < 255; i++)

    {

        COLORREF color = RGB (r, g, b);

        txSetColor (color, 1, dc);
        txSetFillColor (color, dc);

        txRectangle (x + lineX, y, x + lineX + interval, y + height, dc);

        g--;

        lineX += interval;

    }

    for (int i = 0; i < 255; i++)

    {

        COLORREF color = RGB (r, g, b);

        txSetColor (color, 1, dc);
        txSetFillColor (color, dc);

        txRectangle (x + lineX, y, x + lineX + interval, y + height, dc);

        r++;

        lineX += interval;

    }

    for (int i = 0; i < 255; i++)

    {

        COLORREF color = RGB (r, g, b);

        txSetColor (color, 1, dc);
        txSetFillColor (color, dc);

        txRectangle (x + lineX, y, x + lineX + interval, y + height, dc);

        b--;

        lineX += interval;

    }

}

//-----------------------------------------------------------------------------

void drawSubColorBar (int x, int y, int width, int height, COLORREF color, HDC dc)

{

    double r = txExtractColor (color, TX_RED), g = txExtractColor (color, TX_GREEN), b = txExtractColor (color, TX_BLUE);

    double lineX = 0;

    double interval = width / (255.0 * 2.0);

    for (int i = 0; i < 255; i++)

    {

        COLORREF l_color = RGB (r / 255 * i, g / 255 * i, b / 255 * i);

        txSetColor (l_color, 1, dc);
        txSetFillColor (l_color, dc);

        txRectangle (x + lineX, y, x + lineX + interval, y + height, dc);

        lineX += interval;

    }

    for (int i = 0; i < 255; i++)

    {

        COLORREF l_color = RGB (r + (255 - r) / 255 * i, g + (255 - g) / 255 * i, b + (255 - b) / 255 * i);

        txSetColor (l_color, 1, dc);
        txSetFillColor (l_color, dc);

        txRectangle (x + lineX, y, x + lineX + interval, y + height, dc);

        lineX += interval;

    }

}

//-----------------------------------------------------------------------------

void changeColor (Painter * painter)

{

    int colorBarWidth = 300, colorBarHeight = 20;

    COLORREF currentcolor = painter -> color;
    COLORREF subcolor = painter -> color;

    HDC dc = txCreateCompatibleDC (WindWidth, WindHeight);

    Button buttons[] =

    {

        {WindWidth / 2 + colorBarWidth / 2 -  40, WindHeight / 2 + colorBarHeight / 2 + 20, 40, 20,     "Ok", "Arial", 20, dc, RGB (32, 32, 32), TX_WHITE, TX_WHITE, false, 2},
        {WindWidth / 2 + colorBarWidth / 2 - 140, WindHeight / 2 + colorBarHeight / 2 + 20, 80, 20, "Cancel", "Arial", 20, dc, RGB (32, 32, 32), TX_WHITE, TX_WHITE, false, 2},
        EndButton

    };

    drawColorBar (WindWidth / 2 - colorBarWidth / 2, WindHeight / 2 - colorBarHeight / 2, 300, 20, dc);
    drawSubColorBar (WindWidth / 2 - colorBarWidth / 2, WindHeight / 2 - colorBarHeight / 2 - 40, 300, 20, currentcolor, dc);

    while (true)

    {

        POINT mPos = txMousePos ();

        if (mPos.x > WindWidth / 2 - colorBarWidth / 2 && mPos.x < WindWidth / 2 + colorBarWidth / 2 && mPos.y > WindHeight / 2 - colorBarHeight / 2 - 40 && mPos.y < WindHeight / 2 + colorBarHeight / 2)

        {

            if (txMouseButtons () == 1)

            {

                currentcolor = txGetPixel (mPos.x, mPos.y);

                drawSubColorBar (WindWidth / 2 - colorBarWidth / 2, WindHeight / 2 - colorBarHeight / 2 - 40, 300, 20, subcolor, dc);

            }

        }

        if (mPos.x > WindWidth / 2 - colorBarWidth / 2 && mPos.x < WindWidth / 2 + colorBarWidth / 2 && mPos.y > WindHeight / 2 - colorBarHeight / 2 && mPos.y < WindHeight / 2 + colorBarHeight / 2)

        {

            if (txMouseButtons () == 1)

            {

                subcolor = txGetPixel (mPos.x, mPos.y);

                drawColorBar (WindWidth / 2 - colorBarWidth / 2, WindHeight / 2 - colorBarHeight / 2, 300, 20, dc);

            }

        }

        int result = manageButtons (buttons);

        switch (result)

        {

            case 0:

            {

                txSleep (100);

                painter -> color = currentcolor;

                txDeleteDC (dc);

                return;

                break;

            }

            case 1:

            {

                txSleep (100);

                txDeleteDC (dc);

                return;

                break;

            }

        }

        if (GetAsyncKeyState (VK_ESCAPE))

        {

            txSleep (100);

            txDeleteDC (dc);

            return;

        }

        if (GetAsyncKeyState (13))

        {

            txSleep (100);

            painter -> color = currentcolor;

            txDeleteDC (dc);

            return;

        }

        txSetFillColor (currentcolor, dc);
        txSetColor (currentcolor, 1, dc);

        txRectangle (WindWidth / 2 - colorBarWidth / 2, WindHeight / 2 + colorBarHeight / 2 + 20, WindWidth / 2 - colorBarWidth / 2 + 20, WindHeight / 2 + colorBarHeight / 2 + 40, dc);

        txBitBlt (txDC (), 0, 0, 0, 0, painter -> drawField, 0, 0);
        txTransparentBlt (txDC(), 0, 0, 0, 0, dc, 0, 0);

        txSleep (1);

    }

    txDeleteDC (dc);

}
