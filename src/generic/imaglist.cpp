/////////////////////////////////////////////////////////////////////////////
// Name:        src/generic/imaglist.cpp
// Purpose:
// Author:      Robert Roebling
// Copyright:   (c) 1998 Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_IMAGLIST && !defined(wxHAS_NATIVE_IMAGELIST)

#include "wx/imaglist.h"

#ifndef WX_PRECOMP
    #include "wx/dc.h"
    #include "wx/icon.h"
    #include "wx/image.h"
#endif

//-----------------------------------------------------------------------------
//  wxImageList
//-----------------------------------------------------------------------------

wxIMPLEMENT_DYNAMIC_CLASS(wxGenericImageList, wxObject);
wxIMPLEMENT_DYNAMIC_CLASS(wxImageList, wxGenericImageList);

wxGenericImageList::wxGenericImageList( int width, int height, bool mask, int initialCount )
{
    (void)Create(width, height, mask, initialCount);
}

wxGenericImageList::~wxGenericImageList()
{
    (void)RemoveAll();
}

int wxGenericImageList::GetImageCount() const
{
    return m_images.GetCount();
}

bool wxGenericImageList::Create( int width, int height, bool WXUNUSED(mask), int WXUNUSED(initialCount) )
{
    m_size = wxSize(width, height);

    return Create();
}

bool wxGenericImageList::Create()
{
    return true;
}

int wxGenericImageList::Add( const wxBitmap &bitmap )
{
    wxASSERT_MSG( (bitmap.GetScaledWidth() >= m_size.x && bitmap.GetScaledHeight() == m_size.y)
                  || m_size == wxSize(0, 0),
                  wxT("invalid bitmap size in wxImageList: this might work ")
                  wxT("on this platform but definitely won't under Windows.") );

    if (bitmap.IsKindOf(wxCLASSINFO(wxIcon)))
    {
        m_images.Append( new wxIcon( (const wxIcon&) bitmap ) );
    }
    else
    {
        // Mimic behaviour of Windows ImageList_Add that automatically breaks up the added
        // bitmap into sub-images of the correct size
        if (m_size.x > 0 && bitmap.GetScaledWidth() > m_size.x && bitmap.GetScaledHeight() >= m_size.y)
        {
            int numImages = bitmap.GetScaledWidth() / m_size.x;
            for (int subIndex = 0; subIndex < numImages; subIndex++)
            {
                wxRect rect(m_size.x * subIndex, 0, m_size.x, m_size.y);
                wxBitmap tmpBmp = bitmap.GetSubBitmap(rect);
                m_images.Append( new wxBitmap(tmpBmp) );
            }
        }
        else
        {
            m_images.Append( new wxBitmap(bitmap) );
        }
    }

    if ( m_size == wxSize(0, 0) )
    {
        m_size = bitmap.GetScaledSize();
    }

    return m_images.GetCount() - 1;
}

int wxGenericImageList::Add( const wxBitmap& bitmap, const wxBitmap& mask )
{
    wxBitmap bmp(bitmap);
    if (mask.IsOk())
        bmp.SetMask(new wxMask(mask));
    return Add(bmp);
}

int wxGenericImageList::Add( const wxBitmap& bitmap, const wxColour& maskColour )
{
    wxImage img = bitmap.ConvertToImage();
    img.SetMaskColour(maskColour.Red(), maskColour.Green(), maskColour.Blue());
    return Add(wxBitmap(img));
}

const wxBitmap *wxGenericImageList::GetBitmapPtr( int index ) const
{
    wxObjectList::compatibility_iterator node = m_images.Item( index );

    wxCHECK_MSG( node, NULL, wxT("wrong index in image list") );

    return (wxBitmap*)node->GetData();
}

// Get the bitmap
wxBitmap wxGenericImageList::GetBitmap(int index) const
{
    const wxBitmap* bmp = GetBitmapPtr(index);
    if (!bmp)
        return wxNullBitmap;

    if ( bmp->IsKindOf(wxCLASSINFO(wxIcon)) )
        return wxBitmap( *(static_cast<const wxIcon*>(bmp)) );
    else
        return *bmp;
}

// Get the icon
wxIcon wxGenericImageList::GetIcon(int index) const
{
    const wxBitmap* bmp = GetBitmapPtr(index);
    if (!bmp)
        return wxNullIcon;

    if ( bmp->IsKindOf(wxCLASSINFO(wxIcon)) )
        return *(static_cast<const wxIcon*>(bmp));
    else
    {
        wxIcon icon;
        icon.CopyFromBitmap(*bmp);
        return icon;
    }
}

bool wxGenericImageList::Replace( int index, const wxBitmap &bitmap )
{
    wxObjectList::compatibility_iterator node = m_images.Item( index );

    wxCHECK_MSG( node, false, wxT("wrong index in image list") );

    wxBitmap* newBitmap = (bitmap.IsKindOf(wxCLASSINFO(wxIcon))) ?
                               new wxBitmap( (const wxIcon&) bitmap )
                               : new wxBitmap(bitmap) ;

    if (index == (int) m_images.GetCount() - 1)
    {
        delete node->GetData();
        m_images.Erase( node );
        m_images.Append( newBitmap );
    }
    else
    {
        wxObjectList::compatibility_iterator next = node->GetNext();
        delete node->GetData();
        m_images.Erase( node );
        m_images.Insert( next, newBitmap );
    }

    return true;
}

bool wxGenericImageList::Replace( int index, const wxBitmap &bitmap, const wxBitmap &mask )
{
    wxObjectList::compatibility_iterator node = m_images.Item( index );

    wxCHECK_MSG( node, false, wxT("wrong index in image list") );

    wxBitmap* newBitmap = (bitmap.IsKindOf(wxCLASSINFO(wxIcon))) ?
                               new wxBitmap( (const wxIcon&) bitmap )
                               : new wxBitmap(bitmap) ;

    if (index == (int) m_images.GetCount() - 1)
    {
        delete node->GetData();
        m_images.Erase( node );
        m_images.Append( newBitmap );
    }
    else
    {
        wxObjectList::compatibility_iterator next = node->GetNext();
        delete node->GetData();
        m_images.Erase( node );
        m_images.Insert( next, newBitmap );
    }

    if (mask.IsOk())
        newBitmap->SetMask(new wxMask(mask));

    return true;
}

bool wxGenericImageList::Remove( int index )
{
    wxObjectList::compatibility_iterator node = m_images.Item( index );

    wxCHECK_MSG( node, false, wxT("wrong index in image list") );

    delete node->GetData();
    m_images.Erase( node );

    return true;
}

bool wxGenericImageList::RemoveAll()
{
    WX_CLEAR_LIST(wxObjectList, m_images);
    m_images.Clear();

    return true;
}

bool wxGenericImageList::GetSize( int index, int &width, int &height ) const
{
    width = 0;
    height = 0;

    wxObjectList::compatibility_iterator node = m_images.Item( index );

    wxCHECK_MSG( node, false, wxT("wrong index in image list") );

    wxBitmap *bm = (wxBitmap*)node->GetData();
    width = bm->GetScaledWidth();
    height = bm->GetScaledHeight();

    return true;
}

bool wxGenericImageList::Draw( int index, wxDC &dc, int x, int y,
                        int flags, bool WXUNUSED(solidBackground) )
{
    wxObjectList::compatibility_iterator node = m_images.Item( index );

    wxCHECK_MSG( node, false, wxT("wrong index in image list") );

    wxBitmap *bm = (wxBitmap*)node->GetData();

    if (bm->IsKindOf(wxCLASSINFO(wxIcon)))
        dc.DrawIcon( * ((wxIcon*) bm), x, y);
    else
        dc.DrawBitmap( *bm, x, y, (flags & wxIMAGELIST_DRAW_TRANSPARENT) > 0 );

    return true;
}

#endif // wxUSE_IMAGLIST
