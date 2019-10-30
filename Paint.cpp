#include <TXLib.h>

//-----------------------------------------------------------------------------

struct Button

{

    int x;
    int y;
    int width;
    int height;

    const char * text;
    const char * font;
    int fontsize;

    HDC dc;

    COLORREF color;
    COLORREF textcolor;
    COLORREF strokecolor;

    bool stroke;

    int stroke_r;

};

//-----------------------------------------------------------------------------

enum Result

{

    ResultExit,
    ResultRestart

};

//-----------------------------------------------------------------------------

int run (int wWidth, int wHeight, int r, COLORREF color, COLORREF bkcolor);

bool Save (HDC image);

void changeColor (int wWidth, int wHeight, COLORREF * color);

void drawButton (const Button * b);

bool checkButton (Button * b);

void menu (int wWidth, int wHeight, HDC * drawField, COLORREF * color);

bool manageButton (Button * b);

void drawColorBar (int x, int y, int width, int height, HDC * dc);

void drawSubColorBar (int x, int y, int width, int heidgt, COLORREF color, HDC * dc);

void importImage (char * path, HDC * dc);

//-----------------------------------------------------------------------------

int main ()

{

    int wWidth = 800;
    int wHeight = 800;

    int toolbarHeight = 1;

    int r = 10;

    COLORREF color = TX_BLACK;
    COLORREF bkcolor = TX_WHITE;

    txCreateWindow (wWidth, wHeight);
    txDisableAutoPause ();

    MessageBox (txWindow (), "Help:\nm - main menu\nescape - exit\nright mouse button - set the color of the pixel that the cursor points to.", "Welcome", MB_ICONINFORMATION | MB_OK);

    while (true)

    {

        int result = run (wWidth, wHeight, r, color, bkcolor);

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

int run (int wWidth, int wHeight, int r, COLORREF color, COLORREF bkcolor)

{

    HDC drawField = txLoadImage ("Default.bmp");

    while (true)


    {

        POINT mPos = txMousePos ();

        txBitBlt (txDC(), 0, 0, 0, 0, drawField);

        if (txMouseButtons () == 1)

        {

            txSetColor (color, 1, drawField);
            txSetFillColor (color, drawField);


            txEllipse (mPos.x - r / 2, mPos.y - r / 2, mPos.x + r, mPos.y + r, drawField);

        }

        if (txMouseButtons () == 2)

        {

            color = txGetPixel (mPos.x, mPos.y, drawField);

        }

        if (GetAsyncKeyState (VK_ESCAPE))

        {

            txDeleteDC (drawField);

            return ResultExit;

        }

        if (GetAsyncKeyState ('R'))

        {

            txSleep (100);

            txDeleteDC (drawField);

            return ResultRestart;

        }

        if (GetAsyncKeyState ('M'))

        {

            menu (wWidth, wHeight, &drawField, &color);

        }

        if (GetAsyncKeyState (VK_RIGHT))

        {

            r ++;

        }

        if (GetAsyncKeyState (VK_LEFT))

        {

            r --;

        }

        txSetColor (color);
        txSetFillColor (color);

        txEllipse (mPos.x - r / 2, mPos.y - r / 2, mPos.x + r, mPos.y + r);

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

void changeColor (int wWidth, int wHeight, COLORREF * color)

{

    int colorBarWidth = 300, colorBarHeight = 20;

    COLORREF currentcolor = *color;
    COLORREF subcolor = *color;

    HDC dc = txCreateCompatibleDC (wWidth, wHeight);

    Button b1 = {wWidth / 2 + colorBarWidth / 2 -  40, wHeight / 2 + colorBarHeight / 2 + 20, 40, 20,     "Ok", "Arial", 20, dc, RGB (32, 32, 32), TX_WHITE, TX_WHITE, false, 2};
    Button b2 = {wWidth / 2 + colorBarWidth / 2 - 140, wHeight / 2 + colorBarHeight / 2 + 20, 80, 20, "Cancel", "Arial", 20, dc, RGB (32, 32, 32), TX_WHITE, TX_WHITE, false, 2};

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

        if (manageButton (&b1))

        {

            txSleep (100);

            * color = currentcolor;

            txDeleteDC (dc);

            return;

        }

        if (manageButton (&b2))

        {

            txSleep (100);

            txDeleteDC (dc);

            return;

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

            txDeleteDC (dc);

            return;

        }

        txSetFillColor (currentcolor, dc);
        txSetColor (currentcolor, 1, dc);

        txRectangle (wWidth / 2 - colorBarWidth / 2, wHeight / 2 + colorBarHeight / 2 + 20, wWidth / 2 - colorBarWidth / 2 + 20, wHeight / 2 + colorBarHeight / 2 + 40, dc);

        txTransparentBlt (txDC(), 0, 0, 0, 0, dc, 0, 0);

        txSleep (0);

    }

    txDeleteDC (dc);

}

//-----------------------------------------------------------------------------

void drawButton (const Button * b)

{

    txSetColor (b -> color, 1, b -> dc);

    if (b -> stroke)

    {

        txSetColor (b -> strokecolor, b -> stroke_r, b -> dc);

    }

    txSetFillColor (b -> color, b -> dc);

    txRectangle (b -> x, b -> y, b -> x + b -> width, b -> y + b -> height, b -> dc);

    txSetColor (b -> textcolor, b -> stroke_r, b -> dc);

    txSelectFont (b -> font, b -> fontsize, -1, FW_DONTCARE, false, false, false, 0, b -> dc);
    txDrawText (b -> x, b -> y, b -> x + b -> width, b -> y + b -> height, b -> text, DT_CENTER, b -> dc);

}

//-----------------------------------------------------------------------------

bool checkButton (Button * b)

{

    POINT mPos = txMousePos ();

    if (mPos.x >= b -> x && mPos.x < b -> x + b -> width && mPos.y >= b -> y && mPos.y < b -> y + b -> height)

    {

        return true;

    }

    return false;

}

//-----------------------------------------------------------------------------

bool manageButton (Button * b)

{

    if (checkButton (b))

    {

        b -> stroke = true;

    }

    else

    {

        b -> stroke = false;

    }

    drawButton (b);

    if (checkButton (b) && txMouseButtons() == 1)

    {

        return true;

    }

    return false;

}

//-----------------------------------------------------------------------------

void menu (int wWidth, int wHeight, HDC * drawField, COLORREF * color)
{


    HDC buttonsField = txCreateCompatibleDC (wWidth, wHeight);

    Button b1 = {wWidth / 2 - 50, wHeight / 2 - 60, 100, 40,   "Save", "Comic Sans MS", 35, buttonsField, RGB (32, 32, 32), TX_WHITE, TX_LIGHTCYAN, false, 3};
    Button b2 = {wWidth / 2 - 50, wHeight / 2 - 20, 100, 40,   "Load", "Comic Sans MS", 35, buttonsField, RGB (32, 32, 32), TX_WHITE, TX_LIGHTCYAN, false, 3};
    Button b3 = {wWidth / 2 - 50, wHeight / 2 + 20, 100, 40,  "Color", "Comic Sans MS", 32, buttonsField, RGB (32, 32, 32), TX_WHITE, TX_LIGHTCYAN, false, 3};
    Button b4 = {wWidth / 2 - 50, wHeight / 2 + 60, 100, 40, "Import", "Comic Sans MS", 32, buttonsField, RGB (32, 32, 32), TX_WHITE, TX_LIGHTCYAN, false, 3};

    while (true)

    {

        if (manageButton (&b1))

        {

            if (Save (* drawField)) return;

        }

        if (manageButton (&b2))

        {

            const char * input = txInputBox ("Enter load path:", "Load", "Image");

            if (input)

            {

                char path[PATH_MAX] = "";

                sprintf (path, "%s.bmp", input);

                * drawField = txLoadImage (path);

                return;

            }

        }

        if (manageButton (&b3))

        {

            txSleep (100);

            changeColor (wWidth, wHeight, color);

            return;

        }

        if (manageButton (&b4))

        {

            const char * input = txInputBox ("Enter load path (bmp):", "Load", "Image");

            if (input)

            {

                char path[PATH_MAX] = "";

                sprintf (path, "%s.bmp", input);

                importImage (path, drawField);

            }

            return;

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

    HDC image = txLoadImage (path);

    while (true)

    {

        POINT mPos = txMousePos ();

        txBitBlt (txDC (), 0, 0, 0, 0, *dc, 0, 0);
        txTransparentBlt (txDC (), mPos.x, mPos.y, 0, 0, image, 0, 0);

        if (txMouseButtons () == 1)

        {

            txTransparentBlt (* dc, mPos.x, mPos.y, 0, 0, image, 0, 0);

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

        txSleep (1);

    }

}
