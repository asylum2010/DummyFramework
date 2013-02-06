//=============================================================================================================
#include "game.h"

void Game::avatar_shoot()
{
	// itt a mozgás miatti eltérések miatt az aktuálisat kell nézni
	const D3DXVECTOR2& position = avatar.Position.current;
	Projectile p;

	p.Image = &atlas1["projectile"];
	p.Direction = D3DXVECTOR2(1, 0);
	p.Speed = GameVariables::CorrelateW(avatar.ProjectileSpeed);

	for( unsigned short i = 0; i < avatar.Cannons; ++i )
	{
		p.Position = D3DXVECTOR2(
			position.x + avatar.Image->Size.x * avatar.ProjectileOrigin.x,
			position.y + avatar.Image->Size.y * avatar.ProjectileOrigin.y + (avatar.Cannons * -0.5f + i) * p.Image->Size.y);

		avatarprojectiles.push_back(p);
	}
}
//=============================================================================================================
void Game::avatar_dead()
{
	action = GameOver;
	SetState(TransitionHalf);
}
//=============================================================================================================
void Game::avatar_explode()
{
	Explosion ex;

	ex.Image = &atlas1["explosion"];
	ex.Initialize(*game);
	ex.Position = avatar.Position.current + (avatar.Image->Size - ex.Image->Size) * 0.5f;
	ex.Speed = background.Speed * 1.2f;

	explosions.push_front(ex);
	UnapplyPowerup();

	GameVariables::Deaths++;
}
//=============================================================================================================
void Game::enemy_explode(Explosion& ex)
{
	ex.Image = &atlas1["explosion"];
	ex.Initialize(*game);
	ex.Speed = background.Speed * 1.2f;

	AddPowerup(ex.Position.current);

	ex.Position = ex.Position.current - ex.Image->Size * ex.Scale * 0.5f;
	explosions.push_front(ex);
	
	// van olyan ellenfél ami robbanáskor is ad pontot
	hud.Score += ex.Value;
	GameVariables::Score += ex.Value;
	GameVariables::TotalScore = hud.Score;
}
//=============================================================================================================
void Game::enemy_dead(const Enemy& sender)
{
	hud.Score += sender.Value;

	GameVariables::Score += sender.Value;
	GameVariables::TotalScore = hud.Score;
	GameVariables::EnemiesKilled++;
}
//=============================================================================================================
void Game::enemy_shoot1(Projectile& p)
{
	p.Image = &atlas1["projectile"];

	D3DXVECTOR2 pos(
		p.Position.current.x - p.Image->Size.x,
		p.Position.current.y - p.Image->Size.y * 0.5f);

	p.Speed += GameVariables::CorrelateW(60.0f);
	p.Position.previous = pos - p.Direction * p.Speed;
	p.Position.current = pos;

	enemyprojectiles.push_back(p);
}
//=============================================================================================================
void Game::enemy_shoot2(Projectile& p)
{
	p.Image = &atlas1["plasma"];

	D3DXVECTOR2 pos(
		p.Position.current.x - p.Image->Size.x * 0.5f,
		p.Position.current.y - p.Image->Size.y * 0.5f);

	p.Speed = GameVariables::CorrelateH(p.Speed);
	p.Position.previous = pos - p.Direction * p.Speed;
	p.Position.current = pos;

	enemyprojectiles.push_back(p);
}
//=============================================================================================================
void Game::rapidfire_apply()
{
	hud.Announce("Rapid fire!!!");
	UnapplyPowerup();
	avatar.WeaponCooldown /= 2;
}
//=============================================================================================================
void Game::twincannons_apply()
{
	hud.Announce("Twin cannons!!!");
	UnapplyPowerup();
	avatar.Cannons = 2;
}
//=============================================================================================================
void Game::extrahealth_apply()
{
	hud.Announce("Extra life!!!");

	avatar.Health++;
	avatar.Health = min(avatar.Health, 20);
}
//=============================================================================================================
void Game::extrashield_apply()
{
	hud.Announce("Extra shield!!!");

	avatar.Integrity++;
	avatar.Integrity = min(avatar.Integrity, 20);
}
//=============================================================================================================
void Game::onlostdevice()
{
	if( initialized && contentloaded )
	{
		safe_release(rendersurface);
		rendertarget = NULL;

		CForm::onlostdevice();
	}
}
//=============================================================================================================
void Game::onresetdevice()
{
	if( initialized && contentloaded )
	{
		// nagy hack, de kell
		rendertarget = (LPDIRECT3DTEXTURE9)game->Content.GetTextureByID(id);
		game->Content.UnloadTexture(rendertarget);

		rendertarget = game->Content.CreateRenderTarget(game->DisplayMode.Width, game->DisplayMode.Height, D3DFMT_A8R8G8B8);

		if( !rendertarget )
		{
			dsad("Game::LoadContent(): Could not create render target");
			GameVariables::EnableShaders = false;
		}
		else
		{
			id = game->Content.GetTextureID(rendertarget);
			rendertarget->GetSurfaceLevel(0, &rendersurface);
		}

		ResizeSprites();
		CForm::onresetdevice();
	}
}
//=============================================================================================================
void Game::onfocusgained()
{
	CForm::onfocusgained();

	if( state == FadeToMainMenu )
	{
		action = MainMenu;
		SetState(TransitionOut);
	}
	else if( state == FadeToRestart )
	{
		action = Restart;
		SetState(TransitionOut);
	}
	else if( state == FadeToNext )
	{
		action = NextLevel;
		SetState(TransitionOut);
	}
	else
	{
		// az akcio ilyenkor megmarad
		if( action == Restart )
			RestartLevel();

		GameVariables::Sync.Timer().Start();
        
		// most indul/restartol/kövpálya
		if( state != Inactive )
		{
			alpha = 0;
			ticks = 0;
		}
		// a pausememnüböl jöttünk
		else
			alpha = alpha.current;

		alpha.direction = 1;
		SetState(TransitionIn);
	}
}
//=============================================================================================================
void Game::onfocuslost()
{
	CForm::onfocuslost();

	// a pausemenübe lépünk ki
	if( state == TransitionHalf )
		SetState(Inactive);
	// kifadeltünk valami külsö akcio miatt (statmenü vagy pause:mainmenu)
	else
	{
		// ha a statmenubol is lehet restartolni akkor NextLevel
		if( action == LevelComplete )
		{
			script.DeleteGarbage();
		}
		else if( action == MainMenu )
		{
			script.DeleteGarbage();
			script.Flush(); // nem muszáj

			avatar.Health = avatar.MaxHealth = 3;
			avatar.Integrity = avatar.MaxIntegrity = 3;
			hud.Score = 0;
		}

		SetState(Hidden);
	}
}
//=============================================================================================================
void Game::onkeyup(const DummyFramework::skeyboardstate& kstate)
{
	// ez valamiért automatikusan teljesül, de ne hagyjunk helyet a bugoknak
	if( state == FlyOff )
		return;

	switch( kstate.key )
	{
	case VK_ESCAPE:
		action = PauseMenu;
		SetState(TransitionHalf);
		break;

	default:
		break;
	}
}
//=============================================================================================================
