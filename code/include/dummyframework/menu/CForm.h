//=============================================================================================================
#ifndef _CFORM_H_
#define _CFORM_H_

#include "CControl.h"
#include <vector>

namespace DummyFramework
{
    class CForm : public CControl
    {
        friend class CFormManager;

    protected:
        typedef std::vector<CControl*> controllist;

        CFormManager*  manager;
        controllist    controls;
        size_t         action;

    public:
        CForm();
        ~CForm();

        bool Initialize(CGame9& mygame, CSpriteFont9& font);
        bool LoadContent();

        void AddControl(CControl& ctrl);
        void Draw();
        void Update();

        inline void Reserve(size_t count) {
            controls.reserve(count);
        }

        inline size_t GetAction() const {
            return action;
        }

    eventhandlers:
        void onlostdevice();
        void onresetdevice();
    };
}

#endif
//=============================================================================================================
 
 
