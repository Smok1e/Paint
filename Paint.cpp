#include <TXLib.h>
#include "C:\Users\Ը���\Documents\GitHub\Buttons\Buttons.h"
#include "Tools.h"

//-----------------------------------------------------------------------------

enum Result

{

    ResultExit,
    ResultRestart

};

//-----------------------------------------------------------------------------

int run (int wWidth, int wHeight, int r, COLORREF color);

bool Save (HDC image);

void changeColor (int wWidth, int wHeight, COLORREF * color, HDC * drawField, Tool tool);

void menu (int wWidth, int wHeight, HDC * drawField, COLORREF * color, Tool * tool);

void drawColorBar (int x, int y, int width, int height, HDC * dc);

void drawSubColorBar (int x, int y, int width, int heidgt, COLORREF color, HDC * dc);

void importImage (char * path, HDC * dc);

int toolsMenu (int wWidth, int wHeight, HDC * drawField, Tool * tool);

//-----------------------------------------------------------------------------

int main ()

{

    int wWidth = 800;
    int wHeight = 800;

    int toolbarHeight = 1;

    int r = 10;

    COLORREF color = TX_WHITE;

    txCreateWindow (wWidth, wHeight);
    txDisableAutoPause ();

    MessageBox (txWindow (), "Help:\nm - main menu\nescape - exit\nright mouse button - set the color of the pixel that the cursor points to.", "Welcome", MB_ICONINFORMATION | MB_OK);

    while (true)

    {

        int result = run (wWidth, wHeight, r, color);

        if (result == ResultExit)

        {

            break;

        }

        else if (result == ResultRestart)

        {


        }

        else

        {

            char message[80] = "";

            sprintf (message, "%s %d", "Unknown exit code: ", result);

            MessageBox(txWindow (), message, "Error", MB_ICONWARNING | MB_OK);

        }

    }

}

//-----------------------------------------------------------------------------

int run (int wWidth, int wHeight, int r, COLORREF color)

{

    HDC drawField = txLoadImage ("Default.bmp");
    HDC drawField_Old = txCreateCompatibleDC (800, 800);

    Tool tool = {ToolDefault, r, color, drawField};

    bool changeOldField = true;

    while (true)

    {

        POINT mPos = txMousePos ();

        txBitBlt (txDC (), 0, 0, 0, 0, drawField);

        if (txMouseButtons () == 1)

        {

            if (changeOldField)

            {

                txBitBlt (drawField_Old, 0, 0, 0, 0, drawField, 0, 0);
                changeOldField = false;

            }

            tool.use ();

        }

        else

        {

            if (!changeOldField)

            {

                changeOldField = true;

            }

        }

        if (txMouseButtons () == 2)

        {

            color = txGetPixel (mPos.x, mPos.y, drawField);
            tool.color_ = color;

        }

        if (GetAsyncKeyState (VK_ESCAPE))

        {

            txDeleteDC (drawField);
            txDeleteDC (drawField_Old);

            return ResultExit;

        }

        if (GetAsyncKeyState ('R'))

        {

            txSleep (100);

            txDeleteDC (drawField);
            txDeleteDC (drawField_Old);

            return ResultRestart;

        }

        if (GetAsyncKeyState ('M'))

        {

            menu (wWidth, wHeight, &drawField, &color, &tool);

        }

        if (GetAsyncKeyState (VK_RIGHT))

        {

            tool.r_ ++;

        }

        if (GetAsyncKeyState (VK_LEFT))

        {

            tool.r_ --;

        }

        if (GetAsyncKeyState ('Z') && GetAsyncKeyState (VK_CONTROL))

        {

            txSleep (100);
            txBitBlt (drawField, 0, 0, 0, 0, drawField_Old, 0, 0);

        }

        tool.draw ();

        txSleep (1);

    }

    return 228;

}

//-----------------------------------------------------------------------------

bool Save (HDC image)

{

    const char * input = txInputBox ("Enter saving path:", "Save", "Image");

    if (input)

    {

        char path[PATH_MAX] = "";

        sprintf (path, "%s.bmp", input);

        txSaveImage (path, image);

        return true;

    }

    return false;

}

//-----------------------------------------------------------------------------

void drawColorBar (int x, int y, int width, int height, HDC * dc)

{

    int r = 255;
    int g = 0;
    int b = 0;

    double interval = width / (255.0 * 6.0);

    double lineX = 0;

    for (int i = 0; i < 255; i++)

    {

        COLORREF color = RGB (r, g, b);

        txSetColor (color, 1, * dc);
        txSetFillColor (color, * dc);

        txRectangle (x + lineX, y, x + lineX + interval, y + height, * dc);

        lineX += interval;

        g++;

    }

    for (int i = 0; i < 255; i++)

    {

        COLORREF color = RGB (r, g, b);

        txSetColor (color, 1, * dc);
        txSetFillColor (color, * dc);

        txRectangle (x + lineX, y, x + lineX + interval, y + height, * dc);

        lineX += interval;

        r--;

    }

    for (int i = 0; i < 255; i++)

    {

        COLORREF color = RGB (r, g, b);

        txSetColor (color, 1, * dc);
        txSetFillColor (color, * dc);

        txRectangle (x + lineX, y, x + lineX + interval, y + height, * dc);

        b++;

        lineX += interval;

    }

    for (int i = 0; i < 255; i++)

    {

        COLORREF color = RGB (r, g, b);

        txSetColor (color, 1, * dc);
        txSetFillColor (color, * dc);

        txRectangle (x + lineX, y, x + lineX + interval, y + height, * dc);

        g--;

        lineX += interval;

    }

    for (int i = 0; i < 255; i++)

    {

        COLORREF color = RGB (r, g, b);

        txSetColor (color, 1, * dc);
        txSetFillColor (color, * dc);

        txRectangle (x + lineX, y, x + lineX + interval, y + height, * dc);

        r++;

        lineX += interval;

    }

    for (int i = 0; i < 255; i++)

    {

        COLORREF color = RGB (r, g, b);

        txSetColor (color, 1, * dc);
        txSetFillColor (color, * dc);

        txRectangle (x + lineX, y, x + lineX + interval, y + height, * dc);

        b--;

        lineX += interval;

    }

}

//-----------------------------------------------------------------------------

void drawSubColorBar (int x, int y, int width, int height, COLORREF color, HDC * dc)

{

    double r = txExtractColor (color, TX_RED), g = txExtractColor (color, TX_GREEN), b = txExtractColor (color, TX_BLUE);

    double lineX = 0;

    double interval = width / (255.0 * 2.0);

    for (int i = 0; i < 255; i++)

    {

        COLORREF l_color = RGB (r / 255 * i, g / 255 * i, b / 255 * i);

        txSetColor (l_color, 1, * dc);
        txSetFillColor (l_color, * dc);

        txRectangle (x + lineX, y, x + lineX + interval, y + height, * dc);

        lineX += interval;

    }

    for (int i = 0; i < 255; i++)

    {

        COLORREF l_color = RGB (r + (255 - r) / 255 * i, g + (255 - g) / 255 * i, b + (255 - b) / 255 * i);

        txSetColor (l_color, 1, * dc);
        txSetFillColor (l_color, * dc);

        txRectangle (x + lineX, y, x + lineX + interval, y + height, * dc);

        lineX += interval;

    }

}

//-----------------------------------------------------------------------------

void changeColor (int wWidth, int wHeight, COLORREF * color, HDC * drawField, Tool * tool)

{

    int colorBarWidth = 300, colorBarHeight = 20;

    COLORREF currentcolor = *color;
    COLORREF subcolor = *color;

    HDC dc = txCreateCompatibleDC (wWidth, wHeight);

    Button b1 = {wWidth / 2 + colorBarWidth / 2 -  40, wHeight / 2 + colorBarHeight / 2 + 20, 40, 20,     "Ok", "Arial", 20, dc, RGB (32, 32, 32), TX_WHITE, TX_WHITE, false, 2};
    Button b2 = {wWidth / 2 + colorBarWidth / 2 - 140, wHeight / 2 + colorBarHeight / 2 + 20, 80, 20, "Cancel", "Arial", 20, dc, RGB (32, 32, 32), TX_WHITE, TX_WHITE, false, 2};

    Button * buttons[3] = {&b1, &b2, &EndButton};

    drawColorBar (wWidth / 2 - colorBarWidth / 2, wHeight / 2 - colorBarHeight / 2, 300, 20, &dc);
    drawSubColorBar (wWidth / 2 - colorBarWidth / 2, wHeight / 2 - colorBarHeight / 2 - 40, 300, 20, currentcolor, &dc);

    while (true)

    {

        POINT mPos = txMousePos ();

        if (mPos.x > wWidth / 2 - colorBarWidth / 2 && mPos.x < wWidth / 2 + colorBarWidth / 2 && mPos.y > wHeight / 2 - colorBarHeight / 2 - 40 && mPos.y < wHeight / 2 + colorBarHeight / 2)

        {

            if (txMouseButtons () == 1)

            {

                currentcolor = txGetPixel (mPos.x, mPos.y);

                drawSubColorBar (wWidth / 2 - colorBarWidth / 2, wHeight / 2 - colorBarHeight / 2 - 40, 300, 20, subcolor, &dc);

            }

        }

        if (mPos.x > wWidth / 2 - colorBarWidth / 2 && mPos.x < wWidth / 2 + colorBarWidth / 2 && mPos.y > wHeight / 2 - colorBarHeight / 2 && mPos.y < wHeight / 2 + colorBarHeight / 2)

        {

            if (txMouseButtons () == 1)

            {

                subcolor = txGetPixel (mPos.x, mPos.y);

                drawColorBar (wWidth / 2 - colorBarWidth / 2, wHeight / 2 - colorBarHeight / 2, 300, 20, &dc);

            }

        }

        int result = manageButtons (buttons);

        switch (result)

        {

            case 0:

            {

                txSleep (100);

                * color = currentcolor;
                tool -> color_ = *color;

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

            * color = currentcolor;
            tool -> color_ = *color;

            txDeleteDC (dc);

            return;

        }

        txSetFillColor (currentcolor, dc);
        txSetColor (currentcolor, 1, dc);

        txRectangle (wWidth / 2 - colorBarWidth / 2, wHeight / 2 + colorBarHeight / 2 + 20, wWidth / 2 - colorBarWidth / 2 + 20, wHeight / 2 + colorBarHeight / 2 + 40, dc);

        txBitBlt (txDC (), 0, 0, 0, 0, *drawField, 0, 0);
        txTransparentBlt (txDC(), 0, 0, 0, 0, dc, 0, 0);

        txSleep (1);

    }

    txDeleteDC (dc);

}

//-----------------------------------------------------------------------------

void menu (int wWidth, int wHeight, HDC * drawField, COLORREF * color, Tool * tool)
{


    HDC buttonsField = txCreateCompatibleDC (wWidth, wHeight);

    Button b1 = {wWidth / 2 - 50, wHeight / 2 - 100, 100, 40,   "Save", "Comic Sans MS", 32, buttonsField, RGB (32, 32, 32), TX_WHITE, TX_LIGHTCYAN, false, 3};
    Button b2 = {wWidth / 2 - 50, wHeight / 2 -  60, 100, 40,   "Load", "Comic Sans MS", 32, buttonsField, RGB (32, 32, 32), TX_WHITE, TX_LIGHTCYAN, false, 3};
    Button b3 = {wWidth / 2 - 50, wHeight / 2 -  20, 100, 40,  "Color", "Comic Sans MS", 32, buttonsField, RGB (32, 32, 32), TX_WHITE, TX_LIGHTCYAN, false, 3};
    Button b4 = {wWidth / 2 - 50, wHeight / 2 +  20, 100, 40, "Import", "Comic Sans MS", 32, buttonsField, RGB (32, 32, 32), TX_WHITE, TX_LIGHTCYAN, false, 3};
    Button b5 = {wWidth / 2 - 50, wHeight / 2 +  60, 100, 40,  "Tools", "Comic Sans MS", 32, buttonsField, RGB (32, 32, 32), TX_WHITE, TX_LIGHTCYAN, false, 3};

    Button * buttons[6] = {&b1, &b2, &b3, &b4, &b5, &EndButton};

    while (true)

    {

        int result = manageButtons (buttons);

        switch (result)

        {

            case 0:

            {

                if (Save (* drawField)) return;

                break;

            }

            case 1:

            {

                const char * input = txInputBox ("Enter load path:", "Load", "Image");

                if (input)

                {

                    char path[PATH_MAX] = "";

                    sprintf (path, "%s.bmp", input);

                    * drawField = txLoadImage (path);

                    return;

                }

                break;

            }

            case 2:

            {

                txSleep (100);

                changeColor (wWidth, wHeight, color, drawField, tool);

                return;

                break;

            }

            case 3:

            {

                const char * input = txInputBox ("Enter load path (bmp):", "Load", "Image");

                if (input)

                {

                    char path[PATH_MAX] = "";

                    sprintf (path, "%s.bmp", input);

                    importImage (path, drawField);

                }

                return;

                break;

            }

            case 4:

            {

                txSleep (100);

                int result = toolsMenu (wWidth, wHeight, drawField, tool);

                if (result != ResultExit)

                {

                    txSleep (100);

                    return;

                }

                break;

            }

        }

        if (GetAsyncKeyState (VK_ESCAPE))

        {

            txSleep (100);

            txDeleteDC (buttonsField);

            return;

        }


        txBitBlt (txDC (), 0, 0, 0, 0, * drawField, 0, 0);
        txTransparentBlt (txDC() , 0, 0, 0, 0, buttonsField, 0, 0);

        txSleep (1);

    }

}

//-----------------------------------------------------------------------------

void importImage (char * path, HDC * dc)

{

    MessageBox (txWindow (), "a - set alpha color for the imported image\nescape - cancel", "Import: ", MB_ICONINFORMATION | MB_OK);

    HDC image = txLoadImage (path);

    COLORREF Alpha = NULL;

    while (true)

    {

        POINT mPos = txMousePos ();

        txBitBlt (txDC (), 0, 0, 0, 0, *dc, 0, 0);
        txTransparentBlt (txDC (), mPos.x, mPos.y, 0, 0, image, 0, 0, Alpha);

        if (txMouseButtons () == 1)

        {

            txTransparentBlt (* dc, mPos.x, mPos.y, 0, 0, image, 0, 0, Alpha);

            txDeleteDC (image);

            txSleep (100);

            return;

        }

        if (GetAsyncKeyState (VK_ESCAPE) || txMouseButtons () == 2)

        {

            txSleep (100);

            txDeleteDC (image);

            return;

        }

        if (GetAsyncKeyState ('A'))

        {

            while (true)

            {

                POINT mPos1 = txMousePos ();

                if (txMouseButtons () == 1)

                {

                    if (mPos1.x >= mPos.x && mPos1.x < mPos.x + txGetExtentX (image) && mPos1.y >= mPos.y && mPos1.y < mPos.y + txGetExtentY (image))

                    {

                        Alpha = txGetPixel (mPos1.x, mPos1.y);

                        txSleep (100);

                        break;

                    }

                }

                if (GetAsyncKeyState (VK_ESCAPE) || txMouseButtons () == 2)

                {

                    txSleep (100);

                    break;

                }

                txSleep (1);

            }

        }

        txSleep (1);

    }

}

//-----------------------------------------------------------------------------

int toolsMenu (int wWidth, int wHeight, HDC * drawField, Tool * tool)

{

    HDC buttonsField = txCreateCompatibleDC (wWidth, wHeight);

    Button b1 = {wWidth / 2 - 50, wHeight / 2 - 60, 100, 40,   "Default", "Comic Sans MS", 32, buttonsField, RGB (32, 32, 32), TX_WHITE, TX_LIGHTCYAN, false, 3};
    Button b2 = {wWidth / 2 - 50, wHeight / 2 - 20, 100, 40,      "Line", "Comic Sans MS", 35, buttonsField, RGB (32, 32, 32), TX_WHITE, TX_LIGHTCYAN, false, 3};
    Button b3 = {wWidth / 2 - 50, wHeight / 2 + 20, 100, 40, "Rectangle", "Comic Sans MS", 28, buttonsField, RGB (32, 32, 32), TX_WHITE, TX_LIGHTCYAN, false, 3};

    Button * buttons[4] = {&b1, &b2, &b3, &EndButton};

    while (true)

    {

        txBitBlt (txDC (), 0, 0, 0, 0, *drawField);
        txTransparentBlt (txDC (), 0, 0, 0, 0, buttonsField, 0, 0);

        int result = manageButtons (buttons);

        switch (result)

        {

            case 0:

            {

                tool -> type_ = ToolDefault;

                txDeleteDC (buttonsField);

                return -1;

                break;

            }

            case 1:

            {

                tool -> type_ = ToolLine;

                txDeleteDC (buttonsField);

                return -1;

                break;

            }

            case 2:

            {

                tool -> type_ = ToolRectangle;

                txDeleteDC (buttonsField);

                return -1;

                break;

            }

            default:

            {

                if (GetAsyncKeyState (VK_ESCAPE))

                {

                    txSleep (100);

                    return ResultExit;

                }

            }

        }

        txSleep (1);

    }

}
