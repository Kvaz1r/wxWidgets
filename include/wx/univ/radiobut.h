///////////////////////////////////////////////////////////////////////////////
// Name:        wx/univ/radiobut.h
// Purpose:     wxRadioButton declaration
// Author:      Vadim Zeitlin
// Modified by:
// Created:     10.09.00
// RCS-ID:      $Id$
// Copyright:   (c) 2000 SciTech Software, Inc. (www.scitechsoft.com)
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_UNIV_RADIOBUT_H_
#define _WX_UNIV_RADIOBUT_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma interface "univradiobut.h"
#endif

#include "wx/checkbox.h"

// ----------------------------------------------------------------------------
// wxRadioButton
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxRadioButton : public wxCheckBox
{
public:
    // constructors
    wxRadioButton();

    wxRadioButton(wxWindow *parent,
                  wxWindowID id,
                  const wxString& label,
                  const wxPoint& pos = wxDefaultPosition,
                  const wxSize& size = wxDefaultSize,
                  long style = 0,
                  const wxValidator& validator = wxDefaultValidator,
                  const wxString& name = wxRadioButtonNameStr);

    bool Create(wxWindow *parent,
                wxWindowID id,
                const wxString& label,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = 0,
                const wxValidator& validator = wxDefaultValidator,
                const wxString& name = wxRadioButtonNameStr);

    // override some base class methods
    virtual void ChangeValue(bool value);

protected:
    // implement our own drawing
    virtual void DoDraw(wxControlRenderer *renderer);

    // we use the radio button bitmaps for size calculation
    virtual wxSize GetBitmapSize() const;

    // the radio button can only be cleared using this method, not
    // ChangeValue() above - and it is protected as it can only be called by
    // another radiobutton
    void ClearValue();

    // called when the radio button becomes checked: we clear all the buttons
    // in the same group with us here
    virtual void OnCheck();

    // send event about radio button selection
    virtual void SendEvent();

private:
    DECLARE_DYNAMIC_CLASS(wxRadioButton)
};

#endif // _WX_UNIV_RADIOBUT_H_
