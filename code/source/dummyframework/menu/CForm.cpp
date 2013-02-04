//=============================================================================================================
#include <dummyframework/menu/CForm.h>
#include <dummyframework/base/d3d9/CGame9.h>
#include <dummyframework/base/d3d9/CSpriteFont9.h>

namespace DummyFramework
{
	CForm::CForm()
		: CControl()
	{
		manager = NULL;
		action = 0xffffffff;
	}
	//=============================================================================================================
	CForm::~CForm()
	{
		controls.clear();
	}
	//=============================================================================================================
	void CForm::AddControl(CControl& ctrl)
	{
		dnerror(, "CForm::AddControl(): Cannot add self", &ctrl == this);

		for( size_t i = 0; i < controls.size(); ++i )
		{
			if( controls[i] == &ctrl )
				return;
		}

		controls.push_back(&ctrl);

		if( initialized )
		{
			if( ctrl.GroupID == 0xffffffff )
				ctrl.GroupID = GroupID;

			ctrl.Initialize(*game, *spritefont);
		}
	}
	//=============================================================================================================
	void CForm::Draw()
	{
		for( size_t i = 0; i < controls.size(); ++i )
		{
			if( controls[i]->GetState() != CControl::Hidden )
				controls[i]->Draw();
		}

		CControl::Draw();
	}
	//=============================================================================================================
	bool CForm::Initialize(CGame9& mygame, CSpriteFont9& font)
	{
		bool success = CControl::Initialize(mygame, font);

		for( size_t i = 0; i < controls.size(); ++i )
		{
			if( controls[i]->GroupID == 0xffffffff )
				controls[i]->GroupID = GroupID;

			success = (success && controls[i]->Initialize(mygame, font));
		}

		return success;
	}
	//=============================================================================================================
	bool CForm::LoadContent()
	{
		bool success = CControl::LoadContent();

		for( size_t i = 0; i < controls.size(); ++i )
		{
			if( controls[i]->GroupID == 0xffffffff )
				controls[i]->GroupID = GroupID;

			success = (success && controls[i]->LoadContent());
		}

		return true;
	}
	//=============================================================================================================
	void CForm::Update()
	{
		for( size_t i = 0; i < controls.size(); ++i )
		{
			if( controls[i]->GetState() != CControl::Hidden &&
				controls[i]->GetState() != CControl::Inactive )
			{
				controls[i]->Update();
			}
		}

		CControl::Update();
	}
	//=============================================================================================================
	void CForm::onlostdevice()
	{
		if( initialized && contentloaded )
		{
			for( size_t i = 0; i < controls.size(); ++i )
				controls[i]->onlostdevice();
		}
	}
	//=============================================================================================================
	void CForm::onresetdevice()
	{
		if( initialized && contentloaded )
		{
			for( size_t i = 0; i < controls.size(); ++i )
				controls[i]->onresetdevice();
		}
	}
	//=============================================================================================================
}

