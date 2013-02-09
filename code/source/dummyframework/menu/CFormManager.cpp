//=============================================================================================================
#include <dummyframework/menu/CFormManager.h>
#include <dummyframework/menu/CForm.h>
#include <dummyframework/base/d3d9/CSpriteFont9.h>
#include <dummyframework/base/d3d9/CGame9.h>
#include <dummyframework/template/defines.h>

namespace DummyFramework
{
	CFormManager::CFormManager()
	{
		ticks			= 0;
		current			= NULL;
		game			= NULL;
		spritefont		= NULL;
		initialized		= false;
		contentloaded	= false;
		InitialDelay	= 10;
	}
	//=============================================================================================================
	CFormManager::~CFormManager()
	{
	}
	//=============================================================================================================
	bool CFormManager::AddForm(CForm& form)
	{
		bool success = true;
		formmap::iterator it = forms.find(&form);

		if( it == forms.end() )
		{
			form.manager = this;
			form.focuslost.connect(this, &CFormManager::onfocuslost);
			forms[&form].resize(10, NULL);

			if( initialized )
				success = form.Initialize(*game, *spritefont);

			if( success && contentloaded )
				success = form.LoadContent();
		}

		return success;
	}
	//=============================================================================================================
	bool CFormManager::Initialize(CGame9& mygame, CSpriteFont9& font)
	{
		game = &mygame;
		spritefont = &font;

		font.Initialize(mygame);
		bool success = true;

		for( formmap::iterator it = forms.begin(); it != forms.end(); ++it )
			success = (success && it->first->Initialize(mygame, font));

		mygame.lost.connect(this, &CFormManager::onlostdevice);
		mygame.reset.connect(this, &CFormManager::onresetdevice);

		return (initialized = success);
	}
	//=============================================================================================================
	bool CFormManager::LoadContent()
	{
		bool success = true;

		for( formmap::iterator it = forms.begin(); it != forms.end(); ++it )
			success = (success && it->first->LoadContent());

		return (contentloaded = success);
	}
	//=============================================================================================================
	void CFormManager::Connect(CForm& from, size_t action, CForm& to)
	{
		CForm* tmp = current;

		formmap::iterator it1 = forms.find(&from);
		dnerror(, "CFormManager::Connect(): Source form invalid", it1 == forms.end());

		formmap::iterator it2 = forms.find(&to);
		dnerror(, "CFormManager::Connect(): Destination form invalid", it2 == forms.end());

		if( it1->second.size() <= action )
			it1->second.resize(it1->second.size() + 10, NULL);

		it1->second[action] = &to;

		if( current != tmp )
			std::cout << "CFormManager::Connect(): You fucked up something\n";
	}
	//=============================================================================================================
	void CFormManager::Disconnect(CForm& from, size_t action, CForm& to)
	{
		formmap::iterator it = forms.find(&from);
		dnerror(, "CFormManager::Connect(): Source form invalid", it == forms.end());

		if( it->second.size() > action )
			it->second[action] = NULL;
	}
	//=============================================================================================================
	void CFormManager::RemoveForm(CForm& form)
	{
		for( formmap::iterator it = forms.begin(); it != forms.end(); ++it )
		{
			formlist& fl = it->second;

			for( size_t i = 0; i < fl.size(); ++i )
			{
				if( fl[i] == &form )
					fl[i] = NULL;
			}
		}

		forms.erase(&form);
	}
	//=============================================================================================================
	void CFormManager::Update()
	{
		if( ticks >= InitialDelay )
		{
			formmap::iterator next;
			formmap::iterator it = forms.begin();

			while( it != forms.end() )
			{
				if( it->first->GetState() != CControl::Hidden &&
					it->first->GetState() != CControl::Inactive )
				{
					next = it;
					++next;

					it->first->Update();
					it = next;
				}
				else
					++it;
			}
		}
		else
			++ticks;
	}
	//=============================================================================================================
	void CFormManager::Draw()
	{
		for( formmap::iterator it = forms.begin(); it != forms.end(); ++it )
		{
			if( current != it->first && it->first->GetState() != CControl::Hidden )
				it->first->Draw();
		}

		// akié a fókusz azt rajzoljuk legfelülre, igy nem kell zbuffer
		if( current )
			current->Draw();
	}
	//=============================================================================================================
	void CFormManager::onfocuslost(CControl& sender)
	{
		CForm* form = dynamic_cast<CForm*>(&sender);
		derror(, "CFormManager::onfocuslost(): Sender is not a form", form);

		size_t action = form->GetAction();
		formmap::iterator it = forms.find(form);

		dnerror(, "CFormManager::onfocuslost(): Invalid form", it == forms.end());
		dnerror(, "CFormManager::onfocuslost(): Invalid action", it->second.size() <= action);

		CForm* next = it->second[action];
		derror(, "CFormManager::onfocuslost(): Invalid succeeder", next);

		if( current )
			current->onfocuslost();

		current = next;
		current->onfocusgained();
	}
	//=============================================================================================================
	void CFormManager::onkeyup(const DummyFramework::skeyboardstate& kstate)
	{
		if( current )
		{
			if( current->GetState() != CControl::Hidden && current->GetState() != CControl::Inactive )
				current->onkeyup(kstate);
		}
	}
	//=============================================================================================================
	void CFormManager::onkeydown(const DummyFramework::skeyboardstate& kstate)
	{
		if( current )
		{
			if( current->GetState() != CControl::Hidden && current->GetState() != CControl::Inactive )
				current->onkeydown(kstate);
		}
	}
	//=============================================================================================================
	void CFormManager::onmouseup(const smousestate& mstate)
	{
		if( current )
		{
			if( current->GetState() != CControl::Hidden && current->GetState() != CControl::Inactive )
				current->onmouseup(mstate);
		}
	}
	//=============================================================================================================
	void CFormManager::onmousedown(const smousestate& mstate)
	{
		if( current )
		{
			if( current->GetState() != CControl::Hidden && current->GetState() != CControl::Inactive )
				current->onmousedown(mstate);
		}
	}
	//=============================================================================================================
	void CFormManager::onmousemove(const smousestate& mstate)
	{
		if( current )
		{
			if( current->GetState() != CControl::Hidden && current->GetState() != CControl::Inactive )
				current->onmousemove(mstate);
		}
	}
	//=============================================================================================================
	void CFormManager::onlostdevice()
	{
		if( initialized && contentloaded )
		{
			for( formmap::iterator it = forms.begin(); it != forms.end(); ++it )
			{
				it->first->onlostdevice();
			}
		}
	}
	//=============================================================================================================
	void CFormManager::onresetdevice()
	{
		if( initialized && contentloaded )
		{
			for( formmap::iterator it = forms.begin(); it != forms.end(); ++it )
			{
				it->first->onresetdevice();
			}
		}
	}
	//=============================================================================================================
}
