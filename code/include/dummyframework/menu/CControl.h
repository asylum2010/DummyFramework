//=============================================================================================================
#ifndef _CCONTROL_H_
#define _CCONTROL_H_

#include "../template/signal.hpp"

namespace DummyFramework
{
    class CGame9;
    class CSpriteFont9;

    struct skeyboardstate;
    struct smousestate;
    //*************************************************************************************************************
    class CControl : public has_slots
    {
        friend class CForm;
    
    public:
        enum econtrolstate
        {
            Idle,             /*!< végzi a dolgát */
            TransitionIn,     /*!< beúszik */
            TransitionOut,    /*!< kiúszik */
            Hidden,           /*!< akkor sem láthato ha nem övé a fókusz */
            Inactive          /*!< látható, de nem reagál semmire */
        };

        size_t         GroupID;
        unsigned long  Alignment;

        CControl();
		~CControl() {}

        virtual bool Initialize(CGame9& mygame, CSpriteFont9& font);
        virtual bool LoadContent() { return (contentloaded = true); }

        virtual void Draw() {}
        virtual void SetState(unsigned long newstate) { state = newstate; }
        virtual void Update() {}

        inline unsigned long GetState() const {
            return state;
        }

        inline bool HasFocus() const {
            return hasfocus;
        }
        
    protected:
        CGame9*         game;
        CSpriteFont9*   spritefont;
        bool            hasfocus;
        bool            initialized;
        bool            contentloaded;
        unsigned long   state;

    events:
        signal1<CControl&> focuslost;
        signal1<CControl&> focusgained;

    eventhandlers:
        virtual void onfocuslost();
        virtual void onfocusgained();
        virtual void onkeyup(const skeyboardstate& kstate) {}
        virtual void onkeydown(const skeyboardstate& kstate) {}
        virtual void onmouseup(const smousestate& mstate) {}
        virtual void onmousedown(const smousestate& mstate) {}
        virtual void onmousemove(const smousestate& mstate) {}
        virtual void onlostdevice() {}
        virtual void onresetdevice() {}
    };
}

#endif
//=============================================================================================================
 
 
 
 
