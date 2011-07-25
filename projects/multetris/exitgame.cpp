//=============================================================================================================
#include "exitgame.h"

#include <dummyframework/base/CHelper.h>
#include <dummyframework/base/d3d9/CGame9.h>

unsigned long examplecolors5[608] =
{
    0xfeffdab9, 0xf2ffdab9, 0xf2ffdab9, 0xf2ffdab9, 0xf2ffdab9, 0xf2ffdab9, 0xf2ffdab9, 0xf2ffdab9, 0xf2ffdab9, 0xf2ffdab9, 0xf2ffdab9, 0xf5fed9b8, 0xf8fdd8b7, 0xf9fcd8b7, 0xf9fcd8b7, 0xf9fcd8b7, 0xf9fcd8b7, 0xf9fcd8b7, 0xf9fcd8b7, 0xf8fcd8b7, 0xf6fdd9b8, 0xf2ffdab9, 0xf2ffdab9, 0xf2ffdab9, 0xf2ffdab9, 0xf2ffdab9, 0xf2ffdab9, 0xf2ffdab9, 0xf2ffdab9, 0xf2ffdab9, 0xf2ffdab9, 0xfeffdab9, 
    0xf0ffdab9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x42a29275, 0x85948768, 0x91918867, 0x91908866, 0x918f8765, 0x918e8662, 0x918d825e, 0x91968460, 0x889a8e6c, 0x4b7c7050, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xf0ffdab9, 
    0xf0ffdab9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x7d978162, 0xed5d7046, 0xff588656, 0xff619364, 0xff639667, 0xff669868, 0xff6c9264, 0xff9e926d, 0xf6b4af91, 0x9b84795d, 0x3000000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xf0ffdab9, 
    0xf0ffdab9, 0, 0, 0, 0x1ba5600, 0x8cd5f00, 0x10ca5e00, 0xaba5600, 0x3ae5100, 0, 0, 0x7d937e5f, 0xed447b49, 0xff37894f, 0xff4e9965, 0xff42915a, 0xff398d52, 0xff3c894f, 0xff899066, 0xf6a7a082, 0x9b817557, 0x3000000, 0, 0x3b45400, 0xabe5800, 0x10cc5e00, 0x8ca5d00, 0x1b05200, 0, 0, 0, 0xf0ffdab9, 
    0xf0ffdab9, 0, 0, 0x38f4200, 0x1ea04900, 0x5bb25200, 0x7dc25a00, 0x66cf6000, 0x3acd5f00, 0xfb55500, 0, 0x7d917c5d, 0xed346c3a, 0xff0c6528, 0xff016020, 0xff0e682b, 0xff036121, 0xff136729, 0xff81895f, 0xf6948c6e, 0x9b776a4d, 0x3000000, 0xbe76c00, 0x36dc6700, 0x65d16100, 0x7cc35a00, 0x5cb15100, 0x21934400, 0x4723500, 0, 0, 0xf0ffdab9, 
    0xf0ffdab9, 0, 0x1592900, 0x1e723400, 0x8f893f00, 0xf79f4900, 0xffba5600, 0xffd86603, 0xbac96004, 0x33a04b01, 0, 0x7d937e5d, 0xed2a5d2f, 0xff0a5823, 0xff085823, 0xff166130, 0xff0c5b27, 0xff216433, 0xff82855d, 0xf6736d50, 0x9b6c5f41, 0x3000000, 0x24e26a01, 0xa5e16b04, 0xfed96603, 0xffba5600, 0xf89f4900, 0x98813b00, 0x23632d00, 0x2462000, 0, 0xf0ffdab9, 
    0xf0ffdab9, 0, 0xa5a2900, 0x60662f00, 0xed773700, 0xff964703, 0xffbc6111, 0xfed58136, 0xafb77e4b, 0x2d684d34, 0x54c2d0e, 0x7e947e5d, 0xed566943, 0xff3e653d, 0xff3b663d, 0xff446d45, 0xff3f6a40, 0xff4c6e44, 0xff978a62, 0xf6847551, 0x9d6a5b39, 0x7341e09, 0x1bac8057, 0x96d59357, 0xfdd68236, 0xffbc6111, 0xff964703, 0xf0753600, 0x72562800, 0xe421f00, 0, 0xf0ffdab9, 
    0xf0ffdab9, 0, 0xc592800, 0x62653105, 0xd6814a18, 0xffac7038, 0xffcc9154, 0xffe1b88c, 0xd2bba891, 0x767f5b3a, 0x5e683409, 0x906b563b, 0xe8786c4c, 0xcf6a6547, 0xbb696948, 0xba6b6c4c, 0xba72714f, 0xbf807756, 0xd7806b47, 0xf06f5f38, 0xaa4d3c1f, 0x5d683508, 0x6f86603d, 0xc5c7b29a, 0xffe1b88c, 0xffcc9154, 0xffac7038, 0xe17b4617, 0x804c2604, 0x123a1b00, 0, 0xf0ffdab9, 
    0xf0ffdab9, 0x3565a07, 0x24595f07, 0x6761640e, 0xc078712c, 0xffa39453, 0xffc5ad6d, 0xffdcb986, 0xebad8f6d, 0xc96c4421, 0xcf4f2b0b, 0x9d2d2214, 0xd345452e, 0x61323222, 0x3029291e, 0x2d28281c, 0x2e242418, 0x4118180f, 0x741c1c0f, 0xc92d2d17, 0xa821170b, 0xcd4f2b0b, 0xc26c431f, 0xddb39471, 0xffdcb986, 0xffc5ad6d, 0xffa39453, 0xd06f6829, 0x7655580d, 0x28525706, 0x3515506, 0xf0ffdab9, 
    0xf2fed9b8, 0x2d4e5006, 0xa5545706, 0xe05a5e07, 0xf85f640a, 0xff6a6d13, 0xff828027, 0xffa99a4d, 0xe4968549, 0xca615615, 0xa43d390c, 0x65212315, 0x8629291b, 0x22222215, 0x1000000, 0, 0x2000000, 0x1e000000, 0x3b0e0e07, 0x6e222312, 0x7016180c, 0x9c403c0c, 0xbf645713, 0xd59d8a4b, 0xfea99b4e, 0xff828027, 0xff6a6d13, 0xf95e630a, 0xe15a5e07, 0xa7535606, 0x35444605, 0xf2fdd9b7, 
    0xf7fad6b4, 0x874e5006, 0xfd525406, 0xff535506, 0xff565807, 0xff585b07, 0xff5d6207, 0xff6f7414, 0xf2747a18, 0xea6d780c, 0x945b6608, 0x2c3e4411, 0x390a0a07, 0x390a0c0c, 0x661e2426, 0x6c21282a, 0x6e21292a, 0x80181e1e, 0x50060808, 0x37070704, 0x312e3307, 0x7d6c7909, 0xe4707c0c, 0xe9787f19, 0xfe6f7514, 0xff5d6207, 0xff585b07, 0xff565807, 0xff535506, 0xfe525406, 0x9e434505, 0xf9f8d5b3, 
    0xfaf8d4b2, 0xb74f5106, 0xff525406, 0xff535506, 0xff565807, 0xff585b07, 0xff5a5e07, 0xff626808, 0xff6d780a, 0xff727f0a, 0xa25b6608, 0x213f4605, 0x12000000, 0x84282d2a, 0xdf303634, 0xe8303735, 0xe82f3635, 0xe82a3230, 0x901e2221, 0x19000000, 0x165e6908, 0x846f7d0a, 0xff727f0a, 0xff6d780a, 0xff626808, 0xff5a5e07, 0xff585b07, 0xff565807, 0xff535506, 0xff525406, 0xd8434405, 0xfcf6d3b0, 
    0xfbf8d4b1, 0xbc4f5106, 0xff525406, 0xff545607, 0xff565807, 0xff585b07, 0xff5a5e07, 0xff616708, 0xff6d780a, 0xff72800a, 0x9c5a6508, 0x1e3a4105, 0x1040707, 0x925f4b32, 0xd85f4c33, 0xdd58462f, 0xdd4e3f2a, 0xdb423522, 0xa72e2416, 0xf000000, 0x11697609, 0x7f6f7d0a, 0xff72800a, 0xff6d780a, 0xff616708, 0xff5a5e07, 0xff585b07, 0xff565807, 0xff545607, 0xff525406, 0xdf424405, 0xfdf5d2b0, 
    0xfaf8d4b2, 0xb84f5106, 0xff525406, 0xff545607, 0xff565807, 0xff585b07, 0xff5a5e07, 0xff626808, 0xff6d780a, 0xff72800a, 0x95586308, 0x1b313804, 0, 0x2a78552f, 0x3c78532d, 0x3e6c4b27, 0x3e5e401f, 0x3d4b3014, 0x32311f0b, 0x6000000, 0xd657209, 0x766f7d0a, 0xff72800a, 0xff6d780a, 0xff626808, 0xff5b5e07, 0xff585b07, 0xff565807, 0xff545607, 0xff525406, 0xdd424305, 0xfdf5d2b0, 
    0xf9f8d5b2, 0xab4f5106, 0xff525406, 0xff545607, 0xff565807, 0xff585b07, 0xff5b5e07, 0xff626808, 0xff6e790a, 0xff727f0a, 0x87555f08, 0x151b1f03, 0, 0, 0, 0, 0, 0, 0, 0, 0x65a6508, 0x676f7c0a, 0xfe72800a, 0xff6e790a, 0xff626808, 0xff5b5e07, 0xff585b07, 0xff565807, 0xff545607, 0xff505206, 0xd33f4105, 0xfcf6d3b0, 
    0xf7fad6b4, 0x8d4e5006, 0xff525406, 0xff545607, 0xff565807, 0xff585b07, 0xff5b5e07, 0xff626908, 0xff6e790a, 0xe1606b08, 0x58444c06, 0x70a0b01, 0, 0, 0, 0, 0, 0, 0, 0, 0x1505a07, 0x3a687409, 0xc66e7a0a, 0xff6e790a, 0xff626908, 0xff5b5e07, 0xff585b07, 0xff565807, 0xff535507, 0xff494b06, 0xba363804, 0xfaf7d3b1, 
    0xf2fed9b8, 0x29404205, 0x9a424405, 0xd7484a06, 0xf5505206, 0xfe575a07, 0xff5b5e07, 0xfc5f6508, 0xcf555e07, 0x603f4606, 0x112a2f04, 0x1000000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x7606b09, 0x3f5f6a09, 0xb2636c08, 0xf9606708, 0xff5b5e07, 0xfe575a07, 0xf84f5106, 0xe3454606, 0xb2373904, 0x4c212103, 0xf4fbd7b6, 
    0xf0ffdab9, 0x2333404, 0xe3c3d04, 0x19393a04, 0x2d2c2d04, 0x482a2b03, 0x5e323304, 0x59303304, 0x2c2a2e04, 0x82c3004, 0x1000000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x4555f08, 0x1b444b06, 0x4b393c04, 0x62303204, 0x51262703, 0x38232403, 0x202e2e04, 0x11303203, 0x6161602, 0xf0ffdab9, 
    0xfeffdab9, 0xf2ffdab9, 0xf2ffdab9, 0xf3ffdab9, 0xf3fed9b8, 0xf5fcd8b7, 0xf6fcd7b6, 0xf6fcd7b6, 0xf4fed9b8, 0xf2ffdab9, 0xf2ffdab9, 0xf2ffdab9, 0xf2ffdab9, 0xf2ffdab9, 0xf2ffdab9, 0xf2ffdab9, 0xf2ffdab9, 0xf2ffdab9, 0xf2ffdab9, 0xf2ffdab9, 0xf2ffdab9, 0xf2ffdab9, 0xf2ffdab9, 0xf3fedab9, 0xf5fdd8b7, 0xf6fbd7b6, 0xf5fcd7b7, 0xf4fdd8b8, 0xf3fedab9, 0xf2ffdab9, 0xf2ffdab9, 0xfeffdab9, 
};
//=============================================================================================================
ExitGame::ExitGame()
	: CForm()
{
	Atlas = NULL;
	Cell = Glow = NULL;
	
	blink.set(0, 1);
	blink = blink.count() - 1;
	blink.direction = 1;
}
//=============================================================================================================
ExitGame::~ExitGame()
{
}
//=============================================================================================================
bool ExitGame::Initialize(DummyFramework::CGame9& mygame, DummyFramework::CSpriteFont9& font)
{
	CForm::Initialize(mygame, font);
	quads.Initialize(mygame.Graphics, 1300);
	
	return true;
}
//=============================================================================================================
void ExitGame::Draw()
{
	blink.smooth((float)game->Sync.Alpha());

	size_t count = 0;
	float w = GameVariables::ScreenWidth - GameVariables::CorrelateH(276);
	float h = floor(GameVariables::ScreenHeight / Cell->Size.y) * Cell->Size.y;

	float sx = GameVariables::CorrelateW(Cell->Size.x);
	float sy = sx;

	w = floor(w / sx) * sx;

	D3DXVECTOR2 pos;
	D3DXCOLOR color;
	D3DXCOLOR white(0xffffffff);

	float x = w * 0.5f - 16 * sx, y = h * 0.5f - 9 * sy;

	quads.Lock();
    {
		for( int i = 0; i < 19; ++i )
		{
			for( int j = 0; j < 32; ++j )
			{
				pos.x = x + j * sx;
				pos.y = y + i * sy;

				color = examplecolors5[i * 32 + j];
				white.a = color.a = blink.value;

				quads.WriteQuad(count, pos, D3DXVECTOR2(sx, sy), Cell->Texcoords, color);
				quads.WriteQuad(count + 1, pos, D3DXVECTOR2(sx, sy), Glow->Texcoords, white);

				count += 2;
			}
		}
	}
	quads.Unlock();

	game->StateManager->SetTexture(0, Atlas);
    quads.Draw(count);

	CForm::Draw();
}
//=============================================================================================================
void ExitGame::SetState(unsigned long newstate)
{
	switch( newstate )
	{
	case Idle:
		break;
		
	case BlinkIn:
		blink = 0;
		blink.direction = 1;
		break;

	default:
		break;
	}

	CForm::SetState(newstate);
}
//=============================================================================================================
void ExitGame::Update()
{
	blink.step();

	switch( state )
	{
	case TransitionIn:
		SetState(Idle);
		focusgained(*this);
		break;

	case TransitionOut:
        focuslost(*this);
		break;
	
	default:
		break;
	}
	
	CForm::Update();
}
//=============================================================================================================
void ExitGame::onfocusgained()
{
	CForm::onfocusgained();
	SetState(TransitionIn);
}
//=============================================================================================================
void ExitGame::onfocuslost()
{
	CForm::onfocuslost();
	SetState(Inactive);
}
//=============================================================================================================
void ExitGame::onkeyup(const DummyFramework::skeyboardstate& kstate)
{
	if( kstate.key == VK_ESCAPE )
	{
		action = 0;
		SetState(TransitionOut);
	}
}
//=============================================================================================================
void ExitGame::onlostdevice()
{
	quads.onlostdevice();
}
//=============================================================================================================
void ExitGame::onresetdevice()
{
	quads.onresetdevice();
}
//=============================================================================================================
