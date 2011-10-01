//=============================================================================================================
#include <dummyframework/base/CHelper.h>
#include <dummyframework/base/d3d9/CSpriteFont9.h>

#include "game.h"
#include "bomb.h"
#include "worm.h"
#include "razor.h"

float basictexcoords[] = { 0, 0, 1, 1 };
D3DXMATRIX idtransform(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

Game::Game()
    : CForm()
{
    avatar.BindPrimary(0, 0x44);
    avatar.BindPrimary(1, 0x41);
    avatar.BindPrimary(2, 0x57);
    avatar.BindPrimary(3, 0x53);
    avatar.BindPrimary(4, VK_CONTROL);
    avatar.BindSecondary(0, VK_RIGHT);
    avatar.BindSecondary(1, VK_LEFT);
    avatar.BindSecondary(2, VK_UP);
    avatar.BindSecondary(3, VK_DOWN);
    avatar.BindSecondary(4, VK_SHIFT);

    atlastex1      = NULL;
    execdelay      = INITIAL_DELAY;
    ticks          = 0;
    invert         = 0;
    invertmode     = 0xff;
    rendertarget   = NULL;
    rendersurface  = NULL;
	distort        = NULL;

    alpha.set(0.0f, 1.0f);

    AddControl(avatarbubble);
    AddControl(hud);
}
//=============================================================================================================
Game::~Game()
{
    Clear();
    safe_release(rendersurface);
}
//=============================================================================================================
bool Game::Initialize(DummyFramework::CGame9& mygame, DummyFramework::CSpriteFont9& font)
{
    if( initialized )
        return true;
	
    mygame.Application.keydown.connect(&avatar, &Avatar::onkeydown);
    mygame.Application.keyup.connect(&avatar, &Avatar::onkeyup);

    mygame.lost.connect(&quads, &quadbuffer::onlostdevice);
    mygame.reset.connect(&quads, &quadbuffer::onresetdevice);
    
    avatar.shoot.connect(this, &Game::avatar_shoot);
    avatar.dead.connect(this, &Game::avatar_dead);
    avatar.explode.connect(this, &Game::avatar_explode);

    quads.Initialize(mygame.Graphics, MAX_SPRITES);

    avatar.Initialize(mygame);
    background.Initialize(mygame);

    return CForm::Initialize(mygame, font);
}
//=============================================================================================================
bool Game::LoadContent()
{
    if( contentloaded )
        return true;

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

    if( GameVariables::EnableShaders )
    {
		if( game->DeviceCaps.VertexShaderVersion >= D3DVS_VERSION(2, 0) &&
			game->DeviceCaps.PixelShaderVersion >= D3DPS_VERSION(2, 0) )
		{
			distort = game->Content.LoadMaterial("distort.fx");
			GameVariables::EnableShaders = (distort != NULL);
		}
    }

    wave = game->Content.LoadTexture("wave.tga");
    dassert(false, "Game::LoadContent(): Could not load 'wave'", wave);

    std::string path;
    GameVariables::Qualify(path, "atlas1.txt");

    atlastex1 = game->Content.LoadTextureAtlas(atlas1, path, &GameVariables::CorrelateH);
    dassert(false, "Game::LoadContent(): Could not load 'atlas1'", atlastex1);

    avatar.Image = &atlas1["ufo"];
    avatar.Lights = &atlas1["lights"];
    avatar.Shield = &atlas1["shield"];

    avatarbubble.Corner = &atlas1["bubble_corner"];
    avatarbubble.Side = &atlas1["bubble_side"];
    avatarbubble.Inside = &atlas1["bubble_inside"];

    atlas1["ufo"].GenerateMask();
    atlas1["projectile"].GenerateMask();
    atlas1["regular"].GenerateMask();
    atlas1["powerup"].GenerateMask();
    atlas1["worm"].GenerateMask();
    atlas1["bomb"].GenerateMask();
    atlas1["island"].GenerateMask();
	atlas1["razor"].GenerateMask();
	atlas1["plasma"].GenerateMask();

    hud.Panel = &atlas1["hud_panel"];
    hud.Side = &atlas1["bar_side"];
    hud.Middle = &atlas1["bar_middle"];
    
	GameVariables::PlayFieldHeight = GameVariables::ScreenHeight - hud.Panel->Size.y * 2;

    return CForm::LoadContent();
}
//=============================================================================================================
void Game::RestartLevel()
{
    Clear();
    UnapplyPowerup();

    GameVariables::Deaths = 0;
    GameVariables::EnemiesKilled = 0;
    GameVariables::PowerupsAcquired = 0;
    GameVariables::Score = 0;
    GameVariables::TotalScore = savedvars.score;

    avatar.MaxHealth = savedvars.maxhealth;
    avatar.MaxIntegrity = avatar.Integrity = savedvars.maxintegrity;

    avatar.Reset();
    hud.Reset();
    script.Reset();

    avatar.Health = savedvars.health;
    avatar.Integrity = savedvars.integrity;
    hud.Score = savedvars.score;

    avatarbubble.SetState(Hidden);
    avatar.DisableControls(true);

    alpha.direction = -1;
    alpha = 0;
    execdelay = INITIAL_DELAY;
    ticks = 0;
    execticks = 0;

    invert = 0;
    invertmode = 0xff;
}
//=============================================================================================================
void Game::ResizeSprites()
{
    for( textureatlas::iterator it = atlas1.begin(); it != atlas1.end(); ++it )
    {
        it->second.Size.x = GameVariables::CorrelateH(it->second.OriginalSize.x);
        it->second.Size.y = GameVariables::CorrelateH(it->second.OriginalSize.y);
    }

	GameVariables::PlayFieldHeight = GameVariables::ScreenHeight - hud.Panel->Size.y * 2;
    background.ResizeSprites();
}
//=============================================================================================================
void Game::Reset(const std::string& stagefile)
{
    Clear();
    UnapplyPowerup();

    savedvars.health = avatar.Health;
    savedvars.maxhealth = avatar.MaxHealth;
    savedvars.integrity = avatar.Integrity;
    savedvars.maxintegrity = avatar.MaxIntegrity;
    savedvars.score = hud.Score;
    
    avatar.InitialPosition = D3DXVECTOR2(0, (GameVariables::ScreenHeight - avatar.Image->Size.y) * 0.5f);
    avatar.Bounds = D3DXVECTOR4(0, hud.Panel->Size.y, GameVariables::ScreenWidth, GameVariables::ScreenHeight - hud.Panel->Size.y);
    
	if( !distort && rendertarget && rendersurface && GameVariables::EnableShaders )
    {
		if( game->DeviceCaps.VertexShaderVersion >= D3DVS_VERSION(2, 0) &&
			game->DeviceCaps.PixelShaderVersion >= D3DPS_VERSION(2, 0) )
		{
			distort = game->Content.LoadMaterial("distort.fx");
			GameVariables::EnableShaders = (distort != NULL);
		}
    }

    if( !script.Compile(stagefile, *this) )
        game->Application.Close();

    RestartLevel();
}
//=============================================================================================================
void Game::SetState(unsigned long newstate)
{
    switch( newstate )
    {
    case Idle:
        ticks = 0;
        CForm::SetState(newstate);
        break;

    case TransitionIn:
        CForm::SetState(newstate);
        break;

    case TransitionOut:
        alpha = alpha.current();
        CForm::SetState(newstate);
        break;

    case TransitionHalf:
        alpha.direction = -1;
        alpha = alpha.count() - 1;
        ticks = 0;
        CForm::SetState(newstate);
        GameVariables::Sync.Timer().Stop();
        break;

    case Animation:
        avatar.DisableControls(true);
        hud.SetState(Hud::TransitionHalf);
        execdelay = DEFAULT_DELAY;
        break;

    case Play:
        avatar.DisableControls(false);
        hud.SetState(Hud::TransitionIn);
        execdelay = DEFAULT_DELAY;
        break;

    case Invert:
        execdelay = DEFAULT_INVERT;
        hud.Warn("Warning: Spacetime distortion!!!", DEFAULT_INVERT);
        break;
    
    default:
        CForm::SetState(newstate);
        break;
    }
}
//=============================================================================================================
void Game::Update()
{
    switch( state )
    {
    case TransitionIn:
        if( ticks > alpha.count() + 1 )
        {
            SetState(Idle);
        }
        else
        {
            alpha.step();
            ++ticks;
        }
        break;

    case TransitionOut:
        if( ticks > alpha.count() + 1 )
        {
            focuslost(*this);
        }
        else
        {
            alpha.step();
            ++ticks;
        }
        break;

    case TransitionHalf:
        if( ticks >= alpha.count() / 2 )
        {
            focuslost(*this);
        }
        else
        {
            alpha.step();
            ++ticks;
        }
        break;

    default:
        break;
    }
}
//=============================================================================================================
void Game::InternalUpdate()
{
    if( !initialized || !contentloaded )
        return;

    hud.Health = avatar.Health;
    hud.Shield = avatar.Integrity;

    if( state == FlyOff )
    {
		avatar.Update();

        if( avatar.Position.current().x - avatar.Image->Size.x > GameVariables::ScreenWidth )
        {
            action = LevelComplete;
            alpha.direction = -1;
            alpha = alpha.count() - 1;
            ticks = 0;

            SetState(TransitionHalf);
        }
        else
        {
            avatar.Bounds.z = GameVariables::ScreenWidth * 2;
            avatar.Position.extend(D3DXVECTOR2(GameVariables::CorrelateW(100), 0));
        }
    }
    else
    {
        if( state != TransitionIn && state != TransitionOut && state != TransitionHalf )
        {
            if( execdelay == -1 )
            {
                if( enemies.size() == 0 )
                    execdelay = DEFAULT_DELAY;
            }
            else if( execticks == execdelay )
            {
                if( invertmode != 0xff )
                {
                    execdelay = DEFAULT_DELAY;
                    invert = invertmode;
                    invertmode = 0xff;
                    state = Idle;
                }
                else if( !script.ExecuteNext() )
                {
                    if( !(avatar.GetState() & Avatar::Exploded || avatar.GetState() & Avatar::Invincible) )
                    {
                        avatar.DisableControls(true);
                        SetState(FlyOff);
                        hud.SetState(Hud::TransitionOut);
                    }
                    else
                        execdelay = 5;
                }

                execticks = 0;
            }
            else
                ++execticks;
        }

        avatar.Update();
    }
    
    background.Update();

    // robbanások
    {
        explosionlist::iterator it = explosions.begin();
        explosionlist::iterator del;

        while( it != explosions.end() )
        {
            if( it->GetState() == Explosion::Faded )
            {
                del = it;
                ++it;
                explosions.erase(del);
            }
            else
            {
                it->Update();
                ++it;
            }
        }
    }

    // ellenfelek
    {
        enemyset::iterator it = enemies.begin();
        enemyset::iterator del;

        while( it != enemies.end() )
        {
            del = it;
            ++it;

            if( !(*del)->Visible() )
            {
                delete (*del);
                enemies.erase(del);
            }
            else
            {
                // mert magátol is meghalhat (bomba)
                (*del)->Update(avatar);
            }
        }
    }

    // lövedékek
    {
        projectilelist::iterator it = avatarprojectiles.begin();
        projectilelist::iterator del;

        while( it != avatarprojectiles.end() )
        {
            if( it->Position.current().x > GameVariables::ScreenWidth ||
				it->Position.current().y > GameVariables::ScreenHeight ||
                it->Position.current().x < -it->Image->Size.x ||
				it->Position.current().y < -it->Image->Size.y )
            {
                del = it;
                ++it;
                avatarprojectiles.erase(del);
            }
            else
            {
                it->Position.extend(it->Direction * it->Speed);
                ++it;
            }
        }

        it = enemyprojectiles.begin();

        while( it != enemyprojectiles.end() )
        {
            if( it->Position.current().x > GameVariables::ScreenWidth ||
				it->Position.current().y > GameVariables::ScreenHeight ||
                it->Position.current().x < -it->Image->Size.x ||
				it->Position.current().y < -it->Image->Size.y )
            {
                del = it;
                ++it;
                enemyprojectiles.erase(del);
            }
            else
            {
                it->Position.extend(it->Direction * it->Speed);
                ++it;
            }
        }
    }
    
    // powerupok
    {
        poweruplist::iterator it = powerups.begin();
        poweruplist::iterator del;

        while( it != powerups.end() )
        {
            if( it->Position.current().x < -it->Image->Size.x )
            {
                del = it;
                ++it;
                powerups.erase(del);
            }
            else
            {
                it->Update();
                ++it;
            }
        }
    }

    CForm::Update();
}
//=============================================================================================================
void Game::Draw()
{
    float syncalpha = GameVariables::GetAlpha();

    if( state != TransitionOut && state != Inactive && state != TransitionHalf && state != FlyOff )
        UpdateCollisions(syncalpha);

    if( invert > 0 )
    {
		if( rendertarget != NULL && rendersurface != NULL )
        {
            if( invert == 3 )
            {
                avatarbubble.Alignment = DT_RIGHT|DT_BOTTOM;
                avatarbubble.Position.x = GameVariables::ScreenWidth - (avatar.Position.value.x + avatar.Image->Size.x * 1.05f);
                avatarbubble.Position.y = GameVariables::ScreenHeight - (avatar.Position.value.y - avatar.Image->Size.y * 0.3f);
            }
            else if( invert == 2 )
            {
                avatarbubble.Alignment = DT_LEFT|DT_BOTTOM;
                avatarbubble.Position.x = avatar.Position.value.x + avatar.Image->Size.x * 1.05f;
                avatarbubble.Position.y = GameVariables::ScreenHeight - (avatar.Position.value.y - avatar.Image->Size.y * 0.3f);
            }
            else
            {
                avatarbubble.Alignment = DT_RIGHT|DT_TOP;
                avatarbubble.Position.x = GameVariables::ScreenWidth - (avatar.Position.value.x + avatar.Image->Size.x * 1.05f);
                avatarbubble.Position.y = avatar.Position.value.y - avatar.Image->Size.y * 0.3f;
            }
        
			if( GameVariables::EnableShaders )
				DrawWithShadersInverted(syncalpha);
			else
				DrawWithoutShadersInverted(syncalpha);
        }
        else
        {
            avatarbubble.Alignment = DT_LEFT|DT_TOP;
            avatarbubble.Position.x = avatar.Position.value.x + avatar.Image->Size.x * 1.05f;
            avatarbubble.Position.y = avatar.Position.value.y - avatar.Image->Size.y * 0.3f;

            DrawWithoutShaders(syncalpha);
        }
    }
    else
    {
        avatarbubble.Alignment = DT_LEFT|DT_TOP;
        avatarbubble.Position.x = avatar.Position.value.x + avatar.Image->Size.x * 1.05f;
        avatarbubble.Position.y = avatar.Position.value.y - avatar.Image->Size.y * 0.3f;

        DrawWithoutShaders(syncalpha);
    }

    CForm::Draw();
    spritefont->Draw(GroupID);

	// animáció csíkok
    if( state == TransitionIn || state == TransitionOut ||
        state == Inactive || state == TransitionHalf )
    {
        if( state != Inactive )
            alpha.smooth((float)game->Sync.Alpha());

        D3DXCOLOR color(1, 1, 1, 1.0f - alpha.value);
        D3DXMATRIX transform(GameVariables::ScreenWidth, 0, 0, 0, 0, GameVariables::ScreenHeight, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
        
        game->StateManager->SetTexture(0, atlastex1);
        game->DrawImage(transform, hud.Panel->Texcoords, (DWORD)color);
    }
}
//=============================================================================================================
void Game::AddEnemy(long type, long height, long width, long track, long speed)
{
    if( height > 3 || height < 1 )
        return;

    DummyFramework::CSprite9*  sprite;
    D3DXVECTOR2  position;
    Enemy*       enemy;
    float        h = (float)height;
	float        d = GameVariables::PlayFieldHeight;
	float        start = (GameVariables::ScreenHeight - d) * 0.5f;
    float        tw;
    long         cnt;

    // elöször meg kell tudni, hogy melyik grafika lesz
    switch( type )
    {
    case 0:
        sprite = &atlas1["regular"];
        break;

    case 1:
        sprite = &atlas1["worm"];
        break;

    case 2:
        sprite = &atlas1["bomb"];
        break;

	case 3:
        sprite = &atlas1["island"];
        break;

		/*
	case 4:
        sprite = &atlas1["elite"];
        break;
		*/

	case 5:
        sprite = &atlas1["razor"];
        break;

    default:
        break;
    }

    // aztán kiszámolni a helyzetét
    float yspacing = (d - 3.0f * sprite->Size.y) / 4.0f;
    float xspacing = 20;

    if( track == 0 )
    {
        position.y = start + yspacing + sprite->Size.y * 0.5f;
        yspacing = (4.0f - h) * yspacing + (3.0f - h) * sprite->Size.y;
    }
    else if( track == 1 )
    {
        float center = start + d * 0.5f;
        position.y = center - (h - 1) * 0.5f * (sprite->Size.y + yspacing);
    }
    else if( track == 2 )
    {
        position.y = GameVariables::ScreenHeight - start - yspacing - sprite->Size.y * 0.5f;
        yspacing = -((5 - h) * sprite->Size.y + (4 - h) * yspacing);
    }

    // és létrehozni a megfelelö származtatott objektumot
    switch( type )
    {
    case 0:
        for( long i = 0; i < height; ++i )
        {
            position.x = GameVariables::ScreenWidth + sprite->Size.x;

            for( long j = 0; j < width; ++j )
            {
                enemy = new Enemy();

                enemy->shoot.connect(this, &Game::enemy_shoot1);
                enemy->explode.connect(this, &Game::enemy_explode);
                enemy->dead.connect(this, &Game::enemy_dead);

                enemy->Initialize(*game);
                enemy->Image = sprite;
                enemy->Hurt = &atlas1["hurt"];
                enemy->Speed = GameVariables::CorrelateW((float)speed);
                enemy->SetPosition(position);

                enemies.insert(enemy);
                position.x += (sprite->Size.x + xspacing);
            }

            position.y += (sprite->Size.y + yspacing);
        }
        break;

    case 1:
		position.x = GameVariables::ScreenWidth + sprite->Size.x;

		if( height > 1 )
		{
			// egyik
			enemy = new Worm(width);
			((Worm*)enemy)->Direction = 1.0f;

			enemy->explode.connect(this, &Game::enemy_explode);
			enemy->dead.connect(this, &Game::enemy_dead);

			enemy->Initialize(*game);
			enemy->Image = sprite;
			enemy->Hurt = &atlas1["hurt"];
			enemy->Speed = GameVariables::CorrelateH((float)speed);
			enemy->SetPosition(position);

			enemies.insert(enemy);

			// másik
			enemy = new Worm(width);
			((Worm*)enemy)->Direction = -1.0f;

			enemy->explode.connect(this, &Game::enemy_explode);
			enemy->dead.connect(this, &Game::enemy_dead);

			enemy->Initialize(*game);
			enemy->Image = sprite;
			enemy->Hurt = &atlas1["hurt"];
			enemy->Speed = GameVariables::CorrelateH((float)speed);
			enemy->SetPosition(position);

			enemies.insert(enemy);
		}
		else
		{
			enemy = new Worm(width);
			((Worm*)enemy)->Direction = ((track == 0) ? 1.0f : -1.0f);

			enemy->explode.connect(this, &Game::enemy_explode);
			enemy->dead.connect(this, &Game::enemy_dead);

			enemy->Initialize(*game);
			enemy->Image = sprite;
			enemy->Hurt = &atlas1["hurt"];
			enemy->Speed = GameVariables::CorrelateH((float)speed);
			enemy->SetPosition(position);

			enemies.insert(enemy);
		}

        break;

    case 2:
        tw = width * sprite->Size.x + (width - 1) * xspacing;

        for( long i = 0; i < height; ++i )
        {
            if( track == 0 )
                cnt = (width - (i % 2));
            else if( track == 1 )
                cnt = (height == 3 ? (width - 1 + (i % 2)) : width);
            else if( track == 2 )
                cnt = ((i + height) % 2 ? width : width - 1);

            position.x = GameVariables::ScreenWidth + sprite->Size.x
                + (tw - (cnt * sprite->Size.x + (cnt - 1) * xspacing)) * 0.5f;

            for( long j = 0; j < cnt; ++j )
            {
                enemy = new Bomb();
                ((Bomb*)enemy)->Highlight = &atlas1["highlight"];

                enemy->explode.connect(this, &Game::enemy_explode);
                enemy->dead.connect(this, &Game::enemy_dead);

                enemy->Initialize(*game);
                enemy->Image = sprite;
                enemy->Hurt = &atlas1["hurt"];
                enemy->Speed = GameVariables::CorrelateW((float)speed);
                enemy->SetPosition(position);

                enemies.insert(enemy);
                position.x += (sprite->Size.x + xspacing);
            }

            position.y += (sprite->Size.y + yspacing);
        }
        break;

	case 3:
		for( long i = 0; i < height; ++i )
        {
            position.x = GameVariables::ScreenWidth + sprite->Size.x;

            for( long j = 0; j < width; ++j )
            {
                enemy = new Island();
                
                enemy->Initialize(*game);
                enemy->Image = sprite;
                enemy->Speed = GameVariables::CorrelateW((float)speed);
                enemy->SetPosition(position);

                enemies.insert(enemy);
                position.x += (sprite->Size.x + xspacing);
            }

            position.y += (sprite->Size.y + yspacing);
        }
		break;

	case 5:
		position.x = GameVariables::ScreenWidth + sprite->Size.x;
        position.y = start + yspacing + sprite->Size.y * 0.5f;

        enemy = new Razor();

        enemy->shoot.connect(this, &Game::enemy_shoot2);
        enemy->explode.connect(this, &Game::enemy_explode);
        enemy->dead.connect(this, &Game::enemy_dead);

        enemy->Initialize(*game);
        enemy->Image = sprite;
        enemy->Hurt = &atlas1["hurt"];
        enemy->Speed = GameVariables::CorrelateW((float)speed);
        enemy->SetPosition(position);

        enemies.insert(enemy);

		// 2 razornál több már szemétség lenne
		if( width + height > 2 )
		{
			// picit messzebröl induljon, hogy láthato legyen hogy az kettö...
			position.x = GameVariables::ScreenWidth + sprite->Size.x * 3;
			position.y = GameVariables::ScreenHeight - start - yspacing - sprite->Size.y * 0.5f;

			enemy = new Razor();

			enemy->shoot.connect(this, &Game::enemy_shoot2);
			enemy->explode.connect(this, &Game::enemy_explode);
			enemy->dead.connect(this, &Game::enemy_dead);

			enemy->Initialize(*game);
			enemy->Image = sprite;
			enemy->Hurt = &atlas1["hurt"];
			enemy->Speed = GameVariables::CorrelateW((float)speed);
			enemy->SetPosition(position);

			enemies.insert(enemy);
		}

		break;

    default:
        break;
    }
}
//=============================================================================================================
void Game::AddPowerup(const D3DXVECTOR2 pos)
{
    if( !GameVariables::EnablePowerups )
        return;

    int prob = rand() % 200;
    
    // 10% esély h lesz egyáltalán powerup
    if( prob >= 180 )
    {
        Powerup power;

        power.Initialize(*game);
        power.Image = &atlas1["powerup"];
        power.SetPosition(pos);
        power.Speed = background.Speed * 0.5f;

        powerups.push_back(power);

        if( prob >= 180 && prob < 188 )
            powerups.rbegin()->apply.connect(this, &Game::rapidfire_apply);
        else if( prob >= 188 && prob < 196 )
            powerups.rbegin()->apply.connect(this, &Game::twincannons_apply);
        else if( prob >= 196 && prob < 198 )
            powerups.rbegin()->apply.connect(this, &Game::extrahealth_apply);
        else if( prob >= 198 && prob < 200 )
            powerups.rbegin()->apply.connect(this, &Game::extrashield_apply);
        else
            std::cout << "Impossible powerup...\n";
    }
}
//=============================================================================================================
void Game::Clear()
{
    for( enemyset::iterator it = enemies.begin(); it != enemies.end(); ++it )
        delete (*it);
    
    powerups.clear();
    explosions.clear();
    enemies.clear();
    avatarprojectiles.clear();
    enemyprojectiles.clear();
}
//=============================================================================================================
void Game::DrawWithoutShaders(float syncalpha)
{
    size_t written = 0;
    size_t count = 0;

    background.Draw();

    quads.Lock();
    {
        for( enemyset::iterator it = enemies.begin(); it != enemies.end(); ++it )
        {
            written = (*it)->Write(count, quads);
            count += written;
        }

		written = avatar.Write(count, quads);
        count += written;

        for( projectilelist::iterator it = avatarprojectiles.begin(); it != avatarprojectiles.end(); ++it )
        {
            it->Position.smooth(syncalpha);
            quads.WriteQuad(count, it->Position.value, it->Image->Size, it->Image->Texcoords);
            ++count;
        }

        for( projectilelist::iterator it = enemyprojectiles.begin(); it != enemyprojectiles.end(); ++it )
        {
            it->Position.smooth(syncalpha);
            DummyFramework::CHelper::MirrorX(texcoords, it->Image->Texcoords);
            quads.WriteQuad(count, it->Position.value, it->Image->Size, texcoords);
            ++count;
        }

        for( poweruplist::iterator it = powerups.begin(); it != powerups.end(); ++it )
        {
            written = it->Write(count, quads);
            count += written;
        }

        for( explosionlist::iterator it = explosions.begin(); it != explosions.end(); ++it )
        {
            written = it->Write(count, quads);
            count += written;
        }

        written = avatarbubble.Write(count, quads);
        count += written;

        written = hud.Write(count, quads);
        count += written;
    }
    quads.Unlock();
    
	if( count > MAX_SPRITES )
	{
		count = MAX_SPRITES;
		std::cout << "!!!Attempt to buffer overflow!!!\n";
	}

    game->StateManager->SetTexture(0, atlastex1);
    quads.Draw(count);

#ifdef DEBUG_ENTITIES
	std::cout << "quads: " << count << ", e: " << enemies.size() << ", p: " << powerups.size() << ", x: " << explosions.size() << ", a: " <<
		avatarprojectiles.size() << ", ep: " << enemyprojectiles.size() << "               \r";
#endif
}
//=============================================================================================================
void Game::DrawWithShadersInverted(float syncalpha)
{
    size_t written = 0;
    size_t count = 0;

    // texturába rajzoljuk a jelenetet
    oldsurface = NULL;

    game->Graphics->GetRenderTarget(0, &oldsurface);
    game->Graphics->SetRenderTarget(0, rendersurface);
    game->Graphics->Clear(0, NULL, D3DCLEAR_TARGET, 0xff6694ed, 1.0f, 0);

    background.Draw();

    quads.Lock();
    {
        for( enemyset::iterator it = enemies.begin(); it != enemies.end(); ++it )
        {
            written = (*it)->Write(count, quads);
            count += written;
        }

		written = avatar.Write(count, quads);
        count += written;

        for( projectilelist::iterator it = avatarprojectiles.begin(); it != avatarprojectiles.end(); ++it )
        {
            it->Position.smooth(syncalpha);
            quads.WriteQuad(count, it->Position.value, it->Image->Size, it->Image->Texcoords);
            ++count;
        }

        for( projectilelist::iterator it = enemyprojectiles.begin(); it != enemyprojectiles.end(); ++it )
        {
            it->Position.smooth(syncalpha);
            DummyFramework::CHelper::MirrorX(texcoords, it->Image->Texcoords);
            quads.WriteQuad(count, it->Position.value, it->Image->Size, texcoords);
            ++count;
        }

        for( poweruplist::iterator it = powerups.begin(); it != powerups.end(); ++it )
        {
            written = it->Write(count, quads);
            count += written;
        }

        for( explosionlist::iterator it = explosions.begin(); it != explosions.end(); ++it )
        {
            written = it->Write(count, quads);
            count += written;
        }
    }
    quads.Unlock();
    
	if( count > MAX_SPRITES )
	{
		count = MAX_SPRITES;
		std::cout << "!!!Attempt to buffer overflow!!!\n";
	}

    game->StateManager->SetTexture(0, atlastex1);
    quads.Draw(count);

    // a texturát kirajzoljuk hullámzo effekttel
    game->Graphics->SetRenderTarget(0, oldsurface);
    game->StateManager->SetTexture(1, wave);
    game->StateManager->SetTexture(2, rendertarget);
    oldsurface->Release();

    idtransform._11 = GameVariables::ScreenWidth;
    idtransform._22 = GameVariables::ScreenHeight;

	idtransform._41 = -0.5f;
	idtransform._42 = -0.5f;
    
    if( invert == 3 )
        DummyFramework::CHelper::MirrorXY(texcoords, basictexcoords);
    else if( invert == 2 )
        DummyFramework::CHelper::MirrorY(texcoords, basictexcoords);
    else
        DummyFramework::CHelper::MirrorX(texcoords, basictexcoords);

    D3DXVECTOR4 screensize(GameVariables::ScreenWidth, GameVariables::ScreenHeight, 0, 1);

    distort->SetTechnique("distort");
    distort->SetFloat("time", (float)GameVariables::Sync.Timer().Time());
    distort->SetVector("screensize", &screensize);
	
	distort->Begin(NULL, _DUMMY_EFFECT_DONT_SAVE_);
	distort->BeginPass(0);
    {
        game->DrawImage(idtransform, texcoords, 0xffffffff);
    }
	distort->EndPass();
	distort->End();

	game->StateManager->SetVertexShader(NULL);
	game->StateManager->SetPixelShader(NULL);
	game->StateManager->SetTexture(2, NULL);

    // a hudot mindennek a tetejére
#ifdef DEBUG_ENTITIES
    size_t tmp = count;
#endif

    count = 0;

    quads.Lock();
    {
        written = hud.Write(count, quads);
        count += written;

        written = avatarbubble.Write(count, quads);
        count += written;
    }
    quads.Unlock();

	if( count > MAX_SPRITES )
	{
		count = MAX_SPRITES;
		std::cout << "!!!Attempt to buffer overflow!!!\n";
	}

    quads.Draw(count);

#ifdef DEBUG_ENTITIES
	std::cout << "quads: " << (tmp + count) << ", e: " << enemies.size() << ", p: " << powerups.size() << ", x: " << explosions.size() << ", a: " <<
		avatarprojectiles.size() << ", ep: " << enemyprojectiles.size() << "               \r";
#endif
}
//=============================================================================================================
void Game::DrawWithoutShadersInverted(float syncalpha)
{
	size_t written = 0;
    size_t count = 0;

    // texturába rajzoljuk a jelenetet
    oldsurface = NULL;

    game->Graphics->GetRenderTarget(0, &oldsurface);
    game->Graphics->SetRenderTarget(0, rendersurface);
    game->Graphics->Clear(0, NULL, D3DCLEAR_TARGET, 0xff6694ed, 1.0f, 0);

    background.Draw();

    quads.Lock();
    {
        for( enemyset::iterator it = enemies.begin(); it != enemies.end(); ++it )
        {
            written = (*it)->Write(count, quads);
            count += written;
        }

		written = avatar.Write(count, quads);
        count += written;

        for( projectilelist::iterator it = avatarprojectiles.begin(); it != avatarprojectiles.end(); ++it )
        {
            it->Position.smooth(syncalpha);
            quads.WriteQuad(count, it->Position.value, it->Image->Size, it->Image->Texcoords);
            ++count;
        }

        for( projectilelist::iterator it = enemyprojectiles.begin(); it != enemyprojectiles.end(); ++it )
        {
            it->Position.smooth(syncalpha);
            DummyFramework::CHelper::MirrorX(texcoords, it->Image->Texcoords);
            quads.WriteQuad(count, it->Position.value, it->Image->Size, texcoords);
            ++count;
        }

        for( poweruplist::iterator it = powerups.begin(); it != powerups.end(); ++it )
        {
            written = it->Write(count, quads);
            count += written;
        }

        for( explosionlist::iterator it = explosions.begin(); it != explosions.end(); ++it )
        {
            written = it->Write(count, quads);
            count += written;
        }
    }
    quads.Unlock();
    
	if( count > MAX_SPRITES )
	{
		count = MAX_SPRITES;
		std::cout << "!!!Attempt to buffer overflow!!!\n";
	}

    game->StateManager->SetTexture(0, atlastex1);
    quads.Draw(count);

    game->Graphics->SetRenderTarget(0, oldsurface);
    game->StateManager->SetTexture(0, rendertarget);
    oldsurface->Release();

    idtransform._11 = GameVariables::ScreenWidth;
    idtransform._22 = GameVariables::ScreenHeight;

	idtransform._41 = -0.5f;
	idtransform._42 = -0.5f;
    
    if( invert == 3 )
        DummyFramework::CHelper::MirrorXY(texcoords, basictexcoords);
    else if( invert == 2 )
        DummyFramework::CHelper::MirrorY(texcoords, basictexcoords);
    else
        DummyFramework::CHelper::MirrorX(texcoords, basictexcoords);

    game->StateManager->SetVertexShader(NULL);
	game->StateManager->SetPixelShader(NULL);
    game->DrawImage(idtransform, texcoords, 0xffffffff);

    // a hudot mindennek a tetejére
#ifdef DEBUG_ENTITIES
    size_t tmp = count;
#endif

    count = 0;

    quads.Lock();
    {
        written = hud.Write(count, quads);
        count += written;

        written = avatarbubble.Write(count, quads);
        count += written;
    }
    quads.Unlock();

	if( count > MAX_SPRITES )
	{
		count = MAX_SPRITES;
		std::cout << "!!!Attempt to buffer overflow!!!\n";
	}

    game->StateManager->SetTexture(0, atlastex1);
    quads.Draw(count);

#ifdef DEBUG_ENTITIES
	std::cout << "quads: " << (tmp + count) << ", e: " << enemies.size() << ", p: " << powerups.size() << ", x: " << explosions.size() << ", a: " <<
		avatarprojectiles.size() << ", ep: " << enemyprojectiles.size() << "               \r";
#endif
}
//=============================================================================================================
void Game::UnapplyPowerup()
{
    avatar.WeaponCooldown = DEFAULT_SHOOT;
    avatar.Cannons = DEFAULT_CANNONS;
}
//=============================================================================================================
void Game::UpdateCollisions(float syncalpha)
{
	D3DXVECTOR2 hitpos;
    bool collide;

    // ellenfelek vs. avatár és lövedékei
    {
        enemyset::iterator it = enemies.begin();
        enemyset::iterator del;

        while( it != enemies.end() )
        {
            avatar.Position.smooth(syncalpha);
			avatar.Image->Position = avatar.Position.value;

			// itt ezt kell elöbb nézni pl. a bomba miatt
            if( (*it)->Collide(*avatar.Image) )
            {
                if( !(avatar.GetState() & Avatar::Invincible) && !(avatar.GetState() & Avatar::Exploded) )
                {
                    del = it;
                    ++it;

					// ilyenkor nem ér pontot
					(*del)->Value = 0;
                    (*del)->Hit((*del)->Health);

                    avatar.SetState(Avatar::Exploded);
                }
                else
                    ++it;
            }
            else
            {
                projectilelist::iterator pit = avatarprojectiles.begin();
                projectilelist::iterator pdel;
                collide = false;

                // több lövedék is eltalálhatja
                while( pit != avatarprojectiles.end() )
                {
                    pit->Position.smooth(syncalpha);
					pit->Image->Position = pit->Position.value;

                    if( (*it)->Collide(*pit->Image) )
                    {
                        collide = true;
                        pdel = pit;

                        ++pit;
                        avatarprojectiles.erase(pdel);
                    }
                    else
                        ++pit;
                }

                if( collide )
                {
                    del = it;
                    ++it;

                    (*del)->Hit(1); // avatardamage
                }
                else
                    ++it;
            }
        }
    }
    
    // avatár vs. fejlesztések
    {
        poweruplist::iterator it = powerups.begin();
        poweruplist::iterator del;

        while( it != powerups.end() )
        {
            if( !(avatar.GetState() & Avatar::Exploded) )
            {
                it->GetPositionWithOffset(it->Image->Position);

                avatar.Position.smooth(syncalpha);
				avatar.Image->Position = avatar.Position.value;

                if( DummyFramework::CSprite9::Collide(hitpos, *it->Image, *avatar.Image) )
                {
                    it->apply();
                    avatar.SetState(Avatar::Empowered);
                    
                    del = it;
                    ++it;
                    powerups.erase(del);

                    GameVariables::PowerupsAcquired++;
                }
                else
                    ++it;
            }
            else
                ++it;
        }
    }

    // avatár vs. ellenfél lövedékei
    {
        projectilelist::iterator it = enemyprojectiles.begin();
        projectilelist::iterator del;
        collide = false;

        while( it != enemyprojectiles.end() )
        {
            it->Position.smooth(syncalpha);
			it->Image->Position = it->Position.value;

            if( !(avatar.GetState() & Avatar::Invincible) && !(avatar.GetState() & Avatar::Exploded) )
            {
				avatar.Image->Position = avatar.Position.value;

                if( DummyFramework::CSprite9::Collide(hitpos, *avatar.Image, *it->Image) )
                {
                    collide = true;
                    del = it;

                    ++it;
                    enemyprojectiles.erase(del);
                }
                else
                    ++it;
            }
            else
                ++it;
        }

        if( collide )
        {
            avatar.Hit(hitpos);
        }
    }
}
//=============================================================================================================
