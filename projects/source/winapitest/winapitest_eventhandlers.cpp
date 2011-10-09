//=============================================================================================================
#include "winapitest.h"

void WinapiTest::application_keyup(const DummyFramework::skeyboardstate& kstate)
{
	switch( kstate.key )
	{
	case VK_ESCAPE:
		Application.Close();
		break;

	default:
		break;
	}
}
//=============================================================================================================
void WinapiTest::application_notify(const DummyFramework::scommand& cmd)
{
	NMHDR* hdr = (NMHDR*)cmd.lparam;

	if( hdr->idFrom == updown1.ID() )
	{
		if( DummyFramework::uda_valuechanged == updown1.ProcessCommands(cmd.wparam, cmd.lparam) )
		{
			float value = (float)updown1.GetValue();
			unsigned short pos = (unsigned short)(value * 100.0f);

			trackbar1.SetPos(pos);
			bloomrenderer->GetEffect()->SetFloat("bloomintensity", value);
		}
	}
}
//=============================================================================================================
void WinapiTest::application_command(const DummyFramework::scommand& cmd)
{
	WORD id = LOWORD(cmd.wparam);
	WORD msg = HIWORD(cmd.wparam);
	HWND ctrl = (HWND)cmd.lparam;

	if( ctrl == trackbar1.GetHandle() )
	{
		if( DummyFramework::tba_moved == trackbar1.ProcessCommands(cmd.wparam, cmd.lparam) )
		{
			float value = (float)trackbar1.GetPos() / 100.0f;

			updown1.SetValue(value);
			bloomrenderer->GetEffect()->SetFloat("bloomintensity", value);
		}
	}
	else if( id == button1.ID() )
	{
		if( msg == BN_CLICKED )
		{
			bloomrenderer->Enabled = !bloomrenderer->Enabled;

			if( bloomrenderer->Enabled )
				label1.SetText("Bloom toggled on");
			else
				label1.SetText("Bloom toggled off");
		}
	}
	else if( id == menustrip1.ID(0, 0) )
	{
		if( openfile1.Show(Application.GetWindowHandle()) )
		{
			std::string ext;
			D3DXIMAGE_INFO info;

			DummyFramework::CHelper::GetExtension(ext, openfile1.FileName);

			if( current )
			{
				Content.UnloadTexture(current);
				current = NULL;
			}

			current = Content.LoadTexture(openfile1.FileName, &info);
			dassert(, "* Open(): Could not open file", current);

			if( info.Width >= info.Height )
			{
				world._22 = (float)min(info.Height, PresentationParameters.BackBufferHeight);
				world._11 = world._22 * (float)info.Width / (float)info.Height;
			}
			else
			{
				world._11 = (float)min(info.Width, PresentationParameters.BackBufferWidth);
				world._22 = world._11 * (float)info.Height / (float)info.Width;
			}

			world._41 = (PresentationParameters.BackBufferWidth - world._11) * 0.5f;
			world._42 = (PresentationParameters.BackBufferHeight - world._22) * 0.5f;
		}
	}
	else if( id == menustrip1.ID(0, 2) )
	{
		Application.Close();
	}
}
//=============================================================================================================
