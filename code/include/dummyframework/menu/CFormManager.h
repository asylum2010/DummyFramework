//=============================================================================================================
#ifndef _CFORMMANAGER_H_
#define _CFORMMANAGER_H_

#include "../template/signal.hpp"

#include <map>
#include <vector>

namespace DummyFramework
{
    class CForm;
    class CControl;
    class CGame9;
    class CSpriteFont9;

    struct skeyboardstate;
    struct smousestate;

    /**
     * \brief Manages forms
     */
    class CFormManager : public has_slots
    {
    protected:
        typedef std::vector<CForm*> formlist;
        typedef std::map<CForm*, formlist> formmap;
        
        CGame9*        game;
        CSpriteFont9*  spritefont;
        CForm*         current;
        formmap        forms;
        size_t         ticks;
        bool           initialized;
        bool           contentloaded;

    public:
        size_t InitialDelay;

        CFormManager();
        virtual ~CFormManager();

        bool AddForm(CForm& form);
        bool Initialize(CGame9& mygame, CSpriteFont9& font);
        bool LoadContent();

        void Connect(CForm& from, size_t action, CForm& to);
		void Disconnect(CForm& from, size_t action, CForm& to);
        void Draw();
		void RemoveForm(CForm& form);
        void Update();

        inline void SetCurrent(CForm& form) {
            current = &form;
        }

    eventhandlers:
        void onfocuslost(CControl& sender);
        void onkeyup(const skeyboardstate& kstate);
        void onkeydown(const skeyboardstate& kstate);
        void onlostdevice();
        void onresetdevice();
    };
}

#endif
//=============================================================================================================
 
