//=============================================================================================================
#ifndef _CEFFECTRENDERER9_H_
#define _CEFFECTRENDERER9_H_

#include "../../template/signal.hpp"
#include <d3d9.h>
#include <string>

namespace DummyFramework
{
	class CGame9;
	class CStateManager9;

	/**
	 * \brief Base class for effect renderers
	 */
	class CEffectRenderer9
	{
	protected:
		bool canbeapplied;
		CGame9* game;
		CStateManager9* manager;

	public:
		bool Enabled;
		std::string MediaDir;

		CEffectRenderer9();
		virtual ~CEffectRenderer9();

		virtual bool Initialize(CGame9& mygame);
		virtual bool ResetRenderStates();
		virtual bool LoadContent();

		virtual void UnloadContent();
		virtual void Update() {}
		virtual void Draw() {}

		inline bool CanBeApplied() const {
			return (Enabled && canbeapplied);
		}

	_DUMMY_EVENTHANDLERS:
		virtual void onlostdevice();
		virtual void onresetdevice();
	};
}

#endif
//=============================================================================================================
