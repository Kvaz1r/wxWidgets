/////////////////////////////////////////////////////////////////////////////
// Name:        include/wx/msw/taskbarbutton.h
// Purpose:     Defines wxTaskBarButtonImpl class.
// Author:      Chaobin Zhang <zhchbin@gmail.com>
// Created:     2014-06-01
// Copyright:   (c) 2014 wxWidgets development team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef  _WX_MSW_TASKBARBUTTON_H_
#define  _WX_MSW_TASKBARBUTTON_H_

#if wxUSE_TASKBARBUTTON

#include "wx/defs.h"
#include "wx/vector.h"
#include "wx/taskbarbutton.h"

namespace {
class WXDLLIMPEXP_FWD_CORE ITaskbarList3;
class WXDLLIMPEXP_FWD_CORE IObjectArray;
class WXDLLIMPEXP_FWD_CORE ICustomDestinationList;
class WXDLLIMPEXP_FWD_CORE IApplicationDocumentLists;
}

class WXDLLIMPEXP_CORE wxTaskBarButtonImpl : public wxTaskBarButton {
public:
    virtual ~wxTaskBarButtonImpl();

    virtual void SetProgressRange(int range) wxOVERRIDE;
    virtual void SetProgressValue(int value) wxOVERRIDE;
    virtual void PulseProgress() wxOVERRIDE;
    virtual void Show(bool show = true) wxOVERRIDE;
    virtual void Hide() wxOVERRIDE;
    virtual void SetThumbnailTooltip(const wxString& tooltip) wxOVERRIDE;
    virtual void SetProgressState(wxTaskBarButtonState state) wxOVERRIDE;
    virtual void SetOverlayIcon(const wxIcon& icon,
        const wxString& description = wxString()) wxOVERRIDE;
    virtual void SetThumbnailClip(const wxRect& rect) wxOVERRIDE;
    virtual void SetThumbnailContents(const wxWindow *child) wxOVERRIDE;
    virtual bool InsertThumbBarButton(size_t pos,
                                      wxThumbBarButton *button) wxOVERRIDE;
    virtual bool AppendThumbBarButton(wxThumbBarButton *button) wxOVERRIDE;
    virtual bool AppendSeparatorInThumbBar() wxOVERRIDE;
    virtual wxThumbBarButton* RemoveThumbBarButton(
        wxThumbBarButton *button) wxOVERRIDE;
    virtual wxThumbBarButton* RemoveThumbBarButton(int id) wxOVERRIDE;

private:
    friend class wxFrame;
    friend class wxThumbBarButton;
    friend class wxAppProgressIndicator;

    wxTaskBarButtonImpl(WXWidget parent);

    bool InitOrUpdateThumbBarButtons();
    wxThumbBarButton* GetThumbBarButtonByIndex(size_t index);

    WXWidget m_hwnd;
    ITaskbarList3 *m_taskbarList;

    typedef wxVector<wxThumbBarButton*> wxThumbBarButtons;
    wxThumbBarButtons m_thumbBarButtons;

    int m_progressRange;
    bool m_hasInitThumbnailToolbar;
};

class WXDLLIMPEXP_CORE wxJumpListImpl
{
public:
    wxJumpListImpl();
    virtual ~wxJumpListImpl();
    void ShowRecentCategory(bool shown = true);
    void HideRecentCategory();
    void ShowFrequentCategory(bool shown = true);
    void HideFrequentCategory();

    wxJumpListCategory* GetTasks();
    const wxJumpListCategory* GetFrequentCategory();
    const wxJumpListCategory* GetRecentCategory();
    const wxJumpListCategories& GetCustomCategories();

    void AddCategory(wxJumpListCategory* category);
    wxJumpListCategory* RemoveCategory(const wxString& title);
    void DeleteCategory(const wxString& title);
    void Update();

private:
    bool BeginUpdate();
    bool CommitUpdate();
    void AddTasksToDestinationList();
    void AddCustomCategoriesToDestionationList();
    void LoadKnownCategory(const wxString& title);

    ICustomDestinationList    *m_destinationList;
    IObjectArray              *m_objectArray;

    wxScopedPtr<wxJumpListCategory> m_tasks;
    wxScopedPtr<wxJumpListCategory> m_frequent;
    wxScopedPtr<wxJumpListCategory> m_recent;
    wxJumpListCategories m_customCategories;
    bool m_recent_visible;
    bool m_frequent_visible;
};

#endif // wxUSE_TASKBARBUTTON

#endif  // _WX_MSW_TASKBARBUTTON_H_