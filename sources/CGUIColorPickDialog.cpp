/*
 * Copyright (c) 2010, anirul
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Calodox nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY anirul ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL anirul BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "CGUIColorPickDialog.h"
#include <irrlicht.h>

// #ifdef _IRR_COMPILE_WITH_GUI_

namespace irr
{
namespace gui
{

#ifdef WITH_HSL
const s32 CPD_WIDTH = 350;
const s32 CPD_HEIGHT = 190; 
#else
const s32 CPD_WIDTH = 260;
const s32 CPD_HEIGHT = 160;
#endif // WITH HSL

struct sTemplate
{
	const wchar_t *pre;
	const wchar_t *init;
	const wchar_t *pos;
	int x, y;
	int range_down ,range_up;
};

static const sTemplate Template [] =
{
#ifdef WITH_HSL
	{ L"A:", L"0", 0,		20,	55, 0, 255 },
	{ L"R:", L"0", 0,		20,	95, 0, 255 },
	{ L"G:", L"0", 0,		20,	120, 0, 255 },
	{ L"B:", L"0", 0,		20,	145, 0, 255 },
	{ L"H:", L"0", L"°",180,	95, 0, 360 },
	{ L"S:", L"0", L"%",	180,	120, 0, 100 },
	{ L"L:", L"0", L"%",	180,	145, 0, 100 },
#else
	{ L"A:", L"0", 0,		20,	35, 0, 255 },
	{ L"R:", L"0", 0,		20,	75, 0, 255 },
	{ L"G:", L"0", 0,		20,	100, 0, 255 },
	{ L"B:", L"0", 0,		20,	125, 0, 255 },
#endif // WITH_HSL
};

CGUIColorPickDialog::CGUIColorPickDialog(const wchar_t* title, const video::SColor& start, IGUIEnvironment* environment, IGUIElement* parent, s32 id) 
	:	IGUIColorPickDialog(
			environment, 
			parent, 
			id,
			core::rect<s32>(
				(parent->getAbsolutePosition().getWidth() - CPD_WIDTH) / 2,
				(parent->getAbsolutePosition().getHeight() - CPD_HEIGHT) / 2,
				(parent->getAbsolutePosition().getWidth() - CPD_WIDTH) / 2 + CPD_WIDTH,
				(parent->getAbsolutePosition().getHeight() - CPD_HEIGHT) / 2 + CPD_HEIGHT)),
		ColorImage(NULL),
		ColorDisplay(NULL),
		Dragging(false)
{
#ifdef _DEBUG
	IGUIElement::setDebugName("CGUIColorPickDialog");
#endif
	Text = title;
	IGUISkin* skin = Environment->getSkin();

	const s32 buttonw = environment->getSkin()->getSize(EGDS_WINDOW_BUTTON_WIDTH);
	const s32 posx = RelativeRect.getWidth() - buttonw - 4;

	CloseButton = Environment->addButton(
		core::rect<s32>(posx, 3, posx + buttonw, 3 + buttonw),
		this, 
		-1, 
		L"", 
		skin ? skin->getDefaultText(EGDT_WINDOW_CLOSE) : L"Close");
	if (skin && skin->getSpriteBank())
	{
		CloseButton->setSpriteBank(skin->getSpriteBank());
		CloseButton->setSprite(EGBS_BUTTON_UP, skin->getIcon(EGDI_WINDOW_CLOSE), skin->getColor(EGDC_WINDOW_SYMBOL));
		CloseButton->setSprite(EGBS_BUTTON_DOWN, skin->getIcon(EGDI_WINDOW_CLOSE), skin->getColor(EGDC_WINDOW_SYMBOL));
	}
	CloseButton->setSubElement(true);
	CloseButton->setTabStop(false);
	CloseButton->setAlignment(EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_UPPERLEFT);
	CloseButton->grab();
	
	OKButton = Environment->addButton(
		core::rect<s32>(
			RelativeRect.getWidth() - 80, 
#ifdef WITH_HSL
			30, 
#else
			100,
#endif // WITH_HSL
			RelativeRect.getWidth() - 10, 
#ifdef WITH_HSL
			50),
#else
			120),
#endif // WITH_HSL
		this, 
		-1, 
		skin ? skin->getDefaultText(EGDT_MSG_BOX_OK) : L"OK");
	OKButton->setSubElement(true);
	OKButton->setAlignment(EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_UPPERLEFT);
	OKButton->grab();

	CancelButton = Environment->addButton(
		core::rect<s32>(
			RelativeRect.getWidth() - 80, 
#ifdef WITH_HSL
			55, 
#else
			125,
#endif // WITH_HSL
			RelativeRect.getWidth() - 10, 
#ifdef WITH_HSL
			75),
#else
			145),
#endif // WITH_HSL
		this, 
		-1, 
		skin ? skin->getDefaultText(EGDT_MSG_BOX_CANCEL) : L"Cancel");
	CancelButton->setSubElement(true);
	CancelButton->setAlignment(EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_UPPERLEFT);
	CancelButton->grab();

	core::rect<s32> r(20,20, 0,0);

	for ( u32 i = 0; i != sizeof (Template) / sizeof ( sTemplate ); ++i )
	{
		if ( Template[i].pre )
		{
			r.UpperLeftCorner.X = Template[i].x;
			r.UpperLeftCorner.Y = Template[i].y;
			r.LowerRightCorner.X = r.UpperLeftCorner.X + 15;
			r.LowerRightCorner.Y = r.UpperLeftCorner.Y + 20;
			IGUIElement *t = Environment->addStaticText(Template[i].pre, r, false, false, this);
			t->setSubElement(true);
		}

		if ( Template[i].pos )
		{
			r.UpperLeftCorner.X = Template[i].x + 52;
			r.UpperLeftCorner.Y = Template[i].y;
			r.LowerRightCorner.X = r.UpperLeftCorner.X + 15;
			r.LowerRightCorner.Y = r.UpperLeftCorner.Y + 20;
			IGUIElement *t = Environment->addStaticText( Template[i].pos, r, false, false, this);
			t->setSubElement(true);
		}

		SBatteryItem item;
		item.Incoming=0.f;
		item.Outgoing=0.f;

		r.UpperLeftCorner.X = Template[i].x + 15;
		r.UpperLeftCorner.Y = Template[i].y;
		r.LowerRightCorner.X = r.UpperLeftCorner.X + 35;
		r.LowerRightCorner.Y = r.UpperLeftCorner.Y + 20;

		item.Edit = Environment->addEditBox( Template[i].init, r, true, this);
		item.Edit->setSubElement(true);
		item.Edit->grab();

		r.UpperLeftCorner.X = Template[i].x + 70;
		r.UpperLeftCorner.Y = Template[i].y + 4;
		r.LowerRightCorner.X = r.UpperLeftCorner.X + 60;
		r.LowerRightCorner.Y = r.UpperLeftCorner.Y + 12;

		item.Scrollbar = Environment->addScrollBar(true, r, this);
		item.Scrollbar->grab ();
		item.Scrollbar->setSubElement(true);
		item.Scrollbar->setMax ( Template[i].range_up - Template[i].range_down );
		item.Scrollbar->setSmallStep ( 1 );

		Battery.push_back ( item );
	}	

	setColor(start);

	bringToFront(CancelButton);
	bringToFront(OKButton);
}

CGUIColorPickDialog::~CGUIColorPickDialog()
{
	if (CloseButton)
		CloseButton->drop();
}

bool CGUIColorPickDialog::OnEvent(const SEvent& event)
{
	if (IsEnabled)
	{
		switch(event.EventType)
		{
			case EET_GUI_EVENT:
			switch(event.GUIEvent.EventType)
			{
				case EGET_SCROLL_BAR_CHANGED:
				{
					int item = -1;
					s32 value = 0;
					for ( u32 i = 0; i!= Battery.size (); ++i )
					{
						if ( event.GUIEvent.Caller == Battery[i].Scrollbar )
						{
							item = i;
							value = Template[i].range_down + Battery[i].Scrollbar->getPos();
							Battery[i].Edit->setText(core::stringw(value).c_str());
						}
					}
					if (item != -1)
					{
						video::SColor col = Color;
						video::SColorHSL colHSL;
						colHSL.fromRGB(Color);
						switch (item) {
						case 0 : // alpha
							col.setAlpha(value);
							break;
						case 1 : // red
							col.setRed(value);
							break;
						case 2 : // green
							col.setGreen(value);
							break;
						case 3 : // blue
							col.setBlue(value);
							break;
#ifdef WITH_HSL
						case 4 : // H
							colHSL.Hue = (float)value * core::DEGTORAD;
							colHSL.toRGB(col);
							col.setAlpha(Color.getAlpha()); 
							break;
						case 5 : // S
							colHSL.Saturation = (float)value / 100.0f;
							colHSL.toRGB(col);
							col.setAlpha(Color.getAlpha());
							break;
						case 6 : // L
							colHSL.Luminance = (float)value / 100.0f;
							colHSL.toRGB(col);
							col.setAlpha(Color.getAlpha());
							break;
#endif // WITH_HSL
						default :
							return true;
						}
						setColor(col);
					}
					return true;
				}

			case EGET_ELEMENT_FOCUS_LOST:
				Dragging = false;
				break;
			case EGET_BUTTON_CLICKED:
				if (event.GUIEvent.Caller == CloseButton ||
					event.GUIEvent.Caller == CancelButton)
				{
					sendCancelEvent();
					remove();
					return true;
				}
				else
				if (event.GUIEvent.Caller == OKButton)
				{
					sendSelectedEvent();
					remove();
					return true;
				}
				break;

			case EGET_LISTBOX_CHANGED:
			case EGET_LISTBOX_SELECTED_AGAIN:
			default:
				break;
			}
			break;
		case EET_MOUSE_INPUT_EVENT:
			switch(event.MouseInput.Event)
			{
			case EMIE_LMOUSE_PRESSED_DOWN:
				DragStart.X = event.MouseInput.X;
				DragStart.Y = event.MouseInput.Y;
				Dragging = true;
				Environment->setFocus(this);
				return true;
			case EMIE_LMOUSE_LEFT_UP:
				Dragging = false;
				Environment->removeFocus(this);
				return true;
			case EMIE_MOUSE_MOVED:
				if (Dragging)
				{
					// gui window should not be dragged outside its parent
					if (Parent)
						if (event.MouseInput.X < Parent->getAbsolutePosition().UpperLeftCorner.X +1 ||
							event.MouseInput.Y < Parent->getAbsolutePosition().UpperLeftCorner.Y +1 ||
							event.MouseInput.X > Parent->getAbsolutePosition().LowerRightCorner.X -1 ||
							event.MouseInput.Y > Parent->getAbsolutePosition().LowerRightCorner.Y -1)

							return true;

					move(core::position2d<s32>(event.MouseInput.X - DragStart.X, event.MouseInput.Y - DragStart.Y));
					DragStart.X = event.MouseInput.X;
					DragStart.Y = event.MouseInput.Y;
					return true;
				}
			default:
				break;
			}
		default:
			break;
		}
	}

	return IGUIElement::OnEvent(event);
}

//! sends the event that the file has been selected.
void CGUIColorPickDialog::sendSelectedEvent()
{
	SEvent event;
	event.EventType = EET_GUI_EVENT;
	event.GUIEvent.Caller = this;
	event.GUIEvent.Element = 0;
	event.GUIEvent.EventType = EGET_FILE_SELECTED;
	Parent->OnEvent(event);
}


//! sends the event that the file choose process has been canceld
void CGUIColorPickDialog::sendCancelEvent()
{
	SEvent event;
	event.EventType = EET_GUI_EVENT;
	event.GUIEvent.Caller = this;
	event.GUIEvent.Element = 0;
	event.GUIEvent.EventType = EGET_FILE_CHOOSE_DIALOG_CANCELLED;
	Parent->OnEvent(event);
}


void CGUIColorPickDialog::draw() 
{
	if (!IsVisible)
		return;
	
	IGUISkin* skin = Environment->getSkin();
	core::rect<s32> rect = skin->draw3DWindowBackground(
		this,
		true,
		skin->getColor(EGDC_ACTIVE_BORDER),
		AbsoluteRect,
		&AbsoluteClippingRect);
	if (Text.size())
	{
		rect.UpperLeftCorner.X += 2;
		rect.LowerRightCorner.X -= skin->getSize(EGDS_WINDOW_BUTTON_WIDTH) + 5;
		
		IGUIFont* font = skin->getFont(EGDF_WINDOW);
		if (font)
			font->draw(
				Text.c_str(), 
				rect, 
				skin->getColor(EGDC_ACTIVE_CAPTION), 
				false, 
				true,
				&AbsoluteClippingRect);
	}
/* // ???
	IGUIFont* font = Environment->getBuiltInFont();
	if (font)
		font->draw(
			L"+", 
			core::rect<s32>(20,20,50,50), 
			video::SColor(), 
			false, 
			false,
			&AbsoluteClippingRect);
*/
	IGUIElement::draw();
}

video::ITexture* CGUIColorPickDialog::createColorDisplay(
	const core::dimension2d<u32>& dim, 
	const video::SColor& col)
{
	video::IVideoDriver* driver = Environment->getVideoDriver();
	video::IImage* RawImage = driver->createImage(video::ECF_A8R8G8B8, dim);	
	RawImage->fill(Color);
	if (ColorDisplay) ColorDisplay->drop();
	video::ITexture* RawColor = driver->addTexture(L"#ColorDisplay", RawImage);
	RawImage->drop();
	return RawColor;
}

const video::SColor CGUIColorPickDialog::getColor() const
{
	return Color;
}

void CGUIColorPickDialog::setColor(const video::SColor& col)
{
	if (Color == col) return;
	Color = col;
	if (ColorImage) {
		video::IVideoDriver* driver = Environment->getVideoDriver();
		driver->removeTexture(ColorDisplay);	
		ColorDisplay = NULL;
		ColorDisplay = createColorDisplay(core::dimension2d<u32>(45, 45), col);
 		ColorImage->setImage(ColorDisplay);
	} else {
		ColorDisplay = createColorDisplay(core::dimension2d<u32>(45, 45), col);
 		ColorImage = Environment->addImage(
			ColorDisplay, 
#ifdef WITH_HSL
			core::position2d<s32>(200, 30),
#else
			core::position2d<s32>(RelativeRect.getWidth() - 55, 30),
#endif // WITH_HSL
			false,
			this);
	}
	// set the scrollers
	{ // alpha
		Battery[0].Scrollbar->setPos(Template[0].range_down + Color.getAlpha());
		Battery[0].Edit->setText(core::stringw(Color.getAlpha()).c_str());
	}
	{ // red
		Battery[1].Scrollbar->setPos(Template[1].range_down + Color.getRed());
		Battery[1].Edit->setText(core::stringw(Color.getRed()).c_str());
	}
	{ // green
		Battery[2].Scrollbar->setPos(Template[2].range_down + Color.getGreen());
		Battery[2].Edit->setText(core::stringw(Color.getGreen()).c_str());
	}
	{ // blue
		Battery[3].Scrollbar->setPos(Template[3].range_down + Color.getBlue());
		Battery[3].Edit->setText(core::stringw(Color.getBlue()).c_str());
	}
#ifdef WITH_HSL
	video::SColorHSL hsl;
	hsl.fromRGB(Color);
	{ // Hue
		unsigned int value = (unsigned int)(hsl.Hue * core::RADTODEG) % 360;
		Battery[4].Scrollbar->setPos(Template[5].range_down + value);
		Battery[4].Edit->setText(core::stringw(value).c_str());
	}
	{ // Saturation
		unsigned int value = (unsigned int)(hsl.Saturation * 100.0f);
		Battery[5].Scrollbar->setPos(Template[5].range_down + value);
		Battery[5].Edit->setText(core::stringw(value).c_str());
	}
	{ // Luminance
		unsigned int value = (unsigned int)(hsl.Luminance * 100.0f);
		Battery[6].Scrollbar->setPos(Template[6].range_down + value);
		Battery[6].Edit->setText(core::stringw(value).c_str());
	}
#endif // WITH_HSL
}

} // namespace gui
} // namespace irr

// #endif // _IRR_COMPILE_WITH_GUI_

