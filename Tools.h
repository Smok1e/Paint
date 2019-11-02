#include <TXLib.h>

enum Tools

{

    ToolDefault,
    ToolLine,
    ToolRectangle

};

//-----------------------------------------------------------------------------

struct Tool

{

    int type_;
    int r_;
    COLORREF color_;
    HDC dc_;

    void draw () const;
    void use ();

};

//-----------------------------------------------------------------------------

void drawDefault (int r, COLORREF color);

void useDefault (int r, COLORREF color, HDC dc);

void drawLine (int r, COLORREF color);

void useLine (int r, COLORREF color, HDC dc);

void drawRectangle (COLORREF color);

void useRectangle (COLORREF color, HDC dc);

//-----------------------------------------------------------------------------

void Tool::draw () const

{

    switch (type_)

    {

        case ToolDefault:

        {

            drawDefault (r_, color_);
            break;

        }

        case ToolLine:

        {

            drawLine (r_, color_);
            break;

        }

        case ToolRectangle:

        {

            drawRectangle (color_);
            break;

        }

    }

}

//-----------------------------------------------------------------------------

void Tool::use ()

{

    switch (type_)

    {

        case ToolDefault:

        {

            useDefault (r_, color_, dc_);
            break;

        }

        case ToolLine:

        {

            useLine (r_, color_, dc_);
            break;

        }

        case ToolRectangle:

        {

            useRectangle (color_, dc_);
            break;

        }

    }

}

//-----------------------------------------------------------------------------

void drawDefault (int r, COLORREF color)

{

    POINT mPos = txMousePos ();

    txSetColor (color);
    txSetFillColor (color);

    txEllipse (mPos.x - r / 2, mPos.y - r / 2, mPos.x + r / 2, mPos.y + r / 2);

}

//-----------------------------------------------------------------------------

void useDefault (int r, COLORREF color, HDC dc)

{

    POINT OldmPos = txMousePos ();

    while (true)

    {

        POINT mPos = txMousePos ();

        if (txMouseButtons () == 1)

        {

            txSetColor (color, r, dc);
            txSetFillColor (color, dc);

            txLine (OldmPos.x, OldmPos.y, mPos.x, mPos.y, dc);

            OldmPos = mPos;

        }

        else

        {

            return;

        }

        txBitBlt (txDC (), 0, 0, 0, 0, dc, 0, 0);

        txSleep (1);

    }

}

//-----------------------------------------------------------------------------

void drawLine (int r, COLORREF color)

{

    POINT mPos = txMousePos ();

    txSetColor (color);
    txSetFillColor (color);

    txEllipse (mPos.x - r / 2, mPos.y - r / 2, mPos.x + r / 2, mPos.y + r / 2);

}

//-----------------------------------------------------------------------------

void useLine (int r, COLORREF color, HDC dc)

{

    POINT firstPos = txMousePos ();
    POINT mPos;

    while (txMouseButtons() == 1)

    {

        txBitBlt (txDC (), 0, 0, 0, 0, dc, 0, 0);

        mPos = txMousePos ();

        txSetColor (color, r, txDC ());
        txSetFillColor (color, txDC ());

        txLine (firstPos.x, firstPos.y, mPos.x, mPos.y, txDC ());

        txSleep (1);

    }

    mPos = txMousePos ();

    txSetColor (color, r, dc);
    txSetFillColor (color, dc);

    txLine (firstPos.x, firstPos.y, mPos.x, mPos.y, dc);

}

//-----------------------------------------------------------------------------

void drawRectangle (COLORREF color)

{

    POINT mPos = txMousePos ();

    txSetColor (color);
    txSetFillColor (color);

    txRectangle (mPos.x - 2, mPos.y - 2, mPos.x + 2, mPos.y + 2);

}

//-----------------------------------------------------------------------------

void useRectangle (COLORREF color, HDC dc)

{

    POINT firstPos = txMousePos ();
    POINT mPos;

    while (txMouseButtons() == 1)

    {

        txBitBlt (txDC (), 0, 0, 0, 0, dc, 0, 0);

        mPos = txMousePos ();

        txSetColor (color);
        txSetFillColor (color, txDC ());

        txRectangle (firstPos.x, firstPos.y, mPos.x, mPos.y, txDC ());

        txSleep (1);

    }

    mPos = txMousePos ();

    txSetColor (color, 0, dc);
    txSetFillColor (color, dc);

    txRectangle (firstPos.x, firstPos.y, mPos.x, mPos.y, dc);

}

