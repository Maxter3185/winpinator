#include "history_group_header.hpp"

#include "utils.hpp"

namespace gui
{

HistoryGroupHeader::HistoryGroupHeader( wxWindow* parent,
    const wxString& label )
    : wxPanel( parent, wxID_ANY )
    , m_label( nullptr )
{
    wxBoxSizer* sizer = new wxBoxSizer( wxVERTICAL );

    m_label = new wxStaticText( this, wxID_ANY, label );
    m_label->SetFont( Utils::get()->getHeaderFont() );
    m_label->SetForegroundColour( Utils::get()->getHeaderColor() );
    sizer->Add( m_label, 0, wxLEFT, FromDIP( 8 ) );

    m_pen.SetColour( Utils::get()->getHeaderColor() );
    m_pen.SetWidth( FromDIP( 1 ) );

    SetSizer( sizer );

    Fit();

    // Events

    Bind( wxEVT_PAINT, &HistoryGroupHeader::onPaint, this );
    Bind( wxEVT_SIZE, &HistoryGroupHeader::onSize, this );
}

void HistoryGroupHeader::onPaint( wxPaintEvent& event )
{
    wxPaintDC dc( this );
    const int MARGIN = FromDIP( 8 );

    int lineY = dc.GetSize().y / 2;
    int lineX1 = m_label->GetSize().x + 2 * MARGIN;
    int lineX2 = dc.GetSize().x - MARGIN;

    dc.SetPen( m_pen );
    dc.DrawLine( lineX1, lineY, lineX2, lineY );

    event.Skip( true );
}

void HistoryGroupHeader::onSize( wxSizeEvent& event )
{
    Refresh();
}

};
