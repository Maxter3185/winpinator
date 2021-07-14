#include "screen_selector.hpp"

#include "../globals.hpp"

namespace gui
{

ScreenSelector::ScreenSelector( wxWindow* parent )
    : srv::IServiceObserver()
    , wxPanel( parent, wxID_ANY )
    , m_book( nullptr )
    , m_currentPage( SelectorPage::STARTING )
    , m_page0( nullptr )
    , m_page1( nullptr )
    , m_page2( nullptr )
    , m_page3( nullptr )
{
    wxBoxSizer* mainSizer = new wxBoxSizer( wxVERTICAL );

    m_book = new wxSimplebook( this );
    //m_book->SetEffect( wxShowEffect::wxSHOW_EFFECT_SLIDE_TO_LEFT );
    //m_book->SetEffectTimeout( 300 );
    mainSizer->Add( m_book, 1, wxEXPAND );

    m_page0 = new StartingPage( m_book );
    m_book->AddPage( m_page0, wxEmptyString );
    m_page1 = new OfflinePage( m_book );
    m_book->AddPage( m_page1, wxEmptyString );
    m_page2 = new HostListPage( m_book );
    m_book->AddPage( m_page2, wxEmptyString );
    m_page3 = new NoHostsPage( m_book );
    m_book->AddPage( m_page3, wxEmptyString );

    srv::WinpinatorService* serv = Globals::get()->getWinpinatorServiceInstance();

    m_book->SetSelection( 0 );
    onStateChanged();

    SetSizer( mainSizer );

    observeService( serv );

    // Events
    Bind( wxEVT_THREAD, &ScreenSelector::onChangePage, this );
}

void ScreenSelector::onStateChanged()
{
    srv::WinpinatorService* serv = Globals::get()->getWinpinatorServiceInstance();

    if ( !serv->isOnline() && m_currentPage != SelectorPage::OFFLINE )
    {
        m_currentPage = SelectorPage::OFFLINE;

        wxThreadEvent event( wxEVT_THREAD );
        event.SetInt( (int)SelectorPage::OFFLINE );
        wxQueueEvent( this, event.Clone() );
        return;
    }

    if ( serv->isOnline() && !serv->isServiceReady()
        && m_currentPage != SelectorPage::STARTING )
    {
        m_currentPage = SelectorPage::STARTING;
        
        wxThreadEvent event( wxEVT_THREAD );
        event.SetInt( (int)SelectorPage::STARTING );
        wxQueueEvent( this, event.Clone() );
        return;
    }

    if ( serv->isServiceReady() && m_currentPage != SelectorPage::HOST_LIST )
    {
        m_currentPage = SelectorPage::HOST_LIST;

        wxThreadEvent event( wxEVT_THREAD );
        event.SetInt( (int)SelectorPage::HOST_LIST );
        wxQueueEvent( this, event.Clone() );
        return;
    }
}

void ScreenSelector::onChangePage( wxThreadEvent& event )
{
    m_book->SetSelection( event.GetInt() );
}

};