#include <AppKit/textbox.h>
#include <AppKit/window.h>
#include <AppKit/legacytools.h>
#include <AppKit/resources.h>

/* *********************************
 * TextBox Object Oriented Wrapper *
 ***********************************/

akTextBox::akTextBox(akRect rect, string text, akView *parent)
	: akControl(rect, NULL)
{
	SetClassName("akTextBox");
	mText = text;
	AddKeyEventReceiver(this);
	AddMouseEventReceiver(this);
	AddPainter(this);
	AddViewNotificationReceiver(this);

	akRect viewRect = GetRect();
	SDL_Color bgColor, textColor, cursorColor;
	bgColor.r = 255; bgColor.g = 255; bgColor.b = 255;
	textColor.r = 0; textColor.g = 0; textColor.b = 0;
	cursorColor.r = 0; cursorColor.g = 0; cursorColor.b = 0;
	mTextBox = TextBox_Init(viewRect.location.x, viewRect.location.y, viewRect.size.width, viewRect.size.height,
		Resources::GetFontResource(DEFAULT_FONT), bgColor, textColor, cursorColor);

	TextBox_SetText(mTextBox, (char*)text.c_str());
}

string akTextBox::GetText()
{
	string *str = new string(mTextBox->text);
	return *str;
}

void akTextBox::SetText(string text)
{
	TextBox_SetText(mTextBox, (char*)(text.c_str()));
	GetWindow()->Repaint();
}

void akTextBox::KeyPress(akView* sender, akKeyEvent *event)
{
	this->InvokeActionReceivers(this);
	TextBox_OnKeyDown(mTextBox, event->GetScancode(), event->GetKeycode(), event->GetKeymod());
	GetWindow()->Repaint();
}

void akTextBox::TextInput(akView* sender, akKeyEvent *event)
{
    this->InvokeActionReceivers(this);
    TextBox_OnTextInput(mTextBox, event->GetText());
    GetWindow()->Repaint();
}

void akTextBox::KeyRelease(akView* sender, akKeyEvent *event)
{
}

void akTextBox::MousePress(akView* sender, akMouseEvent *event)
{
	/*akView *oldFirstResponder = GetWindow()->GetFirstResponder();
	if (oldFirstResponder) {
		if (oldFirstResponder->GetClassName().compare("akTextBox") == 0) {
			akTextBox *textbox = (akTextBox*)oldFirstResponder;
			TextBox_RemoveFocus(textbox->mTextBox);
		}
	*/
	GetWindow()->SetFirstResponder(this);
	TextBox_SetFocus(mTextBox);
	TextBox_OnMouseButtonDown(mTextBox, event->GetLocationInWindow().x, event->GetLocationInWindow().y);
	GetWindow()->Repaint();
}

void akTextBox::MouseRelease(akView* sender, akMouseEvent *event)
{
}

void akTextBox::MouseMove(akView* sender, akMouseEvent *event)
{
}

void akTextBox::MouseDrag(akView* sender, akMouseEvent *event)
{
}

void akTextBox::MouseWheelUp(akView *sender, akMouseEvent *event)
{
}

void akTextBox::MouseWheelDown(akView *sender, akMouseEvent *event)
{
}

void akTextBox::Paint(akView *view, SDL_Surface *destination)
{
	SDL_Rect p;
	p.x = 0;
	p.y = 0;
	SDL_BlitSurface(mTextBox->bgSurface,NULL,mTextBox->destSurface,&p);
	LegacyTools::Blit(mTextBox->textSurface,mTextBox->blitInitialPosition,0,mTextBox->width,mTextBox->height,mTextBox->destSurface,0,0);

	SDL_Rect dstrect;
	dstrect.x = mTextBox->x - 2;
	dstrect.y = mTextBox->y - 2;
	SDL_BlitSurface(mTextBox->bg, NULL, destination, &dstrect);
	dstrect.x = mTextBox->x;
	dstrect.y = mTextBox->y;
	SDL_BlitSurface(mTextBox->destSurface, NULL, destination, &dstrect);
}

void akTextBox::ViewWillResignFirstResponder()
{
	TextBox_DrawBorder(mTextBox);
	TextBox_RemoveFocus(mTextBox);
	GetWindow()->Repaint();
}

void akTextBox::ViewWillBecameFirstResponder()
{
	akColor focusColor = GetFocusColor();
	SDL_FillRect(mTextBox->bg, NULL, SDL_MapRGB(mTextBox->bg->format, focusColor.r, focusColor.g, focusColor.b));
	TextBox_SetFocus(mTextBox);
	GetWindow()->Repaint();
}

/* ******************************************************
 * LEGACY CODE (To be upgraded later in later versions) *
 ********************************************************/

void TextBox_SetFocus(TextBox *textbox)
{
	textbox->focus = 1;

	/* NEW CODE */
	//SDL_FillRect(textbox->bg, NULL, SDL_MapRGB(SDL_GetVideoSurface()->format, 153, 217, 234));
}

void TextBox_RemoveFocus(TextBox *textbox)
{
	/* NEW CODE */
	//SDL_FillRect(textbox->bg, NULL, SDL_MapRGB(SDL_GetVideoSurface()->format, 255, 255, 255));

	SDL_Rect p;
	textbox->focus = 0;
	SDL_FillRect(textbox->bgSurface,NULL,SDL_MapRGB(textbox->destSurface->format, textbox->bgColor.r, textbox->bgColor.g, textbox->bgColor.b));
	p.x = 0;
	p.y = 0;
	SDL_BlitSurface(textbox->bgSurface,NULL,textbox->destSurface,&p);
	textbox->textSurface = TTF_RenderText_Blended(textbox->font,textbox->text,textbox->textColor);
	LegacyTools::Blit(textbox->textSurface,textbox->blitInitialPosition,0,textbox->width,textbox->height,textbox->destSurface,0,0);
	//MDDrawSurface(textbox->destSurface,textbox->destWindow,textbox->x,textbox->y,0);
}

void TextBox_Delete(TextBox *textbox)
{
	if(textbox->bgSurface != NULL)
		SDL_FreeSurface(textbox->bgSurface);
	if(textbox->cursorSurface != NULL)
		SDL_FreeSurface(textbox->cursorSurface);
	if(textbox->textSurface != NULL)
		SDL_FreeSurface(textbox->textSurface);
	TTF_CloseFont(textbox->font);
	if(textbox->text1 != NULL)
		free(textbox->text1);
	if(textbox->text2 != NULL)
		free(textbox->text2);
	if(textbox->text3 != NULL)
		free(textbox->text3);
	if(textbox->textSelectionBuffer != NULL)
		free(textbox->textSelectionBuffer);
	
}

void TextBox_DrawBorder(TextBox *textbox)
{
	SDL_Surface *s;
	SDL_Rect p;
	int i;

	SDL_FillRect(textbox->bg, NULL, SDL_MapRGB(textbox->bg->format, 255, 255, 255));
	s = SDL_CreateRGBSurface(0,1,1,32,0,0,0,0);
	SDL_FillRect(s,NULL,SDL_MapRGB(s->format, 171, 173, 179));
	for (i=2;i<textbox->bg->w - 2;i++)
	{
		p.x = i;
		p.y = 0;
		SDL_BlitSurface(s,NULL,textbox->bg,&p);
	}
	SDL_FillRect(s,NULL,SDL_MapRGB(s->format, 227, 233, 239));
	for (i=1;i<textbox->bg->w - 1;i++)
	{
		p.x = i;
		p.y = textbox->bg->h-1;
		SDL_BlitSurface(s,NULL,textbox->bg,&p);
	}
	SDL_FillRect(s,NULL,SDL_MapRGB(s->format, 226, 227, 234));
	for (i=1;i<textbox->bg->h - 1;i++)
	{
		p.x = 0;
		p.y = i;
		SDL_BlitSurface(s,NULL,textbox->bg,&p);
	}
	SDL_FillRect(s,NULL,SDL_MapRGB(s->format, 219, 223, 230));
	for (i=1;i<textbox->bg->h - 1;i++)
	{
		p.x = textbox->bg->w - 1;
		p.y = i;
		SDL_BlitSurface(s,NULL,textbox->bg,&p);
	}
	
	// 1/4
	SDL_FillRect(s,NULL,SDL_MapRGB(s->format, 219, 223, 230));
	p.x = 0;
	p.y = 0;
	SDL_BlitSurface(s,NULL,textbox->bg,&p);
	SDL_FillRect(s,NULL,SDL_MapRGB(s->format, 187, 189, 194));
	p.x = 1;
	p.y = 0;
	SDL_BlitSurface(s,NULL,textbox->bg,&p);
	SDL_FillRect(s,NULL,SDL_MapRGB(s->format, 233, 236, 240));
	p.x = 1;
	p.y = 1;
	SDL_BlitSurface(s,NULL,textbox->bg,&p);

	// 2/4
	SDL_FillRect(s,NULL,SDL_MapRGB(s->format, 235, 235, 238));
	p.x = 0;
	p.y = textbox->bg->h - 1;
	SDL_BlitSurface(s,NULL,textbox->bg,&p);
	SDL_FillRect(s,NULL,SDL_MapRGB(s->format, 233, 236, 240));
	p.x = 1;
	p.y = textbox->bg->h - 2;
	SDL_BlitSurface(s,NULL,textbox->bg,&p);

	// 3/4
	SDL_FillRect(s,NULL,SDL_MapRGB(s->format, 187, 189, 194));
	
	p.x = textbox->bg->w - 2;
	p.y = 0;
	SDL_BlitSurface(s,NULL,textbox->bg,&p);
	SDL_FillRect(s,NULL,SDL_MapRGB(s->format, 219, 223, 230));
	p.x = textbox->bg->w - 1;
	p.y = 0;
	SDL_BlitSurface(s,NULL,textbox->bg,&p);
	SDL_FillRect(s,NULL,SDL_MapRGB(s->format, 233, 236, 240));
	p.x = textbox->bg->w - 2;
	p.y = 1;
	SDL_BlitSurface(s,NULL,textbox->bg,&p);

	// 4/4
	SDL_FillRect(s,NULL,SDL_MapRGB(s->format, 235, 235, 238));
	p.x = textbox->bg->w - 1;
	p.y = textbox->bg->h - 1;
	SDL_BlitSurface(s,NULL,textbox->bg,&p);
	SDL_FillRect(s,NULL,SDL_MapRGB(s->format, 233, 236, 240));
	p.x = textbox->bg->w - 2;
	p.y = textbox->bg->h - 2;
	SDL_BlitSurface(s,NULL,textbox->bg,&p);
}

TextBox * TextBox_Init(int x, int y, int width, int height, TTF_Font *font, 
					  SDL_Color bgColor, SDL_Color textColor, SDL_Color cursorColor)
{
	TextBox *textbox;
	SDL_Color black={0,0,0};

	textbox = (TextBox*)malloc(sizeof(TextBox));
	textbox->font = font;
	textbox->textPosition.x = x;
	textbox->textPosition.y = y;
	textbox->textBgPosition.x = x;
	textbox->textBgPosition.y = y;
	textbox->blitInitialPosition = 0;
	textbox->count = 1;
	textbox->cursorIndex = 0;
	textbox->text = NULL;
	textbox->textSelectionBuffer = NULL;
	textbox->bgColor = bgColor;
	textbox->textColor = textColor;
	textbox->destSurface = SDL_CreateRGBSurface(0,width,height,32,0,0,0,0);
	textbox->bg = SDL_CreateRGBSurface(0,width+4,height+4,32,0,0,0,0);
	SDL_FillRect(textbox->bg,NULL,SDL_MapRGB(textbox->bg->format, 255, 255, 255));
	
	// WAS HERE
	TextBox_DrawBorder(textbox);

	//textbox->destWindow = parent;
	textbox->focus = 0;

	textbox->fontSize = 18;
	textbox->textSurface = TTF_RenderText_Blended(textbox->font,"",black);
	textbox->width = width;
	textbox->height = height;
	textbox->x = x;
	textbox->y = y;
	textbox->textBgPosition.x = x;
	textbox->textBgPosition.y = y;
	textbox->textPosition.x = textbox->textBgPosition.x;
	textbox->textPosition.y = textbox->textBgPosition.y;
	textbox->bgSurface = SDL_CreateRGBSurface(0, textbox->width, textbox->height, 32, 0, 0, 0, 0);
	SDL_FillRect(textbox->bgSurface,NULL,SDL_MapRGB(textbox->destSurface->format, bgColor.r, bgColor.g, bgColor.b));
	
	textbox->cursorSurface = SDL_CreateRGBSurface(0, 1, textbox->height, 32, 0, 0, 0, 0);
	SDL_FillRect(textbox->cursorSurface,NULL,SDL_MapRGB(textbox->destSurface->format, cursorColor.r, cursorColor.g, cursorColor.b));
	
	textbox->textSelectionBuffer = (char*)malloc(sizeof(char)*2);
	textbox->text = (char*)malloc(sizeof(char)*textbox->count);
	textbox->text[0] = '\0';
	textbox->id = -1;

	return textbox;
}

void TextBox_Draw(TextBox *textbox)
{
	SDL_Rect p;
	/*SDL_Surface *rectLine=NULL;
	rectLine = SDL_CreateRGBSurface(SDL_HWSURFACE,textbox->width+2,textbox->height+2,32,0,0,0,0);
	SDL_FillRect(rectLine,NULL,SDL_MapRGB(SDL_GetVideoSurface()->format, 127, 157, 185));
	SDL_BlitSurface(rectLine)*/
	p.x = 0;
	p.y = 0;
	SDL_BlitSurface(textbox->bgSurface,NULL,textbox->destSurface,&p);
	LegacyTools::Blit(textbox->textSurface,textbox->blitInitialPosition,0,textbox->width,textbox->height,textbox->destSurface,0,0);
	/* To be updated */
	//MDDrawSurface(textbox->bg,textbox->destWindow,textbox->x-2,textbox->y-2,0);
	//MDDrawSurface(textbox->destSurface,textbox->destWindow,textbox->x,textbox->y,1);
	/*SDL_FreeSurface(rectLine);*/
}

void __TextBox_Draw(TextBox *textbox, int updateWnd)
{
	SDL_Rect p;
	/*SDL_Surface *rectLine=NULL;
	rectLine = SDL_CreateRGBSurface(SDL_HWSURFACE,textbox->width+2,textbox->height+2,32,0,0,0,0);
	SDL_FillRect(rectLine,NULL,SDL_MapRGB(SDL_GetVideoSurface()->format, 127, 157, 185));
	SDL_BlitSurface(rectLine)*/
	p.x = 0;
	p.y = 0;
	SDL_BlitSurface(textbox->bgSurface,NULL,textbox->destSurface,&p);
	LegacyTools::Blit(textbox->textSurface,textbox->blitInitialPosition,0,textbox->width,textbox->height,textbox->destSurface,0,0);
	/* To be updated */
	//MDDrawSurface(textbox->bg,textbox->destWindow,textbox->x-2,textbox->y-2,0);
	//MDDrawSurface(textbox->destSurface,textbox->destWindow,textbox->x,textbox->y,updateWnd);
	/*SDL_FreeSurface(rectLine);*/
}

int TextBox_OnMouseButtonDown(TextBox *textbox, int mouse_x, int mouse_y)
{
	SDL_Surface *schar, *schar1,*schar2;
	int i=0, j=0, test1=0;
	int end1;
	char *text2=NULL,*text3=NULL;
	SDL_Rect p={0,0,0,0};

	if((mouse_x >= textbox->x) &&
		(mouse_y >= textbox->y) &&
		(mouse_x <= textbox->x + textbox->width) &&
		(mouse_y <= textbox->y + textbox->height))
	{
		textbox->focus = 1;
		if(strlen(textbox->text) > 0)
		{
			j=1;
			end1 = 0;
			while(!end1 && ((unsigned int)j<=strlen(textbox->text)))
			{
				textbox->text1 = (char*)malloc(sizeof(char)*(j+1));
				for(i=0; i<j; i++)
					textbox->text1[i] = textbox->text[i];
				textbox->text1[j] = '\0';

				text3 = (char*)malloc(sizeof(char)*(j+1));
				test1 = 0;
				for(i=0; i<j; i++)
					text3[i] = textbox->text1[i];
				if(j > 1)
					text3[j-1] = '\0';
				else
				{
					text3[j] = '\0';
					test1 = 1;
				}

				text2 = (char*)malloc(sizeof(char)*2);
				text2[0] = textbox->text[j-1];
				text2[1] = '\0';

				schar = TTF_RenderText_Blended(textbox->font,textbox->text1,textbox->textColor);
				if(textbox->x + schar->w >= (mouse_x + textbox->blitInitialPosition))
					end1 = 1;

				schar1 = TTF_RenderText_Blended(textbox->font,text2,textbox->textColor);
				schar2 = TTF_RenderText_Blended(textbox->font,text3,textbox->textColor);
				if(!test1)
				{
					if((mouse_x + textbox->blitInitialPosition) <= (textbox->x + schar2->w + (schar1->w / 2)))
					{
						textbox->text1[j-1] = '\0';
					}
				}
				else
				{
					if(mouse_x <= (textbox->x + schar2->w / 2))
					{
						textbox->text1[0] = '\0';
						
					}
				}
				
				SDL_FreeSurface(schar);
				SDL_FreeSurface(schar1);
				SDL_FreeSurface(schar2);
				free(text2);
				free(text3);
				j++;
			}

			//Put the cursor

			schar = TTF_RenderText_Blended(textbox->font,textbox->text1,textbox->textColor);

			
			if(textbox->blitInitialPosition > textbox->width)
			{
				/*if(schar == NULL)
					textbox->blitInitialPosition = 0;
				else
					textbox->blitInitialPosition = schar->w - textbox->width;*/
			}


			if(schar == NULL)
				textbox->cursorPosition.x = 0;
			else
				textbox->cursorPosition.x = schar->w - 1;
			//textbox->cursorPosition.textbox->y = ;
			SDL_BlitSurface(textbox->bgSurface,NULL,textbox->destSurface,&p);
			textbox->textSurface = TTF_RenderText_Blended(textbox->font,textbox->text,textbox->textColor);
			
			SDL_BlitSurface(textbox->cursorSurface,NULL,textbox->textSurface,&textbox->cursorPosition);

			LegacyTools::Blit(textbox->textSurface,textbox->blitInitialPosition,0,textbox->width,textbox->width,textbox->destSurface,0,0);
			/* To be updated */
			//MDDrawSurface(textbox->destSurface,textbox->destWindow,textbox->x,textbox->y,1);
			//SDL_Flip(textbox->destSurface);

			textbox->cursorIndex = strlen(textbox->text1);

			free(textbox->text1);
		}
		else
		{
			p.x = 0;
			p.y = 0;
			SDL_BlitSurface(textbox->bgSurface,NULL,textbox->destSurface,&p);
			textbox->cursorSurface->h = textbox->fontSize + 3;
			p.x = 1;
			p.y	= 1;
			SDL_BlitSurface(textbox->cursorSurface,NULL,textbox->destSurface,&p);
			/* To be updated */
			//MDDrawSurface(textbox->destSurface,textbox->destWindow,textbox->x,textbox->y,1);
		}
		return 1;
	}
	return 0;
}

void TextBox_DeleteChar(TextBox *textbox, unsigned int unicode, int sym)
{
	SDL_Surface *schar=NULL;
	int i=0, j=0, test=0,dontDeleteChar=0;
	SDL_Rect p;

	if(textbox->count > 1)
	{
		dontDeleteChar = 0;

		if(textbox->cursorIndex > 0)
		{
			if(textbox->cursorPosition.x <= textbox->blitInitialPosition)
			{
				for(j=0; j<(textbox->width/textbox->fontSize); j++)
				{
					if(strlen(textbox->text) >= 1)
					{
						dontDeleteChar = 1;
						test = 0;
						textbox->cursorIndex--;
						if(textbox->cursorIndex < 0)
						{
							textbox->cursorIndex = 0;
						}

						if(textbox->textSelectionBuffer != NULL)
							free(textbox->textSelectionBuffer);
						textbox->textSelectionBuffer = (char*)malloc(sizeof(char)*(textbox->cursorIndex+2));
						for(i=0; i<textbox->cursorIndex; i++)
						{
							textbox->textSelectionBuffer[i] = textbox->text[i];
						}
						textbox->textSelectionBuffer[textbox->cursorIndex] = 0;
						schar = TTF_RenderText_Blended(textbox->font,textbox->textSelectionBuffer,textbox->textColor);

						if(schar == NULL)
						{

							textbox->cursorPosition.x = 0;
							textbox->cursorPosition.y = 0;
						}
						else
						{
							textbox->cursorPosition.x = schar->w - 1;
							textbox->cursorPosition.y = 0;
						}

						if(textbox->cursorPosition.x < textbox->blitInitialPosition)
						{
							if(schar == NULL)
								textbox->blitInitialPosition = 0;
							else
								textbox->blitInitialPosition = schar->w - 1;
						}

						p.x = 0;
						p.y = 0;
						SDL_BlitSurface(textbox->bgSurface,NULL,textbox->destSurface,&p);
						textbox->textSurface = TTF_RenderText_Blended(textbox->font,textbox->text,textbox->textColor);

						SDL_BlitSurface(textbox->cursorSurface,NULL,textbox->textSurface,&textbox->cursorPosition);
						LegacyTools::Blit(textbox->textSurface,textbox->blitInitialPosition,0,textbox->width,textbox->height,textbox->destSurface,0,0);
						//DrawSurface(textbox->destSurface,textbox->destWindow,textbox->x,textbox->y,1);
					}
				}

				for(j=0; j<(textbox->width/textbox->fontSize); j++)
				{
					if(strlen(textbox->text) >= 1)
					{
						dontDeleteChar = 1;
						if((unsigned int)textbox->cursorIndex < strlen(textbox->text))
						{
							textbox->cursorIndex++;
						}

						if(textbox->textSelectionBuffer != NULL)
							free(textbox->textSelectionBuffer);
						textbox->textSelectionBuffer = (char*)malloc(sizeof(char)*(textbox->cursorIndex+2));
						for(i=0; i<textbox->cursorIndex; i++)
						{
							textbox->textSelectionBuffer[i] = textbox->text[i];
						}
						textbox->textSelectionBuffer[textbox->cursorIndex] = 0;
						schar = TTF_RenderText_Blended(textbox->font,textbox->textSelectionBuffer,textbox->textColor);

						if(schar == NULL)
						{

							textbox->cursorPosition.x = 0;
							textbox->cursorPosition.y = 0;
						}
						else
						{
							textbox->cursorPosition.x = schar->w - 1;
							textbox->cursorPosition.y = 0;
						}

						if(textbox->cursorPosition.x > textbox->blitInitialPosition+textbox->width)
						{
							if(schar == NULL)
								textbox->blitInitialPosition = 0;
							else
								textbox->blitInitialPosition = schar->w - textbox->width;
						}

						p.x = 0;
						p.y = 0;
						SDL_BlitSurface(textbox->bgSurface,NULL,textbox->destSurface,&p);
						textbox->textSurface = TTF_RenderText_Blended(textbox->font,textbox->text,textbox->textColor);

						SDL_BlitSurface(textbox->cursorSurface,NULL,textbox->textSurface,&textbox->cursorPosition);
						LegacyTools::Blit(textbox->textSurface,textbox->blitInitialPosition,0,textbox->width,textbox->height,textbox->destSurface,0,0);
						//DrawSurface(textbox->destSurface,textbox->destWindow,textbox->x,textbox->y,1);
						//SDL_Flip(textbox->destSurface);
					}
				}
				//SDL_Flip(textbox->destSurface);
			}
		}


		if(!dontDeleteChar)
		{
			if(textbox->cursorIndex > 0)
			{
				textbox->text = (char*)realloc(textbox->text,sizeof(char) * textbox->count);
				for(i=textbox->cursorIndex-1; (unsigned int)i<=strlen(textbox->text); i++)
				{
					textbox->text[i] = textbox->text[i+1];
				}
				textbox->text[textbox->count-1] = '\0';
				textbox->count--;

				textbox->cursorIndex--;
				if(textbox->cursorIndex < 0)
				{
					textbox->cursorIndex = 0;
				}

				if(textbox->textSurface != NULL)
					SDL_FreeSurface(textbox->textSurface);
				textbox->textSurface = TTF_RenderText_Blended(textbox->font,textbox->text,textbox->textColor);
				SDL_FillRect(textbox->bgSurface,NULL,SDL_MapRGB(textbox->destSurface->format, textbox->bgColor.r, textbox->bgColor.g, textbox->bgColor.b));
				p.x = 0;
				p.y = 0;
				SDL_BlitSurface(textbox->bgSurface,NULL,textbox->destSurface,&p);
				//DrawSurface(textbox->destSurface,textbox->destWindow,textbox->x,textbox->y,1);

				if(textbox->textSelectionBuffer != NULL)
					free(textbox->textSelectionBuffer);

				textbox->textSelectionBuffer = (char*)malloc(sizeof(char)*(textbox->cursorIndex+2));
				for(i=0; i<textbox->cursorIndex; i++)
				{
					textbox->textSelectionBuffer[i] = textbox->text[i];
				}
				textbox->textSelectionBuffer[textbox->cursorIndex] = 0;

				schar = TTF_RenderText_Blended(textbox->font,textbox->textSelectionBuffer,textbox->textColor);

				if(schar == NULL)
				{

					textbox->cursorPosition.x = 0;
					textbox->cursorPosition.y = 0;
				}
				else
				{
					textbox->cursorPosition.x = schar->w - 1;
					textbox->cursorPosition.y = 0;
				}

				if(textbox->cursorPosition.x < textbox->blitInitialPosition)
				{
					if(schar == NULL)
						textbox->blitInitialPosition = 1;
					else
						textbox->blitInitialPosition = schar->w - 0;
				}


				
				if(textbox->textSurface == NULL)
				{
					textbox->cursorPosition.x = 1;
					SDL_BlitSurface(textbox->cursorSurface,NULL,textbox->destSurface,&textbox->cursorPosition);
				}
				else
				{
					SDL_BlitSurface(textbox->cursorSurface,NULL,textbox->textSurface,&textbox->cursorPosition);
				}
				
				LegacyTools::Blit(textbox->textSurface,textbox->blitInitialPosition,0,textbox->width,textbox->height,textbox->destSurface,0,0);
				//DrawSurface(textbox->destSurface,textbox->destWindow,textbox->x,textbox->y,1);
				//SDL_Flip(textbox->destSurface);
			}
		}
	}
	else
	{
		p.x = 0;
		p.y = 0;
		SDL_BlitSurface(textbox->bgSurface,NULL,textbox->destSurface,&p);
		textbox->cursorSurface->h = textbox->fontSize + 3;
		p.x = 1;
		p.y	= 0;
		SDL_BlitSurface(textbox->cursorSurface,NULL,textbox->destSurface,&p);
		/* To be updated */
		//MDDrawSurface(textbox->destSurface,textbox->destWindow,textbox->x,textbox->y,0);
	}
}

void TextBox_WriteChar(TextBox *textbox, unsigned int unicode, int sym)
{
	SDL_Surface *schar=NULL;
    int i=0;
	SDL_Rect p;

	textbox->count++;
	textbox->text = (char*)realloc(textbox->text,sizeof(char) * textbox->count);
	for(i=textbox->count-1; i>textbox->cursorIndex; i--)
	{
		textbox->text[i] = textbox->text[i-1];
	}
	textbox->text[textbox->cursorIndex] = unicode;
	textbox->cursorIndex++;

	if(textbox->textSurface != NULL)
		SDL_FreeSurface(textbox->textSurface);
	textbox->textSurface = TTF_RenderText_Blended(textbox->font,textbox->text,textbox->textColor);
	SDL_FillRect(textbox->bgSurface,NULL,SDL_MapRGB(textbox->destSurface->format, textbox->bgColor.r, textbox->bgColor.g, textbox->bgColor.b));
	p.x = 0;
	p.y = 0;
	SDL_BlitSurface(textbox->bgSurface,NULL,textbox->destSurface,&p);

	if(textbox->textSelectionBuffer != NULL)
		free(textbox->textSelectionBuffer);
	textbox->textSelectionBuffer = (char*)malloc(sizeof(char)*(textbox->cursorIndex+2));
	for(i=0; i<textbox->cursorIndex; i++)
	{
		textbox->textSelectionBuffer[i] = textbox->text[i];
	}
	textbox->textSelectionBuffer[textbox->cursorIndex] = 0;
	schar = TTF_RenderText_Blended(textbox->font,textbox->textSelectionBuffer,textbox->textColor);
	if(schar == NULL)
	{

		textbox->cursorPosition.x = 0;
		textbox->cursorPosition.y = 0;
	}
	else
	{
		textbox->cursorPosition.x = schar->w - 1;
		textbox->cursorPosition.y = 0;
	}
	if(textbox->cursorPosition.x > textbox->blitInitialPosition+textbox->width)
	{
		if(schar == NULL)
			textbox->blitInitialPosition = 0;
		else
			textbox->blitInitialPosition = schar->w - textbox->width;
	}

	SDL_BlitSurface(textbox->cursorSurface,NULL,textbox->textSurface,&textbox->cursorPosition);

	LegacyTools::Blit(textbox->textSurface,textbox->blitInitialPosition,0,textbox->width,textbox->height,textbox->destSurface,0,0);
	//DrawSurface(textbox->destSurface,textbox->destWindow,textbox->x,textbox->y,1);
	//SDL_Flip(textbox->destSurface);	
}

void TextBox_MoveCursorRight(TextBox *textbox, unsigned int unicode, int sym)
{
	SDL_Surface *schar=NULL;
	int i=0;
	SDL_Rect p;

	if(strlen(textbox->text) >= 1)
	{
		if((unsigned int)textbox->cursorIndex < (strlen(textbox->text)))
		{
			textbox->cursorIndex++;
		}

		if(textbox->textSelectionBuffer != NULL)
			free(textbox->textSelectionBuffer);
		textbox->textSelectionBuffer = (char*)malloc(sizeof(char)*(textbox->cursorIndex+2));
		for(i=0; i<textbox->cursorIndex; i++)
		{
			textbox->textSelectionBuffer[i] = textbox->text[i];
		}
		textbox->textSelectionBuffer[textbox->cursorIndex] = 0;
		schar = TTF_RenderText_Blended(textbox->font,textbox->textSelectionBuffer,textbox->textColor);

		if(schar == NULL)
		{

			textbox->cursorPosition.x = 0;
			textbox->cursorPosition.y = 0;
		}
		else
		{
			textbox->cursorPosition.x = schar->w - 1;
			textbox->cursorPosition.y = 0;
		}

		if(textbox->cursorPosition.x > textbox->blitInitialPosition+textbox->width)
		{
			if(schar == NULL)
				textbox->blitInitialPosition = 0;
			else
				textbox->blitInitialPosition = schar->w - textbox->width;
		}

		p.x = 0;
		p.y = 0;
		SDL_BlitSurface(textbox->bgSurface,NULL,textbox->destSurface,&p);
		textbox->textSurface = TTF_RenderText_Blended(textbox->font,textbox->text,textbox->textColor);

		SDL_BlitSurface(textbox->cursorSurface,NULL,textbox->textSurface,&textbox->cursorPosition);
		LegacyTools::Blit(textbox->textSurface,textbox->blitInitialPosition,0,textbox->width,textbox->height,textbox->destSurface,0,0);
		//DrawSurface(textbox->destSurface,textbox->destWindow,textbox->x,textbox->y,1);
		//SDL_Flip(textbox->destSurface);
	}
}

void TextBox_MoveCursorLeft(TextBox *textbox, unsigned int unicode, int sym)
{
	SDL_Surface *schar=NULL;
	int i=0, test=0;
	SDL_Rect p;

	if(strlen(textbox->text) >= 1)
	{
		test = 0;

		textbox->cursorIndex--;
		if(textbox->cursorIndex < 0)
		{
			textbox->cursorIndex = 0;
		}

		if(textbox->textSelectionBuffer != NULL)
			free(textbox->textSelectionBuffer);
		textbox->textSelectionBuffer = (char*)malloc(sizeof(char)*(textbox->cursorIndex+2));
		for(i=0; i<textbox->cursorIndex; i++)
		{
			textbox->textSelectionBuffer[i] = textbox->text[i];
		}
		textbox->textSelectionBuffer[textbox->cursorIndex] = 0;
		schar = TTF_RenderText_Blended(textbox->font,textbox->textSelectionBuffer,textbox->textColor);

		if(schar == NULL)
		{

			textbox->cursorPosition.x = 0;
			textbox->cursorPosition.y = 0;
		}
		else
		{
			textbox->cursorPosition.x = schar->w - 1;
			textbox->cursorPosition.y = 0;
		}

		if(textbox->cursorPosition.x < textbox->blitInitialPosition)
		{
			if(schar == NULL)
				textbox->blitInitialPosition = 0;
			else
				textbox->blitInitialPosition = schar->w - 1;
		}

		p.x = 0;
		p.y = 0;
		SDL_BlitSurface(textbox->bgSurface,NULL,textbox->destSurface,&p);
		textbox->textSurface = TTF_RenderText_Blended(textbox->font,textbox->text,textbox->textColor);

		SDL_BlitSurface(textbox->cursorSurface,NULL,textbox->textSurface,&textbox->cursorPosition);
		LegacyTools::Blit(textbox->textSurface,textbox->blitInitialPosition,0,textbox->width,textbox->height,textbox->destSurface,0,0);
		//DrawSurface(textbox->destSurface,textbox->destWindow,textbox->x,textbox->y,1);
		//SDL_Flip(textbox->destSurface);
	}
}

void TextBox_OnTextInput(TextBox *textbox, std::string text)
{
    SDL_Surface *schar=NULL;
    
    if(!textbox->focus)
        return;
    
    TextBox_WriteChar(textbox,text.at(0),0);
    
    if(schar != NULL)
        SDL_FreeSurface(schar);
}

void TextBox_OnKeyDown(TextBox *textbox, int32_t scancode, int32_t keycode, int32_t keymod)
{
	SDL_Surface *schar=NULL;

	if(!textbox->focus)
		return;

	if(keycode == SDLK_BACKSPACE)
	{
		TextBox_DeleteChar(textbox,0,0);
		// To be updated
		//MDDrawSurface(textbox->destSurface,textbox->destWindow,textbox->x,textbox->y,1);
	}

	switch(keycode)
	{
	case SDLK_RIGHT:
		TextBox_MoveCursorRight(textbox,0,0);
		// To be updated
		//MDDrawSurface(textbox->destSurface,textbox->destWindow,textbox->x,textbox->y,1);
		break;

	case SDLK_LEFT:
		TextBox_MoveCursorLeft(textbox,0,0);
		// To be updated
		//MDDrawSurface(textbox->destSurface,textbox->destWindow,textbox->x,textbox->y,1);
		break;
	}
	
	if(schar != NULL)
		SDL_FreeSurface(schar);
}

void TextBox_AppendText(TextBox *textbox, char *text)
{
	int i,l;
	TextBox_SetFocus(textbox);
	l = strlen(textbox->text);
	for (i=0; i<l; i++)
	{
        TextBox_OnKeyDown(textbox, SDL_SCANCODE_RIGHT, SDLK_RIGHT, 0);
	}
	if (text != NULL)
	{
        TextBox_OnTextInput(textbox, text);
	}
}

void TextBox_SetText(TextBox *textbox, char *text)
{
	int i,l;
	TextBox_SetFocus(textbox);
	l = strlen(textbox->text);
	for (i=0; i<l; i++)
	{
		TextBox_MoveCursorRight(textbox,0,SDLK_RIGHT);
	}

	TextBox_DeleteChar(textbox,8,SDLK_6);
	while(strcmp(textbox->text,""))
	{
		TextBox_DeleteChar(textbox,8,SDLK_6);
	}
	if (text != NULL)
	{
		for(unsigned i=0; i<strlen(text); i++)
		{
			TextBox_WriteChar(textbox,text[i],SDLK_6);
		}
	}
	//DrawSurface(textbox->destSurface,textbox->destWindow,textbox->x,textbox->y,1);
	TextBox_RemoveFocus(textbox);
}

char *TextBox_GetText(TextBox *textbox)
{
	return textbox->text;
}

void TextBox_SetCursor(TextBox *textbox, int index)
{
	int i,l;
	TextBox_SetFocus(textbox);
	l = strlen(textbox->text);
	for (i=0; i<l; i++)
	{
        TextBox_OnKeyDown(textbox, SDL_SCANCODE_LEFT, SDLK_LEFT, 0);
	}

	for (i=0; i<index; i++)
	{
        TextBox_OnKeyDown(textbox, SDL_SCANCODE_RIGHT, SDLK_RIGHT, 0);
	}
}
