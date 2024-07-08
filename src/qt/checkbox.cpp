/////////////////////////////////////////////////////////////////////////////
// Name:        src/qt/checkbox.cpp
// Author:      Peter Most, Mariano Reingart
// Copyright:   (c) 2009 wxWidgets dev team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#if wxUSE_CHECKBOX

#include "wx/checkbox.h"
#include "wx/qt/private/utils.h"
#include "wx/qt/private/converter.h"
#include "wx/qt/private/winevent.h"

#include <QtWidgets/QCheckBox>

class wxQtCheckBox : public wxQtEventSignalHandler< QCheckBox, wxCheckBox >
{
public:
    wxQtCheckBox( wxWindow *parent, wxCheckBox *handler );

private:
    void clicked( bool checked );
};

wxQtCheckBox::wxQtCheckBox( wxWindow *parent, wxCheckBox *handler )
    : wxQtEventSignalHandler< QCheckBox, wxCheckBox >( parent, handler )
{
    connect(this, &QCheckBox::clicked, this, &wxQtCheckBox::clicked);
}

void wxQtCheckBox::clicked( bool checked )
{
    wxCheckBox *handler = GetHandler();
    if ( handler )
    {
        wxCommandEvent event( wxEVT_CHECKBOX, handler->GetId() );
        if (!handler->Is3rdStateAllowedForUser() &&
            checkState() == Qt::PartiallyChecked)
            setCheckState(Qt::Checked);

        event.SetInt( checked );
        EmitEvent( event );
    }
}


wxCheckBox::wxCheckBox()
{
}

wxCheckBox::wxCheckBox( wxWindow *parent, wxWindowID id, const wxString& label,
        const wxPoint& pos, const wxSize& size, long style, const wxValidator& validator,
        const wxString& name )
{
    Create( parent, id, label, pos, size, style, validator, name );
}

bool wxCheckBox::Create(wxWindow *parent, wxWindowID id, const wxString& label,
            const wxPoint& pos, const wxSize& size, long style, const wxValidator& validator,
            const wxString& name )
{
    m_qtWindow = new wxQtCheckBox( parent, this );

    GetQCheckBox()->setText( wxQtConvertString( label ) );

    // Do the initialization here as WXValidateStyle may fail in unit tests
    bool ok = wxCheckBoxBase::Create( parent, id, pos, size, style, validator, name );

    WXValidateStyle(&style);

    if ( style & wxCHK_2STATE )
        GetQCheckBox()->setTristate( false );
    else if ( style & wxCHK_3STATE )
        GetQCheckBox()->setTristate( true );
    if ( style & wxALIGN_RIGHT )
        GetQCheckBox()->setLayoutDirection( Qt::RightToLeft );

    return ok;
}

QCheckBox* wxCheckBox::GetQCheckBox() const
{
    return static_cast<QCheckBox*>(m_qtWindow);
}

void wxCheckBox::SetValue(bool value)
{
    GetQCheckBox()->setChecked( value );
}

bool wxCheckBox::GetValue() const
{
    return GetQCheckBox()->isChecked();
}

void wxCheckBox::DoSet3StateValue(wxCheckBoxState state)
{
    switch (state)
    {
    case wxCHK_UNCHECKED:
        GetQCheckBox()->setCheckState(Qt::Unchecked);
        break;

    case wxCHK_CHECKED:
        GetQCheckBox()->setCheckState(Qt::Checked);
        break;

    case wxCHK_UNDETERMINED:
        GetQCheckBox()->setCheckState(Qt::PartiallyChecked);
        break;
    }
}

wxCheckBoxState wxCheckBox::DoGet3StateValue() const
{
    switch (GetQCheckBox()->checkState())
    {
    case Qt::Unchecked:
        return wxCHK_UNCHECKED;

    case Qt::Checked:
        return wxCHK_CHECKED;

    case Qt::PartiallyChecked:
        return wxCHK_UNDETERMINED;
    }

    wxFAIL_MSG( "unknown QCheckBox state" );
    return wxCHK_UNDETERMINED;
}

wxString wxCheckBox::GetLabel() const
{
    return wxQtConvertString( GetQCheckBox()->text() );
}

void wxCheckBox::SetLabel(const wxString& label)
{
    wxCheckBoxBase::SetLabel( label );

    GetQCheckBox()->setText( wxQtConvertString(label) );
}

#endif // wxUSE_CHECKBOX
